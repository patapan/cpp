#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>

#define PORT 80

/*
Fetch HTML from a website using TCP sockets in C++:

1. Create a TCP socket
2. Resolve the hostname to an IP address. (DNS lookup)
3. Connect the socket to the web server's IP address and port 80 (the default HTTP port).
4. Send an HTTP GET request to the server.
5. Receive the HTTP response.
6. Close the socket.
*/

int main() {
    const char *hostname = "www.google.com";
    const char *path = "/";

    // Resolve the hostname to an IP address
    struct hostent *he;
    struct in_addr **addr_list;
    if ((he = gethostbyname(hostname)) == NULL) { // udp
        herror("gethostbyname");
        return 1;
    }
    addr_list = (struct in_addr **)he->h_addr_list;

    char ip[100];
    strcpy(ip, inet_ntoa(*addr_list[0]));

    // Create a socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket");
        return 1;
    }

    // Set up the server address struct
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, ip, &server_addr.sin_addr) <= 0) {
        perror("inet_pton");
        close(sock);
        return 1;
    }

    // Connect to the server
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect");
        close(sock);
        return 1;
    }

    // Prepare the HTTP GET request
    char request[200];
    snprintf(request, sizeof(request), "GET %s HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n", path, hostname);

    // Send the request
    if (send(sock, request, strlen(request), 0) < 0) {
        perror("send");
        close(sock);
        return 1;
    }

    // Receive the response
    char response[10000];
    int bytes_received;
    while ((bytes_received = recv(sock, response, sizeof(response) - 1, 0)) > 0) {
        response[bytes_received] = '\0';  // Null-terminate the response
        std::cout << response;
    }

    if (bytes_received < 0) {
        perror("recv");
    }

    // Close the socket
    close(sock);

    return 0;
}
