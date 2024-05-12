#include <iostream>
#include <string>
#include <cctype>

using namespace std;

bool check_string(string input_value) {
    for (auto ch : input_value) {
        if (!isalnum(ch) && !isspace(ch) && ch != ':')
            return false;
    }

    return true;
}

bool check_number(string input_value) {
	for (auto ch : input_value)
		if (!isdigit(ch))
			return false;

	return true;
}

bool check_string_with_no_spaces(string input_value) {
	for (auto ch : input_value)
		if (isspace(ch) || !isalnum(ch))
			return false;

	return true;
}
