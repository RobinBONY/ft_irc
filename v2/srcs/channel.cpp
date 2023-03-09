/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vducoulo <vducoulo@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 18:31:51 by vducoulo          #+#    #+#             */
/*   Updated: 2023/03/09 11:39:13 by vducoulo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "channel.hpp"

Channel::Channel(std::string name, std::string pass)
: _name(name), _password(pass)
{
	
}

Channel::~Channel()
{}

void Channel::pushBroadcast(std::string msg)
{
	std::vector<User *>::iterator iter;
	
	for (iter = _users_ptr.begin(); iter != _users_ptr.end(); iter++)
	{
		(*iter)->push(msg);
	}
}

void Channel::welcomeToChannel(User *user)
{	
	_users_ptr.push_back(user);
	
	std::string chans_users_nicks;
	std::vector<User *>::iterator iter;
	
	for (iter = _users_ptr.begin(); iter != _users_ptr.end(); iter++)
		chans_users_nicks.append((*iter)->getNickName() + " ");
	
	user->push(RPL_NAMREPLY(user->getNickName(), _name, chans_users_nicks));
	user->push(RPL_ENDOFNAMES(user->getNickName(), _name));
	this->pushBroadcast(RPL_JOIN(user->getNickName(), _name));
}