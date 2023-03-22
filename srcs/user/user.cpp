/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vducoulo <vducoulo@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/03 17:33:53 by vducoulo          #+#    #+#             */
/*   Updated: 2023/03/22 12:11:37 by vducoulo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "user.hpp"

User::User(int user_fd)
: _fd(user_fd), _hostname("localhost"), _current_channel(nullptr), _nickname("ft_user" + std::to_string(user_fd))
{
	
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
		std::cerr << "->    [DEBUG] " << "server answered with =" << message.substr(0, message.length() - 2) << std::endl;
}

void User::welcomeToIrc(void)
{
	push(RPL_WELCOME(_nickname));
	
	_state = CONNECTED;	
}