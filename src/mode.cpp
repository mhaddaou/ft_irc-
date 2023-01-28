/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddaou <mhaddaou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/28 13:47:47 by mhaddaou          #+#    #+#             */
/*   Updated: 2023/01/28 17:26:26 by mhaddaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/mode.hpp"
#include "../includes/server.hpp"

int modes(char mode){
    if (mode == 'i' || mode == 'm' || mode == 'n' || mode == 'p' || mode == 's' || mode == 't' || mode == 'l' || mode == 'b' || mode == 'o' || mode == 'v' || mode == '-' || mode == '+')
        return (EXIT_SUCCESS);
    return (EXIT_FAILURE);
}

std::string getMode(std::vector<std::string> cmd){
    int i = 0;
         while (cmd[2][i]){
            if (modes(cmd[2][i]) == EXIT_FAILURE)
                return ("UnknownMODE");
            i++;
         }
    return (cmd[2]);
}
int position(std::string cmd, char c){
    int i = 0;
    while (cmd[i]){
        if (cmd[i] == c)
            return i;
        i++;
    }
    return (-1);
}
void checkMode(Server *server, std::vector<std::string> cmd, int fd){
    std::string mode;
    std::string rpl;
    // int posi;
    // int nega;
    if (cmd.size() == 4)
    {
        mode = getMode(cmd);
        if (mode == "UnknownMODE"){
            rpl = ":localhost 501 " + server->map_clients[fd].getNickName() + ": Unknown MODE flag \r\n";
            send(fd, rpl.c_str(), rpl.size(), 0);
            return ;
        }
        else{
            

            
        }
            
        
    }
    else if (cmd.size() == 2)
        return ;
    else{
        rpl = ":localhost 461 "+ server->map_clients[fd].getNickName() +": Not enough parameters \r\n";
        send(fd, rpl.c_str(), rpl.size(),0);
    }
}