/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddaou <mhaddaou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/28 13:45:25 by mhaddaou          #+#    #+#             */
/*   Updated: 2023/02/01 23:56:09 by mhaddaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef MODE_HPP
#define MODE_HPP

#include "server.hpp"
#include "channel.hpp"

void checkMode(Server *server, std::vector<std::string> cmd, int fd);
std::string getMode(std::vector<std::string> cmd);
int modes(char mode);
int addLimit(Server *server, std::vector<std::string> cmd, int fd);
int checkModesChannel(Server *server, std::vector<std::string> cmd, int fd);
int setUserOperator(Server *server, std::vector<std::string> cmd, int fd);
void rmBan(Server *server, std::vector<std::string> cmd);


#endif