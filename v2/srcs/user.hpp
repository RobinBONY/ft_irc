/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vducoulo <vducoulo@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/03 17:33:55 by vducoulo          #+#    #+#             */
/*   Updated: 2023/03/07 19:30:40 by vducoulo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CPP_USER_HPP
# define CPP_USER_HPP

#include "../includes/Irc.hh"

class User
{
	private : 
		int					_fd;
		std::string		 	_hostname;

		std::string			_nickname;
		std::string			_username;
		std::string			_realname;
		int					_state;

	public :
		User(int user_fd);
		~User() {}

		std::string getNickName() const {return _nickname; }
		std::string getRealName() const {return _realname; }
		std::string getUserName() const {return _username; }
		int			getFd()		  const {return _fd; }

		void setNickName(std::string nick) {_nickname = nick; }
		void setUserkName(std::string username) {_username = username; }
		void setRealName(std::string realname) {_realname = realname; }
};

#endif