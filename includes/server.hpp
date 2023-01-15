/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddaou < mhaddaou@student.1337.ma>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/12 10:57:51 by mhaddaou          #+#    #+#             */
/*   Updated: 2023/01/14 20:00:09 by mhaddaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <iostream>
# include <string>
# include <cstring>
# include <netdb.h>
# include <cstdlib>
# include <sys/socket.h>
# include <netinet/in.h>
# include <fstream>
# include <unistd.h>
# include <vector>
# include <sstream>
# include "../includes/client.hpp"
# define MAX_CLIENT (10)
# define BUF_SIZE (1024)

class Server{
    private:
        int _port;
        std::string _passwd;
    public:
        Client infoClients[MAX_CLIENT];
        Server(char * port, char * passwd);
        std::vector<int> clients;
        int serverfd;
        struct sockaddr_in server_address;
        fd_set sockets_list;
        fd_set readfds;
        int client_socket;
        struct sockaddr_in client_address;
        char buffer[BUF_SIZE];
        void setaddrinfo();
        void check_error(std::string err);
        int _socket();
        int _bind();
        int getPort();
        
        int _listen();
        int _select();
        int _accept();
        void listOfSockets();
        std::vector<std::string> splitCMD(std::string msg);
        struct timeval timeout;
        void setTime();
        
};