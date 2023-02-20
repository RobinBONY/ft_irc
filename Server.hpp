/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vducoulo <vducoulo@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 20:39:19 by rbony             #+#    #+#             */
/*   Updated: 2023/02/20 12:36:56 by vducoulo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

class User;
class Channel;

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <errno.h>
#include <fcntl.h>
#include <fstream>
#include <vector>
#include <poll.h>
#include <string>

#ifndef SERVER_HPP
#define SERVER_HPP

#include "User.hpp"
#include "Channel.hpp"

#define MAXINACTIVETIMEOUT	120
#define MAXRESPONSETIMEOUT	60

#ifdef __APPLE__
#define IRC_NOSIGNAL SO_NOSIGPIPE
#else
#define IRC_NOSIGNAL MSG_NOSIGNAL
#endif

class Server
{

private:
    std::string                         _name;
    int			                        _port;
	const id_t	                        _timeout;
    std::string	                        _password;
    int			                        _sockfd;
	sockaddr_in	                        _sockaddr;
    std::vector<struct pollfd>          _userFDs;
    std::vector<User *>                 _connectedUsers;
    std::map<std::string, Channel *>    _channels;
    int                                 _debug;

    Server();
    Server(const Server &form);
    Server &operator=(const Server &form);

    void		notifyUsers(User &user, const std::string &notification);
    
public:
    Server(int port, const std::string &password);
    ~Server();

	const int	&getSockfd() const;

	void		createSocket();
	void		bindSocket();
	void		listenSocket();
    void        grabConnection();
    void        checkConnectionWithUsers();
    void        deleteBrokenConnections();
};

std::ostream &operator<<(std::ostream &os, const Server &f);

#endif