/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vducoulo <vducoulo@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 04:14:22 by rbony             #+#    #+#             */
/*   Updated: 2023/02/20 12:43:12 by vducoulo         ###   ########.fr       */
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

	signal(SIGINT, sigHandler); //listening for kill command 
	//?(tous les signaux écoutés ?)

	while (run)
	{
		server.grabConnection();
		server.checkConnectionWithUsers();
		server.deleteBrokenConnections();
	}
}