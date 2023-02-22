#include <string>
#include <vector>
#include "utils.hpp"

class Message
{
	private:
		std::string						prefix;
		std::string						command;
		std::vector<std::string>		params;

		Message();
		Message(const Message& copy);
		Message	&operator=(const Message& other);
	public:
		Message(const std::string &str);
		virtual ~Message();
		const std::string				&getPrefix() const;
		const std::string				&getCommand() const;
		const std::vector<std::string>	&getParams() const;
};