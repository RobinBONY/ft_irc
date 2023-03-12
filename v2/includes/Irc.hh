/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Irc.hh                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vducoulo <vducoulo@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/01 17:35:01 by vducoulo          #+#    #+#             */
/*   Updated: 2023/03/12 18:36:52 by vducoulo         ###   ########.fr       */
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
	DEFINE FOR SERVER DEBUG
*/

# define DEBUG 1

/*
	IRC MESSAGES
*/

# define RPL_NICK ""
# define RPL_WELCOME(nick) "001 " + nick + " :Welcome " + nick + " to the irc network"
# define RPL_NAMREPLY(nick, chan, chan_users_nicks) "353 " + nick + " = " + chan + " :" + chan_users_nicks
# define RPL_ENDOFNAMES(nick, chan)	"366 " + nick + " " + chan + " :End of /NAMES list."
# define RPL_JOIN(chan) " JOIN :" + chan
# define RPL_PING(host) " PONG :" + host



/*
	IRC ERRORS
*/

# define ERR_NEEDMOREPARAMS(nick, cmd_name) "461 " + nick + " " + cmd_name + " :Not enough parameters"
# define ERR_ALREADYREGISTERED(nick) "462 " + nick + " :You may not reregister"
# define ERR_TOOMANYCHANNELS(nick, chan_name) "405 " + nick + " " + chan_name + " :You have joined too many channels"
# define ERR_CHANOPRIVSNEEDED(nick, chan_name)	"482 " + nick + " " + chan_name + " :You're not channel operator"

/*
	FT_IRC SPECIFIC ERRORS
*/

# define FTIRC_ERR_BADCHANNELNAME(nick, chan_name) "403 " + nick + " " + chan_name + " :No such channel : chanel name must start with #"
#endif