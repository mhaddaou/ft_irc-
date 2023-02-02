# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mhaddaou <mhaddaou@student.1337.ma>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/12 11:22:07 by mhaddaou          #+#    #+#              #
#    Updated: 2023/02/02 18:59:39 by mhaddaou         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ircserv
FLAGS = -Wall -Wextra -Werror -std=c++98 
SRC = src/main.cpp src/server.cpp src/client.cpp src/tools.cpp src/channel.cpp src/mode.cpp src/bot.cpp
CC = c++
HEADER = includes/server.hpp includes/client.hpp includes/channel.hpp includes/mode.hpp 
RM = rm -rf


%.o: %.cpp
	$(CC) $(FLAGS) -c -o $@ $<

OBJ = $(SRC:%.cpp=%.o)

all: ${NAME}

${NAME} : ${OBJ} ${HEADER}
	${CC} ${FLAGS} ${OBJ} -o ${NAME}  

clean:
	${RM} ${OBJ}
	
fclean: clean
	${RM} ${NAME}

re : fclean all