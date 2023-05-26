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


Command::Command(std::string name, std::vector<std::string> params, User *relativeuser, Server *relativeserver)
: _name(name), _parameters(params), _relative_user(relativeuser), _relative_server(relativeserver)
{
	_cmd_ptr["USER"] = &Command::cmdUser;
	_cmd_ptr["CAP"] = &Command::cmdCap;
	_cmd_ptr["PASS"] = &Command::cmdPass;
	_cmd_ptr["NICK"] = &Command::cmdNick;
	_cmd_ptr["JOIN"] = &Command::cmdJoin;
	_cmd_ptr["MODE"] = &Command::cmdMode;
	_cmd_ptr["KICK"] = &Command::cmdKick;
	_cmd_ptr["PING"] = &Command::cmdPing;
	_cmd_ptr["QUIT"] = &Command::cmdQuit;
	_cmd_ptr["PART"] = &Command::cmdPart;
	_cmd_ptr["PRIVMSG"] = &Command::cmdPrivmsg;
	
	try
	{
		_cmd_ptr.at(_name);
	}
	catch(const std::out_of_range& e)
	{
		_cmd_ptr[_name] = &Command::errUnknowCommand;
	}
	if ((_name != "PASS" && _name != "CAP" && _name != "NICK" && _name != "QUIT" && _name != "USER") && relativeuser->getState() != CONNECTED)
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
	if (errNeedMoreParams(1))
		return;
	if (!(_relative_user->getState() == HANDSHAKING))
		_relative_user->push(ERR_ALREADYREGISTERED(_relative_user->getNickName()));
	if (_relative_server->getPassword() == _parameters.front() || _relative_server->getPassword().empty())
	{
		_relative_user->setState(HANDSHAKED);
	}
	else
		std::cerr << "[INFO] User pass (" << _parameters.front() <<  ") doesn't match server pass (" << _relative_server->getPassword() << ")" << std::endl;
}

void Command::cmdNick(void)
{
	if (errNeedMoreParams(1))
		return;
	try
	{
		_relative_server->getReltiveUserPerNick(_parameters[0]);
		_relative_user->push(ERR_NICKNAMEINUSE(_relative_user->getNickName(), _parameters[0]));
	}
	catch (const std::exception &e)
	{
		_relative_user->push(RPL_NICK(_parameters[0]));
		_relative_user->setNickName(_parameters[0]);
		if (_relative_user->getChannel() != NULL)
			_relative_user->getChannel()->broadcastNames(_relative_user);
	}
}

void Command::cmdJoin(void)
{
	if (errNeedMoreParams(1))
		return;

	std::string channel_name(_parameters.front());
	std::string channel_pass;
	Channel		*channel;


	for (std::vector<std::string>::iterator iter = _parameters.begin() + 1; iter != _parameters.end(); iter++)
	{
		std::cout << "param :" << *iter << std::endl;
	}

	if (_relative_user->getChannel() != NULL)
		return (_relative_user->push(ERR_TOOMANYCHANNELS(_relative_user->getNickName(), channel_name)));
	if (channel_name[0] != '#')
		return (_relative_user->push(FTIRC_ERR_BADCHANNELNAME(_relative_user->getNickName(), channel_name)));
	try
	{	
		if (_parameters.size() > 1)
		{
			std::cerr << "debug param:" << _parameters[1] << std::endl;
			channel_pass = _parameters[1];
			std::cerr << _parameters[1] << std::endl;
			if (_parameters.size() > 2 && channel_pass == "+k")
			{
				std::cerr << "ENTERED" << std::endl;
				channel_pass = _parameters[2];
			}
		}
	}
	catch (const std::out_of_range &e)
	{	channel_pass = "";				}
	
	channel = _relative_server->getSetRelativeChannel(channel_name, channel_pass);

	if (channel_pass == channel->getPassword() && !channel->isBanned(_relative_user) && !channel->isOnUserLimit())
	{
		_relative_user->setChannel(channel);
		channel->welcomeToChannel(_relative_user);
		if (channel->getOperator() == NULL)
		{
			channel->setOperator(_relative_user);
			_relative_user->push(RPL_YOUREOPER(_relative_user->getNickName()));
		}
	}
	else if (channel->isBanned(_relative_user))
		_relative_user->push(ERR_BANNEDFROMCHAN(_relative_user->getNickName(), channel_name));
	else if (channel->isOnUserLimit())
		_relative_user->push(ERR_CHANNELISFULL(_relative_user->getNickName(), channel_name));
	else if (channel->getUsersPtr().empty())
		_relative_server->deleteChannel(channel_name);
	else
		_relative_user->push(ERR_BADCHANNELKEY(_relative_user->getNickName(), channel_name));
}

