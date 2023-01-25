/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbony <rbony@corobizar.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 04:14:22 by rbony             #+#    #+#             */
/*   Updated: 2023/01/25 14:30:41 by rbony            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <fcntl.h>
#include "Server.hpp"
#include <csignal>

bool	run = true;

void	sigHandler(int signum)
{
	(void)signum;
	run = false;
}

int main(int argc, char **argv)
{
	if (argc < 3)
	{
		std::cout << "Usage: ./ircserv <port> <password>" << std::endl;
		exit(EXIT_FAILURE);
	}

	int port = atoi(argv[1]);

	if (port < 1024 || port > 49151)
	{
		std::cout << "Wrong port!" << std::endl;
		exit(EXIT_FAILURE);
	}
	
	Server	server(port, argv[2]);

	server.createSocket();
	server.bindSocket();
	server.listenSocket();

	signal(SIGINT, sigHandler);

	while (run)
	{
		server.grabConnection();
		
		server.checkConnectionWithUsers();

		server.deleteBrokenConnections();
	}
}