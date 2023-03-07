/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vducoulo <vducoulo@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/01 17:35:58 by vducoulo          #+#    #+#             */
/*   Updated: 2023/03/07 15:09:26 by vducoulo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CPP_SERVER_HPP
# define CPP_SERVER_HPP

# include "../includes/Irc.hh"
# include "user.hpp"
# include "command.hpp"

class Server
{
	private :
		bool					_active;
		std::string				_password;
		sockaddr_in				_server_sockaddr;

		std::vector<pollfd> 	_pfds;
		std::vector<User>		_users;

		std::vector<Command *> 	_commands_array;

		bool					_debug;
		
	public :
		Server(char *port, char *pass);

		int 					setSocketFd(int port);
		void					runLoop(void);
		void					userHandShake(void);
		void					receiveMsg(int fd);
		void					commandHandler(std::string command, std::vector<std::string> parameters, User relative_user);

		User					getRelativeUser(int fd);
};
#endif