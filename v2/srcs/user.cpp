/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vducoulo <vducoulo@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/03 17:33:53 by vducoulo          #+#    #+#             */
/*   Updated: 2023/03/13 00:35:41 by vducoulo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/Irc.hh"
# include "user.hpp"

User::User(int user_fd)
: _fd(user_fd), _hostname("localhost"), _current_channel(nullptr), _nickname("default" + std::to_string(user_fd))
{
	
}

void User::push(std::string msg)
{
	std::string senderPrefix;
	if (!(_username.empty()))
		senderPrefix = ":" + _nickname + "!" + _username + "@" + _hostname + " ";
	else
		senderPrefix = ":" + _nickname + "@" + _hostname + " ";
	
	std::string message(senderPrefix + msg + "\r\n");
	send (_fd , message.c_str(), message.length(), 0);
	if (DEBUG)
		std::cerr << "->    [DEBUG] " << "server answered with =" << message.substr(0, message.length() - 2) << std::endl;
}

void User::welcomeToIrc(void)
{
	push(RPL_WELCOME(_nickname));
	
	_state = CONNECTED;	
}