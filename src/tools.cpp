/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddaou <mhaddaou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/21 15:29:16 by mhaddaou          #+#    #+#             */
/*   Updated: 2023/01/22 19:01:32 by mhaddaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/channel.hpp"
#include "../includes/client.hpp"
#include "../includes/server.hpp"


void desconectedClient(Server *server, int fd, int i){
    std::cout << server->map_clients[fd].getNickName() << " Disconnected" << std::endl;
    server->map_clients.erase(fd);
    server->fds.erase(server->fds.begin() + i);
    close(fd); 
}
std::string handlemsg(std::vector<std::string> msg){
    std::string ret;
    for (size_t i = 2; i < msg.size(); i++){
        ret += msg[i];
        if (i != msg.size() - 1)
            ret += " ";
    }
    ret += "\r\n";
    return (ret);
}
int checkIsRoot(Server *server, std::string buffer, int fd){
    buffer.erase(std::remove(buffer.begin(), buffer.end(), '\n'), buffer.cend());
    buffer.erase(std::remove(buffer.begin(), buffer.end(), '\r'), buffer.cend());
    std::cout << buffer << std::endl;
    if (strcmp(buffer.c_str(), "root") == 0)
    {
        server->map_clients[fd].setName("root");
        server->map_clients[fd].setNickName("root");
        server->map_clients[fd].setRealName("root");
        return (EXIT_SUCCESS);
    }
    return (EXIT_FAILURE);
}