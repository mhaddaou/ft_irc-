/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddaou < mhaddaou@student.1337.ma>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/12 11:10:21 by mhaddaou          #+#    #+#             */
/*   Updated: 2023/01/12 14:48:10 by mhaddaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/server.hpp"

Server::Server(){}

Server::~Server(){}

const char* Server::WrongArgument::what() const throw(){
    return ("ERROR : invalid arguments");
}
void error_handler(int err)
{
    if (err == 0)
        std::cerr << "ERROR:\t Port Or Password Not Provided" << std::endl;
    else if (err == 1)
        std::cerr << "ERROR:\t Opening Socket" << std::endl;
    else if (err == 2)
        std::cerr << "ERROR:\t Sending Failed" << std::endl;
    else if (err == 3)
        std::cerr << "ERROR:\t Listening failed" << std::endl;
        
    exit(1);
}