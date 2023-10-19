import pandas as pd

def make_graph(file_name, file_out):
    df_info = pd.read_csv(file_name)
    df_info = df_info[df_info["count"] > 1000]
    df_info = df_info[["node1", "node2"]]
    df_info.to_csv(file_out, index = False)

make_graph("../info-edges/info_graph_sushiswap.csv", "../Without-Weight-Graph/Graphs/limit-sushiswap.csv")
make_graph("../info-edges/info_graph_uniswap.csv", "../Without-Weight-Graph/Graphs/limit-uniswap.csv")
