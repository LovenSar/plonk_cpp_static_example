g++ -o plonk_client plonk_client.cpp plonk_lib.cpp \
    -I/usr/local/include -I/usr/local/include/libff \
    -L/usr/local/lib -lff -lgmp -lcrypto -lpthread -static -s

g++ -o plonk_server plonk_server.cpp plonk_lib.cpp \
    -I/usr/local/include -I/usr/local/include/libff \
    -L/usr/local/lib -lff -lgmp -lcrypto -lpthread -static -s
