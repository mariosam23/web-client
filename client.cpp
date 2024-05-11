#include <arpa/inet.h>
#include <netdb.h>		/* struct hostent, gethostbyname */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <stdlib.h>		/* exit, atoi, malloc, free */
#include <string.h>		/* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <unistd.h>		/* read, write, close */

#include <iostream>
#include <nlohmann/json.hpp>
#include <string>
#include <unordered_set>
#include <vector>

#include "helpers.hpp"
#include "requests.hpp"
#include "utils.hpp"
#include "validator.hpp"

using namespace std;

int main(int argc, char *argv[]) {
	unordered_set<string> all_users;
	unordered_set<string> logged_users;
	int sockfd;
	string jwt_token, cookie;

	cout << INIT_MSG;

	while (true) {
		string command;
		getline(cin, command);

		if (command == "exit") {
			return SUCCESS_EXIT_CODE;
		} else if (command == "register") {
			string username, password;
			cout << "username=";
			getline(cin, username);
			cout << "password=";
			getline(cin, password);

			if (!check_string_with_no_spaces(username) ||
				!check_string_with_no_spaces(password)) {
				cout << "Invalid username or password\n";
				continue;
			}

			if (all_users.find(username) != all_users.end()) {
				cout << "User already exists\n";
				continue;
			}

			if (logged_users.find(username) != logged_users.end()) {
				cout << "User already logged in\n";
				continue;
			}

			all_users.insert(username);
			all_users.insert(username);
			nlohmann::json j;
			j["username"] = username;
			j["password"] = password;

			string payload = j.dump();

			sockfd = open_connection(HOST_IP, PORT, AF_INET, SOCK_STREAM, 0);
			char *message =
				compute_post_request(HOST_IP, REGISTER_ROUTE, (char *)CONTENT_TYPE,
									 (char **)payload.c_str(), 1, NULL, 0);
			send_to_server(sockfd, message);
            char *response = receive_from_server(sockfd);
            cout << response << endl;
			close(sockfd);
		} else {
			cout << "Unknown command\n";
		}
	}

	return SUCCESS_EXIT_CODE;
}
