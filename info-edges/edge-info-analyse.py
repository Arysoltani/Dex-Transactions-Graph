import pandas as pd

file_name = "info_graph_uniswap.csv"

df_uni = pd.read_csv(file_name)
df_uni  = df_uni.sort_values("count")
print(df_uni.tail(10))

file_name = "info_graph_sushiswap.csv"

df_sushi = pd.read_csv(file_name)
df_sushi  = df_sushi.sort_values("count")
print(df_sushi.tail(10))