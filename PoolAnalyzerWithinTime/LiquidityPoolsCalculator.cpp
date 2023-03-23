#include<bits/stdc++.h>
#include <cstdint>
#include <iomanip>
#include <iostream>

#include <boost/multiprecision/cpp_int.hpp>


using namespace std;
using namespace boost::multiprecision;

int num_blocks;

#define IdIndex 0
#define AddressIndex 1
#define Token0AddressIndex 2
#define Token0SymbolIndex 3
#define Token1AddressIndex 4
#define Token1SymbolIndex 5

#define TokenIndexTransfer 0
#define FromIndexTransfer 1
#define ToIndexTransfer 2
#define Value 3
#define BlockNumber 6

#define FIRST_BLOCK 10060850
#define LAST_BLOCK 15076596

int block_dis;

#define ConfigPath "./config.txt"


struct Token
{
	string address;
	string name;
};

struct BlocksCountTransfer 
{
	vector<int>count_transfers;
};

struct PoolInfo
{
    Token token1;
    Token token2;
    int512_t  liquidity1;
    int512_t  liquidity2;
    string first_block;
    string last_block;
	string dex_name;
	BlocksCountTransfer count_transfer;
};


map<string, string> first_block;
map<string, string> last_block;
map<string, PoolInfo*> info_pool;

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

int find_block(int block_now)
{
	int index = (block_now - FIRST_BLOCK) / block_dis;
	if(index >= num_blocks)
		return -1;
	else 
		return index;
}

std::string make_lower(std::string str)
{
	std::string lower_str = "";
	for (auto c : str)
		lower_str += tolower(c);
	return lower_str;
}

Token make_token(string address, string name)
{
    Token new_token = {address, name};
    return new_token;
}

void get_new_pool(string line, string dex_name)
{
	vector<std::string> info_pool_vec = parse_csv_line(line);
	string pool_address = make_lower(info_pool_vec[AddressIndex]);
	Token token1 = make_token(make_lower(info_pool_vec[Token0AddressIndex]), info_pool_vec[Token0SymbolIndex]);
	Token token2 = make_token(make_lower(info_pool_vec[Token1AddressIndex]), info_pool_vec[Token1SymbolIndex]);
	vector<int> vec_count(num_blocks, 0);
	BlocksCountTransfer count_transfer;
	count_transfer.count_transfers = vec_count;
	PoolInfo* new_pool = new PoolInfo{token1, token2, 0, 0, "", "", dex_name, count_transfer};
	info_pool[pool_address] = new_pool;
}

void load_pools(std::string pools_dir)
{
	std::vector<std::string> pool_names = {"UniSwapV2", "SushiSwap"};

	for (std::string pool_name : pool_names)
	{
		std::string pool_file_path = pools_dir + "/" + pool_name + ".csv";
		std::ifstream pool_file(pool_file_path);
		std::string line;
		std::getline(pool_file, line);

		while (std::getline(pool_file, line))
		{
			line.pop_back();
			get_new_pool(line, pool_name);
		}
	}
}

// void to_double(string inp)
// {
// 	int512_t  now = 0;
// 	int pnt = 0;
// 	while(pnt != (int)inp.size() && inp[pnt] != ".")
// 	{
// 		now *= 10;
// 		now += inp[pnt] - '0';
// 	}
// 	if(inp[pnt] == '.')
// 		pnt++;
// 	if(pnt == inp.size())
// 		return now;
// }

int512_t  to_double(string inp)
{
	stringstream stream_double;
	stream_double << inp;
	int512_t  ans;
	stream_double >> ans;
	return ans;
}

void add_liquidity(string token_address, string from_address, string value, string block_number,
	int multiply)
{
	int512_t  value_ld = to_double(value);
	PoolInfo* pool_now = info_pool[from_address];
	if(pool_now->first_block == "")
		pool_now->first_block = block_number;
	pool_now->last_block = block_number;
	if(token_address == pool_now->token1.address)
		pool_now->liquidity1 += multiply * value_ld;
	else
		pool_now->liquidity2 += multiply * value_ld;

	int bl_number = stoi(block_number);
	int index = find_block(bl_number);

	if(index != -1)
    	pool_now->count_transfer.count_transfers[index] += 1;
}

