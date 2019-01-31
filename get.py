import requests


channel_token=''
header={'Authorization':'Bearer {}'.format(channel_token)}

data=requests.get('https://api.line.me/things/v1/trial/products',headers=header)
print(data.text)
