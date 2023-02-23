/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vducoulo <vducoulo@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/23 16:18:29 by vducoulo          #+#    #+#             */
/*   Updated: 2023/02/23 22:40:01 by vducoulo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <poll.h>
#include <vector>

bool g_run = false;

struct testuser {
	int sock_fd;
};

struct testserver {
	int 				sock_fd;
	std::vector<pollfd>				pfds;
	sockaddr_in 		sockaddr;
};
testserver server;
testuser usr;

void receive_msg(int fd)
{
	char msgbuff[513];
	size_t MsgLen = recv(fd, &msgbuff, 512, 0);
	msgbuff[512] = 0;
	std::cout << "msg (fd " << fd << ") =" << msgbuff << std::endl;
}

int userHandshake()
{
	size_t addr_len = sizeof(server.sockaddr);
	pollfd userpfd;
	int NewConnection = accept(server.sock_fd, (struct sockaddr *)&server.sockaddr, (socklen_t*)&addr_len);
	if (NewConnection >= 0)
	{
		usr.sock_fd = NewConnection;
		userpfd.fd = NewConnection;
		userpfd.events = POLLIN;
		server.pfds.push_back(userpfd);
		std::cout << "connection ! (" << NewConnection << ")" << std::endl;
		g_run = true;
		return (1);
	}
	return (0);
}

void initTestServer(void)
{
	const int flag = 1;
	pollfd servpfd;
	
	server.sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	std::cout << "serv_skt :" << server.sock_fd << std::endl;
	
	std::cout << setsockopt(server.sock_fd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(int)) << std::endl;
	
	server.sockaddr.sin_addr.s_addr = INADDR_ANY;
	server.sockaddr.sin_family = AF_INET;
	server.sockaddr.sin_port = htons(9090);
	
	std::cout << bind(server.sock_fd, (struct sockaddr*)&server.sockaddr, sizeof(server.sockaddr)) << std::endl; // alternative : getnameinfo <netdb.h>

	std::cout << listen (server.sock_fd, 1) << std::endl;
	fcntl (server.sock_fd, F_SETFL, O_NONBLOCK);

	servpfd.fd = server.sock_fd;
	servpfd.events = POLLIN;
	server.pfds.push_back(servpfd);
}

int serverRoutine(void)
{
	if(poll(&server.pfds[0], server.pfds.size(), 100) < 0)
		std::cerr << "poll error" << std::endl;
	if (server.pfds[0].revents == POLLIN)
		return (userHandshake());
	for (std::vector<pollfd>::iterator iter = server.pfds.begin(); iter != server.pfds.end(); iter++)
		if ((*iter).revents == POLLIN)
			receive_msg((*iter).fd);
	return 1;
}

int main(void)
{
	std::cout << std::endl << "TEST SERVER PORT : 9090" << std::endl;
	std::cout << "TEST SERVER PASS : TEST" << std::endl << std::endl;
	usr.sock_fd = 0;
	initTestServer();
	while (serverRoutine())
		usleep(10000);

}