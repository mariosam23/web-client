#pragma once

#include <string>
#include <unordered_map>

#include "utils.hpp"

using namespace std;

enum Command {
	REGISTER,
	LOGIN,
	ENTER_LIBRARY,
	GET_BOOKS,
	GET_BOOK,
	ADD_BOOK,
	DELETE_BOOK,
	LOGOUT,
	EXIT,
	UNKNOWN
};

// Returns a map with the command name as key and the corresponding enum as value
unordered_map<string, Command> get_command_map();

// Computes POST request for registering a user
void register_command(user_data_t &user_data);

// Computes POST request for logging in a user
void login_command(user_data_t &user_data);

// Computes GET request for entering the library
void enter_library_command(user_data_t &user_data);

// Computes GET request for getting all books
void get_books_command(user_data_t &user_data);

// Computes GET request for getting a book by id
void get_book_command(user_data_t &user_data);

// Computes POST request for adding a book
void add_book_command(user_data_t &user_data);

// Computes DELETE request for deleting a book by id
void delete_book_command(user_data_t &user_data);

// Computes GET request for logging out
void logout_command(user_data_t &user_data);
