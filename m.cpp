/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   m.cpp                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddaou < mhaddaou@student.1337.ma>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/14 22:09:01 by mhaddaou          #+#    #+#             */
/*   Updated: 2023/01/15 10:59:40 by mhaddaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <iostream>
# include <string>
# include <cstring>
# include <netdb.h>
# include <cstdlib>
# include <sys/socket.h>
# include <netinet/in.h>
# include <fstream>
# include <unistd.h>
# include <vector>
# include <sstream>
const int MAX_CLIENTS = 10;
const int BUF_SIZE = 1024;

int create_socket() {
    // Create socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        std::cerr << "Error creating socket" << std::endl;
        exit(1);
    }
    return sockfd;
}

void set_server_addr(int sockfd, sockaddr_in &server_addr) {
    // Set server address information
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(6668);
}

void bind_socket(int sockfd, sockaddr_in &server_addr) {
    // Bind socket to address and port
    if (bind(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Error binding socket" << std::endl;
        exit(1);
    }
}

void start_listening(int sockfd) {
    // Start listening for incoming connections
    if (listen(sockfd, MAX_CLIENTS) < 0) {
        std::cerr << "Error listening on socket" << std::endl;
        exit(1);
    }
}

void handle_new_connection(int sockfd, std::vector<int> &clients) {
    int clientfd = accept(sockfd, NULL, NULL);
    if (clientfd < 0) {
        std::cerr << "Error accepting connection" << std::endl;
        return;
    }
    // Add new client to connected clients vector
    clients.push_back(clientfd);
    std::cout << "New client connected" << std::endl;
}

void handle_disconnected_client(int i, std::vector<int> &clients) {
    close(clients[i]);
    clients.erase(clients.begin() + i);
    std::cout << "Client disconnected" << std::endl;
}
void handle_received_message(int i, std::vector<int> &clients, char buf[], int bytes_received) {
    std::cout << "Received message: " << buf << std::endl;
    memset(buf, 0, BUF_SIZE);
    // Add code here to handle the received message and send it to other clients as necessary
    // for (int j = 0; j < clients.size(); j++) {
    //     if (j != i) {
    //         send(clients[j], buf, bytes_received, 0);
    //     }
    // }
}
void handle_client_activity(int i, std::vector<int>& clients, char* buf) {
    int bytes_received = recv(clients[i], buf, BUF_SIZE, 0);
    if (bytes_received <= 0) {
        handle_disconnected_client(i, clients);
    } else {
        handle_received_message(i, clients, buf, bytes_received);
    }
}
void handle_clients(int sockfd) {
    // Vector to store connected client file descriptors
    std::vector<int> clients;
    char buf[BUF_SIZE];

    // Set up file descriptor set for select()
    fd_set readfds;

    while (true) {
        // Clear the file descriptor set
        FD_ZERO(&readfds);

        // Add the socket and connected clients to the file descriptor set
        FD_SET(sockfd, &readfds);
        for (int i = 0; i < clients.size(); i++) {
            FD_SET(clients[i], &readfds);
        }

        // Wait for activity on any file descriptor
        int activity = select(FD_SETSIZE, &readfds, NULL, NULL, NULL);
        if (activity < 0) {
            std::cerr << "Error in select" << std::endl;
            exit(1);
        }

        // Check if there is a new incoming connection
        if (FD_ISSET(sockfd, &readfds)) {
            handle_new_connection(sockfd, clients);
        }

        // Check if any connected clients have sent data
        for (int i = 0; i < clients.size(); i++) {
            if (FD_ISSET(clients[i], &readfds)) {
                handle_client_activity(i, clients, buf);
            }
        }
    }
}





int main() {
    // Create socket
    int sockfd = create_socket();

    // Set server address information and bind the socket
    struct sockaddr_in server_addr;
    set_server_addr(sockfd, server_addr);
    bind_socket(sockfd, server_addr);

    // Start listening for incoming connections
    start_listening(sockfd);

    handle_clients(sockfd);
    return 0;
}

