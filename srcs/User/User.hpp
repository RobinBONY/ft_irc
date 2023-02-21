/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vducoulo <vducoulo@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 22:22:37 by rbony             #+#    #+#             */
/*   Updated: 2023/02/21 21:11:46 by vducoulo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

class Server;
class Channel;

#ifndef USER_HPP
#define USER_HPP

#include "../../includes/Irc.hh"

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
	int								_sockfd; //socket personnel de l'user
	std::string						_username;
	std::string						_hostname; //ip serveur en human readable ex : 127.0.0.1
	const std::string				&_servername; //nom du serveur ex : foo
	time_t							_registrationTime;
	time_t							_timeOfLastMessage;
	time_t							_timeAfterPing;
	unsigned int					_status; //les defines au-dessus
	std::vector<const Channel *>	_channels;

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