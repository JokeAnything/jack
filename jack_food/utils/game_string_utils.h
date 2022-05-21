#ifndef _GAME_STRING_UTILS_H
#define _GAME_STRING_UTILS_H
#include <memory>
#include <string>
#include <vector>

class game_string_utils
{
public:

    static std::vector<std::string> split(std::string str, std::string pattern);
};

#endif //_GAME_STRING_UTILS_H
