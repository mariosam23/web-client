#include "commands.hpp"

#include <arpa/inet.h>
#include <netdb.h>		/* struct hostent, gethostbyname */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <stdlib.h>		/* exit, atoi, malloc, free */
#include <string.h>		/* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <unistd.h>		/* read, write, close */

#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "helpers.hpp"
#include "nlohmann/json.hpp"
#include "requests.hpp"
#include "utils.hpp"
#include "validator.hpp"

using namespace std;
using json = nlohmann::json;

// All commands available in the application
std::unordered_map<std::string, Command> get_command_map() {
	return {{"register", REGISTER},
			{"login", LOGIN},
			{"enter_library", ENTER_LIBRARY},
			{"get_books", GET_BOOKS},
			{"get_book", GET_BOOK},
			{"add_book", ADD_BOOK},
			{"delete_book", DELETE_BOOK},
			{"logout", LOGOUT},
			{"exit", EXIT}};
}

void register_command(user_data_t& user_data) {
	string username, password;
	cout << "username=";
	getline(cin, username);
	cout << "password=";
	getline(cin, password);

	if (!check_string_with_no_spaces(username) ||
		!check_string_with_no_spaces(password)) {
		cout << "Invalid username or password\n";
		return;
	}

	json j;
	j["username"] = username;
	j["password"] = password;

	string payload = j.dump();

	int sockfd = open_connection(HOST_IP, PORT, AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		cout << "Failed to open connection\n";
		return;
	}

	string message = compute_post_request(HOST_IP, REGISTER_ROUTE,
										  (char*)payload.c_str(), user_data);
	send_to_server(sockfd, (char*)message.c_str());
	string response = receive_from_server(sockfd);

	if (response.find("201 Created") != string::npos)
		cout << "User registered with SUCCESS!\n";
	else if (response.find("error") != string::npos)
		cout << "ERROR! User already exists\n";

	close(sockfd);
}

void login_command(user_data_t& user_data) {
	string username, password;
	cout << "username=";
	getline(cin, username);
	cout << "password=";
	getline(cin, password);

	if (!check_string_with_no_spaces(username) ||
		!check_string_with_no_spaces(password)) {
		cout << "Invalid username or password\n";
		return;
	}

	if (user_data.is_logged_in) {
		cout << "User already logged in!\n";
		return;
	}

	json j;
	j["username"] = username;
	j["password"] = password;

	string payload = j.dump();

	int sockfd = open_connection(HOST_IP, PORT, AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		cout << "Failed to open connection\n";
		return;
	}

	string message = compute_post_request(HOST_IP, LOGIN_ROUTE,
										  (char*)payload.c_str(), user_data);
	send_to_server(sockfd, (char*)message.c_str());
	string response = receive_from_server(sockfd);

	if (response.find("200 OK") != string::npos) {
		cout << "User logged in with SUCCESS!\n";
		user_data.is_logged_in = true;
		user_data.cookie = get_cookie(response);
	} else if (response.find("error") != string::npos) {
		cout << "ERROR! Invalid username or password\n";
	}

	close(sockfd);
}

void enter_library_command(user_data_t& user_data) {
	if (!user_data.is_logged_in) {
		cout << "ERROR! You must be logged in to access the library!\n";
		return;
	}

	if (user_data.has_library_access) {
		cout << "You already have access to the library!\n";
		return;
	}

	int sockfd = open_connection(HOST_IP, PORT, AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		cout << "Failed to open connection\n";
		return;
	}

	string message =
		compute_get_request(HOST_IP, LIBRARY_ACCESS_ROUTE, NULL, user_data);
	send_to_server(sockfd, (char*)message.c_str());
	string response = receive_from_server(sockfd);

	if (response.find("200 OK") != string::npos) {
		cout << "You have access to the library!\n";
		user_data.has_library_access = true;
		user_data.jwt_token = get_jwt_token(response);
	} else if (response.find("error") != string::npos) {
		cout << "ERROR! Something went wrong\n";
	}

	close(sockfd);
}

void get_books_command(user_data_t& user_data) {
	if (!user_data.is_logged_in) {
		cout << "ERROR! You must be logged in to access the library!\n";
		return;
	}

	if (!user_data.has_library_access) {
		cout << "ERROR! You don't have access to the library!\n";
		return;
	}

	int sockfd = open_connection(HOST_IP, PORT, AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		cout << "Failed to open connection\n";
		return;
	}

	string message =
		compute_get_request(HOST_IP, LIBRARY_BOOKS_ROUTE, NULL, user_data);
	send_to_server(sockfd, (char*)message.c_str());
	string response = receive_from_server(sockfd);

	if (response.find("200 OK") != string::npos) {
		size_t json_start = response.find("[");
		size_t json_end = response.find("]") + 1;
		string json_str = response.substr(json_start, json_end - json_start);
		cout << json_str << endl;
	} else if (response.find("error") != string::npos) {
		cout << "ERROR! Something went wrong accessing library\n";
	}

	close(sockfd);
}

