### Third PCOM homework
---
#### Description
- I implemented a client which sends http requests to an already implemented server.
---
#### Commands
- `register` - client is asked to input a username and a password. Program checks if username and password are valid (no spaces). If they are correct, a POST request is sent to server with username and password in JSON format. The answer from server will clarify if the registration was successful or not.
- `login` - similar to register. If everything is correct, a session cookie will be obtained from server response and used for further commands.
- `enter_library` - sends a GET request to server with the session cookie. If the cookie is valid, the server will respond with a token which will be used for library manipulation.
- `get_books` - if user is logged_in, has access to library and everything works good, sends a GET request to server with the token obtained from `enter_library` command. The server will respond with a list of books.
- `get_book` - similar to `get_books`, but the server will get the book with the id specified by the user, if it exists.
- `add_book` - if user is logged_in, has access to library and everything works good, sends a POST request to server with the token obtained from `enter_library` command and a JSON with the book details. The server response will clarify if the book was added or not.
- `delete_book` - server will delete the book with the id specified by the user, if it exists and user has access to library.
- `logout` - sends a GET request to server. If everything is correct, the server will respond with a message that the user is logged out, and the user credentials will be deleted from the client.
---
#### NLOHMANN JSON
- I used it because it is a very easy to use library for JSON manipulation. It is very easy to install. It is also very well documented.
---
© 2024 Mario Sampetru 321CA
