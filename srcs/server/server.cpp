/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbony <rbony@corobizar.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/01 17:42:01 by vducoulo          #+#    #+#             */
/*   Updated: 2023/05/21 17:44:13 by rbony            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

# include "server.hpp"
# include "../command/command.hpp"

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

Server::~Server()
{
	std::vector<User *>::iterator usr_iter;
	std::vector<Channel *>::iterator chan_iter;

	std::cerr << "DESTRUCTOR CALLED" << std::endl;
	for (usr_iter = _users.begin(); usr_iter != _users.end(); usr_iter++)
	{
		delete *usr_iter;
	}
	_users.clear();

	for (chan_iter = _channels.begin(); chan_iter != _channels.end(); chan_iter++)
	{
		delete *chan_iter;
	}
	_channels.clear();
}

int Server::setSocketFd(uint16_t port)
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
	Channel *channel = new Channel(name, pass);
	_channels.push_back(channel);
	
	return channel;
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
	throw std::runtime_error("No such user");
}

User *Server::getReltiveUserPerNick(std::string usrnick)
{
	std::vector<User *>::iterator iter;

	for (iter = _users.begin(); iter != _users.end(); iter++)
	{
		if ((*iter)->getNickName() == usrnick)
			return *iter;
	}
	throw std::runtime_error("No such user");
}

Channel *Server::getSetRelativeChannel(std::string name, std::string pass)
{
	std::vector<Channel *>::iterator iter;
	
	for (iter = _channels.begin(); iter != _channels.end(); iter++)
	{
		if ((*iter)->getName() == name)
			return *iter;
	}
	return createChannel(name, pass);
}

void Server::deleteChannel(std::string chan_name)
{
	std::vector<Channel *>::iterator iter;
	
	for (iter = _channels.begin(); iter != _channels.end(); iter++)
	{
		if ((*iter)->getName() == chan_name)
		{
			delete *iter;
			_channels.erase(iter);
			return;
		}
	}
}

void Server::deleteUser(User *usr)
{
	std::vector<User *>::iterator iter;
	std::vector<pollfd>::iterator pfd_iter;

	for (iter = _users.begin(); iter != _users.end(); iter++)
	{
		if (*iter && (*iter) == usr)
		{
			for (pfd_iter = _pfds.begin(); pfd_iter != _pfds.end(); pfd_iter++)
			{
				if ((*pfd_iter).fd == (*iter)->getFd())
				{
					(*pfd_iter).events = POLLHUP;
					break;
				}
			}
			delete *iter;
			_users.erase(iter);
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
	
	std::cerr << "trying to read from fd :" << fd << std::endl;
	while (raw_message.find("\r\n") == std::string::npos)
	{
		memset(msgbuff, '\0', 513);
		size_t MsgLen = recv(fd, &msgbuff, 512, 0);
		if (MsgLen > 0)
		{
			msgbuff[MsgLen + 1] = '\0';
			raw_message.append(msgbuff);
		}
		else
		{
			getRelativeUser(fd)->setMsgs(std::vector<std::string>());
			std::vector<pollfd>::iterator pfd_iter;
			for (pfd_iter = _pfds.begin(); pfd_iter != _pfds.end(); pfd_iter++)
			{
				if ((*pfd_iter).fd == fd)
				{
					(*pfd_iter).events = POLLHUP;
					(*pfd_iter).revents = POLLHUP;

					break;
				}
			}
			return;
			//throw::std::runtime_error("Can't read buffer !");
		}
	}
	
	while (last_breaker_pos != std::string::npos)
	{
		std::string cmd_str;
		size_t		tmp_breaker_pos = 0;

		tmp_breaker_pos = raw_message.find("\n", last_breaker_pos + 1);
		if (tmp_breaker_pos < std::string::npos)
		{
				cmd_str = raw_message.substr(last_breaker_pos, tmp_breaker_pos - 1);
				cmd_str = cmd_str.substr(0, cmd_str.find("\r\n"));
				raw_msgs.push_back(cmd_str);
				tmp_breaker_pos ++;
				if (DEBUG)
					std::cerr << "<- [DEBUG] " << "fd " << fd << " received \"" << cmd_str << "\"" << std::endl;
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
	if(poll(&_pfds[0], _pfds.size(), 100) < 0)
		throw std::runtime_error("Can't poll");

	if (_pfds[0].revents == POLLIN)
	{
		userHandShake();
		return;
	}
	for (std::vector<pollfd>::iterator iter = _pfds.begin(); iter != _pfds.end(); iter++)
	{
		if ((*iter).revents == POLLIN)
		{
			receiveMsgs((*iter).fd);
			executeMsgs((*iter).fd);
		}
		else if ((*iter).revents == POLLHUP)
		{
			_pfds.erase(iter);
			break;
		}
	}
}