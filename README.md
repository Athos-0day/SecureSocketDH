# SecureSocketDH

This project implements a simple secure communication channel using Diffie-Hellman key exchange over sockets in C.

## Features

- TCP client-server communication
- Diffie-Hellman key exchange to establish a shared secret
- Basic protection against Man-in-the-Middle attacks using digital signatures
- Example code for generating and exchanging keys

## Usage

1. Compile the server and client programs. `make`
2. Run the server: `./server`
3. Run the client: `./client`
4. Observe the key exchange and communication logs.

## Resources

- Network programming: [La programmation réseau en C : les sockets](http://vidalc.chez.com/lf/socket.html)
- Cryptography basics: [Crypto 101](https://crypto101.io)
- Project source code and additional ciphers: [GitHub Repository](https://github.com/Athos-0day/SecureSocketDH)

## Author

Athos-0day

---

Feel free to explore, modify, and improve the project!