void Command::cmdMode(void)
{
	if (errNeedMoreParams(2) || errNotUserChannel(_parameters[0]) || errNotOperator(_parameters[0]))
		return;
	if (_parameters[1].empty() || _parameters[1].length() < 2 || (_parameters[1][0] != '+' && _parameters[1][0] != '-'))
		return (_relative_user->push(ERR_UNKNOWNMODE(_relative_user->getNickName(), "undefined")));

	Channel *channel = _relative_user->getChannel();
	bool grant = _parameters[1][0] == '+' ? true : false;
	size_t parameter_idx = 1;
	bool k = false; bool l = false; bool n = false; bool o = false; bool b = false;
	
	for (size_t i = 1; i < _parameters[1].length(); i++)
	{
		if (_parameters[1][i] == 'n' && !n)
		{
			std::string str = channel->getOutsideAccess() ? "true" : "false";
			grant ? channel->setOutsideAccess(true) : channel->setOutsideAccess(false);
			channel->pushBroadcast(RPL_MODE(_relative_user->getSenderPrefix(), _parameters[0], _parameters[1][0] + 'n', str));
			n = true;
		}
		else if (grant && parameter_idx + 1 >= _parameters.size())
				return;
		else if (_parameters[1][i] == 'b' && !b)
		{
			try
			{
				parameter_idx++;
				grant ? channel->setNewBan(_parameters[parameter_idx]) : channel->removeBan(_parameters[parameter_idx]);
				channel->pushBroadcast(RPL_MODE(_relative_user->getSenderPrefix(), _parameters[0], _parameters[1][0] + 'b', _parameters[parameter_idx]));
			}
			catch(const std::runtime_error& e)
			{
				_relative_user->push(ERR_NOSUCHNICK(_relative_user->getNickName(), ""));
			}
			k = true;
		}
		else if (_parameters[1][i] == 'k' && !k)
		{
			grant ? parameter_idx++, channel->setPassword(_parameters[parameter_idx]) : channel->setPassword("");
			channel->pushBroadcast(RPL_MODE(_relative_user->getSenderPrefix(), _parameters[0], _parameters[1][0] + 'k', channel->getPassword()));
			k = true;
		}
		else if (_parameters[1][i] == 'l' && !l)
		{
			std::stringstream ss;
			ss << channel->getMaxUsers();
			grant ? parameter_idx++, channel->setMaxUsersLimit(atoll(_parameters[parameter_idx].c_str())) : channel->setMaxUsersLimit(INT_MAX);
			channel->pushBroadcast(RPL_MODE(_relative_user->getSenderPrefix(), _parameters[0], _parameters[1][0] + 'l', ss.str()));
			l = true;
		}
		else if (_parameters[1][i] == 'o' && !o)
		{
			if (grant)
			{
				parameter_idx++;
				try
				{
					User *target = channel->getUserPerNick(_parameters[parameter_idx]);
					channel->setOperator(target);
					target->push(RPL_YOUREOPER(target->getNickName()));
					channel->broadcastNames(target);
				}
				catch(const std::exception& e)
				{
					_relative_user->push(ERR_NOSUCHNICK(_relative_user->getNickName(), ""));
				}
			}
			else
				channel->setOperator(NULL);
			o = true;
		}
		else if (_parameters[1][i] != 'k' || _parameters[1][i] != 'l' || _parameters[1][i] != 'n' || _parameters[1][i] != 'o' || _parameters[1][i] != 'b')
		{
			return (_relative_user->push(ERR_UNKNOWNMODE(_relative_user->getNickName(), _parameters[1][i])));
		}
	}
}

