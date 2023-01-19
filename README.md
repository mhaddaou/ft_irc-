# ft_irc-


# functions 

`listen()` function is a socket function that is used to listen for incoming connections on a specified socket. It sets the maximum number of connections that can be queued for a specific socket.<br>
exemple:
```console
#include <sys/types.h>
#include <sys/socket.h>

int listen(int socket, int backlog);
```
The first parameter, `socket`, is the socket descriptor that you want to listen on.<br> The second parameter, `backlog`, is the maximum number of connections that can be queued for the socket.<br>
`return value:`<br>
`0` : was successfully and the socket now is listening for incoming connections<br>
`1` : indicated that an error occurred<br>
`select()` function is a system call that allows a program to monitor multiple file descriptors, waiting until one or more of the file descriptors become "ready" for some class of I/O operation (e.g. input possible). It can be used to monitor multiple sockets or other file descriptors for readability, writability, or errors.<br>
exemple:
```console
#include <sys/select.h>

int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);
```
`nfds` is the highest-numbered file descriptor in any of the descriptor sets, plus 1 or can use FD_SETSIZE.<br>
`FD_SETSIZE` is a macro define in the header `<sys/select.h>` that specifies the maximum number of the file descriptor that can be monitorin by the `select()`<br>
`readfds` is a descriptor set containing the file descriptors to be checked for readability<br>
`writefds` is a descriptor set containing the file descriptors to be checked for writability<br>
`exceptfds` is a descriptor set containing the file descriptors to be checked for errors<br>
`timeout` is a pointer to a struct timeval that specifies the maximum time to wait for an event to occur before returning.<br>
`FD_SET(sockfd, &readfds)` : is use to set the `sockfd` to the set of readability<br>
`FD_ZERO(&readfds)` : is use to clear the set of readability<br>
`FD_ISSET(sockfd, &readfds)` : is use to check if this file descriptor `sockfd` inside the set of readability or not<br>



