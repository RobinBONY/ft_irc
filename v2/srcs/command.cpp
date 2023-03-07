/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vducoulo <vducoulo@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/07 13:49:08 by vducoulo          #+#    #+#             */
/*   Updated: 2023/03/07 14:00:53 by vducoulo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command.hpp"


Command::Command(std::string name, std::vector<std::string> params)
: _name(name), _parameters(params)
{
	_cmd_ptr["USER"] = &Command::cmdUser;
	_cmd_ptr["CAP"] = &Command::cmdCap;
	_cmd_ptr["PASS"] = &Command::cmdPass;
	_cmd_ptr["NICK"] = &Command::cmdNick;
}

Command::~Command()
{
	
}

void Command::cmdUser()
{
		std::cout << "new cmd USER called" << std::endl;
}

void Command::cmdCap()
{
		std::cout << "new cmd CAP called" << std::endl;
}

void Command::cmdPass()
{
		std::cout << "new cmd PASS called" << std::endl;
}

void Command::cmdNick()
{
		std::cout << "new cmd Nick called" << std::endl;
}