/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vducoulo <vducoulo@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 18:30:30 by vducoulo          #+#    #+#             */
/*   Updated: 2023/03/20 17:43:21 by vducoulo         ###   ########.fr       */
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
	User				*_operator;

	std::string			_password;
	long long			_max_users;
	bool				_outside_access;

	std::vector<User *> _users_ptr;
	std::vector<User *> _banned_users_ptr;

	public : 

	Channel(std::string name, std::string pass);
	~Channel();

	std::string 			getName(void)		{return _name; }
	std::string				getPassword()		{return _password; }
	std::vector<User *> 	getUsersPtr()		{return _users_ptr; }
	User					*getOperator()		{return _operator; }
	long long				getMaxUsers()		{return _max_users; }
	bool					getOutsideAccess() 	{return _outside_access; }
	User					*getUserPerNick(std::string usrnick);
	bool					isBanned(User *to_verify);

	void		setOperator(User *user) 			{_operator = user; };
	void		setPassword(std::string pass) 		{_password = pass; }
	void		setMaxUsersLimit(long long limit) 	{_max_users = limit; }
	void		setOutsideAccess(bool authorize)	{_outside_access = authorize; }
	void		setNewBan(std::string banned_nick);
	void		removeBan(std::string banned_nick);

	void		welcomeToChannel(User *user);
	void		pushBroadcast(std::string msg, User *initiator = nullptr);
	void		quitChannel(User *user);
	void		broadcastNames(User *initiator);
};

#endif