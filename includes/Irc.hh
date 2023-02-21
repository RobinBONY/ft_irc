/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Irc.hh                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vducoulo <vducoulo@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 11:02:09 by vducoulo          #+#    #+#             */
/*   Updated: 2023/02/21 11:12:28 by vducoulo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CPP_IRC_HH
# define CPP_IRC_HH

/*
*	Lib includes
*/

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ctime>
#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <errno.h>
#include <fcntl.h>
#include <fstream>
#include <poll.h>
#include <string>
#include <time.h>
#include <sstream>
#include <set>
#include <algorithm>
#include <csignal>

// std

#include <vector>
#include <map>
#include <queue>

/*
 * Local includes 
 */

#include "../srcs/Channel/Channel.hpp"
#include "../srcs/User/User.hpp"
#include "../srcs/Server/Server.hpp"

#endif