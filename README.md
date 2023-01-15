# ft_irc-


# functions 

`listen()` function is a socket function that is used to listen for incoming connections on a specified socket. It sets the maximum number of connections that can be queued for a specific socket.<br>
exemple:
```console
#include <sys/types.h>
#include <sys/socket.h>

int listen(int socket, int backlog);
```


