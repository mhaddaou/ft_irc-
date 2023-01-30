/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddaou <mhaddaou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/21 15:29:16 by mhaddaou          #+#    #+#             */
/*   Updated: 2023/01/30 20:43:49 by mhaddaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/channel.hpp"
#include "../includes/client.hpp"
#include "../includes/server.hpp"
#include "../includes/mode.hpp"
#include <stdlib.h>


void desconectedClient(Server *server, int fd, int i){
    if (server->map_clients[fd].is_verified == true)
        std::cout << server->map_clients[fd].getNickName() << " Disconnected" << std::endl;
    else
         std::cout << "Guest Disconnected" << std::endl;
    server->map_clients.erase(fd);
    server->fds.erase(server->fds.begin() + i);
    close(fd); 
}
std::string handlemsg(std::vector<std::string> msg){
    std::string ret;
    if (msg[2][0] == ':')
        msg[2].erase(0, 1);
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
    if (buffer == "root")
    {
        server->map_clients[fd].setName("admin_server");
        server->map_clients[fd].setNickName("root");
        server->map_clients[fd].setRealName("admin");
        server->map_clients[fd].verif = 3;
        return (EXIT_SUCCESS);
    }
    return (EXIT_FAILURE);
}
void setNoticeMsg(Server *server, std::vector<std::string> cmd, int fd){
    if (cmd.size() < 3)
        return;
    std::string rpl;
    std::string msg = handlemsg(cmd);
    for (Iterator it = server->map_clients.begin(); it != server->map_clients.end(); ++it){
        if (it->second.getNickName() == cmd[1]){
            if (server->map_clients[fd].isClient == false)
                rpl = ":" + server->map_clients[fd].getNickName() + " NOTICE " + it->second.getNickName()+ " : " + msg;
            else
                rpl = "NOTICE from " + server->map_clients[fd].getNickName() + " : " + msg;
            send(it->first, rpl.c_str(), rpl.size(),0);
            std::cout << "NOTICE OK" << std::endl;
        }
    }
}
void whoIs(Server *server, std::vector<std::string> cmd, int fd){
    std::string rpl;
    // int r = server->client_address.sin_addr.s_addr;
    if (cmd.size() == 3){
        // if this user is operator 
        // 313     RPL_WHOISOPERATOR
                        // "<nick> :is an IRC operator"
        for (Iterator it = server->map_clients.begin(); it != server->map_clients.end(); ++it){
            if (it->second.getNickName() == cmd[1]){
                // The first line contains the nickname, username, hostname, and realname of the user being queried.
                // The second line contains the channels the user is currently joined to.
                // The third line contains the server that the user is connected to, and any additional server info.
                // The fourth line indicates the end of the WHOIS list.
                // char * ip = (it->second.client_address.sin_addr.s_addr);
                std::string ip;
                std::stringstream ss;
                ss << it->second.client_address.sin_addr.s_addr;
                ss >> ip;
                rpl = ":localhost 311 " + server->map_clients[fd].getNickName() + " " + it->second.getNickName() + " " + it->second.getName() + " " + ip + " * : " + it->second.getRealName() + "\r\n"
                ":localhost 319 "+ server->map_clients[fd].getNickName() + " " + it->second.getNickName() + " :" + getChannels(server, cmd[1]) +"\r\n"
               ":localhost 312 " + server->map_clients[fd].getNickName() + " " + it->second.getNickName() + " localhost :Irc Server\r\n"
                ":localhost 318 " + server->map_clients[fd].getNickName() + " " +it->second.getNickName() + " :End of /WHOIS list.";
                send(fd, rpl.c_str(), rpl.size(), 0);
                return ;
            }
        }       
    }
    //:<server name> 311 <requesting nickname> <nickname> <username> <hostname> * :<realname>
}

int checkInvalidChar(std::string nick){
    for (size_t i = 0; i < nick.size(); i++){
        if (nick[i] == '!' || nick[i] == '@' || nick[i] == '#' || nick[i] == '$' || nick[i] == '%' || nick[i] == '^' || nick[i] == '&' 
            || nick[i] == '*' || nick[i] == '(' || nick[i] == ')' || nick[i] == '-' || nick[i] == '+' || nick[i] == '=' || nick[i] == '{' 
                || nick[i] == '}' || nick[i] == '[' || nick[i] == ']' || nick[i] == '|' || nick[i] == '\\' || nick[i] == ':' || nick[i] == ';'
                    || nick[i] == '"' || nick[i] == '\'' || nick[i] == '<' || nick[i] == '>' || nick[i] == ',' || nick[i] == '.' || nick[i] == '/' 
                        || nick[i] == '?')
            return (EXIT_FAILURE);
        }
    
    return (EXIT_SUCCESS);
}

void Nick( Server *server, std::vector<std::string> cmd, int fd){
    std::string rpl;
    //from limechat
    if (server->map_clients[fd].isClient == true){
        if (cmd.size() == 2 && cmd[1].size() == 1){
            // std::cout << cmd[1].size() << std::endl;
            rpl = ":localhost 431 " + server->map_clients[fd].getNickName()+  " :No nickname given\r\n";
            send(fd, rpl.c_str(), rpl.size(), 0);
            return ;
        }
        else if (cmd.size() > 3 || checkInvalidChar(cmd[1]) == EXIT_FAILURE){
            rpl = ":localhost 432 " + server->map_clients[fd].getNickName()+  " :Erroneous nickname\r\n";
            send(fd, rpl.c_str(), rpl.size(), 0);
            return ;
        }
        else{
            server->map_clients[fd].setNickName(cmd[1]);
            std::cout << "NICK OK" << std::endl;
            return ;
        }
        for (Iterator it = server->map_clients.begin(); it != server->map_clients.end(); ++it){
            if (it->second.getNickName() == cmd[1]){
                rpl = ":localhost 433 " + server->map_clients[fd].getNickName()+  " :Nickname is already in use\r\n";
                send(fd, rpl.c_str(), rpl.size(), 0);
                return ;
            }
        }
    }//from netcat
    else{
        if (cmd.size() == 1){
            rpl = "ERROR \tNo nickname given\r\n";
            send(fd, rpl.c_str(), rpl.size(), 0);
            return ;
        }
        else if (cmd.size() > 2 || checkInvalidChar(cmd[1]) == EXIT_FAILURE){
            rpl = "ERROR \tErroneous nickname\r\n";
            send(fd, rpl.c_str(), rpl.size(), 0);
            return ;
        }
        else{
            server->map_clients[fd].setNickName(cmd[1]);
            std::cout << "NICK OK" << std::endl;
            return ;
        }
        for (Iterator it = server->map_clients.begin(); it != server->map_clients.end(); ++it){
            if (it->second.getNickName() == cmd[1]){
                rpl = "ERROR \tNickname is already in use\r\n";
                send(fd, rpl.c_str(), rpl.size(), 0);
                return ;
            }
        }
            
    }
}
int notEnoghtPrmt(Server *server, std::vector<std::string> buffer, int fd){
    std::string rpl;
    int check = 0;
   if (server->map_clients[fd].isClient == true)
   {
            if (buffer.size() < 3 )
            {
                if (buffer.size() == 2 && buffer[1].size() == 1)
                {
                    rpl = ":localhost 461 " + server->map_clients[fd].getNickName() + " : Not enough parameters\r\n";
                    check = 1;
                }
            }
        }
        else
        {
            if (buffer.size() < 2){
                rpl = "Not enough parameters\n";
                check = 1;
            }
        }
        send(fd, rpl.c_str(), rpl.size(), 0);
        return (check);
}

int checkCmd(std::string cmd){
    if (cmd.find(",") != std::string::npos)
        return(EXIT_SUCCESS);
    return(EXIT_FAILURE);
}
// int checkNameChannels(std::vector<std::string> channels){
    
    
// }
void splitChannelsAndPasswd(Server *server, std::string  command, int fd)
{
    std::cout << command << std::endl;
    bool check = true;
    command.erase(0, 5);
    size_t len = 0;
    std::string result;
    size_t i = 1;
    for(size_t j = 0; command[j] != ' '; j++){
        if (command[j] == ',')
            i++;
    }
    std::vector<std::string> cmd = server->splitCMD(command, ' ');
    std::string channels = cmd[0];
    std::string passwd = cmd[1];
    std::vector<std::string> _cha = server->splitCMD(channels, ',');
    // if (checkNameChannels(_cha) == EXIT_FAILURE){
    //     result = 
    // }
    std::vector<std::string> _pass = server->splitCMD(passwd, ',');
    
    if (_cha.size() < _pass.size())
        std::cerr << "error: invalid arguments" << std::endl;
    else{
        while (len < i){
            result = "JOIN " + _cha[len];
            if (_pass.size() > len )
                result += " " + _pass[len];
            if (_cha[len][0] != '#' && _cha[len][0] != '&'){
                check = false;
                break;
            }
            if (join(server, result, fd) == EXIT_FAILURE)
                return ;
            len++;
        }
    }
    if (check == false){
        result = ":localhost 461 "+ server->map_clients[fd].getNickName() +": Not enough parameters \r\n";
        send(fd, result.c_str(), result.size(), 0);
        return ;
    }
}
int checkIsBan(Server *server, std::vector<std::string> cmd, int fd){
    unsigned int ip = server->map_clients[fd]._ip;
    std::vector<unsigned int> vec = server->map_channels[cmd[1]]._bans;
    for(size_t i = 0; i < vec.size(); i++)
    {
        if (vec[i] == ip)
            return EXIT_FAILURE;
        
    }
    return (EXIT_SUCCESS);
    
    
}

int join(Server *server, std::string cmd, int fd)
{
    std::vector<std::string> buffer = server->splitCMD(cmd, ' ');

    if (checkChannel(server, buffer[1]) == EXIT_SUCCESS)
        createNewChannel(server, buffer, fd);
    else{
        if (checkIsBan(server, buffer, fd) == EXIT_FAILURE){
            // :server.name 474 #channel username :Cannot join channel (+b)
            std::string rpl = ":localhost 474 " + buffer[1] + " " + server->map_clients[fd].getNickName() + ":Cannot join channel (+b)\r\n";
            send(fd, rpl.c_str(), rpl.size(), 0);
            return (EXIT_FAILURE);
        }
        if (checkModesChannel(server, buffer, fd) == EXIT_FAILURE)
            return (EXIT_FAILURE);
        if (joinToExistingChannel(server, buffer, fd) == EXIT_FAILURE)
            return EXIT_FAILURE;
    }
    return (EXIT_SUCCESS);
}
void invcmd(Server *server, std::vector<std::string> cmd, int fd){
    std::string rpl;
    int fdtarget = -1;
    std::cout << "size == " << cmd.size() << std::endl;
    if (cmd.size() == 3){
        for (Iterator it = server->map_clients.begin(); it !=  server->map_clients.end(); it++){
            if (it->second.getNickName() == cmd[1]){
                fdtarget = it->first;
                it->second._invite.push_back(cmd[2]);
                break;
            }
        }
        if (fdtarget == -1)
            rpl = ":localhost 401 " + cmd[1] + " : No such nick/channel\r\n";
        else if (checkChannel(server, cmd[2]) == EXIT_SUCCESS)
            rpl = ":localhost 401 " + cmd[1] + " : No such nick/channel\r\n";
        // :sender!user@host INVITE client #channel
        else{
            rpl = ":" +server->map_clients[fd].getNickName() + " INVITE " + cmd[1] + " " + cmd[2] + "\r\n";
            send(fdtarget, rpl.c_str(), rpl.size(), 0);
            return ;
        }
        send(fd, rpl.c_str(), rpl.size(), 0);
    }
    else{
        rpl = ":localhost 461 "+ server->map_clients[fd].getNickName() +": Not enough parameters \r\n";
        send(fdtarget, rpl.c_str(), rpl.size(), 0);  
    }
    
}
