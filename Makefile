# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: smia <smia@student.42.fr>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/12 11:22:07 by mhaddaou          #+#    #+#              #
#    Updated: 2023/01/15 18:25:39 by smia             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ircserv
FLAGS = -Wall -Wextra -Werror -std=c++98
SRC = src/main.cpp src/server.cpp src/client.cpp
CC = c++
HEADER = includes/server.hpp includes/client.hpp
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