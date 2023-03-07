/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vducoulo <vducoulo@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/03 17:33:55 by vducoulo          #+#    #+#             */
/*   Updated: 2023/03/07 15:02:02 by vducoulo         ###   ########.fr       */
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

		std::string getNickName() {return _nickname; }
		int			getFd() {return _fd; }
};

#endif