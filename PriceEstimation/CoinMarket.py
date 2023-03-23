import requests
from bs4 import BeautifulSoup
import pandas as pd

for block_num in range(10060850, 15076596, (15076596 - 10060850) // 10):
    url = "https://api.coinmarketcap.com/data-api/v3/cryptocurrency/listing?start=1&limit=209&sortBy=market_cap&sortType=desc&convert=USD,BTC,ETH&cryptoType=all&tagType=all&audited=false&aux=ath,atl,high24h,low24h,num_market_pairs,cmc_rank,date_added,max_supply,circulating_supply,total_supply,volume_7d,volume_30d,self_reported_circulating_supply,self_reported_market_cap&tagSlugs=ethereum-ecosystem"

    payload={}
    headers = {}

    response = requests.request("GET", url, headers=headers, data=payload)
    all_json = response.json()
    # print(all_json['data']['cryptoCurrencyList'][0]['id'], all_json['data']['cryptoCurrencyList'][0]['name'], all_json['data']['cryptoCurrencyList'][0]['slug'])
    token_list = []
    ethereum = True

    df = pd.DataFrame(columns = ['address', 'price'])
    cnt = 0
    for currency in all_json['data']['cryptoCurrencyList']:
        if cnt > 10:
            break
        if ethereum:
            ethereum = False
            continue
        try:
            id = currency['id']
            slug = currency['slug']
            print(currency['name'], slug)
            url = 'https://coinmarketcap.com/currencies/' + slug
            page = requests.get(url)
            soup = BeautifulSoup(page.content, "html.parser")
            res = soup.find_all('a', class_ = 'cmc-link')
            addr = res[55]['href'].split('/')[4].strip()
            
            url = f"https://api.coinmarketcap.com/data-api/v3/cryptocurrency/detail/chart?id={id}&range={block_num}~{block_num}"
            payload={}
            headers = {}
            response = requests.request("GET", url, headers=headers, data=payload)
            if(response.json()['data']['points'] == {}):
                continue
            price = response.json()['data']['points'][str(block_num)]['v'][0]
            file = open("PriceForBlocks/" + str(block_num) + "Price.txt", "a")  # append mode
            file.write(f"{addr}, {price}\n")
            file.close()

            df.loc[len(df)] = [addr, price]
            
        except:
            file = open("PriceForBlocks/" + str(block_num) + "Price.txt", "a")  # append mode
            file.write(f"Error in {currency['name']}, {currency['slug']}\n")
            file.close()

    df.to_csv('HistoricalPrices.csv')