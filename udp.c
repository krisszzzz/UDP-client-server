#include "udp.h"
#include <sys/types.h>
#include <sys/socket.h> // Use linux socket API
#include <netinet/in.h> // include sockaddr_in structure
                        // for working with ip addresses and
                        // ports

int client_connect(const int ip_addr, const int port)
{
        
        char message       = 0; // message that will be passed to server

        //                          domain   type        protocol (default protocol for SOCK_DGRAM is UDP)
        int  client_socket = socket(AF_INET, SOCK_DGRAM, DEFAULT_PROTOCOL);

        if (client_socket == INCORRECT_SOCKET) {
                perror("Client socket creation error");
                return CLIENT_CONNECT_ERROR;
        }

        struct sockaddr_in server_addr = {
                .sin_family = AF_INET,     // set address family (internet domain in this case)
                .sin_addr   = ip_addr,     // set ip address
                .sin_port   = htons(port), // set port (htons convert to correct value)
                .sin_zero   = {0}          // zero data (only used for correct type cast to sockaddr)
        };

        if(sendto(client_socket,                  &message,     sizeof(message),
                  SENDTO_FLAGS, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SENDTO_ERROR) {
                perror("sendto error");
                return CLIENT_CONNECT_ERROR;
        }

        close(client_socket);

        return 0;
}



int client_cli(const int argc, char* argv[])
{
        switch(argc) {
        case USR_CLI_DEFAULT_IP_N_PORT: {
                printf("Set to default ip: 127.0.0.1 (localhost) and default port: 80\n");
                return client_connect(htonl(DEFAULT_IP), DEFAULT_PORT);
        }
                
        case USR_CLI_DEFAULT_PORT: {
                int ip_address = inet_addr(argv[argc - 1]);

                if (ip_address == INADDR_NONE) {
                        printf("Inccorect ip address: %s\n",
                               argv[argc - 1]);
                        
                        return INCORRECT_IP;
                }
                
                printf("Set to default port: 80\n");
                
                return client_connect(ip_address, DEFAULT_PORT);
        }
                
        case USR_ALL_ARGS_PASSED: {
                int ip_address = inet_addr(argv[argc - 2]);

                if (ip_address == INADDR_NONE) {
                        printf("Inccorect ip address: %s\n",
                               argv[argc - 2]);
                        
                        return INCORRECT_IP;
                }

                int port = atoi(argv[argc - 1]);
                if (port > 65535 || port < 0) {
                        printf("Inccorect port number: %d; The port should be in [0; 65535]\n", port);
                        return INCORRECT_PORT;
                }
                
                return client_connect(ip_address, port);
        }

        default:
                printf("Unexpected argument count = %d, expected value is 2, 3 or 4\n", argc);
                return INCORRECT_ARG_NUM;
        };
        
        return 0;
}


int server_start(int port)
{
        char buffer[MAX_BUFFER_SIZE] = {0}; // buffer to take package

        int server_socket = socket(AF_INET, SOCK_DGRAM, 0);

        struct sockaddr_in client_addr = {
                .sin_family = AF_INET,
                .sin_addr   = htonl(INADDR_ANY), // Take package from any ip addresses
                .sin_port   = htons(port),
                .sin_zero   = {0}
        };

        if (server_socket == INCORRECT_SOCKET) {
                perror("Server socket creation error");
                return SERVER_ERROR;
        }

        if (bind(server_socket, (struct sockadrr*)&client_addr, sizeof(client_addr)) < 0) {
                perror("Server bind socket error");
                return SERVER_ERROR;
        }

        // Close server if some message will be received

        //       to_socket,     buffer_ptr, buffer_max_len, flags, from_addr_ptr, from_addr_len
        recvfrom(server_socket, buffer,     MAX_BUFFER_SIZE, 0,     NULL,         NULL);

        printf("Package was received\n"); // print message if package was received
    
        close(server_socket);
          
        return 0;
}

int server_cli(int argc, char* argv[])
{
        switch(argc) {
        case SERVER_CLI_DEFAULT_PORT: {
                printf("Set server port to default: 80\n");
                return server_start(DEFAULT_PORT);
        }
                
        case SERVER_ALL_ARGS_PASSED: {
                int port = atoi(argv[argc - 1]);
                if (port > 65535 || port < 0) {
                        printf("Inccorect port number: %d; The port should be in [0; 65535]\n", port);
                        return INCORRECT_PORT;
                }

                return server_start(port);
        }

        default:
                printf("Unexpected argument count. It should be 2 or 3\n");
                return INCORRECT_ARG_NUM;
        };
        
        return 0;
};

void write_help()
{
        printf("This program is using UDP sockets to connect to network with passed ip address and port or\n"
               "you can use this program as server to listen on a passed port.\n"
               "Usage: ./udp [options] <optional_arguments>\n"
               "Options:\n"
               "--help               Display this information.\n"
               "--server <port>      Start listen on a chosen port. If no port was provided the default port (80) will be used\n"
               "--client <ip> <port> Connect using UDP with selected ip address and port.\n"
               "                     You can leave out the port as an argument, in which case port 80 will be used by default.\n"
               "                     You can also leave out the ip address along with it, in which case it will be set \n"
               "                     to 127.0.0.1 (localhost). But passing a port without passing an ip address will result \n"
               "                     in an error: an ip address is always expected as the first argument.\n");
}
