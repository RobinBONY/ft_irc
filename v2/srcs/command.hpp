/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vducoulo <vducoulo@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/07 12:49:24 by vducoulo          #+#    #+#             */
/*   Updated: 2023/03/07 15:08:22 by vducoulo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CPP_COMMAND_HPP
# define CPP_COMMAND_HPP

# include "../includes/Irc.hh"
# include "user.hpp"

class Command
{
	private :

	std::string 	_name;
	bool			_auth_required;
	
	public :
	
	Command(std::string name, bool authrequiered);
	virtual ~Command();

	std::string 	getName() { return (_name); }
	virtual void	launch(std::vector<std::string> parmeters, User relative_user) = 0;
};

class cmdUser : public Command
{
	public : 
		cmdUser(std::string name, bool authrequiered)
		: Command(name, authrequiered){}
		~cmdUser() {}
		void launch(std::vector<std::string> params, User relative_user)
		{
			std::cout << "CMD USER EXECUTED" << std::endl;
		};
};

#endif