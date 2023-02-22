#include "../Server/Server.hpp"

int Server::killCmd(const Message &msg, User &user) {

	if (!(user.getFlags() & IRCOPERATOR)) {
		return (sendError(user, "This user does not have required privileges."));
	}

	if (msg.getParams().size() < 2) {
		return (sendError(user, "Need more parameters."));
	}

	string username = msg.getParams()[0];
	if (username == this->name) {
		return (sendError(user, "Cannot kill server."));	
	}

	if (!containsNickname(username)) {
		sendError(user, "No such nickname : ", msg.getParams()[0]);
	}
	User *userToKill = getUserByName(username);	
	userToKill->sendMessage(msg.getParams()[1] + "\n");
	userToKill->setFlag(BREAKCONNECTION);
	return 0;
}

int Server::restartCmd(User &user) {

	if (!(user.getFlags() & IRCOPERATOR)) {
		return (Error(user, "This user does not have required privileges."));
	}

	std::vector<User *>::iterator beg = connectedUsers.begin();
	std::vector<User *>::iterator end = connectedUsers.end();
	for (; beg != end; beg++) {
		(*beg)->setFlag(BREAKCONNECTION);
	}

	loadConfig();
	close(sockfd);
	createSocket();
	bindSocket();
	listenSocket();
	return 0;
}