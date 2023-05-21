# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vducoulo <vducoulo@student.42lyon.fr>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/17 21:28:58 by rbony             #+#    #+#              #
#    Updated: 2023/02/21 10:58:36 by vducoulo         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ircserv

SRCS =	srcs/channel/channel.cpp \
		srcs/user/user.cpp \
		srcs/server/server.cpp \
		srcs/command/command.cpp \
		main.cpp

HEADERS =	srcs/channel/channel.hpp \
			srcs/user/user.hpp \
			srcs/command/command.hpp \
			srcs/server/server.hpp

CC = c++
RM = rm -f

FLAGS = #-fsanitize=address -g #-Wall -Wextra -Werror -std=c++98 

OBJS = ${SRCS:.cpp=.o}

all: $(NAME)

%.o: %.cpp $(HEADERS) Makefile
	${CC} ${FLAGS} -c $< -o $@

$(NAME): $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -o $(NAME)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re:	fclean all

.PHONY: all clean fclean re