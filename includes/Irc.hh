/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Irc.hh                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vducoulo <vducoulo@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/01 17:35:01 by vducoulo          #+#    #+#             */
/*   Updated: 2023/03/21 23:26:28 by vducoulo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CPP_IRC_HH
# define CPP_IRC_HH

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <poll.h>
#include <vector>
#include <climits>
#include <cstring>
#include <cstdlib>
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

# define RPL_WELCOME(nick) "001 " + nick + " :Welcome " + nick + " to the irc network"
# define RPL_NAMREPLY(nick, chan_name, chan_users_nicks) "353 " + nick + " = " + chan_name + " :" + chan_users_nicks
# define RPL_ENDOFNAMES(nick, chan_name)	"366 " + nick + " " + chan_name + " :End of /NAMES list."
# define RPL_JOIN(sender_prefix, chan_name) ":" + sender_prefix + " JOIN :" + chan_name
# define RPL_PING(host) " PONG :" + host
# define RPL_PRIVMSG(sender_prefix, send_to, message) ":" + sender_prefix + " PRIVMSG " + send_to + " :" + message
# define RPL_PART(sender_prefix, chan_name) ":" + sender_prefix + " PART :" + chan_name
# define RPL_QUIT(sender_prefix) ":" + sender_prefix + " QUIT :Quit: "
# define RPL_NICK(newnick) " NICK " + newnick
# define RPL_YOUREOPER(nick) "381 " + nick + " :You are now an IRC operator"
# define RPL_MODE(sender_prefix, chan_name, new_mode, parameter)	":" + sender_prefix + " MODE " + chan_name + " " + new_mode + " " + parameter
/*
	IRC ERRORS
*/

# define ERR_NEEDMOREPARAMS(nick, cmd_name) "461 " + nick + " " + cmd_name + " :Not enough parameters"
# define ERR_ALREADYREGISTERED(nick) "462 " + nick + " :You may not reregister"
# define ERR_NOTREGISTERED(nick) "451 " + nick + " :You have not registered"
# define ERR_TOOMANYCHANNELS(nick, chan_name) "405 " + nick + " " + chan_name + " :You have joined too many channels"
# define ERR_CHANOPRIVSNEEDED(nick, chan_name)	"482 " + nick + " " + chan_name + " :You're not channel operator"
# define ERR_BADCHANNELKEY(nick, chan_name) "475 " + nick + " " + chan_name + " :Cannot join channel (+k)"
# define ERR_NOSUCHCHANNEL(nick, chan_name)	"403 " + nick + " " + chan_name + " :No such channel"
# define ERR_NOSUCHNICK(nick, send_to) "401 " + nick + " " + send_to + " :No such nick/channel"
# define ERR_NICKNAMEINUSE(nick, targetnick) "422 " + nick + " " + targetnick + " :Nickname is already in use." 
# define ERR_USERNOTINCHANNEL(nick, targetnick, chan_name) "441 " + nick + " " + targetnick + " " + chan_name + " :They aren't on that channel"
# define ERR_UNKNOWNMODE(nick, mode) "472 " + nick + " " + mode + " :is unknown mode char to me"
# define ERR_BANNEDFROMCHAN(nick, chan_name) "474 " + nick + " " + chan_name + " :you're banned from this server"
# define ERR_CHANNELISFULL(nick, chan_name) "471 " + nick + " " + chan_name + " :Cannot join channel (+l)"
/*
	FT_IRC SPECIFIC ERRORS
*/

# define FTIRC_ERR_BADCHANNELNAME(nick, chan_name) "403 " + nick + " " + chan_name + " :No such channel : chanel name must start with #"
#endif