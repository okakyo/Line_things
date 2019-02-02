import requests

channel_token='ZC8x49i2Pcmj5il0zeLzRkT0PRfYKz3+VTYYCygGfwpXiLiGjkPDe17QF8Ri7tS3vTaoApzTblDlc1Efo/vEHO8ZEmiGklVvC9XWLXMPcJiGXhIOM5rWJr97bf6OoDhOeQ45GXpkKZV1X5s+YOyP+FGUYhWQfeY8sLGRXgo3xvw='
header={'Authorization':'Bearer {}'.format(channel_token)}

data=requests.get('https://api.line.me/things/v1/trial/products',headers=header)
print(data.text)
