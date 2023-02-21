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

SRCS =	srcs/Channel/Channel.cpp \
		srcs/User/User.cpp \
		srcs/Server/Server.cpp \
		main.cpp

HEADERS =	srcs/Channel/Channel.hpp \
			srcs/User/User.hpp \
			srcs/Server/Server.hpp

CC = c++
RM = rm -f

FLAGS = -Wall -Wextra -Werror -std=c++98

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