/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbony <rbony@corobizar.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/01 16:13:58 by vducoulo          #+#    #+#             */
/*   Updated: 2023/05/21 18:29:56 by rbony            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "includes/irc.hh"
#include "srcs/server/server.hpp"
bool g_isactive = true;

void sig_handler(int signum)
{
	g_isactive = false;
	std::cout << signum << "called" << std::endl;
}

int main(int argc, char** argv)
{
	if (argc == 3)
	{
		try {
			
			struct sigaction sig;
			memset(&sig, '\0', sizeof(sig));
			sig.sa_handler = sig_handler;
			sig.sa_flags = 0;
			sigfillset(&sig.sa_mask);
			sigaction(SIGINT, &sig, NULL);
			
			Server server(argv[1], argv[2]);
			while (true)
			{
				server.runLoop();
				if (!g_isactive)
					exit(EXIT_SUCCESS);
			}
		}
		catch (const std::exception &e)
		{
			std::cerr << "[ERROR] " << e.what() << std::endl;
		}
	}
	else
		std::cout << "Usage : ./ircserv [port] [password]" << std::endl;
	return (0);
	
}