/*
 * listener.c
 *	http://beej.us/guide/bgipc/output/html/multipage/unixsock.html
 *  Created on: May 12, 2016
 *      Author: sbartholomew
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCK_PATH "/home/sbartholomew/echo_socket"

int main(void)
{
    int s, t, len, done;
    struct sockaddr_un remote;
    char str[100];

    if ((s = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    printf("Trying to connect...\n");

    remote.sun_family = AF_UNIX;
    strcpy(remote.sun_path, SOCK_PATH);
    len = strlen(remote.sun_path) + sizeof(remote.sun_family);
    if (connect(s, (struct sockaddr *)&remote, len) == -1) {
        perror("connect");
        exit(1);
    }

    printf("Connected. on %d\n", s);

    done = 0;

    do{
    	/*if (send(s, str, strlen(str), 0) == -1) {
            perror("send");
            exit(1);
        }*/

        if ((t=recv(s, str, 100, 0)) > 0) {
            str[t] = '\0';
            printf("echo> %s", str);
        } else {
            if (t < 0) perror("recv");
            else printf("Server closed connection\n");
            exit(1);
        }
    } while (!done);

    close(s);

    return 0;
}
