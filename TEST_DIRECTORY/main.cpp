/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vducoulo <vducoulo@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/23 16:18:29 by vducoulo          #+#    #+#             */
/*   Updated: 2023/02/24 18:14:45 by vducoulo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <poll.h>
#include <vector>
#include <map>

# define USR_HANDSHAKE 128
# define USR_LIVE 256
# define USR_TOKILL 512

class Command {
	
	protected:
	bool _needLogin;
	
	public:
	
	explicit Command(bool needLogin = true) : _needLogin(needLogin){};
	virtual ~Command(){};
	virtual void launch(std::vector<std::string> params) = 0;
};

class cmdPass : public Command
{
	public : 
		cmdPass(bool needLogin) : Command(needLogin){};
		~cmdPass();
		void launch(std::vector<std::string> params)
		{
			std::cout << "CMD PASS EXECUTED" << std::endl;
		}
};

class cmdUser : public Command
{
	public : 
		cmdUser(bool needLogin) : Command(needLogin){};
		~cmdUser();
		void launch(std::vector<std::string> params)
		{
			std::cout << "CMD USER EXECUTED" << std::endl;
		}
};

class cmdNick : public Command
{
	public : 
		cmdNick(bool needLogin) : Command(needLogin){};
		~cmdNick();
		void launch(std::vector<std::string> params)
		{
			std::cout << "CMD NICK EXECUTED" << std::endl;
		}
};

class commandProxy {

	private:
	std::map<std::string, Command *> _commands;

public:
	commandProxy()
	{
		_commands["PASS"] = new cmdPass(false);
		_commands["NICK"] = new cmdNick(false);
		_commands["USER"] = new cmdUser(false);
	}
	~commandProxy() {};


	void callCommand(const std::string cmd, std::vector<std::string> params)
	{
		Command *command = _commands.at(cmd);
		command->launch(params);
	}
};

struct testmessage {
	std::string 				prefix;
	std::vector<std::string>	params;
	std::string 				cmd;
};

struct testuser {
	int sock_fd;
	int state;
};

struct testserver {
	int 							sock_fd;
	std::vector<pollfd>				pfds;
	sockaddr_in 					sockaddr;
};
testserver server;
testuser usr;
testmessage msg;

std::vector<std::string> split(std::string str, std::string delimiter)
{
	std::vector<std::string> values = std::vector<std::string>();

	size_t position;
	while ((position = str.find(delimiter)) != std::string::npos)
	{
		values.push_back(str.substr(0, position));
		str.erase(0, position + delimiter.length());
	}
	values.push_back(str);

	return values;
}

void setMsgStructValues(std::string message)
{
	std::cout << std::endl << "new_msg =" << message << std::endl;
	size_t pos;
	if ((pos = message.find(std::string(":"))) && pos != std::string::npos)
	{
		std::string tmp = message.substr(0, pos);
		message.erase(0, pos + 1);
		msg.cmd = message;
		message = tmp;
	}

	msg.params = split(message, " ");
	msg.prefix = *(msg.params.begin());
	msg.params.erase(msg.params.begin());

	std::cout << "prefix =" << msg.prefix << std::endl;
	for (std::vector<std::string>::iterator iter = msg.params.begin(); iter != msg.params.end(); iter++)
		if (*iter != " ")
			std::cout << "parameters = " << *iter << std::endl;
	std::cout << "cmd :" << msg.cmd << std::endl;
	commandProxy cmdprox;
	cmdprox.callCommand(msg.cmd, msg.params);
}

void interpretMsg(char msgbuff[513])
{
	int pos;
	std::string rawmessage(msgbuff);
	std::cout << "rawmsg =" << rawmessage << "\\\\ENDOFMSG" << std::endl << std::endl;
	while ((pos = rawmessage.find(std::string("\r\n"))) && pos != std::string::npos)
	{
		std::string message = rawmessage.substr(0, pos);
		rawmessage.erase(0, pos + 2);
		if (!message.length())
			continue;
		setMsgStructValues(message);
	}
}
void receive_msg(int fd)
{
	char msgbuff[513];
	size_t MsgLen = recv(fd, &msgbuff, 512, 0);
	msgbuff[512] = 0;
	
	interpretMsg(msgbuff);
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
		userpfd.revents = 0;
		userpfd.events = POLLIN;
		usr.state = USR_HANDSHAKE;
		server.pfds.push_back(userpfd);
		std::cout << "connection ! (" << NewConnection << ")" << std::endl;
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