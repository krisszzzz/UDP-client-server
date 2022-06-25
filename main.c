#include "udp.h"
#include <string.h>

#define NO_ARGS  1
#define FLAG_ARG 1

int main(int argc, char* argv[])       
{
        if (argc <= NO_ARGS) {
                printf("Expected arguments, type --help for more info\n");
                return 0;
        }

        if (strncmp(argv[FLAG_ARG], "--client", strlen("--client")) == 0) {
                return client_cli(argc, argv);
        }

        return 0;
}
