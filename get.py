import requests

# これは、python　で自身が登録したIDを確認するためのコードである。
# 個人のchannel_token を入力して、実行すること 

channel_token=''
header={'Authorization':'Bearer {}'.format(channel_token)}

data=requests.get('https://api.line.me/things/v1/trial/products',headers=header)
print(data.text)
