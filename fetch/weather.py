import requests
import os

api_url = 'http://api.openweathermap.org/data/2.5/weather?q={}&units=metric&APPID={}'
api_location = os.environ['CLOCK_LOCATION']
api_key      = os.environ['CLOCK_OVM_KEY']

final_url = api_url.format(api_location, api_key)

response = requests.get(final_url)
data = response.json()

faceText = ''
faceText += data['weather'][0]['main'] + ' '
faceText += str(data['main']['humidity']) + '% '
faceText += str(data['main']['pressure']) + 'mb '
faceText += str(round(data['wind']['speed'])) + 'm/s '
faceText += str(round(data['main']['temp'])) + 'C'

print(faceText)
