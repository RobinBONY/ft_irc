/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vducoulo <vducoulo@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 18:31:51 by vducoulo          #+#    #+#             */
/*   Updated: 2023/03/14 11:06:12 by vducoulo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "channel.hpp"

Channel::Channel(std::string name, std::string pass)
: _name(name), _password(pass)
{
	
}

Channel::~Channel()
{}

void Channel::pushBroadcast(std::string msg, User *initiator)
{
	std::vector<User *>::iterator iter;
	
	for (iter = _users_ptr.begin(); iter != _users_ptr.end(); iter++)
	{
		std::cerr << "will notify " << (*iter)->getNickName() << std::endl;
		if (*iter != initiator)
			(*iter)->push(msg, true);
	}
}

void Channel::welcomeToChannel(User *user)
{	
	_users_ptr.push_back(user);
	
	std::cerr << "[DEBUG] user requesting : name: " << user->getRealName() << " nick :" << user->getNickName() << std::endl;
	std::cerr << "[DEBUG] last user in channel : name: " << _users_ptr.back()->getRealName() << " nick :" << _users_ptr.back()->getNickName() << std::endl;
	std::string chans_users_nicks;
	std::vector<User *>::iterator iter;
	 // add differenciation for admin user : add @
	for (iter = _users_ptr.begin(); iter != _users_ptr.end(); ++iter)
	{
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
		std::cerr << "usr : " << (*iter)->getNickName() << std::endl;
		std::cerr << "requesting usr " << user->getNickName() << std::endl;
		if ((*iter)->getFd() == user->getFd())
		{
		 	_users_ptr.erase(iter);
			return;
		}
	}
}