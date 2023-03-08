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

# include "command.hpp"
# include "user.hpp"


Command::Command(std::string name, std::vector<std::string> params, User relativeuser, Server relativeserver)
: _name(name), _parameters(params), _relative_user(relativeuser), _relative_server(relativeserver)
{
	_cmd_ptr["USER"] = &Command::cmdUser;
	_cmd_ptr["CAP"] = &Command::cmdCap;
	_cmd_ptr["PASS"] = &Command::cmdPass;
	_cmd_ptr["NICK"] = &Command::cmdNick;
	_cmd_ptr["JOIN"] = &Command::cmdJoin;
	
	try
	{
		_cmd_ptr.at(_name);
	}
	catch(const std::out_of_range& e)
	{
		_cmd_ptr[_name] = &Command::unknowCommand;
	}
}

Command::~Command()
{}

void Command::execute()
{
	try
	{
		(this->*_cmd_ptr[_name])();
	}
	catch(const std::out_of_range& e)
	{
		std::cerr << e.what() << '\n';
	}
}
void Command::cmdUser()
{
	_relative_user.setUserkName(_parameters.front());
	_relative_user.setRealName(_parameters.back());
	if (_relative_user.getState() == HANDSHAKED || _relative_server.getPassword().empty())
		_relative_user.welcomeToIrc();
}

void Command::cmdCap()
{
	/*
		Not in use
	*/
}

void Command::cmdPass()
{
	if (_relative_server.getPassword() == _parameters.front() || _relative_server.getPassword().empty())
	{
		_relative_user.setState(HANDSHAKED);
	}
	else
		std::cerr << "[INFO] User pass (" << _parameters.front() <<  ") doesn't match server pass (" << _relative_server.getPassword() << ")" << std::endl;
}

void Command::cmdNick()
{
	_relative_user.setNickName(_parameters[0]);
}

void Command::cmdJoin()
{

}

void Command::unknowCommand()
{
	std::cerr << "unknow command" << std::endl;
}