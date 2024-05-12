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
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "commands.hpp"
#include "helpers.hpp"
#include "requests.hpp"
#include "utils.hpp"
#include "validator.hpp"

using namespace std;
using json = nlohmann::json;

int main(int argc, char *argv[]) {
	user_data_t user_data = user_data_t();

	cout << INIT_MSG;

	unordered_map<string, Command> command_map = get_command_map();

	while (true) {
		string command;
		getline(cin, command);

		Command current_command = UNKNOWN;
		if (command_map.find(command) != command_map.end()) {
			current_command = command_map[command];
		}

		switch (current_command) {
			case REGISTER:
				register_command(user_data);
				break;
			case LOGIN:
				login_command(user_data);
				break;
			case ENTER_LIBRARY:
				enter_library_command(user_data);
				break;
			case GET_BOOKS:
				get_books_command(user_data);
				break;
			case GET_BOOK:
				get_book_command(user_data);
				break;
			case ADD_BOOK:
				add_book_command(user_data);
				break;
			case DELETE_BOOK:
				delete_book_command(user_data);
				break;
			case LOGOUT:
				logout_command(user_data);
				break;
			case EXIT:
				return SUCCESS_EXIT_CODE;
			default:
				cout << "Unknown command\n";
		}
	}

	return SUCCESS_EXIT_CODE;
}
