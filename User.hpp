/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbony <rbony@corobizar.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 22:22:37 by rbony             #+#    #+#             */
/*   Updated: 2023/01/25 13:34:33 by rbony            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

class Server;
class Channel;

#include <string>
#include <time.h>
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <vector>
#include <queue>
#include <set>
#include <sys/socket.h>
#include <algorithm>
#include <fcntl.h>

#ifndef USER_HPP
#define USER_HPP

#include "Server.hpp"
#include "Channel.hpp"

#define REGISTERED		1
#define INVISIBLE		2
#define RECEIVENOTICE	4
#define RECEIVEWALLOPS	8
#define IRCOPERATOR		16
#define AWAY			32
#define PINGING			64
#define BREAKCONNECTION	128

class User
{
private:
	int								sockfd;
	std::string						username;
	std::string						hostname;
	const std::string				&servername;
	time_t							registrationTime;
	time_t							timeOfLastMessage;
	time_t							timeAfterPing;
	unsigned int					status;
	std::vector<const Channel *>	channels;

    User();
	User(const User& copy);
	User	&operator=(const User& other);
	
public:
    User(int sockfd, const std::string &host, std::string &servername);
	~User();

	const time_t						&getTimeOfLastMessage() const;
	const time_t						&getTimeAfterPing() const;
	const std::string					&getUsername() const;
	int									getSockfd() const;
	unsigned int						getStatus() const;
	const std::vector<const Channel *>	&getChannels() const;

	void								setUsername(const std::string &username);
	void								setStatus(unsigned int status);

	void								sendMessage(const std::string &msg) const;
	void								updateTimeOfLastMessage();
	void								updateTimeAfterPing();
	void								removeChannel(const std::string &name);
	void								addChannel(const Channel &channel);
};

std::ostream &operator<<(std::ostream &os, const User &f);

#endif