void Command::cmdPrivmsg(void)
{
	if (errNeedMoreParams(2))
		return;
	
	std::vector<std::string>::iterator iter;
	std::string message;
	std::string send_to(_parameters[0]);
	User		*dest_usr;

	if (_parameters[1][0] == ':')
		_parameters[1] = _parameters[1].substr(1, _parameters[1].length());
	
	for (iter = _parameters.begin() + 1; iter != _parameters.end(); iter++)
	{
		message += *iter + " ";
		std::cout << "param :" << message << std::endl;
	}
	
	if (send_to.c_str()[0] == '#')
	{
		if (errNotUserChannel(send_to))
			return;
		
		Channel *channel = _relative_user->getChannel();
		channel->pushBroadcast(RPL_PRIVMSG(_relative_user->getSenderPrefix(), send_to, message), _relative_user); //untested
	}
	else
	{
		try
		{
			dest_usr = _relative_server->getReltiveUserPerNick(send_to);
			if (dest_usr->getState() != CONNECTED)
				throw std::runtime_error("user is not alive");
		}
		catch (const std::runtime_error &e)
		{
			_relative_user->push(ERR_NOSUCHNICK(_relative_user->getNickName(), send_to));
			return;
		}
		dest_usr->push(RPL_PRIVMSG(_relative_user->getSenderPrefix() ,send_to, message), true);
	}
}

void Command::cmdPing(void)
{
	if (errNeedMoreParams(1))
		return;
	_relative_user->push(RPL_PING(_parameters.front())); //better with server_addr
}

void Command::cmdQuit(void)
{
	if (_relative_user->getChannel() != NULL)
	{
		std::cerr << "USER HAS CHANNEL" << std::endl;
		_parameters[0] = _relative_user->getChannel()->getName();
		cmdPart();
	}
	_relative_user->push(RPL_QUIT(_relative_user->getSenderPrefix()));
	_relative_server->deleteUser(_relative_user);
}

void Command::cmdPart(void)
{
	if (errNeedMoreParams(1) || errNotUserChannel(_parameters[0]))
		return;
	Channel *channel = _relative_user->getChannel();

	if (channel->getOperator() == _relative_user && channel->getUsersPtr().size() > 1)
	{
		std::vector<User *> users = channel->getUsersPtr();
		std::vector<User *>::iterator iter = users.begin();
		User *new_operator = *(++iter);
		channel->setOperator(new_operator);
		new_operator->push(RPL_YOUREOPER(new_operator->getNickName()));
	}
	else if (channel->getUsersPtr().size() == 1)
	{
		_relative_server->deleteChannel(channel->getName());
		_relative_user->setChannel(NULL);
		return ;
	}
	channel->pushBroadcast(RPL_PART(_relative_user->getSenderPrefix(), _parameters[0]));
	channel->quitChannel(_relative_user);
	channel->broadcastNames(_relative_user);
	_relative_user->setChannel(NULL);
}

void Command::cmdKick()
{
	if (errNeedMoreParams(2) || errNotOperator(_parameters[0]) || errNotUserChannel(_parameters[0]))
		return ;
	
	try {
		User *target;
		target = _relative_user->getChannel()->getUserPerNick(_parameters[1]);
		target->setChannel(NULL);
		_relative_user->getChannel()->quitChannel(target);
		_relative_user->getChannel()->pushBroadcast(RPL_PART(target->getSenderPrefix(), _parameters[0]));
	}
	catch (const std::exception &e)
	{
		_relative_user->push(ERR_USERNOTINCHANNEL(_relative_user->getNickName(), _parameters[1], _parameters[0]));
	}
}

void Command::errUnknowCommand(void)
{
	std::cerr << "[ERROR] unknow command (" << _name << ")" << std::endl;
}

int Command::errNeedMoreParams(int minimalparameterscount)
{
	if (_parameters.size() < (size_t)minimalparameterscount)
	{
		_relative_user->push(ERR_NEEDMOREPARAMS(_relative_user->getNickName(), _name));
		return 1;
	}
	return 0;
}

int Command::errNotOperator(std::string chan_name)
{
	if (_relative_user->getChannel()->getOperator() != _relative_user)
	{
		_relative_user->push(ERR_CHANOPRIVSNEEDED(_relative_user->getNickName(), chan_name));
		return 1;
	}
	return (0);
}

int Command::errNotUserChannel(std::string chan_name)
{
	if (_relative_user->getChannel()==NULL || _relative_user->getChannel()->getName() != chan_name)
	{
		_relative_user->push(ERR_NOSUCHCHANNEL(_relative_user->getNickName(), chan_name));
		return 1;
	}
	return (0);
}

void Command::errNotRegistred(void)
{
	_relative_user->push(ERR_NOTREGISTERED(_relative_user->getNickName()));
}