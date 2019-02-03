import requests

# Line things でWindowsでcurl がうまくいかない場合にこのpythonコードを記述して実行する。
# また、これは、LIFF側で、デバイスが反応できるようにするための

channel_tolen='' #
name='' # 名前は自由に設定可能
liff_id=''# 
headers = {
    'Authorization': 'Bearer {}'.format(channel_token),
    'Content-Type':'application/json'
    }


data='{"name":"{}","liffId":"{}"}'.format(name,liff_id)

response = requests.post('https://api.line.me/things/v1/trial/products', headers=headers,data=data)
data=response.text
print(data)
with open("data.txt","w") as w:
    w.write(data)
