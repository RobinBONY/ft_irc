/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alakhdar <alakhdar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/01 17:35:58 by vducoulo          #+#    #+#             */
/*   Updated: 2023/06/01 13:10:45 by alakhdar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CPP_SERVER_HPP
# define CPP_SERVER_HPP

# include "../../includes/irc.hh"
# include "../user/user.hpp"
# include "../channel/channel.hpp"

class Server
{
	private :
		std::string				_password;
		sockaddr_in				_server_sockaddr;

		std::vector<pollfd> 	_pfds;
		std::vector<User *>		_users;
		std::vector<Channel *>	_channels;
		
		Channel					*createChannel(std::string name, std::string pass);
		
	public :
		Server(char *port, char *pass);
		~Server();

		void							runLoop(void);
		void							userHandShake(void);
		void							receiveMsgs(int fd);
		void							executeMsgs(int fd);

		User							*getRelativeUser(int fd);
		User							*getReltiveUserPerNick(std::string usrnick);
		Channel							*getSetRelativeChannel(std::string name, std::string pass);
		std::string						getPassword(void) { return _password; };

		int 							setSocketFd(uint16_t port);
		void							deleteChannel(std::string chan_name);
		void							deleteUser(User *usr);
};
#endif