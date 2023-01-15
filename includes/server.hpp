/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smia <smia@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/12 10:57:51 by mhaddaou          #+#    #+#             */
/*   Updated: 2023/01/15 18:45:06 by smia             ###   ########.fr       */
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
#include <map>
#include <algorithm>
# define MAX_CLIENT (10)
# define BUF_SIZE (1024)
using namespace std;
class Server{
    private:
        int _port;
        string _passwd;
    public:
        Server(char * port, char * passwd);
        vector<int> clients;
        int serverfd;
        struct sockaddr_in server_address;
        struct sockaddr_in client_address;
        fd_set sockets_list;
        fd_set readfds;
        int client_socket;
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
        vector<string> splitCMD(string msg);
        struct timeval timeout;
        void setTime();
        /// @brief 
        map<int, Client> infoClients;
        string setNickname(int fd);
        string setName(int fd);
    
        
};