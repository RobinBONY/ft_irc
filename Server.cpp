/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vducoulo <vducoulo@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 21:01:34 by rbony             #+#    #+#             */
/*   Updated: 2023/02/20 12:16:02 by vducoulo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(int port, const std::string &password) : _name("ircserv"), _port(port), _timeout(1), _password(password)
{}

Server::~Server()
{
	close(this->_sockfd);
}

const int	&Server::getSockfd() const
{
	return (_sockfd);
}

void	Server::createSocket()
{
	this->_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_sockfd == -1)
	{
		std::cout << "Failed to create socket. errno: " << errno << std::endl;
		exit(EXIT_FAILURE);
	}
	printf("create");
}

void	Server::bindSocket()
{
	const int trueFlag = 1;
	if (setsockopt(this->_sockfd, SOL_SOCKET, SO_REUSEADDR, &trueFlag, sizeof(int)) < 0)
	{
		std::cout << "setsockopt failed" << std::endl;
		exit(EXIT_FAILURE);
	}
	this->_sockaddr.sin_addr.s_addr = INADDR_ANY;
	this->_sockaddr.sin_family = AF_INET;
	this->_sockaddr.sin_port = htons(this->_port);
	if (bind(this->_sockfd, (struct sockaddr*)&this->_sockaddr, sizeof(this->_sockaddr)) < 0)
	{
		std::cout << "Failed to bind to port " << this->_port << ". errno: " << errno << std::endl;
		exit(EXIT_FAILURE);
	}
	printf("bind");
}

void	Server::listenSocket()
{
	if (listen(this->_sockfd, 128) < 0)
	{
		std::cout << "Failed to listen on socket. errno: " << errno << std::endl;
		exit(EXIT_FAILURE);
	}
	fcntl(this->_sockfd, F_SETFL, O_NONBLOCK);
	printf("listen");
}

void	Server::grabConnection()
{
	//std::cout<< connectedUsers.size() << std::endl;

	size_t addrlen = sizeof(this->_sockaddr);
	int connection = accept(this->_sockfd, (struct sockaddr*)&this->_sockaddr, (socklen_t*)&addrlen);
	if (connection >= 0)
	{
		char	host[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &(this->_sockaddr.sin_addr), host, INET_ADDRSTRLEN);
		struct pollfd	pfd;
		pfd.fd = connection;
		pfd.events = POLLIN;
		pfd.revents = 0;
		this->_userFDs.push_back(pfd);
		this->_connectedUsers.push_back(new User(connection, host, this->_name));
		printf("grab");
	}
}

void	Server::checkConnectionWithUsers()
{
	for (size_t i = 0; i < this->_connectedUsers.size(); i++)
	{
		if (this->_connectedUsers[i]->getStatus() & REGISTERED)
		{
			if (time(0) - this->_connectedUsers[i]->getTimeOfLastMessage() > static_cast<time_t>(MAXINACTIVETIMEOUT) )
			{
				this->_connectedUsers[i]->sendMessage(":" + this->_name + " PING :" + this->_name + "\n");
				this->_connectedUsers[i]->updateTimeAfterPing();
				this->_connectedUsers[i]->updateTimeOfLastMessage();
				this->_connectedUsers[i]->setStatus(PINGING);
			}
			if ((this->_connectedUsers[i]->getStatus() & PINGING) && time(0) - this->_connectedUsers[i]->getTimeAfterPing() > static_cast<time_t>(MAXRESPONSETIMEOUT))
				this->_connectedUsers[i]->setStatus(BREAKCONNECTION);
		}
	}
}

void	Server::deleteBrokenConnections()
{
	for (size_t i = 0; i < this->_connectedUsers.size(); ++i)
	{
		if (this->_connectedUsers[i]->getStatus() & BREAKCONNECTION)
		{
			notifyUsers(*(this->_connectedUsers[i]), ":" + this->_connectedUsers[i]->getUsername() + " QUIT : Fin\n");
			close(this->_connectedUsers[i]->getSockfd());
			std::map<std::string, Channel *>::iterator	beg = this->_channels.begin();
			std::map<std::string, Channel *>::iterator	end = this->_channels.end();
			for (; beg != end; ++beg)
				if ((*beg).second->containsNickname(this->_connectedUsers[i]->getUsername()))
					(*beg).second->disconnect(*(this->_connectedUsers[i]));
			delete this->_connectedUsers[i];
			this->_connectedUsers.erase(this->_connectedUsers.begin() + i);
			this->_userFDs.erase(this->_userFDs.begin() + i);
			--i;
			printf("erase");
		}
	}
}

void	Server::notifyUsers(User &user, const std::string &notification)
{
	const std::vector<const Channel *> chans = user.getChannels();
	for (size_t i = 0; i < this->_connectedUsers.size(); i++)
	{
		for (size_t j = 0; j < chans.size(); j++)
		{
			if (chans[j]->containsNickname(this->_connectedUsers[i]->getUsername()))
			{
				this->_connectedUsers[i]->sendMessage(notification);
				break ;
			}
		}
	}
	printf("notify");
}
