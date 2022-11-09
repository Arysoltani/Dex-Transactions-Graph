import pandas as pd
import networkx as nx

class DexGraph:

    def __init__(self, file_path):
        self.crypto_swaps_graph = nx.Graph()
        df_swaps = pd.read_csv(file_path)
        for row in df_swaps.index:
            sell_token = df_swaps[' sell token symbol'][row]
            buy_token = df_swaps[' buy token symbol'][row]
            self.add_node(self.crypto_swaps_graph, buy_token)
            self.add_node(self.crypto_swaps_graph, sell_token)
            if(not(self.crypto_swaps_graph.has_edge(buy_token, sell_token))):
                self.crypto_swaps_graph.add_edge(buy_token, sell_token)

    def add_node(self, graph_input, token_input):
      if(not graph_input.has_node(token_input)):
            graph_input.add_node(token_input)
    
    def get_list_degree(self):
        degree_node_list = self.crypto_swaps_graph.degree()
        degree_list = [deg[1] for deg in degree_node_list]
        degree_list.sort()
        return degree_list
