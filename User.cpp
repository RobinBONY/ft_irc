/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vducoulo <vducoulo@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 22:22:39 by rbony             #+#    #+#             */
/*   Updated: 2023/02/20 12:53:27 by vducoulo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"

User::User(int sockfd, const std::string &host, std::string &servername) 
: _sockfd(sockfd), _username("NOUSERNAME"), _hostname(host), _servername(servername), _registrationTime(time(0)),
_timeOfLastMessage(NULL), _timeAfterPing(NULL), _status(RECEIVENOTICE)
{}
//_channels uninitialized

User::~User()
{}

int		User::getSockfd() const
{
	return this->_sockfd;
}

const std::string	&User::getUsername() const
{
	return this->_username;
}

const std::vector<const Channel *>	&User::getChannels() const
{
	return this->_channels;
}

const time_t	&User::getTimeOfLastMessage() const
{
	return this->_timeOfLastMessage;
}

const time_t	&User::getTimeAfterPing() const
{
	return this->_timeAfterPing;
}

void	User::sendMessage(const std::string &msg) const
{
	if (msg.size() > 0)
		send(_sockfd, msg.c_str(), msg.size(), IRC_NOSIGNAL);
}

void	User::setUsername(const std::string &username)
{
	this->_username = username;
}

void	User::setStatus(unsigned int status)
{
	this->_status = status;
}

unsigned int	User::getStatus() const
{
	return this->_status;
}

void	User::updateTimeOfLastMessage()
{
	this->_timeOfLastMessage = time(0);
}

void	User::updateTimeAfterPing()
{
	this->_timeAfterPing = time(0);
}