import requests

#以下のところにaccess_token,デバイスの名前、LIFFIDを入力すれば目的のIDを取得できる。
headers = {
    'Authorization': 'Bearer ',
    'Content-Type': 'application/json',
}

data = '{"name": "",  "liffId": ""}'

response = requests.post('https://api.line.me/things/v1/trial/products', headers=headers, data=data)
print(response.text)