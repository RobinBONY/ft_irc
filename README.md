# FT_IRC, a simple c++ 98 IRC server.

## Get started

Ft_irc is a school project created at Ecole42.
It supports basic IRC operations like creating and interacting with a channel, send private messages to other users and sonme IRCops operations.
This server is intended to work with Weechaat client.


## Supported Commands



### Joining a server

the server must be added in the Weechat client side, with the "--password=xxx" option if provided at server launch

**/server add {DESIRED SERVER NAME} {IP}/{SERVER SPECIFIED PORT} --password={SERVER SPECIFIED PASSWORD}**

then to connect :

**/connect server_name**



### Set a proper user nickname

at connect, ft_irc automatically set a nickname dor the user in the form "ft_user + digit". user can change his name with the /nick command

**/nick {DESIRED NICK}**



### Join a channel

to create or join a channel, use /join command. **The server name must start with "#".**

If the channel is a new channel, the user is automatically this channel IRCop.

**/join {CHANNEL_NAME}**



### Quit a channel

**/part**


### Send a private message

**/msg {RECEIVER NICK}**



### Server Administration

an IRCop user can administrate his own channel thanks to /kick and /mode commands

#kick a connected user from the channel

**/kick {TOKICK_NICKNAME}**

#set channel passord

**/mode +k {PASS}**

#set a max users limit

**/mode +l {INT_LIMIT}**

#ban a user

**/mode +b {TOBAN_NICK}**

#block server outside message access

**/mode +n**

#transmit IRCop priviledge to anothe channel user

**/mode +o {NICK}**

#remove policies

**/mode -{n / b / l / k / o} {TARGETTED_NICK}**



### Leave the server

**/bye**
