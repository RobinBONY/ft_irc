/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbony <rbony@corobizar.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 21:01:34 by rbony             #+#    #+#             */
/*   Updated: 2023/01/25 14:35:02 by rbony            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(int port, const std::string &password) : name("ircserv"), port(port), timeout(1), password(password)
{}

Server::~Server()
{
	close(sockfd);
}

const int	&Server::getSockfd() const
{
	return (sockfd);
}

void	Server::createSocket()
{
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
	{
		std::cout << "Failed to create socket. errno: " << errno << std::endl;
		exit(EXIT_FAILURE);
	}
	printf("create");
}

void	Server::bindSocket()
{
	const int trueFlag = 1;
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &trueFlag, sizeof(int)) < 0)
	{
		std::cout << "setsockopt failed" << std::endl;
		exit(EXIT_FAILURE);
	}
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_addr.s_addr = INADDR_ANY;
	sockaddr.sin_port = htons(port);
	if (bind(sockfd, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) < 0)
	{
		std::cout << "Failed to bind to port " << port << ". errno: " << errno << std::endl;
		exit(EXIT_FAILURE);
	}
	printf("bind");
}

void	Server::listenSocket()
{
	if (listen(sockfd, 128) < 0)
	{
		std::cout << "Failed to listen on socket. errno: " << errno << std::endl;
		exit(EXIT_FAILURE);
	}
	fcntl(sockfd, F_SETFL, O_NONBLOCK);
	printf("listen");
}

void	Server::grabConnection()
{
	//std::cout<< connectedUsers.size() << std::endl;

	size_t addrlen = sizeof(sockaddr);
	int connection = accept(sockfd, (struct sockaddr*)&sockaddr, (socklen_t*)&addrlen);
	if (connection >= 0)
	{
		char	host[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &(sockaddr.sin_addr), host, INET_ADDRSTRLEN);
		struct pollfd	pfd;
		pfd.fd = connection;
		pfd.events = POLLIN;
		pfd.revents = 0;
		userFDs.push_back(pfd);
		connectedUsers.push_back(new User(connection, host, name));
		printf("grab");
	}
}

void	Server::checkConnectionWithUsers()
{
	for (size_t i = 0; i < connectedUsers.size(); i++)
	{
		if (this->connectedUsers[i]->getStatus() & REGISTERED)
		{
			if (time(0) - this->connectedUsers[i]->getTimeOfLastMessage() > static_cast<time_t>(MAXINACTIVETIMEOUT) )
			{
				this->connectedUsers[i]->sendMessage(":" + this->name + " PING :" + this->name + "\n");
				this->connectedUsers[i]->updateTimeAfterPing();
				this->connectedUsers[i]->updateTimeOfLastMessage();
				this->connectedUsers[i]->setStatus(PINGING);
			}
			if ((connectedUsers[i]->getStatus() & PINGING) && time(0) - connectedUsers[i]->getTimeAfterPing() > static_cast<time_t>(MAXRESPONSETIMEOUT))
				connectedUsers[i]->setStatus(BREAKCONNECTION);
		}
	}
}

void	Server::deleteBrokenConnections()
{
	for (size_t i = 0; i < connectedUsers.size(); ++i)
	{
		if (connectedUsers[i]->getStatus() & BREAKCONNECTION)
		{
			notifyUsers(*(connectedUsers[i]), ":" + connectedUsers[i]->getUsername() + " QUIT : Fin\n");
			close(connectedUsers[i]->getSockfd());
			std::map<std::string, Channel *>::iterator	beg = channels.begin();
			std::map<std::string, Channel *>::iterator	end = channels.end();
			for (; beg != end; ++beg)
				if ((*beg).second->containsNickname(connectedUsers[i]->getUsername()))
					(*beg).second->disconnect(*(connectedUsers[i]));
			delete connectedUsers[i];
			connectedUsers.erase(connectedUsers.begin() + i);
			userFDs.erase(userFDs.begin() + i);
			--i;
			printf("erase");
		}
	}
}

void	Server::notifyUsers(User &user, const std::string &notification)
{
	const std::vector<const Channel *> chans = user.getChannels();
	for (size_t i = 0; i < connectedUsers.size(); i++)
	{
		for (size_t j = 0; j < chans.size(); j++)
		{
			if (chans[j]->containsNickname(connectedUsers[i]->getUsername()))
			{
				connectedUsers[i]->sendMessage(notification);
				break ;
			}
		}
	}
	printf("notify");
}