void parse_transfer_info(string token_address, string from_address, string to_address,
	string value, string block_number)
{
	if(info_pool.find(from_address) != info_pool.end())
		add_liquidity(token_address, from_address, value, block_number, -1);
	if(info_pool.find(to_address) != info_pool.end())
		add_liquidity(token_address, to_address, value, block_number, 1);
}

void liquidity_pools_find(ifstream& transfer_stream, string output_dir)
{
	string line_to_be_ignore;
	getline(transfer_stream, line_to_be_ignore);

	string new_line;
	vector<string> transfer_info;
    int cnt = 0;
	while(getline(transfer_stream, new_line))
	{
		new_line.pop_back();
        cnt++;
	    if(cnt % 100000 == 0)
            cout << cnt << endl;
		transfer_info = parse_csv_line(new_line);
		parse_transfer_info(transfer_info[TokenIndexTransfer], transfer_info[FromIndexTransfer],
			transfer_info[ToIndexTransfer], transfer_info[Value], transfer_info[BlockNumber]);
	}
}

void print_pools_dex(string dex_name, string output_dir,
	vector<pair<pair<string, string>, pair<string, PoolInfo*> > >& sorted_pools)
{
	string output_file = output_dir + "/" + dex_name + ".csv";
	ofstream output_stream(output_file);
	output_stream << "PoolAddress,FirstBlock,LastBlock,LiquidityToken0,LiquidityToken1,AddressToken0,SymbolToken0,AddressToken1,SymbolToken1,CntTransfer" << endl;
	
	for(int i = 0; i < sorted_pools.size(); i++)
	{
		PoolInfo* pool_now = sorted_pools[i].second.second;
		if(pool_now->dex_name != dex_name)
			continue;
		else
			output_stream << sorted_pools[i].second.first << "," << pool_now->first_block << "," <<
				pool_now->last_block << "," << pool_now->liquidity1 << ","
				<< pool_now->liquidity2 << "," << pool_now->token1.address << "," <<
				pool_now->token1.name << "," << pool_now->token2.address << ","
				<< pool_now->token2.name << "," << pool_now->cnt_transfer << endl;
	}
}

vector<pair<pair<string, string>, pair<string, PoolInfo*> > > sort_pools()
{
	vector<pair<pair<string, string>, pair<string, PoolInfo*> > > ans;
	for(auto it = info_pool.begin(); it != info_pool.end(); it++)
	{
		PoolInfo* pool_now = it->second;
		ans.push_back(make_pair(make_pair(pool_now->token1.name, pool_now->token2.name),
			make_pair(it->first, pool_now)));
	}
	sort(ans.begin(), ans.end());
	return ans;
}

int main()
{

	ifstream config_stream(ConfigPath);

	std::string transfer_file_path, pools_dir, output_dir, str_blocks;

    getline(config_stream, transfer_file_path);
	getline(config_stream, pools_dir);
	getline(config_stream, output_dir);
	getline(config_stream, str_blocks);

	num_blocks = stoi(str_blocks);
	block_dis = (LAST_BLOCK - FIRST_BLOCK) / (num_blocks + 1);

	transfer_file_path.pop_back();
	pools_dir.pop_back();
	output_dir.pop_back();

	ifstream transfer_stream(transfer_file_path);

	load_pools(pools_dir);
    liquidity_pools_find(transfer_stream, output_dir);
	vector<pair<pair<string, string>, pair<string, PoolInfo*> > >sorted_pools = sort_pools();
	print_pools_dex("UniSwapV2", output_dir, sorted_pools);
	print_pools_dex("SushiSwap", output_dir, sorted_pools);
}
