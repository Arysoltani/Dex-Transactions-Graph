#include "Libraries_and_Defines.hpp"
#include "SwapsReader.hpp"

using namespace std;

set<pair<string, string> > edges1;
set<pair<string, string> > edges2;
set<pair<string, string> > edges_all;

int main()
{
    SwapsReader* swap_reader = new SwapsReader(SWAP_FILE, BAD_LINES);
    vector<string> bad_lines;
    int cnt = 0;
    while (!swap_reader->is_finish()) 
    {
        cnt++;
        if(cnt % 100000 == 0)
        {
            cout << cnt << endl;
        }
        string name_amm;
        string token1, token2;
        vector<int>pos = {BUY_TOKEN_ADDRESS, SELL_TOKEN_ADDRESS, DEX_LABEL_COL};
        vector<string> ans = swap_reader->get_words(pos);
        name_amm = ans[2];
        token1 = ans[0];
        token2 = ans[1];
        if(token1 > token2)
            swap(token1, token2);
        pair<string, string> edge = make_pair(token1, token2);
        if(name_amm == "UniSwapV2")
        {
            if(edges1.find(edge) == edges1.end())
            {
                edges1.insert(edge);
            }
        }
        else if(name_amm == "SushiSwap")
        {
            if(edges2.find(edge) == edges2.end())
            {
                edges2.insert(edge);
            }
        }
        else 
        {
            cout << "ERROR name is: " << name_amm << endl;
        }
    }
    ofstream graph_uniswap;
    ofstream graph_sushiswap;
    ofstream graph_all;
    graph_uniswap.open(GRAPH_UNISWAP);
    graph_sushiswap.open(GRAPH_SUSHISWAP); 
    graph_all.open(GRAPH_FILE);
    graph_uniswap << "node1,node2" << endl;
    graph_sushiswap << "node1,node2" << endl;
    graph_all << "node1,node2" << endl;
    edges_all.insert(edges1.begin(), edges1.end());
    edges_all.insert(edges2.begin(), edges2.end());
    for(auto& edge: edges1)
    {
        graph_uniswap << edge.first << ',' << edge.second << endl;
    }
    for(auto& edge: edges2)
    {
        graph_sushiswap << edge.first << ',' << edge.second << endl;
    }
    for(auto& edge: edges_all)
    {
        graph_all << edge.first << ',' << edge.second << endl;
    }
}