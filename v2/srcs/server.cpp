/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vducoulo <vducoulo@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/01 17:42:01 by vducoulo          #+#    #+#             */
/*   Updated: 2023/03/16 12:38:48 by vducoulo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/Irc.hh"
# include "server.hpp"
# include "command.hpp"

Server::Server(char *port, char *pass)
: _active(1), _password(pass)
{
	if (atoi(port) < 1024 || atoi(port) > 49151)
		throw std::runtime_error("Bad port range");

	pollfd servpfd;

	servpfd.events = POLLIN;
	servpfd.fd = this->setSocketFd(atoi(port));
		
	_pfds.push_back(servpfd);
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

Channel *Server::createChannel(std::string name, std::string pass)
{
	Channel channel(name, pass);
	_channels.push_back(channel);
	
	return &_channels.back();
}

void Server::userHandShake(void)
{
	size_t addr_len = sizeof(_server_sockaddr);

	int NewConnection = accept(_pfds[0].fd, (struct sockaddr *)&_server_sockaddr, (socklen_t*)&addr_len);
	if (NewConnection >= 0)
	{
		User *new_user = new User(NewConnection);
		pollfd 	user_pfd;
	
		user_pfd.fd = NewConnection;
		user_pfd.revents = 0;
		user_pfd.events = POLLIN;
		_pfds.push_back(user_pfd);
		_users.push_back(new_user);

		if (DEBUG)
			std::cerr << "[DEBUG] " << "new user just entered HANDSHAKE mode" << std::endl;
	}
}

User *Server::getRelativeUser(int fd)
{
	std::vector<User *>::iterator iter;

	for (iter = _users.begin(); iter != _users.end(); iter++)
	{
		if ((*iter)->getFd() == fd)
			return *iter;
	}
	throw std::runtime_error("No such user"); // to change
}

User *Server::getReltiveUserPerNick(std::string usrnick)
{
	std::vector<User *>::iterator iter;

	for (iter = _users.begin(); iter != _users.end(); iter++)
	{
		if ((*iter)->getNickName() == usrnick)
			return *iter;
	}
	throw std::runtime_error("No such user"); // to change
}

Channel *Server::getSetRelativeChannel(std::string name, std::string pass)
{
	std::vector<Channel>::iterator iter;
	
	for (iter = _channels.begin(); iter != _channels.end(); iter++)
	{
		if ((*iter).getName() == name)
			return iter.base();
	}
	return createChannel(name, pass);
}

void Server::deleteChannel(std::string chan_name)
{
	std::vector<Channel>::iterator iter;
	
	for (iter = _channels.begin(); iter != _channels.end(); iter++)
	{
		if ((*iter).getName() == chan_name)
		{
			_channels.erase(iter);
			return;
		}
	}
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

void Server::receiveMsgs(int fd)
{
	char 						msgbuff[513];
	size_t						last_breaker_pos = 0;
	std::string 				raw_message;
	std::vector<std::string> 	raw_msgs;
	
	while (raw_message.find("\r\n") == std::string::npos)
	{
		bzero(msgbuff, 513);
		size_t MsgLen = recv(fd, &msgbuff, 512, 0);
		if (MsgLen >= 0)
		{
			msgbuff[MsgLen + 1] = '\0';
			raw_message.append(msgbuff);
		}
		else
			throw::std::runtime_error("Can't read buffer !");
	}
	
	while (last_breaker_pos != std::string::npos)
	{
		std::string cmd_str;
		size_t		tmp_breaker_pos = 0;

		std::cerr << std::endl;
		tmp_breaker_pos = raw_message.find("\n", last_breaker_pos + 1);
		if (tmp_breaker_pos < std::string::npos)
		{
				cmd_str = raw_message.substr(last_breaker_pos, tmp_breaker_pos - 1);
				cmd_str = cmd_str.substr(0, cmd_str.find("\r\n"));
				raw_msgs.push_back(cmd_str);
				tmp_breaker_pos ++;
				if (DEBUG)
					std::cerr << "<- [DEBUG] " << "fd " << fd << " received " << cmd_str << "\"" << std::endl;
		}
		last_breaker_pos = tmp_breaker_pos;
	}

	getRelativeUser(fd)->setMsgs(raw_msgs);
}

void Server::executeMsgs(int fd)
{
	User *relative_user = getRelativeUser(fd);
	std::vector<std::string> msgs = relative_user->getMsgs();
	std::vector<std::string>::iterator iter;
	
	for (iter = msgs.begin(); iter != msgs.end(); iter++)
	{
		std::string command((*iter).substr(0, (*iter).find(" ")));
		std::vector<std::string> parameters = getSplittedParams((*iter).substr(command.length(), (*iter).length()));

		Command new_command(command, parameters, relative_user, this);
		new_command.execute();
	}
}

void Server::runLoop(void)
{	
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
			{
				receiveMsgs((*iter).fd);
				executeMsgs((*iter).fd);
			}
			else if ((*iter).revents == POLLHUP)
				std::cout << "user " << _users.at((*iter).fd)->getNickName() << " is away, need to disconnect him " << std::endl;
		}
	}
}