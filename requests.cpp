#include "requests.hpp"

#include <arpa/inet.h>
#include <netdb.h>		/* struct hostent, gethostbyname */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <stdio.h>
#include <stdlib.h>		/* exit, atoi, malloc, free */
#include <string.h>		/* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <unistd.h>		/* read, write, close */

#include <string>

#include "helpers.hpp"
#include "utils.hpp"

using namespace std;

string compute_get_request(char *host, char *url, char *query_params,
						   user_data_t &user_data) {
	string message = "";
	string line = "";

	if (query_params)
		line = "GET " + string(url) + "?" + query_params + " HTTP/1.1";
	else
		line = "GET " + string(url) + " HTTP/1.1";

	message += line + "\r\n";

	line = "Host: " + string(host);
	message += line + "\r\n";

	if (user_data.has_library_access) {
		line = "Authorization: Bearer " + user_data.jwt_token;
		message += line + "\r\n";
	}

	if (!user_data.cookie.empty()) {
		line = "Cookie: " + user_data.cookie;
		message += line + "\r\n";
	}

	message += "\r\n";
	return message;
}

string compute_delete_request(char *host, char *url, char *query_params,
							  user_data_t &user_data) {
	string message = "";
	string line = "";

	if (query_params)
		line = "DELETE " + string(url) + "?" + query_params + " HTTP/1.1";
	else
		line = "DELETE " + string(url) + " HTTP/1.1";

	message += line + "\r\n";

	line = "Host: " + string(host);
	message += line + "\r\n";

	if (user_data.has_library_access) {
		line = "Authorization: Bearer " + user_data.jwt_token;
		message += line + "\r\n";
	}

	if (!user_data.cookie.empty()) {
		line = "Cookie: " + user_data.cookie;
		message += line + "\r\n";
	}

	message += "\r\n";
	return message;
}

string compute_post_request(char *host, char *url, string payload,
							user_data_t &user_data) {
	string message = "";
	string line = "";

	line = "POST " + string(url) + " HTTP/1.1";
	message += line + "\r\n";

	line = "Host: " + string(host);
	message += line + "\r\n";

	line = "Content-Type: " + CONTENT_TYPE;
	message += line + "\r\n";

	if (user_data.has_library_access) {
		line = "Authorization: Bearer " + user_data.jwt_token;
		message += line + "\r\n";
	}

	if (!user_data.cookie.empty()) {
		line = "Cookie: " + user_data.cookie;
		message += line + "\r\n";
	}

	line = "Content-Length: " + to_string(payload.size());
	message += line + "\r\n";

	message += "\r\n" + payload + "\r\n";

	return message;
}
