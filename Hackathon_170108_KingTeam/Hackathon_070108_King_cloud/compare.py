# -*- coding: utf-8 -*-
import difflib
#openlight_list = [u"把灯打开", u"开灯", u"打开灯"]
#closlight_lis = [u"把灯关闭", u"关灯", u"把灯关掉"]
#homeweather_list = [u"家里什么情况", u"现在家里什么情况"]
#outdorweather_list= [u"今天天气怎么样", u"今天的天气"]

def compare_to_list(str, list):
    for m in range(len(list)):
        cmp_val = difflib.SequenceMatcher(None, str, list[m]).ratio()
        if cmp_val > 0.6:
            print  cmp_val
            return True
    print cmp_val
    return False

#print  (compare_to_list(u"现在家里什么情况", homeweather_list))