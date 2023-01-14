# ft_irc-


# functions 

int listen(int sockfd, int backlog);<br>
`sockfd` is the file descriptor of socket<br>
`backlog` is the number of connections a system can be handel it a single time<br>

`accept` function also return a file descriptor it stok new variable in `newsockfd` <br>
newsockfd = accept(sockfd, (struct sockadd *)&addr, &addrlen)<br>
so wy we require the new sock fd because whatever communication is going to a place after that we are going to use new sock at the file descriptor all communication would be taken on new sock every file descriptor<br>
it return 0 if the func is access<br>
and return 1 if the func is failure<br>

The `gethostbyname()` function returns a pointer to a hostent struct, which contains various information about the host, such as its name, aliases, and IP addresses.<br>
Here is a list of the most commonly used members of the hostent struct:<br>

`h_name`: a null-terminated string containing the official name of the host.<br>
`h_aliases`: a pointer to an array of null-terminated strings, where each string is an alias or alternative name for the host. The array is terminated by a null pointer.<br>
`h_addrtype`: an integer value that indicates the address type of the host. It is usually set to AF_INET for IPv4 addresses or AF_INET6 for IPv6 addresses.<br>
`h_length`: an integer value that indicates the length (in bytes) of the addresses stored in the h_addr_list member.<br>
`h_addr_list`: a pointer to an array of pointers to binary addresses for the host. The addresses are stored in network byte order.<br>

`struct sockaddr_in` is a C data structure used to hold information about an internet socket address. It is defined in the <netinet/in.h> header file, and is typically used with the socket API in C to create and manipulate internet sockets. It includes the following fields:<br>

`sin_family:` the address family (usually set to AF_INET for IPv4)<br>
`sin_port:` the port number in network byte order<br>
`sin_addr:` the internet address in network byte order<br>
`sin_zero:` padding to make the structure the same size as struct sockaddr<br>
It is used when creating socket ,binding socket, accept socket and connect socket<br>

`socklen_t` is a data type defined in the <sys/socket.h> header file in C. It is used to specify the size of a socket address structure, such as `struct sockaddr_in`. The `socklen_t` type is used as the third argument in many socket functions, such as `bind()`, `connect()`, `accept()`, and `getsockname()`, to specify the size of the socket address structure passed as an argument.<br>
`select()` function is a system call that allows a program to monitor multiple file descriptors, waiting until one or more of the file descriptors become "ready" for a specific type of I/O operation (e.g. reading, writing). The function takes three sets of file descriptors as arguments:<br>

The first set, `readfds`, contains file descriptors to be checked for readability<br>
The second set, `writefds`, contains file descriptors to be checked for writability<br>
The third set, `exceptfds`, contains file descriptors to be checked for exceptions<br>
