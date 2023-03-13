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


Command::Command(std::string name, std::vector<std::string> params, User *relativeuser, Server *relativeserver)
: _name(name), _parameters(params), _relative_user(relativeuser), _relative_server(relativeserver)
{
	_cmd_ptr["USER"] = &Command::cmdUser;
	_cmd_ptr["CAP"] = &Command::cmdCap;
	_cmd_ptr["PASS"] = &Command::cmdPass;
	_cmd_ptr["NICK"] = &Command::cmdNick;
	_cmd_ptr["JOIN"] = &Command::cmdJoin;
	_cmd_ptr["MODE"] = &Command::cmdMode;
	_cmd_ptr["PRIVMSG"] = &Command::cmdPrivmsg;
	_cmd_ptr["PING"] = &Command::cmdPing;
	_cmd_ptr["QUIT"] = &Command::cmdQuit;
	
	try
	{
		_cmd_ptr.at(_name);
	}
	catch(const std::out_of_range& e)
	{
		_cmd_ptr[_name] = &Command::errUnknowCommand;
	}
	if ((_name != "PASS" && _name != "CAP" && _name != "QUIT" && _name != "USER") && relativeuser->getState() != CONNECTED)
	{
		_cmd_ptr[_name] = &Command::errNotRegistred;

	}
}

Command::~Command()
{}

void Command::execute(void)
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
void Command::cmdUser(void)
{
	if (_relative_user->getState() == CONNECTED)
		_relative_user->push(ERR_ALREADYREGISTERED(_relative_user->getNickName()));
	if (errNeedMoreParams(4))
		return;
	
	_relative_user->setUserkName(_parameters.front());
	_relative_user->setRealName(_parameters.back());
	if (_relative_user->getState() == HANDSHAKED || _relative_server->getPassword().empty())
		_relative_user->welcomeToIrc();
}

void Command::cmdCap(void)
{
	/*
		Not in use
	*/
}

void Command::cmdPass(void)
{
	if (_relative_server->getPassword() == _parameters.front() || _relative_server->getPassword().empty())
	{
		_relative_user->setState(HANDSHAKED);
	}
	else
		std::cerr << "[INFO] User pass (" << _parameters.front() <<  ") doesn't match server pass (" << _relative_server->getPassword() << ")" << std::endl;
}

void Command::cmdNick(void)
{
	_relative_user->setNickName(_parameters[0]);

	// maybe weechat need a response
}

void Command::cmdJoin(void)
{
	if (errNeedMoreParams(1))
		return;

	std::string channel_name(_parameters.front());
	std::string channel_pass;
	Channel		*channel;

	if (_relative_user->getChannel())
		return (_relative_user->push(ERR_TOOMANYCHANNELS(_relative_user->getNickName(), channel_name)));
	if (channel_name[0] != '#')
		return (_relative_user->push(FTIRC_ERR_BADCHANNELNAME(_relative_user->getNickName(), channel_name)));
	try
	{	channel_pass = _parameters[2];	}
	catch (const std::out_of_range &e)
	{	channel_pass = "";				}
	
	channel = _relative_server->getSetRelativeChannel(channel_name, channel_pass);

	if (channel_pass == channel->getPassword())
	{
		_relative_user->setChannel(channel);
		channel->welcomeToChannel(_relative_user);
	}
	else if (channel->getUsersPtr().empty())
		_relative_server->deleteChannel(channel_name);
	else
		_relative_user->push(ERR_BADCHANNELKEY(_relative_user->getNickName(), channel_name));
}

void Command::cmdMode(void)
{
	if (errNeedMoreParams(2))
		return;
	if (_parameters[0] != _relative_user->getChannel()->getName())
		_relative_user->push(ERR_CHANOPRIVSNEEDED(_relative_user->getNickName(), _parameters[0]));
}

void Command::cmdPrivmsg(void)
{
	if (errNeedMoreParams(2))
		return;
	
	std::vector<std::string>::iterator iter;
	std::string message;
	std::string send_to(_parameters[0]);

	if (_parameters[1][0] == ':')
		_parameters[1] = _parameters[1].substr(1, _parameters[0].length());
	
	for (iter = _parameters.begin() + 1; iter != _parameters.end(); iter++)
		message += *iter + " ";
	
	std::cout << "send to " << send_to << " with payload : " << message << std::endl;
	if (send_to.front() == '#')
	{
		Channel *channel = _relative_user->getChannel();
		if (!channel || channel->getName() != send_to)
			return (_relative_user->push(ERR_NOSUCHCHANNEL(_relative_user->getNickName(), send_to)));
		
		channel->pushBroadcast(RPL_PRIVMSG(_relative_user->getSenderPrefix(), send_to, message), _relative_user); //untested
	}
	else
	{
		try
		{
			_relative_server->getReltiveUserPerNick(send_to);
		}
		catch (const std::runtime_error &e)
		{
			_relative_user->push(ERR_NOSUCHNICK(_relative_user->getNickName(), send_to));
			return;
		}
		_relative_server->getReltiveUserPerNick(send_to)->push(RPL_PRIVMSG(_relative_user->getSenderPrefix() ,send_to, message), true);
		//_relative_user->push(RPL_PRIVMSG(_relative_server->getReltiveUserPerNick(send_to).getNickName(), message));
	}
	
	//tofinish

}

void Command::cmdPing(void)
{
	if (errNeedMoreParams(1))
		return;
	_relative_user->push(RPL_PING(_parameters.front())); //better with server_addr
}

void Command::cmdQuit(void)
{
	std::cerr << "QUIT COMMAND RECEIVED" << std::endl;
}

void Command::errUnknowCommand(void)
{
	std::cerr << "[ERROR] unknow command (" << _name << ")" << std::endl;
}

int Command::errNeedMoreParams(int minimalparameterscount)
{
	if (_parameters.size() < minimalparameterscount)
	{
		_relative_user->push(ERR_NEEDMOREPARAMS(_relative_user->getNickName(), _name));
		return 1;
	}
	return 0;
}

void Command::errNotRegistred(void)
{
	_relative_user->push(ERR_NOTREGISTERED(_relative_user->getNickName()));
}