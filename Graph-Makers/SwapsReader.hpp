#ifndef SWAP_READER
#define SWAP_READER 

#include "Libraries_and_Defines.hpp"

class SwapsReader 
{
    public:
        SwapsReader(std::string file_name_input, std::string bad_lines_file_name_input);
        std::string get_word(int num);
        bool is_finish();
        std::vector<std::string> get_words(std::vector<int>&pos);
    private:
        std::ifstream file_swaps;
        std::ifstream file_bad_lines;
        std::string get_word(int num, std::string new_line);
        std::string read_line();
        std::vector<std::string> get_words(std::vector<int>&pos, std::string new_line);

};

#endif