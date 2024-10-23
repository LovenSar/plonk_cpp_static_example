// plonk_server.cpp

#include "plonk.h"
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <cstring>
#include <unistd.h> // For close()
#include <arpa/inet.h> // For inet_pton()
#include <cstdlib> // For atoi()

using json = nlohmann::json;

int main(int argc, char* argv[]) {
    if (argc != 5) {
        std::cerr << "Usage: " << argv[0] << " <IP> <Port> <ptau file path> <Secret message>\n";
        return 1;
    }

    // Parse command-line arguments
    std::string server_ip = argv[1];
    int server_port = std::atoi(argv[2]);
    std::string ptau_file_path = argv[3];
    std::string secret_message = argv[4];

    // Initialize curve parameters
    init_curve();

    // Load setup
    Setup setup(ptau_file_path);

    // Define circuit equations
    std::vector<std::string> eqs = {"y public", "y === s * s"};

    // Generate verification key
    auto vk = make_verification_key(setup, 8, eqs);

    // Networking setup
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        std::cerr << "Socket creation failed\n";
        return 1;
    }

    sockaddr_in address;
    int addrlen = sizeof(address);
    std::memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(server_ip.c_str());
    address.sin_port = htons(server_port);

    if (bind(server_fd, (sockaddr*)&address, sizeof(address)) < 0) {
        std::cerr << "Bind failed\n";
        close(server_fd);
        return 1;
    }

    if (listen(server_fd, 3) < 0) {
        std::cerr << "Listen failed\n";
        close(server_fd);
        return 1;
    }

    std::cout << "Server is listening on " << server_ip << ":" << server_port << std::endl;

    int new_socket = accept(server_fd, (sockaddr*)&address, (socklen_t*)&addrlen);
    if (new_socket < 0) {
        std::cerr << "Accept failed\n";
        close(server_fd);
        return 1;
    }

    // Receive public input y
    char buffer[1024] = {0};
    read(new_socket, buffer, 1024);
    std::string public_input_str(buffer);
    json public_input_json = json::parse(public_input_str);
    uint64_t public_y = public_input_json["public_y"];

    // Send acknowledgement
    std::string ack = "ACK";
    send(new_socket, ack.c_str(), ack.size(), 0);

    // Receive proof
    std::memset(buffer, 0, sizeof(buffer));
    read(new_socket, buffer, 1024);
    std::string proof_str(buffer);
    json proof_json = json::parse(proof_str);

    // Deserialize proof
    auto proof = deserialize_proof(proof_json);

    // Verify proof
    bool is_valid = verify_proof(setup, 8, vk, proof, {public_y}, true);

    if (is_valid) {
        std::cout << "Proof verified successfully\n";
        // Send the provided secret message
        send(new_socket, secret_message.c_str(), secret_message.size(), 0);
    } else {
        std::cout << "Proof verification failed\n";
        std::string fail_message = "Proof verification failed";
        send(new_socket, fail_message.c_str(), fail_message.size(), 0);
    }

    close(new_socket);
    close(server_fd);
    return 0;
}
