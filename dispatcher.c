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
#include <sys/time.h>
#include <sys/stat.h>




#define SOCK_PATH "/home/sbartholomew/echo_socket"

/*void * get_in_addr (struct sockaddr *sa){
  if (sa->sa_family == AF_INET) {
    return &(((struct sockaddr_in *) sa)->sin_addr);
  }

  return &(((struct sockaddr_in6 *) sa)->sin6_addr);
}*/


int main(void)
{
	int s, s2, nbytes, len;
    struct sockaddr_un local;
    char str[100];
    fd_set master;		// master file descriptor list
    fd_set read_fds;		// temp file descriptor list for select()
    FD_ZERO (&master);		// clear the master and temp sets
    FD_ZERO (&read_fds);
    struct sockaddr_storage remoteaddr;	// client address
    socklen_t addrlen;
    int fd_MAX;


    //mkdir("/home/sbartholomew/temp", 777);

    if ((s = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }
    int enable = 1;
    if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
        perror("setsockopt(SO_REUSEADDR) failed");


    local.sun_family = AF_UNIX;
    strcpy(local.sun_path, SOCK_PATH);
    unlink(local.sun_path);
    len = strlen(local.sun_path) + sizeof(local.sun_family);
    if (bind(s, (struct sockaddr *)&local, len) == -1) {
        perror("bind");
        exit(1);
    }

    chmod(SOCK_PATH, 0777);
    if (listen(s, 5) == -1) {
        perror("listen");
        exit(1);
    }
    fd_MAX = s;
    FD_SET (s, &master);
    FD_SET (STDIN_FILENO, &master);

    for(;;) {
    	read_fds = master;		// master list of all descriptors
    	    if (select (fd_MAX + 1, &read_fds, NULL, NULL, NULL) == -1) {
    	      //file desriptor (int) select
    	      perror ("select");
    	      exit (4);
    	    }
    	    // run through the existing connections looking for data to read
    	    int i;
    	    for (i = 0; i <= fd_MAX; i++) {
    	    	if (FD_ISSET (i, &read_fds)) {	// we got one!!
    	    		if (i == s) {
    	    			// handle new connections
    	    			addrlen = sizeof remoteaddr;
    	    			s2 =
    	    					accept (s, (struct sockaddr *) &remoteaddr, &addrlen);

    	    			if (s2 == -1) {
    	    				perror ("accept");
    	    			} else {
    	    				FD_SET (s2, &master);	// add to master set
    	    				if (s2 > fd_MAX) {	// keep track of the max
    	    					fd_MAX = s2;
    	    				}
    	    			}
    	    		} else if (i == STDIN_FILENO) {
    	    			if ((nbytes = read (i, str, sizeof str)) > 0) {
    	    				printf ("Read from stdin :%s\n", str);
    	    				for (i = 0; i <= fd_MAX; i++) {
    	    					if (FD_ISSET (i, &master) && i != STDIN_FILENO && i != s) {
    	    						//master is all connected clients listening socket and STDIN
    	    						printf ("Sending STDIN to client socket: %d\n", i);
    	    						//i is a client socket & read froms STDIN
    	    						if (send (i, str, nbytes, 0) == -1) {
    	    							perror ("send");
    	    						}
    	    					}
    	    				}

    	    			}
    	    		}
    	    	}
    	    }
    }



    return 0;
}
