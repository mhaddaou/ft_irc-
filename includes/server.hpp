/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddaou <mhaddaou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/12 10:57:51 by mhaddaou          #+#    #+#             */
/*   Updated: 2023/02/02 18:59:24 by mhaddaou         ###   ########.fr       */
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
#include <sys/time.h>
# include <sstream>
#include <ctime>
# include "../includes/client.hpp"
# define MAX_CLIENT (10)
# define BUF_SIZE (1024)

#include <map>
#include <algorithm>

class Channel;
class Client;
typedef std::map<int, Client>::iterator Iterator;
typedef std::map<std::string, Channel>::iterator IteratorChannel;

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
        std::vector<std::string> splitCMD(std::string msg, char l);
        struct timeval timeout;
        void setTime();
        std::map<int, Client> map_clients;
        std::vector<int> fds;
        std::vector<unsigned int> bans;
        std::map<std::string ,Channel > map_channels;
        int _id_channel;
        std::vector<std::string> Channels;
};
int connect(Server *server, char * buffer, int fd, int i);
int nick(Server *server, std::vector<std::string> cmd, int fd, int i);
int user(Server *server, std::vector<std::string> cmd, int fd, int i);
int passwd(Server *server, std::vector<std::string> cmd, int fd, int i);
void handleCmd(Server *server, char * buffer, int fd);
int setPrvMsg(Server *server, std::vector<std::string> cmd, int fd, std::string line);
void desconectedClient(Server *server, int fd, int i);
std::string handlemsg(std::vector<std::string> msg);
int checkIsRoot(Server *server, std::string buffer, int fd); 
void setNoticeMsg(Server *server, std::vector<std::string> cmd, int fd);
void whoIs(Server *server, std::vector<std::string> cmd, int fd);
int NickError( Server *server, std::vector<std::string> cmd);
int checkInvalidChar(std::string nick);
int join (Server *server, std::string buffer, int fd);
int checkCmd(std::string cmd);
void splitChannelsAndPasswd(Server *server, std::string  command, int fd);
void kick(Server* server, std::string buffer, int fd, char c);
int checkIsBan(Server *server, std::vector<std::string> cmd, int fd);
void invcmd(Server *server, std::vector<std::string> cmd, int fd);
int checkBuffer(Server *server, std::string  buffer);
void getCmd(Server *server, std::string cmd , int fd, int i);
int setNick(Server *server, int fd, int check);
void botHelp(Server *server, std::string buffer, int fd);