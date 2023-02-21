/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vducoulo <vducoulo@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/25 13:03:44 by rbony             #+#    #+#             */
/*   Updated: 2023/02/20 12:29:48 by vducoulo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel(const std::string &name, const User &creator, const std::string &pass) : _name(name), _password(pass)
{
	this->_users.push_back(&creator);
	this->_operators.push_back(&creator);
	sendMessage("JOIN :" + this->_name + "\n", creator, true);
}

Channel::~Channel()
{}

void	Channel::sendMessage(const std::string &message, const User &from, bool includeUser) const
{
	std::string	msg;
	msg += ":" + from.getUsername() + " " + message;
	std::vector<const User *>::const_iterator	begin = this->_users.begin();
	std::vector<const User *>::const_iterator	end = this->_users.end();
	for (; begin != end; ++begin)
	{
		if (includeUser || *begin != &from)
			(*begin)->sendMessage(msg);
	}
}

const std::string	&Channel::getName() const
{
	return (this->_name);
}

bool	Channel::isOperator(const User &user) const
{
	for (size_t i = 0; i < this->_operators.size(); i++)
		if (this->_operators[i]->getUsername() == user.getUsername())
			return true;
	return false;
}

bool	Channel::containsNickname(const std::string &nickname) const
{
	std::vector<const User *>::const_iterator	beg = this->_users.begin();
	std::vector<const User *>::const_iterator	end = this->_users.end();
	for (; beg != end; ++beg)
		if ((*beg)->getUsername() == nickname)
			return (true);
	return (false);
}

void	Channel::addOperator(const User &user)
{
	if (!isOperator(user))
		this->_operators.push_back(&user);
}

void	Channel::removeOperator(const User &user)
{
	if (isOperator(user))
	{
		size_t	i;
		for (i = 0; i < this->_operators.size(); i++)
			if (this->_operators[i] == &user)
				break;
		this->_operators.erase(this->_operators.begin() + i);
		if (this->_operators.size() == 0 && this->_users.size() > 0)
		{
			this->_operators.push_back(this->_users[0]);
			sendMessage("MODE " + this->_name + " +o "  + this->_users[0]->getUsername() + "\n", user, true);
		}
	}
}

void	Channel::disconnect(const User &user)
{
	std::vector<const User *>::iterator	begin = this->_users.begin();
	std::vector<const User *>::iterator	end = this->_users.end();
	for (; begin != end; ++begin)
		if (*begin == &user)
			break ;
	this->_users.erase(begin);
	removeOperator(user);
}