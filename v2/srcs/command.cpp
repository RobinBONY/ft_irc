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


Command::Command(std::string name, std::vector<std::string> params, User &relativeuser, Server *relativeserver)
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
		_cmd_ptr[_name] = &Command::errUnknowCommand;
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
	if (_relative_user.getState() == CONNECTED)
		_relative_user.push(ERR_ALREADYREGISTERED(_relative_user.getNickName()));
	if (_parameters.size() < 4)
	{
		errNeedMoreParams();
		return;
	}
	
	_relative_user.setUserkName(_parameters.front());
	_relative_user.setRealName(_parameters.back());
	if (_relative_user.getState() == HANDSHAKED || _relative_server->getPassword().empty())
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
	if (_relative_server->getPassword() == _parameters.front() || _relative_server->getPassword().empty())
	{
		_relative_user.setState(HANDSHAKED);
	}
	else
		std::cerr << "[INFO] User pass (" << _parameters.front() <<  ") doesn't match server pass (" << _relative_server->getPassword() << ")" << std::endl;
}

void Command::cmdNick()
{
	_relative_user.setNickName(_parameters[0]);
}

void Command::cmdJoin()
{
	if (_parameters.empty())
	{
		errNeedMoreParams();
		return;
	}

	std::string channel_name;
	std::string channel_pass;
	Channel		*channel;

	try
	{	channel_pass = _parameters[2];	}
	catch (const std::out_of_range &e)
	{	channel_pass = "";				}
	channel_name = _parameters.front();
	
	if (!(_relative_user.getChannel() == nullptr))
	{
		return (_relative_user.push(ERR_TOOMANYCHANNELS(_relative_user.getNickName(), channel_name)));
	}
	channel = _relative_server->getSetRelativeChannel(channel_name, channel_pass);
	channel->welcomeToChannel(&_relative_user);
}

void Command::errUnknowCommand()
{
	std::cerr << "[ERROR] unknow command (" << _name << ")" << std::endl;
}

void Command::errNeedMoreParams()
{
	_relative_user.push(ERR_NEEDMOREPARAMS(_relative_user.getNickName(), _name));
}