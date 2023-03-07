/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vducoulo <vducoulo@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/01 16:13:58 by vducoulo          #+#    #+#             */
/*   Updated: 2023/03/07 19:35:55 by vducoulo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/Irc.hh"
#include "srcs/server.hpp"

int main(int argc, char** argv)
{
	if (argc == 3)
	{
		try {
			Server server(argv[1], argv[2]);
			server.runLoop();
		}
		catch (const std::exception &e)
		{
			std::cerr << "[ERROR] " << e.what() << std::endl;
		}
	}
	
}