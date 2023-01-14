/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddaou <mhaddaou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/12 10:57:51 by mhaddaou          #+#    #+#             */
/*   Updated: 2023/01/14 03:09:05 by mhaddaou         ###   ########.fr       */
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
# include <signal.h>
# include <vector>

extern int n_MaxFd; 

class Server{
    public:
        int sockfd;
        int newsockfd;
        int port;
        int n;
        std::string password;
        std::ifstream inputFile;
        char buffer[255];
        std::string bfr;
        struct sockaddr_in ser_add;
        struct sockaddr_in cli_add;
        
        socklen_t clilen;
        Server();
        ~Server();
        class WrongArgument : public std::exception{
            public:
                virtual const char * what() const throw();
        };
        
};

/// @brief 
/// @param err error code
void error_handler(int err);

extern std::vector<Server> servers;