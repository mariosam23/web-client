#ifndef _REQUESTS_
#define _REQUESTS_

#include <string>

#include "utils.hpp"

using namespace std;

// computes and returns a GET request string (query_params
// can be set to NULL if not needed)
string compute_get_request(char *host, char *url, char *query_params,
						   user_data_t &user_data);

// computes and returns a DELETE request string (query_params
// can be set to NULL if not needed)
string compute_delete_request(char *host, char *url, char *query_params,
							  user_data_t &user_data);

// computes and returns a POST request string
string compute_post_request(char *host, char *url, string payload,
							user_data_t &user_data);

#endif
