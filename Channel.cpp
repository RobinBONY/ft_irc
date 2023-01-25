/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbony <rbony@corobizar.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/25 13:03:44 by rbony             #+#    #+#             */
/*   Updated: 2023/01/25 14:25:19 by rbony            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel(const std::string &name, const User &creator, const std::string &pass) : name(name), password(pass)
{
	users.push_back(&creator);
	operators.push_back(&creator);
	sendMessage("JOIN :" + this->name + "\n", creator, true);
}

Channel::~Channel()
{}

void	Channel::sendMessage(const std::string &message, const User &from, bool includeUser) const
{
	std::string	msg;
	msg += ":" + from.getUsername() + " " + message;
	std::vector<const User *>::const_iterator	begin = users.begin();
	std::vector<const User *>::const_iterator	end = users.end();
	for (; begin != end; ++begin)
	{
		if (includeUser || *begin != &from)
			(*begin)->sendMessage(msg);
	}
}

const std::string	&Channel::getName() const
{
	return (name);
}

bool	Channel::isOperator(const User &user) const
{
	for (size_t i = 0; i < operators.size(); i++)
		if (operators[i]->getUsername() == user.getUsername())
			return true;
	return false;
}

bool	Channel::containsNickname(const std::string &nickname) const
{
	std::vector<const User *>::const_iterator	beg = users.begin();
	std::vector<const User *>::const_iterator	end = users.end();
	for (; beg != end; ++beg)
		if ((*beg)->getUsername() == nickname)
			return (true);
	return (false);
}

void	Channel::addOperator(const User &user)
{
	if (!isOperator(user))
		operators.push_back(&user);
}

void	Channel::removeOperator(const User &user)
{
	if (isOperator(user))
	{
		size_t	i;
		for (i = 0; i < operators.size(); i++)
			if (operators[i] == &user)
				break;
		operators.erase(operators.begin() + i);
		if (operators.size() == 0 && users.size() > 0)
		{
			operators.push_back(users[0]);
			sendMessage("MODE " + this->name + " +o "  + users[0]->getUsername() + "\n", user, true);
		}
	}
}

void	Channel::disconnect(const User &user)
{
	std::vector<const User *>::iterator	begin = users.begin();
	std::vector<const User *>::iterator	end = users.end();
	for (; begin != end; ++begin)
		if (*begin == &user)
			break ;
	users.erase(begin);
	removeOperator(user);
}