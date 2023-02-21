/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vducoulo <vducoulo@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/25 13:03:08 by rbony             #+#    #+#             */
/*   Updated: 2023/02/21 11:06:13 by vducoulo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

class Server;
class User;

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "../../includes/Irc.hh"

class Channel
{
    private:
        std::string                 _name;
        std::string                 _password;
        std::vector<const User *>   _users;
        std::vector<const User *>   _operators;
        
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