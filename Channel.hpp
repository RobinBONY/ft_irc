/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbony <rbony@corobizar.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/25 13:03:08 by rbony             #+#    #+#             */
/*   Updated: 2023/01/25 14:24:09 by rbony            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

class Server;
class User;

#include <string>
#include <vector>
#include <map>
#include <ctime>

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "User.hpp"
#include "Server.hpp"

class Channel
{
private:
    std::string                 name;
    std::string                 password;
    std::vector<const User *>   users;
    std::vector<const User *>   operators;
    
    Channel();
	Channel(const Channel& copy);
	Channel	&operator=(const Channel& other);

public:
    Channel(const std::string &name, const User &creator, const std::string &pass = "");
    ~Channel();

    const std::string   &getName() const;
    bool                isOperator(const User &user) const;
    bool                containsNickname(const std::string &nickname) const;
    void                disconnect(const User &user);
    void                addOperator(const User &user);
    void                removeOperator(const User &user);
    void                sendMessage(const std::string &message, const User &from, bool includeUser) const;
};

std::ostream &operator<<(std::ostream &os, const Channel &f);

#endif