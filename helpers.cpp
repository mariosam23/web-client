#include "helpers.hpp"

#include <arpa/inet.h>
#include <netdb.h>		/* struct hostent, gethostbyname */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <stdio.h>
#include <stdlib.h>		/* exit, atoi, malloc, free */
#include <string.h>		/* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <unistd.h>		/* read, write, close */

#include <iostream>
#include <string>

#include "buffer.hpp"

using namespace std;

#define HEADER_TERMINATOR "\r\n\r\n"
#define HEADER_TERMINATOR_SIZE (sizeof(HEADER_TERMINATOR) - 1)
#define CONTENT_LENGTH "Content-Length: "
#define CONTENT_LENGTH_SIZE (sizeof(CONTENT_LENGTH) - 1)

void error(const char *msg) {
	perror(msg);
	exit(0);
}

void compute_message(char *message, const char *line) {
	strcat(message, line);
	strcat(message, "\r\n");
}

int open_connection(char *host_ip, int portno, int ip_type, int socket_type,
					int flag) {
	struct sockaddr_in serv_addr;
	int sockfd = socket(ip_type, socket_type, flag);
	if (sockfd < 0) error("ERROR opening socket");

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = ip_type;
	serv_addr.sin_port = htons(portno);
	inet_aton(host_ip, &serv_addr.sin_addr);

	/* connect the socket */
	if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
		error("ERROR connecting");

	return sockfd;
}

void close_connection(int sockfd) { close(sockfd); }

void send_to_server(int sockfd, char *message) {
	int bytes, sent = 0;
	int total = strlen(message);

	do {
		bytes = write(sockfd, message + sent, total - sent);
		if (bytes < 0) {
			error("ERROR writing message to socket");
		}

		if (bytes == 0) {
			break;
		}

		sent += bytes;
	} while (sent < total);
}

char *receive_from_server(int sockfd) {
	char response[BUFLEN];
	buffer buffer = buffer_init();
	int header_end = 0;
	int content_length = 0;

	do {
		int bytes = read(sockfd, response, BUFLEN);

		if (bytes < 0) {
			error("ERROR reading response from socket");
		}

		if (bytes == 0) {
			break;
		}

		buffer_add(&buffer, response, (size_t)bytes);

		header_end =
			buffer_find(&buffer, HEADER_TERMINATOR, HEADER_TERMINATOR_SIZE);

		if (header_end >= 0) {
			header_end += HEADER_TERMINATOR_SIZE;

			int content_length_start = buffer_find_insensitive(
				&buffer, CONTENT_LENGTH, CONTENT_LENGTH_SIZE);

			if (content_length_start < 0) {
				continue;
			}

			content_length_start += CONTENT_LENGTH_SIZE;
			content_length =
				strtol(buffer.data + content_length_start, NULL, 10);
			break;
		}
	} while (1);
	size_t total = content_length + (size_t)header_end;

	while (buffer.size < total) {
		int bytes = read(sockfd, response, BUFLEN);

		if (bytes < 0) {
			error("ERROR reading response from socket");
		}

		if (bytes == 0) {
			break;
		}

		buffer_add(&buffer, response, (size_t)bytes);
	}
	buffer_add(&buffer, "", 1);
	return buffer.data;
}

char *basic_extract_json_response(char *str) { return strstr(str, "{\""); }

string get_cookie(string response) {
	size_t cookie_start = response.find("Set-Cookie: ");
	if (cookie_start != string::npos) {
		cookie_start += strlen("Set-Cookie: ");
		size_t cookie_end = response.find(";", cookie_start);
		if (cookie_end != string::npos)
			return response.substr(cookie_start, cookie_end - cookie_start);
	}
    
    return "";
}

string get_jwt_token(string response) {
    size_t token_start = response.find("token");
    if (token_start != string::npos) {
        token_start += strlen("token\":\"");
        size_t token_end = response.find("\"", token_start);
        if (token_end != string::npos)
            return response.substr(token_start, token_end - token_start);
    }

    return "";
}
