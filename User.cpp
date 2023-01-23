/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbony <rbony@corobizar.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 22:22:39 by rbony             #+#    #+#             */
/*   Updated: 2023/01/18 13:23:05 by rbony            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

User::User(int sockfd, const std::string &host, std::string &servername) : sockfd(sockfd), hostname(host), servername(servername), registrationTime(time(0)), flags(RECEIVENOTICE)
{}

User::~User()
{}

int		User::getSockfd() const
{
	return sockfd;
}

const std::string	&User::getUsername() const
{
	return username;
}

const time_t	&User::getTimeOfLastMessage() const
{
	return timeOfLastMessage;
}

const time_t	&User::getTimeAfterPing() const
{
	return timeAfterPing;
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

void	User::setStatus(unsigned char flag)
{
	status = flag;
}

unsigned char	User::getStatus() const
{
	return status;
}

void	User::updateTimeOfLastMessage()
{
	this->timeOfLastMessage = time(0);
}

void	User::updateTimeAfterPing()
{
	this->timeAfterPing = time(0);
}