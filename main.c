#define WIN32_LEAN_AND_MEAN
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "winsock2.h"

/*
Get characters from stdin until newline or EOF

*/
int get_input(char *output, int length)
{

        int i = 0;
        char c;

        output[length - 1] = '\0';

        c = getchar();

        while (c != EOF) {

                output[i] = c;
                i++;

                if (c == '\n')
                        break;
                else if (i >= length)
                        break;

                c = getchar();
        }

        output[i] = '\0';

        return 0;
}

SOCKET connect_socket(struct sockaddr_in *address)
{       
        SOCKET active_socket;

        active_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

        if (active_socket == INVALID_SOCKET)
                return INVALID_SOCKET;

        if (connect(active_socket, (struct sockaddr *)address, sizeof(*address)) == SOCKET_ERROR)
                return INVALID_SOCKET;

        return active_socket;
}

void socket_setup()
{
        WORD versionRequested;
        WSADATA wsaData;

        versionRequested = MAKEWORD(2, 2);

        WSAStartup(versionRequested, &wsaData);
}

struct sockaddr_in get_address_struct(char *address, int port)
{
        struct sockaddr_in tmp;
        tmp.sin_family = AF_INET;
        tmp.sin_addr.s_addr = inet_addr(address);
        tmp.sin_port = htons(port);
        return tmp;
} 

int data_send_loop(SOCKET active_socket)
{
        char input[512];
        
        while (1) {
                get_input(input, sizeof(input));
                send(active_socket, input, strlen(input), 0);
        }
        
        return 0;
}

struct sockaddr_in parse_arguments(int argc, char *argv[])
{
        char *address_string;
        int port;

        if (argc < 3) {
                printf("Invalid arguments! \nprogram.exe address port\n");
                exit(1);
        }

        address_string = argv[1];

        if (sscanf(argv[2], "%d", &port) == 0) {
                printf("Invalid port!");
                exit(1);
        }

        return get_address_struct(address_string, port);


}

int main(int argc, char *argv[])
{
        struct sockaddr_in address;

        SOCKET active_socket;

        socket_setup();

        address = parse_arguments(argc, argv);

        if ((active_socket = connect_socket(&address)) == INVALID_SOCKET) {
                printf("Error as fuck\n");
                printf("%d\n", WSAGetLastError());
        }
        
        data_send_loop(active_socket);

        return 0;
}

/*
struct sockaddr {
        ushort  sa_family;
        char    sa_data[14];
        123.123.123.123
};
*/