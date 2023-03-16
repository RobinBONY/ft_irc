/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vducoulo <vducoulo@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 18:30:30 by vducoulo          #+#    #+#             */
/*   Updated: 2023/03/16 11:42:28 by vducoulo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CPP_CHANNEL_HPP
# define CPP_CHANNEL_HPP

# include "../includes/Irc.hh"

class Channel;

#include "user.hpp"

class Channel
{
	private : 

	std::string			_name;
	std::string			_password;
	User				*_operator;

	std::vector<User *> _users_ptr;
	std::vector<User *> _banned_users_ptr;

	public : 

	Channel(std::string name, std::string pass);
	~Channel();

	std::string 			getName(void)	{return _name; }
	std::string				getPassword()	{return _password; }
	std::vector<User *> 	getUsersPtr()	{return _users_ptr; }
	User					*getOperator()	{return _operator; }

	void		setOperator(User *user) {_operator = user; };

	void		welcomeToChannel(User *user);
	void		pushBroadcast(std::string msg, User *initiator = nullptr);
	void		quitChannel(User *user);
};

#endif