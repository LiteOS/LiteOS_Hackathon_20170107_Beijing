# -*- coding: utf-8 -*-
import sys
reload(sys)
sys.setdefaultencoding('utf-8')
import time
import tornado.ioloop
import tornado.web
from tornado import gen

from tornado.websocket import websocket_connect
from controller.base import WebRequest, WebSocket
from tornado.escape import json_encode, json_decode

from setting import settings

import mraa
print "mraa version: %s" % mraa.getVersion()


class MainHandler(WebRequest):
    def get(self):
        self.render('./template/index.html')


WS_URL = "ws://www.hotpoor.org:8036/ws?aim_id=LITEOSDEMO"
HEADIMG = "http://wx.qlogo.cn/mmopen/viasnlibZqap4GwK7ua20icUVLcTas3GFia5lLleaYmTsgnO1Aj3icibMwY9UlSoeD1Smur9yUIPNSTbOWfuUYQvjm8TuXzgY7Wia54/0"
NICKNAME = "Helen小布熊"


u=mraa.Uart(0)
u.setBaudRate(115200)
u.setMode(8, mraa.UART_PARITY_NONE, 1)
u.setFlowcontrol(False, False)


@gen.coroutine
def connect_cloud(url):
    conn = yield websocket_connect(url)
    print "edison connected to cloud"
    while True:
        msg = yield conn.read_message()
        if msg is None: break
        msg = json_decode(msg)
        print "received one message from cloud"
        print msg
        protocal = msg[0]
        if protocal == "LITEOSDEMO":
            content = msg[1]["content"]
            if content.startswith("HUAWEI//CMD//"):
                command = content.split("HUAWEI//CMD//")[1]
                print "receive command from cloud: %s" % command
                # cmd 格式  01$1!
                u.writeStr(cmd.encode("ascii"))
                print "send keyboard move: %s" % cmd
            else:
                print "error content"
        else:
            print "unsupported comment"


if __name__ == "__main__":
    # connect to hotpoor.org
    connect_cloud(WS_URL)

    application = tornado.web.Application([
        (r"/static/(.*)", tornado.web.StaticFileHandler, dict(path=settings["static_path"])),
        (r"/", MainHandler),
        (r"/.*", MainHandler),
    ], **settings)
    application.listen(3000)
    tornado.ioloop.IOLoop.current().start()
