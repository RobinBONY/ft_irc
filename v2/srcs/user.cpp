/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vducoulo <vducoulo@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/03 17:33:53 by vducoulo          #+#    #+#             */
/*   Updated: 2023/03/08 15:31:11 by vducoulo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/Irc.hh"
# include "user.hpp"

User::User(int user_fd)
: _fd(user_fd), _hostname("localhost")
{
	
}

void User::welcomeToIrc(void)
{
	std::string senderPrefix(":" + _nickname + "!" + _realname + "@" + _hostname + " ");
	std::string message(senderPrefix + RPL_WELCOME(_nickname) + "\r\n");
	send (_fd , message.c_str(), message.length(), 0);
	_state = CONNECTED;	
}