/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbony <rbony@corobizar.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/03 17:33:53 by vducoulo          #+#    #+#             */
/*   Updated: 2023/05/21 17:42:15 by rbony            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

# include "user.hpp"

User::User(int user_fd)
: _fd(user_fd), _hostname("localhost"), _current_channel(NULL)
{
	std::stringstream ss("");
	std::string fd_str;

	ss << user_fd;
	fd_str = ss.str();
	_nickname = std::string("ft_user");
	_nickname += fd_str;
}

std::string User::getSenderPrefix() const
{
	std::string sender_prefix;

	if (!(_username.empty()))
		sender_prefix = _nickname + "!" + _username + "@" + _hostname;
	else
		sender_prefix = _nickname + "@" + _hostname;
	
	return sender_prefix;
}

void User::push(std::string msg, bool raw)
{
	std::string message;
	if (!raw)
		message =":" + getSenderPrefix() + " " + msg + "\r\n";
	else
		message = msg + "\r\n";
	
	send (_fd , message.c_str(), message.length(), 0);
	if (DEBUG)
		std::cerr << "->    [DEBUG] " << "server answered with =\"" << message.substr(0, message.length() - 2) << "\"" << std::endl;
}

void User::welcomeToIrc(void)
{
	push(RPL_WELCOME(_nickname));
	
	_state = CONNECTED;	
}