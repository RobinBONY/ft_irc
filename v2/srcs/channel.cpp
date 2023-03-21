/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vducoulo <vducoulo@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 18:31:51 by vducoulo          #+#    #+#             */
/*   Updated: 2023/03/21 10:47:35 by vducoulo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "channel.hpp"

Channel::Channel(std::string name, std::string pass)
: _name(name), _password(pass), _operator(nullptr)
{
	
}

Channel::~Channel()
{}

User *Channel::getUserPerNick(std::string usrnick)
{
	std::vector<User *>::iterator iter;

	for (iter = _users_ptr.begin(); iter != _users_ptr.end(); iter++)
	{
		if ((*iter)->getNickName() == usrnick)
			return *iter;
	}
	throw std::runtime_error("No such user"); // to change
}

User *Channel::getBannedUserPerNick(std::string bannednick)
{
	std::vector<User *>::iterator iter;

	for (iter = _banned_users_ptr.begin(); iter != _banned_users_ptr.end(); iter++)
	{
		if ((*iter)->getNickName() == bannednick)
			return *iter;
	}
	throw std::runtime_error("No such user"); // to change
}

bool Channel::isBanned(User *toverify)
{
	std::vector<User *>::iterator iter;

	for (iter = _banned_users_ptr.begin(); iter != _banned_users_ptr.end(); iter++)
	{
		if (*iter && *iter == toverify)
			return true;
	}
	return false;
}

bool Channel::isOnUserLimit()
{
	if (_users_ptr.size() >= _max_users)
		return true;
	return false;
}

void Channel::setNewBan(std::string banned_nick)
{
	std::vector<User *>::iterator iter;
	User *toban = getUserPerNick(banned_nick);
	
	_banned_users_ptr.push_back(toban);
	toban->setChannel(nullptr);
	quitChannel(toban);
	pushBroadcast(RPL_PART(toban->getSenderPrefix(), _name));
}

void Channel::removeBan(std::string banned_nick)
{
	std::vector<User *>::iterator iter;
	User *todeban = getBannedUserPerNick(banned_nick);
	
	for (iter = _banned_users_ptr.begin(); iter != _banned_users_ptr.end(); iter++)
	{
		if (*iter && todeban == *iter)
		{
			_banned_users_ptr.erase(iter);
			return;
		}
	}
}

void Channel::pushBroadcast(std::string msg, User *initiator)
{
	std::vector<User *>::iterator iter;
	
	for (iter = _users_ptr.begin(); iter != _users_ptr.end(); iter++)
	{
		if (*iter != initiator)
			(*iter)->push(msg, true);
	}
}

void Channel::broadcastNames(User *initiator)
{
	std::string chans_users_nicks;
	std::vector<User *>::iterator iter;

	for (iter = _users_ptr.begin(); iter != _users_ptr.end(); ++iter)
	{
		if (*iter == _operator)
			chans_users_nicks.append("@" + (*iter)->getNickName() + " ");
		else
			chans_users_nicks.append((*iter)->getNickName() + " ");
	}
	this->pushBroadcast(RPL_NAMREPLY(initiator->getNickName(), _name, chans_users_nicks));
	this->pushBroadcast(RPL_ENDOFNAMES(initiator->getNickName(), _name));
}

void Channel::welcomeToChannel(User *user)
{	
	_users_ptr.push_back(user);
	
	this->pushBroadcast(RPL_JOIN(user->getSenderPrefix(), _name));
	broadcastNames(user);
}

void Channel::quitChannel(User *user)
{
	std::vector<User *>::iterator iter;

	for (iter = _users_ptr.begin(); iter != _users_ptr.end(); iter++)
	{
		if ((*iter)->getFd() == user->getFd())
		{
		 	_users_ptr.erase(iter);
			return;
		}
	}
}