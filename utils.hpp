constexpr auto HOST_IP = "34.246.184.49";
constexpr auto PORT = 8080;
constexpr auto CONTENT_TYPE = "application/json";
constexpr auto LIBRARY_ACCESS_ROUTE = "/api/v1/tema/library/access";
constexpr auto LIBRARY_BOOKS_ROUTE = "/api/v1/tema/library/books";
constexpr auto LOGIN_ROUTE = "/api/v1/tema/auth/login";
constexpr auto REGISTER_ROUTE = "/api/v1/tema/auth/register";
constexpr auto LOGOUT_ROUTE = "/api/v1/tema/auth/logout";
constexpr auto SUCCESS_EXIT_CODE = 0;
constexpr auto INIT_MSG = "Hello! You can choose one of the following commands:\n"
                          "1. register\n"
                          "2. login\n"
                          "3. enter_library\n"
                          "4. get_books\n"
                          "5. get_book\n"
                          "6. add_book\n"
                          "7. delete_book\n"
                          "8. logout\n"
                          "9. exit\n";
