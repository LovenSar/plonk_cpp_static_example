// plonk_client.cpp

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
    // Ensure the correct number of arguments are provided
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <IP> <Port> <ptau file path>\n";
        return 1;
    }

    // Parse command-line arguments
    std::string server_ip = argv[1];
    int server_port = std::atoi(argv[2]);
    std::string ptau_file_path = argv[3];

    init_curve(); // Initialize curve parameters

    // Load setup
    Setup setup(ptau_file_path);

    // Define circuit equations
    std::vector<std::string> eqs = {"y public", "y === s * s"};

    // Secret and public values
    uint64_t secret_s = 7;
    uint64_t public_y = secret_s * secret_s;

    // Variable assignments
    std::map<std::string, uint64_t> assignments = {{"s", secret_s}, {"y", public_y}};

    // Generate proof
    auto proof = prove_from_witness(setup, 8, eqs, assignments);

    // Serialize proof
    json proof_json = serialize_proof(proof);

    // Networking setup
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        std::cerr << "Error creating socket\n";
        return 1;
    }

    sockaddr_in serv_addr;
    std::memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(server_port);

    if (inet_pton(AF_INET, server_ip.c_str(), &serv_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address\n";
        close(sockfd);
        return 1;
    }

    if (connect(sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Connection failed\n";
        close(sockfd);
        return 1;
    }

    // Send public input y
    json public_input_json = {{"public_y", public_y}};
    std::string public_input_str = public_input_json.dump();
    send(sockfd, public_input_str.c_str(), public_input_str.size(), 0);

    // Receive acknowledgement
    char buffer[1024] = {0};
    ssize_t bytes_read = read(sockfd, buffer, 1024);
    if (bytes_read <= 0) {
        std::cerr << "Failed to receive acknowledgement\n";
        close(sockfd);
        return 1;
    }
    std::string ack(buffer, bytes_read);
    if (ack != "ACK") {
        std::cerr << "Server did not acknowledge public input\n";
        close(sockfd);
        return 1;
    }

    // Send proof
    std::string proof_str = proof_json.dump();
    send(sockfd, proof_str.c_str(), proof_str.size(), 0);

    // Receive server's response
    std::memset(buffer, 0, sizeof(buffer));
    bytes_read = read(sockfd, buffer, 1024);
    if (bytes_read > 0) {
        std::string response(buffer, bytes_read);
        std::cout << "Received from server: " << response << std::endl;
    } else {
        std::cerr << "Failed to receive response from server\n";
    }

    close(sockfd);
    return 0;
}
