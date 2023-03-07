/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vducoulo <vducoulo@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/01 17:42:01 by vducoulo          #+#    #+#             */
/*   Updated: 2023/03/07 15:10:15 by vducoulo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Irc.hh"
#include "server.hpp"

Server::Server(char *port, char *pass)
: _active(1), _password(pass), _debug(1)
{
	if (atoi(port) < 1024 || atoi(port) > 49151)
		throw std::runtime_error("Bad port range");

	pollfd servpfd;

	servpfd.events = POLLIN;
	servpfd.fd = this->setSocketFd(atoi(port));

	_commands_array.push_back(new cmdUser("USER", false));
	
	_pfds.push_back(servpfd);
}

void Server::commandHandler(std::string command, std::vector<std::string> parameters, User relative_user)
{
	for (std::vector<Command *>::iterator iter = _commands_array.begin(); iter != _commands_array.end(); iter++)
	{
		if ((*iter)->getName() == command)
			(*iter)->launch(parameters, relative_user);
	}
}

int Server::setSocketFd(int port)
{
	int 				sock_fd;
	const int 			flag = 1;
	struct sockaddr_in 	serv_sock_addr;

	serv_sock_addr.sin_addr.s_addr	 = INADDR_ANY;
	serv_sock_addr.sin_family		 = AF_INET;
	serv_sock_addr.sin_port 		 = htons(port);
	
	sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (sock_fd < 0)
		throw std::runtime_error("Can't create socket");
	if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(int)) < 0)
		throw std::runtime_error("Can't set socket options");
	if (bind(sock_fd, (struct sockaddr*)&serv_sock_addr, sizeof(serv_sock_addr)) < 0)
		throw std::runtime_error("Can't bind socket");
	if (fcntl(sock_fd, F_SETFL, O_NONBLOCK) < 0)
		throw std::runtime_error("Can't set socket non-block attribute");
	if (listen(sock_fd, 1) < 0 )
		throw std::runtime_error("Can't listen to socket");
	
	this->_server_sockaddr = serv_sock_addr;

	std::cout << "[INFO] server up and listening on " << _server_sockaddr.sin_addr.s_addr << ":" << port << std::endl;
	return (sock_fd);
}

void Server::userHandShake(void)
{
	size_t addr_len = sizeof(_server_sockaddr);

	int NewConnection = accept(_pfds[0].fd, (struct sockaddr *)&_server_sockaddr, (socklen_t*)&addr_len);
	if (NewConnection >= 0)
	{
		User	new_user(NewConnection);
		pollfd 	user_pfd;
	
		user_pfd.fd = NewConnection;
		user_pfd.revents = 0;
		user_pfd.events = POLLIN;
		_pfds.push_back(user_pfd);
		_users.push_back(new_user);

		if (_debug)
			std::cerr << "[DEBUG] " << "new user just entered HANDSHAKE mode" << std::endl;
	}
}

User Server::getRelativeUser(int fd)
{
	std::vector<User>::iterator iter;

	for (iter = _users.begin(); iter != _users.end(); iter++)
	{
		if ((*iter).getFd() == fd)
			return *iter;
	}
	return NULL;
}

std::vector<std::string> getSplittedParams(std::string hay)
{
	std::string 				needle(" ");
	size_t						needle_position = 0;
	std::vector<std::string> 	res;
	
	while (needle_position != std::string::npos)
	{
		size_t tmp_needle = hay.find(needle, needle_position + 1);
		res.push_back(hay.substr(needle_position + 1, tmp_needle - needle_position - 1));
		needle_position = tmp_needle;
	}
	return res;
}

void Server::receiveMsg(int fd)
{
	char 			msgbuff[513];
	int				pos;

	User relative_user = getRelativeUser(fd);
	
	size_t MsgLen = recv(fd, &msgbuff, 512, 0);
	msgbuff[512] = 0;

	std::string 	raw_message(msgbuff);
	raw_message.substr(0, raw_message.find("\r\n"));
	
	std::string command(raw_message.substr(0, raw_message.find(" ")));
	raw_message.erase(0, command.length());
	
	std::vector<std::string> parameters = getSplittedParams(raw_message);
	
	commandHandler(command, parameters, relative_user);
}

void Server::runLoop(void)
{
	//signal(SIGINT, Server::onSIGINT);
	
	while (_active)
	{
		if(poll(&_pfds[0], _pfds.size(), 100) < 0)
			throw std::runtime_error("Can't poll");

		if (_pfds[0].revents == POLLIN)
		{
			userHandShake();
			continue ;
		}
		for (std::vector<pollfd>::iterator iter = _pfds.begin(); iter != _pfds.end(); iter++)
		{
			if ((*iter).revents == POLLIN)
				receiveMsg((*iter).fd);
			else if ((*iter).revents == POLLHUP)
				std::cout << "user " << _users.at((*iter).fd).getNickName() << " is away, need to disconnect him " << std::endl;
		}
	}
}