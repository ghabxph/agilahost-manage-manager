#pragma once
#include "includes.hpp"
#define MAX_PACKET_SIZE 4096

/**
 * onClientConnect Parameter
 */
struct OnClientConnectStruct {

    /**
     * File Descriptor of Client Socket
     */
    long fdClientSocket;

    /**
     * Buffer Size / Max size of packet
     */
    int bufferSize;
};

/**
 * Function to be called when client attempts to connect
 */
void* onClientConnect(void* onClientConnectParam);

/**
 * TCP Server
 *  - A general purpose TCP Server
 */
class TCPServer
{
    public:

        /**
        * Constructor
        */
        TCPServer();

        /**
        * Destructor
        */
        virtual ~TCPServer();

        /**
         * Connects to server
         */
        void connectToServer(std::string IP, int port);

        /**
        * Sets buffer size
        */
        void setBufferSize(int bufferSize);

        /**
        * Starts the server
        */
        void startServer(std::string IP, int port);

    protected:

    private:

        /**
        * Buffer size / Max packet size
        */
        int _bufferSize;

        /**
        * Socket addr / port
        */
        struct sockaddr_in _sockaddr_in;

        /**
        * IP Address
        */
        void setAddress(std::string address);

        /**
        * Sets port
        */
        void setPort(int port);

        /**
        * Creates a TCP Socket
        */
        int createSocket();

        /**
        * Creates a TCP Server
        */
        int createServer(int fdSocket);

        /**
        * Waits for client to connect to this server
        */
        void receive(int fdSocket);
};
