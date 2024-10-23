# PLONK C++ Example - README

## Project Overview

This `plonk_cpp_example` project is a C++ implementation of the zero-knowledge proof protocol known as PLONK. It is based on the [Ethereum Research py_plonk project](https://github.com/ethereum/research/tree/master/py_plonk) and translates the original Python implementation into C++. With this project, you can generate and verify zero-knowledge proofs using the PLONK protocol in C++.

## Dependencies

To compile and run this project, you will need the following key dependencies:

1. **nlohmann/json**: A JSON handling library (used for proof serialization and deserialization).
2. **libff**: A C++ library for finite fields and elliptic curves, which is essential for the PLONK implementation.

### Installation Steps for Dependencies

#### 1. Installing nlohmann/json

[nlohmann/json](https://github.com/nlohmann/json) is a lightweight JSON library that supports modern C++ standards. It is used in the project for proof serialization and network communication.

You can install it as follows:

1. Clone the source code:
   ```bash
   git clone https://github.com/nlohmann/json
   ```

2. Or install it using a package manager (for example, on Ubuntu):
   ```bash
   sudo apt-get install nlohmann-json3-dev
   ```

3. Include it in the compilation process by using the `-I` flag:
   ```bash
   g++ -I/path/to/nlohmann/json -o plonk_example plonk_client.cpp plonk_lib.cpp -L/usr/local/lib -lff -lgmp -lcrypto -lpthread
   ```

#### 2. Installing libff

[libff](https://github.com/scipr-lab/libff) is a C++ library for elliptic curve and finite field arithmetic, commonly used in cryptographic protocols like zero-knowledge proofs.

Steps for installation:

1. Clone the repository:
   ```bash
   git clone https://github.com/scipr-lab/libff
   ```

2. Install required dependencies:
   Ensure that `gmp` and `cmake` are installed by running:
   ```bash
   sudo apt-get install libgmp-dev cmake
   ```

3. Build and install `libff`:
   ```bash
   cd libff
   mkdir build
   cd build
   cmake ..
   make
   sudo make install
   ```

4. During compilation, include the `libff` library using the `-I` and `-L` flags:
   ```bash
   g++ -I/usr/local/include -L/usr/local/lib -lff -lgmp -lcrypto -lpthread -o plonk_example plonk_client.cpp plonk_lib.cpp
   ```

## Compilation and Execution

### Compilation

Once the dependencies are installed, you can compile the project with the following command:

```bash
g++ -o plonk_client plonk_client.cpp plonk_lib.cpp \
    -I/usr/local/include -I/path/to/nlohmann/json \
    -L/usr/local/lib -lff -lgmp -lcrypto -lpthread -static -s
```

### Execution

Run the `plonk_client` and `plonk_server` programs, passing the IP address, port, and path to the `ptau` file as command-line arguments.

Example command to run:

```bash
./plonk_client 192.168.180.130 55554 /path/to/powersOfTau28_hez_final_11.ptau
```

## References

- [py_plonk](https://github.com/ethereum/research/tree/master/py_plonk): This project is based on the Ethereum Research implementation of the PLONK protocol in Python.
- [nlohmann/json](https://github.com/nlohmann/json): A modern C++ JSON library used for handling JSON data in the project.
- [libff](https://github.com/scipr-lab/libff): A C++ library for working with elliptic curves and finite fields, crucial for implementing cryptographic proofs.

## License

This project follows the license and usage guidelines of the Ethereum Research project. Please refer to the upstream project's [LICENSE](https://github.com/ethereum/research/blob/master/LICENSE) for more details.
