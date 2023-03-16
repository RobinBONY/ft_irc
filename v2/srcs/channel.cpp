/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vducoulo <vducoulo@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 18:31:51 by vducoulo          #+#    #+#             */
/*   Updated: 2023/03/16 12:28:19 by vducoulo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "channel.hpp"

Channel::Channel(std::string name, std::string pass)
: _name(name), _password(pass), _operator(nullptr)
{
	
}

Channel::~Channel()
{}

void Channel::pushBroadcast(std::string msg, User *initiator)
{
	std::vector<User *>::iterator iter;
	
	for (iter = _users_ptr.begin(); iter != _users_ptr.end(); iter++)
	{
		if (*iter != initiator)
			(*iter)->push(msg, true);
	}
}

void Channel::welcomeToChannel(User *user)
{	
	_users_ptr.push_back(user);
	
	std::string chans_users_nicks;
	std::vector<User *>::iterator iter;
	for (iter = _users_ptr.begin(); iter != _users_ptr.end(); ++iter)
	{
		if (iter == _users_ptr.begin())
			chans_users_nicks.append("@" + (*iter)->getNickName() + " ");
		else
			chans_users_nicks.append((*iter)->getNickName() + " ");
	}
	
	user->push(RPL_NAMREPLY(user->getNickName(), _name, chans_users_nicks));
	user->push(RPL_ENDOFNAMES(user->getNickName(), _name));
	this->pushBroadcast(RPL_JOIN(user->getSenderPrefix(), _name));
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