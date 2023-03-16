/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vducoulo <vducoulo@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 18:31:51 by vducoulo          #+#    #+#             */
/*   Updated: 2023/03/16 17:28:00 by vducoulo         ###   ########.fr       */
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