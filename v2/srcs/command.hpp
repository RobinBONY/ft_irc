/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vducoulo <vducoulo@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/07 12:49:24 by vducoulo          #+#    #+#             */
/*   Updated: 2023/03/07 19:36:27 by vducoulo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CPP_COMMAND_HPP
# define CPP_COMMAND_HPP

# include "../includes/Irc.hh"
# include "user.hpp"

class Command
{
	private :

	typedef void 					(Command::* cmdPtr)();
	std::map<std::string, cmdPtr> 	_cmd_ptr;
	std::string 					_name;
	bool							_auth_required;
	std::vector<std::string>		_parameters;
	User							_relative_user;
	
	void							cmdUser();
	void							cmdCap();
	void							cmdNick();
	void							cmdPass();
	
	public :

	Command(std::string name, const std::vector<std::string> params, User relativeuser);
	~Command();
	
	void execute() {(this->*_cmd_ptr[_name])(); }
};

#endif