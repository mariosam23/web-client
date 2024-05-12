#pragma once

#include <iostream>

#define HOST_IP (char *)"34.246.184.49"
#define PORT 8080
#define CONTENT_TYPE (string)"application/json"
#define LIBRARY_ACCESS_ROUTE (char *)"/api/v1/tema/library/access"
#define LIBRARY_BOOKS_ROUTE (char *)"/api/v1/tema/library/books"
#define LOGIN_ROUTE (char *)"/api/v1/tema/auth/login"
#define REGISTER_ROUTE (char *)"/api/v1/tema/auth/register"
#define LOGOUT_ROUTE (char *)"/api/v1/tema/auth/logout"
#define SUCCESS_EXIT_CODE 0
#define INIT_MSG "Hello! You can choose one of the following commands:\n" \
					"1. register\n" \
					"2. login\n" \
					"3. enter_library\n" \
					"4. get_books\n" \
					"5. get_book\n" \
					"6. add_book\n" \
					"7. delete_book\n" \
					"8. logout\n" \
					"9. exit\n"

struct user_data_t {
	bool is_logged_in;
	bool has_library_access;
	std::string cookie;
	std::string jwt_token;

	user_data_t() : is_logged_in(false), has_library_access(false), cookie(""), jwt_token("") {}
};
