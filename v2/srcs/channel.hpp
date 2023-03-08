/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vducoulo <vducoulo@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 18:30:30 by vducoulo          #+#    #+#             */
/*   Updated: 2023/03/08 19:28:26 by vducoulo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CPP_CHANNEL_HPP
# define CPP_CHANNEL_HPP

# include "../includes/Irc.hh"

class Channel
{
	private : 

	std::string		_name;
	std::string		_password;

	public : 

	Channel(std::string name, std::string pass);
	~Channel();

	std::string getName(void)	{return _name; }
};

#endif