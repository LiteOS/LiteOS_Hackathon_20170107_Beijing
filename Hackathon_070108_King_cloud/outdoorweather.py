# -*- coding: utf-8 -*-
# 从阿里云api获取当前天气
import urllib, urllib2, sys
import json

def get_outdoor_weather():
    host = 'http://ali-weather.showapi.com'
    path = '/area-to-weather'
    method = 'GET'
    appcode = ''
    querys = 'areaid=101010100&need3HourForcast=0&needAlarm=0&needHourData=0&needIndex=0&needMoreDay=0'
    bodys = {}
    url = host + path + '?' + querys
    request = urllib2.Request(url)
    request.add_header('Authorization', 'APPCODE ' + appcode)
    response = urllib2.urlopen(request)
    content = response.read()
    if (content):
        print(content)
    json_data = json.loads(content)
    #night_air_temperature
    night_air_temperature = json_data["showapi_res_body"]["f1"]["night_air_temperature"]
    #day_air_temperature
    day_air_temperature = json_data["showapi_res_body"]["f1"]["day_air_temperature"]
    #day_weather
    day_weather = json_data["showapi_res_body"]["f1"]["day_weather"]
    #night_weather
    night_weather = json_data["showapi_res_body"]["f1"]["night_weather"]
    #quality
    quality = json_data["showapi_res_body"]["now"]["aqiDetail"]["quality"]
    #sd
    sd = json_data["showapi_res_body"]["now"]["sd"]
    #temperature
    temperature = json_data["showapi_res_body"]["now"]["temperature"]
    today_weather = u"北京，现在温度：%s℃, 湿度：%s，空气质量：%s；今天白天温度：%s℃，%s；今天夜间温度：%s℃，%s" \
                    % (temperature, sd, quality, day_air_temperature, day_weather, night_air_temperature, night_weather)
    print  today_weather
    return today_weather
#today = get_outdoor_weather()
#print  today
#print  type(today)  #unicode


