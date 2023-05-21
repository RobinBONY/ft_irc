/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbony <rbony@corobizar.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/03 17:33:55 by vducoulo          #+#    #+#             */
/*   Updated: 2023/05/21 18:29:10 by rbony            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef CPP_USER_HPP
# define CPP_USER_HPP

# include "../../includes/irc.hh"
class User;
# include "../channel/channel.hpp"

class User
{
	private : 
		int							_fd;
		std::string		 			_hostname;
		Channel				*_current_channel;
		std::string			_nickname;
		
		std::vector<std::string> 	_msgs;
		std::string			_username;
		std::string			_realname;
		int					_state;

	public :
		User(int user_fd);
		~User() {}

		std::string 				getNickName() const {return _nickname; }
		std::string 				getRealName() const {return _realname; }
		std::string 				getUserName() const {return _username; }
		int							getFd()		  const {return _fd; }
		int							getState()	  const {return _state; }
		Channel						*getChannel() const {return _current_channel; }
		std::vector<std::string> 	getMsgs()	  const {return _msgs ;}
		std::string 				getSenderPrefix() const;

		void setNickName(std::string nick) 			{_nickname = nick; }
		void setUserkName(std::string username)		{_username = username; }
		void setRealName(std::string realname)		{_realname = realname; }
		void setState(int state)					{_state = state; }
		void setChannel(Channel *chan)				{_current_channel = chan; }
		void setMsgs(std::vector<std::string> msgs) {_msgs = msgs; }

		void welcomeToIrc();
		void push(std::string msg, bool raw = false);
};

#endif