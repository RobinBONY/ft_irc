/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vducoulo <vducoulo@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 11:04:42 by vducoulo          #+#    #+#             */
/*   Updated: 2023/02/21 11:05:06 by vducoulo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Irc.hh"

int main() {
    // Create socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // Connect to server
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(1234);
    connect(sockfd, (struct sockaddr*) &server_addr, sizeof(server_addr));

    // Read and write data to the server
    // ...

    // Close the socket
    close(sockfd);

    return 0;
}