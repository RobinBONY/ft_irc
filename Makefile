# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rbony <rbony@corobizar.com>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/17 21:28:58 by rbony             #+#    #+#              #
#    Updated: 2023/05/21 18:28:46 by rbony            ###   ########lyon.fr    #
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
			srcs/server/server.hpp	\
			includes/irc.hh

CC = c++
RM = rm -f

FLAGS = -Wall -Wextra -Werror -std=c++98 -Wpointer-arith -Wcast-qual -Wno-missing-braces -fsanitize=address -g


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
