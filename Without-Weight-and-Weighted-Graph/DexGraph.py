import pandas as pd
import networkx as nx
import csv

COLUMN_NAME = ["AddressToken0","AddressToken1"]

class DexGraph:

    def __init__(self, file_path):
        self.crypto_swaps_graph = nx.Graph()
        df_swaps = pd.read_csv(file_path)
        for row in df_swaps.index:
            sell_token = df_swaps['AddressToken0'][row]
            buy_token = df_swaps['AddressToken1'][row]
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

    def print(self, file_path_output):
        graph_file = open(file_path_output, 'w')
        graph_writer = csv.writer(graph_file)
        graph_writer.writerow(COLUMN_NAME)
        for edge in self.crypto_swaps_graph.edges:
             graph_writer.writerow(edge)
    def get_best_vertices(self, num):
        degree_node_list = self.crypto_swaps_graph.degree()
        degree_node = sorted(degree_node_list, key = lambda vertex: vertex[1])    
        print(degree_node)
# dex_graph = DexGraph("Swaps/swaps.csv")
# dex_graph.print("Graphs/graph1.csv")
