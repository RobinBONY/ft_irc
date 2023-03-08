/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Irc.hh                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vducoulo <vducoulo@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/01 17:35:01 by vducoulo          #+#    #+#             */
/*   Updated: 2023/03/08 15:32:11 by vducoulo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CPP_IRC_HH
# define CPP_IRC_HH

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <poll.h>
#include <vector>
#include <map>

/*
	USER STATES DEFINES
*/

# define HANDSHAKING 	2
# define HANDSHAKED 	4
# define CONNECTED  	8
# define AWAY			16

/*
	IRC MESSAGES
*/

# define RPL_WELCOME(nick) "001 " + nick + " :Welcome " + nick + " to the irc network"
#endif