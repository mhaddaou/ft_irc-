/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddaou < mhaddaou@student.1337.ma>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/12 12:54:55 by mhaddaou          #+#    #+#             */
/*   Updated: 2023/01/14 20:09:21 by mhaddaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../includes/server.hpp"
// #include <typeinfo>

#include <typeinfo>


int main (int ac, char **av){
    if (ac < 3)
        std::cerr << "invalid arguments" << std::endl;
    else{
        Server server(av[1], av[2]);
        server.serverfd = socket(AF_INET, SOCK_STREAM, 0);
        if (server.serverfd < 0)
            std::cerr << "socket error" << std::endl;
        server.server_address.sin_family = AF_INET;
        server.server_address.sin_addr.s_addr = INADDR_ANY;
        server.server_address.sin_port = htons(server.getPort());
        if (bind(server.serverfd, (struct sockaddr *)&server.server_address, sizeof(server.server_address) < 0))
            std::cerr << "bind error" << std::endl;
        
    }
}