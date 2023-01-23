/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbony <rbony@corobizar.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 22:22:37 by rbony             #+#    #+#             */
/*   Updated: 2023/01/18 13:19:10 by rbony            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

class Server;

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
	int					sockfd;
	std::string			hostname;
	const std::string	&servername;
	time_t				registrationTime;
	time_t				timeOfLastMessage;
	time_t				timeAfterPing;
	char				status;

    User();
	User(const User& copy);
	User	&operator=(const User& other);
	
public:
    User(int sockfd, const std::string &host, std::string &servername);
	~User();

	const time_t		&getTimeOfLastMessage() const;
	const time_t		&getTimeAfterPing() const;
	const std::string	&getUsername() const;
	int					getSockfd() const;
	char				getStatus() const;

	void				setUsername(const std::string &username);
	void				setStatus(char flag);

	void				sendMessage(const std::string &msg) const;
	void				updateTimeOfLastMessage();
	void				updateTimeAfterPing();
};

std::ostream &operator<<(std::ostream &os, const User &f);

#endif