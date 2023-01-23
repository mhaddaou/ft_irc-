/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smia <smia@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/12 10:57:51 by mhaddaou          #+#    #+#             */
/*   Updated: 2023/01/23 16:05:40 by smia             ###   ########.fr       */
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

#include <map>
#include <algorithm>

typedef std::map<int, Client>::iterator iterator;


class Server{
    private:
        int _port;
        std::string _passwd;
    public:
        std::string name;
        std::string nick;
        std::string pass;
        Server(char * port, char * passwd);
        int serverfd;
        struct sockaddr_in server_address;
        struct sockaddr_in client_address;
        socklen_t lencli;
        fd_set sockets_list;
        fd_set readfds;
        fd_set writefds;
        int  client_socket;
        char buffer[BUF_SIZE];
        void setaddrinfo();
        void check_error(std::string err);
        int _socket();
        int isClient(std::string str);
        int _bind();
        int getPort();
        std::string getPass(void) const {return _passwd;}
        std::string encrypt(std::string passwd);
        std::string decrypt(std::string passwd);
        int _listen();
        int checkQuit(std::string buffer);
        int _select();
        int _accept();
        void listOfSockets();
        std::vector<std::string> splitCMD(std::string msg);
        struct timeval timeout;
        void setTime();
        std::map<int, Client> map_clients;
        std::vector<int> fds;
        // channel
        std::vector<Channel> channels;
                void lsChannels(int fd)
        {
            std::string rpl;
            for (size_t i = 0; i < channels.size(); i++)
            {
                // fill rpl
            }
            send(fd, rpl.c_str(), rpl.size(), 0);
        }
};
int connect(Server *server, std::string buffer, int fd, int i);
int nick(Server *server, std::vector<std::string> cmd, int fd, int i);
void user(Server *server, std::vector<std::string> cmd, int fd);
int passwd(Server *server, std::vector<std::string> cmd, int fd, int i);
void handleCmd(Server *server, std::string buffer, int fd);
int setPrvMsg(Server *server, std::vector<std::string> cmd, int fd);
void desconectedClient(Server *server, int fd, int i);
std::string handlemsg(std::vector<std::string> msg);
int checkIsRoot(Server *server, std::string buffer, int fd); 
void setNoticeMsg(Server *server, std::vector<std::string> cmd, int fd);
void whoIs(Server *server, std::vector<std::string> cmd, int fd);
void Nick( Server *server, std::vector<std::string> cmd, int fd);
int checkInvalidChar(std::string nick);

// channel
