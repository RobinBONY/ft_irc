/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vducoulo <vducoulo@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/01 17:35:58 by vducoulo          #+#    #+#             */
/*   Updated: 2023/03/08 14:29:19 by vducoulo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CPP_SERVER_HPP
# define CPP_SERVER_HPP

# include "../includes/Irc.hh"
# include "user.hpp"

class Server
{
	private :
		bool					_active;
		std::string				_password;
		sockaddr_in				_server_sockaddr;

		std::vector<pollfd> 	_pfds;
		std::vector<User>		_users;

		bool					_debug;
		
	public :
		Server(char *port, char *pass);

		int 					setSocketFd(int port);
		void					runLoop(void);
		void					userHandShake(void);
		void					receiveMsg(int fd);

		User					getRelativeUser(int fd);
		std::string				getPassword(void) { return _password; }
};
#endif