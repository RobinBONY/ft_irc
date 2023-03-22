/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vducoulo <vducoulo@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/07 12:49:24 by vducoulo          #+#    #+#             */
/*   Updated: 2023/03/22 12:10:54 by vducoulo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CPP_COMMAND_HPP
# define CPP_COMMAND_HPP

# include "../../includes/Irc.hh"
# include "../user/user.hpp"
# include "../server/server.hpp"

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
	void							cmdKick();
	
	void							errUnknowCommand();
	int								errNeedMoreParams(int minimalparameterscount);
	int								errNotOperator(std::string chan_name);
	int								errNotUserChannel(std::string chan_name);
	void							errNotRegistred();
	
	public :

	Command(std::string name, const std::vector<std::string> params, User *relativeuser, Server *realtiveserver);
	~Command();
	
	void execute();
};

#endif