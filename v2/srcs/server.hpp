/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vducoulo <vducoulo@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/01 17:35:58 by vducoulo          #+#    #+#             */
/*   Updated: 2023/03/08 19:25:15 by vducoulo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CPP_SERVER_HPP
# define CPP_SERVER_HPP

# include "../includes/Irc.hh"
# include "user.hpp"
# include "channel.hpp"

class Server
{
	private :
		bool					_active;
		std::string				_password;
		sockaddr_in				_server_sockaddr;

		std::vector<pollfd> 	_pfds;
		std::vector<User>		_users;
		std::vector<Channel>	_channels;

		bool					_debug;
		
		Channel					*createChannel(std::string name, std::string pass);
		
	public :
		Server(char *port, char *pass);

		void							runLoop(void);
		void							userHandShake(void);
		void							receiveMsg(int fd);

		User							&getRelativeUser(int fd);
		Channel							*getSetRelativeChannel(std::string name, std::string pass);
		std::string						getPassword(void) { return _password; };

		int 							setSocketFd(int port);
};
#endif