void get_book_command(user_data_t& user_data) {
	if (!user_data.is_logged_in) {
		cout << "ERROR! You must be logged in to access the library!\n";
		return;
	}

	if (!user_data.has_library_access) {
		cout << "ERROR! You don't have access to the library!\n";
		return;
	}

	string id;
	cout << "id=";
	getline(cin, id);

	if (!check_number(id)) {
		cout << "Invalid book id\n";
		return;
	}

	int sockfd = open_connection(HOST_IP, PORT, AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		cout << "Failed to open connection\n";
		return;
	}

	string book_id_url = (string)LIBRARY_BOOKS_ROUTE + "/" + id;
	string message = compute_get_request(HOST_IP, (char*)book_id_url.c_str(),
										 NULL, user_data);
	send_to_server(sockfd, (char*)message.c_str());
	string response = receive_from_server(sockfd);
	if (response.find("200 OK") != string::npos) {
		size_t json_start = response.find("{");
		size_t json_end = response.find("}") + 1;
		string json_str = response.substr(json_start, json_end - json_start);
		cout << json_str << endl;
	} else if (response.find("error") != string::npos) {
		cout << "ERROR! Invalid book id\n";
	}

	close(sockfd);
}

void add_book_command(user_data_t& user_data) {
	if (!user_data.is_logged_in) {
		cout << "ERROR! You must be logged in to access the library!\n";
		return;
	}

	if (!user_data.has_library_access) {
		cout << "ERROR! You don't have access to the library!\n";
		return;
	}

	string title, author, genre, publisher, page_count;

	cout << "title=";
	getline(cin, title);
	cout << "author=";
	getline(cin, author);
	cout << "genre=";
	getline(cin, genre);
	cout << "publisher=";
	getline(cin, publisher);
	cout << "page_count=";
	getline(cin, page_count);

	if (!check_number(page_count)) {
		cout << "Invalid page number\n";
		return;
	}

	json j;
	j["title"] = title;
	j["author"] = author;
	j["genre"] = genre;
	j["publisher"] = publisher;
	j["page_count"] = stoi(page_count);

	string payload = j.dump();

	int sockfd = open_connection(HOST_IP, PORT, AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		cout << "Failed to open connection\n";
		return;
	}

	string message = compute_post_request(HOST_IP, LIBRARY_BOOKS_ROUTE,
										  (char*)payload.c_str(), user_data);
	send_to_server(sockfd, (char*)message.c_str());
	string response = receive_from_server(sockfd);

	if (response.find("200 OK") != string::npos)
		cout << "Book added with SUCCESS!\n";
	else if (response.find("error") != string::npos)
		cout << "ERROR! Invalid book data\n";

	close(sockfd);
}

void delete_book_command(user_data_t& user_data) {
	if (!user_data.is_logged_in) {
		cout << "ERROR! You must be logged in to access the library!\n";
		return;
	}

	if (!user_data.has_library_access) {
		cout << "ERROR! You don't have access to the library!\n";
		return;
	}

	string id;
	cout << "id=";
	getline(cin, id);

	if (!check_number(id)) {
		cout << "Invalid book id\n";
		return;
	}

	int sockfd = open_connection(HOST_IP, PORT, AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		cout << "Failed to open connection\n";
		return;
	}

	string book_id_url = (string)LIBRARY_BOOKS_ROUTE + "/" + id;
	string message = compute_delete_request(HOST_IP, (char*)book_id_url.c_str(),
											NULL, user_data);
	send_to_server(sockfd, (char*)message.c_str());
	string response = receive_from_server(sockfd);

	if (response.find("200 OK") != string::npos)
		cout << "Book deleted with SUCCESS!\n";
	else if (response.find("error") != string::npos)
		cout << "ERROR! Invalid book id\n";

	close(sockfd);
}

void logout_command(user_data_t& user_data) {
	if (!user_data.is_logged_in) {
		cout << "ERROR! You must be logged in to logout!\n";
		return;
	}

	int sockfd = open_connection(HOST_IP, PORT, AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		cout << "Failed to open connection\n";
		return;
	}

	string message = compute_get_request(HOST_IP, LOGOUT_ROUTE, NULL, user_data);
	send_to_server(sockfd, (char*)message.c_str());
	string response = receive_from_server(sockfd);

	if (response.find("200 OK") != string::npos) {
		cout << "User logged out with SUCCESS!\n";
		user_data = user_data_t();
	} else if (response.find("error") != string::npos) {
		cout << "ERROR! Something went wrong\n";
	}

	close(sockfd);
}
