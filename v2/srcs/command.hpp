/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vducoulo <vducoulo@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/07 12:49:24 by vducoulo          #+#    #+#             */
/*   Updated: 2023/03/13 19:20:44 by vducoulo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CPP_COMMAND_HPP
# define CPP_COMMAND_HPP

# include "../includes/Irc.hh"
# include "user.hpp"
# include "server.hpp"

class Command
{
	private :

	typedef void 					(Command::* cmdPtr)();
	std::map<std::string, cmdPtr> 	_cmd_ptr;
	std::string 					_name;
	bool							_auth_required;
	std::vector<std::string>		_parameters;
	User							*_relative_user;
	Server							*_relative_server;
	
	void							cmdUser();
	void							cmdCap();
	void							cmdNick();
	void							cmdPass();
	void							cmdJoin();
	void							cmdMode();
	void							cmdPrivmsg();
	void							cmdPing();
	void							cmdQuit();
	void							cmdPart();
	
	void							errUnknowCommand();
	int								errNeedMoreParams(int minimalparameterscount);
	void							errNotRegistred();
	
	public :

	Command(std::string name, const std::vector<std::string> params, User *relativeuser, Server *realtiveserver);
	~Command();
	
	void execute();
};

#endif