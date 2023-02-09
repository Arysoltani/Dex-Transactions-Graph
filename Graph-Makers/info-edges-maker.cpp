#include "Libraries_and_Defines.hpp"
#include "SwapsReader.hpp"
#include <boost/multiprecision/cpp_int.hpp>


using namespace std;
using int2048_t = boost::multiprecision::number<boost::multiprecision::cpp_int_backend<1024, 1024, boost::multiprecision::unsigned_magnitude, boost::multiprecision::unchecked, void>, boost::multiprecision::et_off>;

int2048_t max = 0;

struct Edge 
{
    int2048_t amount1;
    int2048_t amount2;
    int2048_t count;
};

map< pair< string, string> , Edge > edges1;

map< pair< string, string> , Edge> edges2;



void add_edge_complete(string token1, string token2, string name_amm, int2048_t amount1, int2048_t amount2)
{
    pair<string, string> edge = make_pair(token1, token2);
    if(name_amm == "UniSwapV2")
    {
        edges1[edge].amount1 += amount1;
        edges1[edge].amount2 += amount2;
        edges1[edge].count += 1;
    }
    else if(name_amm == "SushiSwap")
    {
        edges2[edge].amount1 += amount1;
        edges2[edge].amount2 += amount2;
        edges2[edge].count ++;
    }
    else 
    {
        cout << "ERROR name is: " << name_amm << endl;
    }
}

int2048_t to_int(string inp)
{
    stringstream stream_now;
    stream_now << inp;
    int2048_t ans;
    stream_now >> ans;
    return ans;
}

void add_edge(SwapsReader* swap_reader)
{
    int2048_t amount1;
    int2048_t amount2;
    string token1, token2, name_amm, pool_address;
    vector<int>pos = {POOL_ADDRESS_COL, BUY_TOKEN_COL, BUY_TOKEN_AMOUNT, SELL_TOKEN_COL, SELL_TOKEN_AMOUNT, 
        DEX_LABEL_COL, TRANSACTION_HASH};
    vector<string> ans = swap_reader->get_words(pos);
    if(ans[5] != "UniSwapV2" && ans[5] != "SushiSwap")
        return;
    pool_address = ans[0];
    token1 = ans[1];
    token2 = ans[3];
    amount1 = to_int(ans[2]);
    amount2 = to_int(ans[4]);
    // if(token1 == "USDT" && token2 == "USDC" && (long double)amount1 / (long double)amount2 < 0.9)
    // {
    //     cout << pool_address << " " << amount1 << " " << amount2 << " " << ans[5] << "  " << 
    //         (double)amount1 / (double)amount2 << " " << ans[6] << endl;
    // }
    name_amm = ans[5];
    add_edge_complete(token1, token2, name_amm, amount1, amount2);
}

void write_files()
{
    ofstream graph_uniswap;
    ofstream graph_sushiswap;
    
    graph_uniswap.open(INFO_GRAPH_UNISWAP);
    graph_sushiswap.open(INFO_GRAPH_SUSHISWAP); 
    
    graph_uniswap << "node1,node2,amount1,amount2,ratio,count" << endl;
    graph_sushiswap << "node1,node2,amount1,amount2,ratio,count" << endl;


    auto it = edges1.begin();
    for(auto it = edges1.begin(); it != edges1.end(); it++)
    {
        pair<string, string> tokens = it -> first;
        Edge info = it->second;
        graph_uniswap << tokens.first << "," << tokens.second << "," <<
            info.amount1 << "," << info.amount2 << "," << 
            (long double)info.amount1 / (long double)info.amount2 << 
            "," << info.count << endl;
    }
    for(auto it = edges2.begin(); it != edges2.end(); it++)
    {
        pair<string, string> tokens = it -> first;
        Edge info = it->second;
        graph_sushiswap << tokens.first << "," << tokens.second << "," <<
            info.amount1 << "," << info.amount2 << "," << 
            (long double)info.amount1 / (long double)info.amount2 << 
            "," << info.count << endl;
    }
}

void write_file_ratio()
{
    ofstream ratio_file;
    ratio_file.open(RATIO_EDGE_DEX);
    ratio_file << "token1,token2,ratio-amount1,ratio-amount2" << endl;
    for(auto it = edges1.begin(); it != edges1.end(); it++)
    {
        pair<string, string> tokens = it -> first;
        Edge info = it->second;
        if(edges2[tokens].amount1 != 0)
        {
            Edge info2 = edges2[tokens];
            ratio_file << tokens.first << "," << tokens.second << 
                "," << (long double)info.amount1 / (long double)info2.amount1 << "," << 
                (long double)info.amount2 / (long double)info2.amount2 << "," << 
                (long double)info.count / (long double)info2.count << endl;
        }
    }
}

int main()
{
    SwapsReader* swap_reader = new SwapsReader(SWAP_FILE, BAD_LINES);
    vector<string> bad_lines;
    int cnt = 0;
    while (!swap_reader->is_finish()) 
    {
       cnt++;
       if(cnt % 1000000 == 0)
       {
            cout << cnt << endl;
       }
       add_edge(swap_reader);
    }
    write_files();
    write_file_ratio();
}