#include "TCPServer.hpp"

/**
 * TCP Server Thread
 */
void* TCPServerThread(void* param);

/**
 * TCP Server Thread
 */
void* TCPClientThread(void* param);

/**
 * Entry Point
 */
int main()
{
    pthread_t serverThread, clientThread;

    pthread_create(&serverThread, NULL, TCPServerThread, NULL);
    pthread_create(&clientThread, NULL, TCPClientThread, NULL);

    pthread_join(serverThread, NULL);
    pthread_join(clientThread, NULL);

    std::cout << "test" << std::endl;


    // Bye ~
    return 0;
}

/**
 * TCP Server Thread
 */
void* TCPServerThread(void* param)
{
    pthread_detach(pthread_self());

    std::cout << "Starting TCP Server" << std::endl;

    // TCP Server instance
    TCPServer tcp;

    // Sets TCP Buffer Size
    tcp.setBufferSize(4096);

    // Starts TCP Server
    tcp.startServer("0.0.0.0", 9002);

    return 0;
}

/**
 * TCP Server Thread
 */
void* TCPClientThread(void* param)
{
    pthread_detach(pthread_self());

    std::cout << "Starting TCP Client" << std::endl;

    // TCP Server instance
    TCPServer tcp;

    // Sets TCP Buffer Size
    tcp.setBufferSize(4096);

    // Starts TCP Server
    tcp.connectToServer("127.0.0.1", 9002);

    return 0;
}
