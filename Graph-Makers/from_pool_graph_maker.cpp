#include<bits/stdc++.h>

using namespace std;

#define PATH_POOLS_DIR "../PoolsLiquidities/LiquidityPools/"
#define OUTPUT_DIR "../Graphs/"
#define UNISWAP_FILE "UniSwapV2"
#define SUSHISWAP_FILE "SushiSwap"

vector< pair<string, string> > edges;

vector<string> parse_csv_line(std::string line)
{
	std::vector<std::string> parsed_line;
	std::string current_value = "";
	bool in_quotes = false;

	for (char c : line)
	{
		if (c == '"')
		{
			in_quotes = !in_quotes;
		}
		else if (c == ',' && !in_quotes)
		{
			parsed_line.push_back(current_value);
			current_value = "";
		}
		else
		{
			current_value += c;
		}
	}

	parsed_line.push_back(current_value);
	return parsed_line;
}

void make_graph(string file_name)
{
    string inp_path = PATH_POOLS_DIR + file_name + ".csv";
    ifstream dex_pools(inp_path);

    string out_path = OUTPUT_DIR + file_name + "Pools.csv";
    ofstream output_stream(out_path);

    string line_to_be_ignore, new_line;

    getline(dex_pools, line_to_be_ignore);

    output_stream << "node1,node2,name1,name2";

    while(getline(dex_pools, new_line))
    {
        new_line.pop_back();
        vector<string> parsed_csv = parse_csv_line(new_line);
        output_stream << 
    }
}

int main()
{   
    make_graph(UNISWAP_FILE);
    make_graph(SUSHISWAP_FILE);
}