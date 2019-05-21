#include "TCPServer.hpp"

/**
 * Constructor
 */
TCPServer::TCPServer()
{
    _sockaddr_in.sin_family = AF_INET;
}

/**
 * Destructor
 */
TCPServer::~TCPServer() {}

/**
 * Connects to server
 */
void TCPServer::connectToServer(std::string IP, int port)
{
    std::cout << "Connecting to server..." << std::endl;
    int client = createSocket();
    setAddress(IP);
    setPort(port);

    usleep(1000000);
    if (connect(client, (struct sockaddr*)&_sockaddr_in, sizeof(_sockaddr_in)) == 0) {
        std::cout << "Connection success!" << std::endl;
        send(client, "This is a test message. Wahehe\n\0", 32, 0);
        usleep(1000000);
        send(client, "This is a test message. Wahehe1\n\0", 32, 0);
        usleep(1000000);
        send(client, "This is a test message. Wahehe2\n\0", 32, 0);
    } else {
        std::cout << "Connection failed." << std::endl;
        fprintf(stderr, "Client: %s (%d)\n", strerror(errno), errno);
    }
    usleep(1000000);
    close(client);
}

/**
* Sets buffer size
*/
void TCPServer::setBufferSize(int bufferSize)
{
    _bufferSize = bufferSize;
}

/**
 * Starts the server
 */
void TCPServer::startServer(std::string IP, int port)
{
    setAddress(IP);
    setPort(port);
    receive(createServer(createSocket()));
}

/**
 * IP Address
 */
void TCPServer::setAddress(std::string address)
{
    _sockaddr_in.sin_addr.s_addr = inet_addr(address.c_str());
}

/**
 * Sets port
 */
void TCPServer::setPort(int port)
{
    _sockaddr_in.sin_port = htons(port);
}

/**
 * Creates a TCP Socket
 */
int TCPServer::createSocket() {
    int fdSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (fdSocket == -1) {
        std::cout << "Socket creation error: " << fdSocket << std::endl;
    }

    return fdSocket;
}

/**
 * Creates a TCP Server
 */
int TCPServer::createServer(int fdSocket)
{
	bind(fdSocket, (struct sockaddr *)&_sockaddr_in, sizeof(_sockaddr_in));
 	listen(fdSocket, 255);
 	return fdSocket;
}

/**
 * Waits for client to connect to this server
 */
void TCPServer::receive(int fdSocket)
{
    std::string ipAddress;
    while(true) {
        pthread_t thread;
		OnClientConnectStruct *onClientConnectParam = new OnClientConnectStruct;
        struct sockaddr_in clientAddress;
		socklen_t sosize = sizeof(clientAddress);
		onClientConnectParam->fdClientSocket = accept(fdSocket, (struct sockaddr*)&clientAddress, &sosize);
        onClientConnectParam->bufferSize = _bufferSize;
		ipAddress = inet_ntoa(clientAddress.sin_addr);
		pthread_create(&thread, NULL, &onClientConnect, onClientConnectParam);
    }
    close(fdSocket); // Looks nonsense, and will never be called... I need to call this whenever process terminates.
}

/**
 * Function to be called when client attempts to connect
 */
void* onClientConnect(void* onClientConnectParam)
{
    struct OnClientConnectStruct* param = (struct OnClientConnectStruct*)onClientConnectParam;

    char msg[param->bufferSize];
    int bytes;
    pthread_detach(pthread_self());
    while(1) {
        bytes = recv(param->fdClientSocket, msg, param->bufferSize, 0);
        if (bytes <= 0) {
            std::cout << "Client closed the connection." << std::endl;
            close(param->fdClientSocket);
            break;
        }
        msg[bytes] = 0;
        std::cout << " Message: " << msg << std::endl;
        send(param->fdClientSocket, std::string(msg).c_str(), std::string(msg).length(), 0);
    }
    if (bytes == -1) {
        fprintf(stderr, "recv: %s (%d)\n", strerror(errno), errno);
    }
    pthread_exit(0);
    return 0;
}
