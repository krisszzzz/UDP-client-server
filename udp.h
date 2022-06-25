#if !defined UDP_H_INCLUDED
#define UDP_H_INCLUDED

#include <stdio.h>

enum CLIENT_ERRORS {
        INCCORECT_PORT       = -3,
        INCCORECT_IP         = -2,
        CLIENT_CONNECT_ERROR = -1,  // code of client error that will returned in client_connect function
        INCORRECT_SOCKET     = -1,  // if socket() function get error it return -1
        SENDTO_ERROR         = -1,  // if sendto() function get error it return -1
};

enum CLI_ARGS_COUNT {
        CLI_DEFAULT_IP_N_PORT = 2, // the value 2 is argument count passed through command line. If the
                                   // program work like a client, that it require 4 arguments (first 
                                   // argument - the executable name, second is flag to indetify, that 
                                   // program working as client (--client), third - ip address, fourth -
                                   // port. But last two arguments can be omitted. The default ip address
                                   // 127.0.0.1 (localhost), default port - 80.

        CLI_DEFAULT_PORT      = 3, // 3 argument passed, the only missing one in port. Default port is 80
        ALL_ARGS_PASSED       = 4                            
};


#define DEFAULT_PROTOCOL       0
#define SENDTO_FLAGS           0 // flags used in sendto function

#define DEFAULT_PORT 80
#define DEFAULT_IP   INADDR_LOOPBACK


int client_connect(const int ip_addr, const int port);


int client_cli(const int argc, char* argv[]);

#endif
