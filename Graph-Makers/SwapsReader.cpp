#include "SwapsReader.hpp"

using namespace std;

SwapsReader::SwapsReader(string file_name_input, string bad_lines_file_name_input)
{
    file_swaps.open(file_name_input);
    string new_line;
    getline(file_swaps, new_line);
    cout << new_line << endl;
    file_bad_lines.open(bad_lines_file_name_input);
}   

string SwapsReader::get_word(int num)
{
    string new_line;
    getline(file_swaps, new_line);
    string label_type = get_word(DEX_LABEL_COL, new_line);
    if(label_type != UNISWAP_LABEL && label_type != SUSHISWAP_LABEL)
    {
        getline(file_bad_lines, new_line);
    }
    return get_word(num, new_line);
}

string SwapsReader::get_word(int num, string new_line)
{
    stringstream stream_line = stringstream(new_line);
    string new_word;
    for(int i = 0; i < num; i++)
    {
        getline(stream_line, new_word, ',');
    }
    return new_word;
}

bool SwapsReader::is_finish()
{
    return file_swaps.eof();
}

vector<string> SwapsReader::get_words(vector<int>&pos)
{
    string new_line;
    getline(file_swaps, new_line);
    string label_type = get_word(DEX_LABEL_COL, new_line);
    if(label_type != UNISWAP_LABEL && label_type != SUSHISWAP_LABEL)
    {
        getline(file_bad_lines, new_line);
    }
    return get_words(pos, new_line);
}


vector<string> SwapsReader::get_words(vector<int>&pos, string new_line)
{
    vector<string>ans;
    stringstream stream_line = stringstream(new_line);
    string new_word;
    int pnt = 0, cnt = 0;
    while(pnt != pos.size())
    {
        cnt++;
        getline(stream_line, new_word, ',');
        if(pos[pnt] == cnt)
        {
            ans.push_back(new_word);
            pnt++;
        }
    }
    return ans;
}