# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mhaddaou < mhaddaou@student.1337.ma>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/12 11:22:07 by mhaddaou          #+#    #+#              #
#    Updated: 2023/01/12 12:26:45 by mhaddaou         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ircserv
FLAGS = -Wall -Wextra -Werror -std=c++98
SRC = src/main.cpp src/server.cpp
CC = c++
HEADER = includes/server.hpp
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