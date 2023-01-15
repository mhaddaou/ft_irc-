# ft_irc-


# functions 

`listen()` marks the socket referred to by sockfd as a passive
       socket, that is, as a socket that will be used to accept incoming
       connection requests using accept<br>

`listen()` function is a socket function that is used to listen for incoming connections on a specified socket. It sets the maximum number of connections that can be queued for a specific socket.<br>
```console
#include <sys/types.h>
#include <sys/socket.h>

int listen(int socket, int backlog);
```

