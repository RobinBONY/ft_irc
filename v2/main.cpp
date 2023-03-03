/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vducoulo <vducoulo@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/01 16:13:58 by vducoulo          #+#    #+#             */
/*   Updated: 2023/03/01 17:48:15 by vducoulo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/Irc.hh"

int main(int argc, char** argv)
{
	if (argc == 3)
	{
		try {
			Server server(argv[1], argv[2]);
		}
		catch (const std::exception &e)
		{
			std::cerr << "[ERROR] " << e.what() << std::endl;
		}
	}
	
}