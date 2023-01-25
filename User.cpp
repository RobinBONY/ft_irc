/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbony <rbony@corobizar.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 22:22:39 by rbony             #+#    #+#             */
/*   Updated: 2023/01/25 14:26:11 by rbony            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"

User::User(int sockfd, const std::string &host, std::string &servername) : sockfd(sockfd), hostname(host), servername(servername), registrationTime(time(0)), status(RECEIVENOTICE)
{}

User::~User()
{}

int		User::getSockfd() const
{
	return this->sockfd;
}

const std::string	&User::getUsername() const
{
	return this->username;
}

const std::vector<const Channel *>	&User::getChannels() const
{
	return channels;
}

const time_t	&User::getTimeOfLastMessage() const
{
	return this->timeOfLastMessage;
}

const time_t	&User::getTimeAfterPing() const
{
	return this->timeAfterPing;
}

void	User::sendMessage(const std::string &msg) const
{
	if (msg.size() > 0)
		send(sockfd, msg.c_str(), msg.size(), IRC_NOSIGNAL);
}

void	User::setUsername(const std::string &username)
{
	this->username = username;
}

void	User::setStatus(unsigned int status)
{
	this->status = status;
}

unsigned int	User::getStatus() const
{
	return this->status;
}

void	User::updateTimeOfLastMessage()
{
	this->timeOfLastMessage = time(0);
}

void	User::updateTimeAfterPing()
{
	this->timeAfterPing = time(0);
}