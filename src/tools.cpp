/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddaou <mhaddaou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/21 15:29:16 by mhaddaou          #+#    #+#             */
/*   Updated: 2023/01/21 17:10:22 by mhaddaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/channel.hpp"
#include "../includes/client.hpp"
#include "../includes/server.hpp"


void desconectedClient(Server *server, int fd, bool check){
    
    if (check == false)
        std::cout  <<"Guest is Disconnected" << std::endl;
    if (check == true){
        std::cout << server->map_clients[fd].getNickName() << " Disconnected" << std::endl;
        server->map_clients.erase(fd);
    }
    
    close(fd); 
}