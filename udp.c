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
                perror("Socket creation error");
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
        case CLI_DEFAULT_IP_N_PORT: {
                printf("Set to default ip: 127.0.0.1 (localhost) and default port: 80\n");
                return client_connect(DEFAULT_IP, DEFAULT_PORT);
        }
                
        case CLI_DEFAULT_PORT: {
                int ip_address = inet_addr(argv[argc - 1]);

                if (ip_address == INADDR_NONE) {
                        printf("Inccorect ip address: %s\n",
                               argv[argc - 1]);
                        
                        return INCCORECT_IP;
                }
                
                printf("Set to default port: 80\n");
                
                return client_connect(ip_address, DEFAULT_PORT);
        }
                
        case ALL_ARGS_PASSED: {
                int ip_address = inet_addr(argv[argc - 2]);

                if (ip_address == INADDR_NONE) {
                        printf("Inccorect ip address: %s\n",
                               argv[argc - 2]);
                        
                        return INCCORECT_IP;
                }

                int port = atoi(argv[argc - 1]);
                if (port > 65535 || port < 0) {
                        printf("Inccorect port number: %d; The port should be in [0; 65535]\n", port);
                        return INCCORECT_PORT;
                }
                
                return client_connect(ip_address, port);
        }

        default:
                printf("Unexpected argument count = %d, expected value is 2, 3 or 4\n", argc);
        };
        
        return 0;
}

