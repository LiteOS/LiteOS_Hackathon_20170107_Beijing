# -*- coding: utf-8 -*-
# filename: handle.py
import hashlib
import web
import reply
import receive
import auto_reply
import compare
import outdoorweather
import do_json
import pub
openlight_list = [u"把灯打开", u"开灯", u"打开灯"]
closlight_lis = [u"把灯关闭", u"关灯", u"把灯关掉"]
homeweather_list = [u"家里什么情况", u"现在家里什么情况"]
outdorweather_list= [u"今天天气怎么样", u"今天的天气"]
music_rand_list = [u"我要听音乐", u"来一首音乐", u"听音乐"]
music_jay_list = [u"周杰伦歌曲", u"周杰伦的歌曲", u"来首周杰伦的歌曲"]
music_andy_list = [u"刘德华歌曲", u"来首刘德华歌曲"]
music_stop_list = [u"停止吧", u"停止音乐", u"停止"]
map_list = [u"宝贝在哪呢", u"在哪呢", u"定位宝贝"]
# key和url填入自己申请到的图灵key以及图灵请求url
tl_auto_reply = auto_reply.TulingAutoReply("d12dac18ace0400ba3a3a39a7d97f537", "http://www.tuling123.com/openapi/api")
class Handle(object):
    def GET(self):
        try:
            data = web.input()
            if len(data) == 0:
                return "hello, this is handle view"
            signature = data.signature
            timestamp = data.timestamp
            nonce = data.nonce
            echostr = data.echostr
            token = "weixin" #请按照公众平台官网\基本配置中信息填写

            list = [token, timestamp, nonce]
            list.sort()
            sha1 = hashlib.sha1()
            map(sha1.update, list)
            hashcode = sha1.hexdigest()
            print "handle/GET func: hashcode, signature: ", hashcode, signature
            if hashcode == signature:
                return echostr
            else:
                return ""
        except Exception, Argument:
            return Argument

    def POST(self):
        try:
            webData = web.data()
            print "Handle Post webdata is ", webData  # 后台打日志
            recMsg = receive.parse_xml(webData)
            if isinstance(recMsg, receive.Msg):
                toUser = recMsg.FromUserName
                fromUser = recMsg.ToUserName
                print recMsg.MsgType
                if recMsg.MsgType == 'text':
                    #文字类型返回
                    content = recMsg.Content
                    print content
                    unicode_str = unicode(content, "utf-8")
                    #有待改进的算法
                    #如果满足是用户自定义的单词就选择图灵机器人
                    cmp1_val = compare.compare_to_list(unicode_str, openlight_list)
                    cmp2_val = compare.compare_to_list(unicode_str, closlight_lis)
                    cmp3_val = compare.compare_to_list(unicode_str, homeweather_list)
                    cmp4_val = compare.compare_to_list(unicode_str, outdorweather_list)
                    cmp5_val = compare.compare_to_list(unicode_str, map_list)
                    rand_val = compare.compare_to_list(unicode_str, music_rand_list)
                    jay_val = compare.compare_to_list(unicode_str, music_jay_list)
                    andy_val = compare.compare_to_list(unicode_str, music_andy_list)
                    stop_val = compare.compare_to_list(unicode_str, music_stop_list)
                    if cmp1_val is True:
                        print cmp1_val
                        tl_reply = u"好的，打开灯"
                        print tl_reply
                        print type(tl_reply)
                    
                    elif cmp2_val is True:
                        print cmp2_val
                        tl_reply = u"好的，关闭灯"
                        print tl_reply

                    elif cmp3_val is True:
                        print cmp3_val
                        #加入读取json文件代码
                    	air_t, air_h, air_l = do_json.process_get_json_val("data_out.json")
                        tl_reply = u"当前，温度：%.1f℃，湿度：%.1f%%, 光照：%dlux" %(air_t, air_h, air_l)
                        #tl_reply = u"家里面温度12℃，湿度34%，光照123lux"
                    	print tl_reply

                    elif cmp4_val is True:
                        #从阿里云获取天气预报
                        tl_reply = outdoorweather.get_outdoor_weather()
                    elif cmp5_val is True:
                        #发送地图
                        tl_reply = u"下面链接可以找到我哦\r\n" + u"http://smart-lifestyle.cn:8080/manager.html?service_id=132206&ak=K8eBldF0NlV2lcVf1X0OUVX6XpCPXByR"
                        #tl_reply = u"北京的现在的温度-1℃，天气重度雾霾"
                    #if cmp1_val is not True and cmp2_val is not True and cmp3_val is not True and cmp4_val is not True:
                    #需要unicode变量,如果不是以上信息，就交给图灵机器人吧
                    elif rand_val is True:
                    	pub.pub_msg("{\"music_id\":1}", "music")
                    	tl_reply = u"好的，随便来一首啦"
                    elif jay_val is True:
                    	pub.pub_msg("{\"music_id\":2}", "music")
                    	tl_reply = u"好的，播放周杰伦的歌曲哟"
                    elif andy_val is True:
                    	pub.pub_msg("{\"music_id\":3}", "music")
                    	tl_reply = u"好的，播放华仔的歌曲哟"
                    elif stop_val is True:
                    	pub.pub_msg("{\"music_id\":4}", "music")
                    	tl_reply = u"啧啧啧，不播了"
                    else:
                        tl_reply = tl_auto_reply.reply(unicode_str)
                        print tl_reply
                    #需要str变量
                    replyMsg = reply.TextMsg(toUser, fromUser, tl_reply.encode("utf-8"))
                    return replyMsg.send()
                elif recMsg.MsgType == 'image':
                    mediaId = recMsg.MediaId
                    replyMsg = reply.ImageMsg(toUser, fromUser, mediaId)
                    return replyMsg.send()
                elif recMsg.MsgType == 'voice':
                    #语音类型，开启语音识别功能
                    voice_content = recMsg.Recognition
                    #他居然是unicode类型
                    print voice_content
                    #print type(voice_content)
                    #unicode_str = unicode(voice_content, "utf-8")
                    #需要unicode变量
                    tl_reply = tl_auto_reply.reply(voice_content)
                    print tl_reply
                    #需要str变量,语音识别后的，返回文字式对白，未认证公众号无法返回语音
                    replyMsg = reply.TextMsg(toUser, fromUser, tl_reply.encode("utf-8"))
                    return replyMsg.send()
                else:
                    return reply.Msg().send()
            else:

                print "暂且不处理"
                return reply.Msg().send()
        except Exception, Argment:
            return Argment

