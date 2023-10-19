#include<bits/stdc++.h>

using namespace std;

#define SWAP_FILE "./Swaps/swaps-huge.csv"
#define GRAPH_FILE "./Graphs/tokens_graph.csv"
#define GRAPH_UNISWAP "./Graphs/graph_uniswap.csv"
#define GRAPH_SUSHISWAP "./Graphs/graph_sushiswap.csv"
#define BAD_LINES "./bad-lines.csv"


set<pair<string, string> > edges1;
set<pair<string, string> > edges2;
set<pair<string, string> > edges_all;


void read_tokens(string new_line, string &token1, string &token2, string& amm)
{
    stringstream stream_line = stringstream(new_line);
    string new_word;
    getline (stream_line, new_word, ',');
    getline (stream_line, new_word, ',');
    getline (stream_line, token1, ',');
    getline (stream_line, new_word, ',');
    getline (stream_line, new_word, ',');
    getline (stream_line, token2, ',');
    getline (stream_line, new_word, ',');
    getline (stream_line, new_word, ',');
    getline (stream_line, new_word, ',');
    getline (stream_line, amm, ',');
}

int main()
{
    vector<string> bad_lines;
    ifstream file_swaps(SWAP_FILE);
    ifstream file_bad_lines(BAD_LINES);
    int cnt = 0;
    while (file_swaps) 
    {
        string new_line;
        cnt++;
        getline (file_swaps, new_line);
        cout << new_line << endl;
        if(cnt == 10)
            break;
        if(cnt == 1)
        {
            continue;
        }
        if(cnt % 100000 == 0)
        {
            cout << cnt << endl;
        }
        string name_amm;
        string token1, token2;
        read_tokens(new_line, token1, token2, name_amm);
        if(token1 > token2)
            swap(token1, token2);
        pair<string, string> edge = make_pair(token1, token2);
        if(name_amm != "UniSwapV2" && name_amm !=  "SushiSwap")
        {
            getline (file_bad_lines, new_line);
            read_tokens(new_line, token1, token2, name_amm);
            pair<string, string> edge = make_pair(token1, token2);
        }
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
            bad_lines.push_back(new_line);
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