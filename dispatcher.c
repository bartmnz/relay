/*
 * dispatcher.c
 *	http://beej.us/guide/bgipc/output/html/multipage/unixsock.html
 *  Created on: May 12, 2016
 *      Author: sbartholomew
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCK_PATH "echo_socket"

int main(void)
{
    int s, s2, t, len;
    struct sockaddr_un local, remote;
    //char str[100];

    if ((s = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    local.sun_family = AF_UNIX;
    strcpy(local.sun_path, SOCK_PATH);
    unlink(local.sun_path);
    len = strlen(local.sun_path) + sizeof(local.sun_family);
    if (bind(s, (struct sockaddr *)&local, len) == -1) {
        perror("bind");
        exit(1);
    }

    if (listen(s, 5) == -1) {
        perror("listen");
        exit(1);
    }

    for(;;) {
        printf("Waiting for a connection...\n");
        t = sizeof(remote);

        if ((s2 = accept(s, (struct sockaddr *)&remote, (socklen_t *)&t)) == -1) {
            perror("accept");
            exit(1);
        }

        printf("Connected.\n");


        char str2[100];
        while(printf("> "), fgets(str2, 100, stdin), !feof(stdin)) {
            /*n = recv(s2, str, 100, 0);
            if (n <= 0) {
                if (n < 0) perror("recv");
                done = 1;
            }*/

        	if (send(s2, str2, strlen(str2), 0) == -1) {
        	            perror("send");
        	            exit(1);
        	        }

            /*if (!done)
                if (send(s2, str, n, 0) < 0) {
                    perror("send");
                    done = 1;
                }*/
        }

        close(s2);
    }

    return 0;
}
