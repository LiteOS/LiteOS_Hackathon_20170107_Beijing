# -*- encoding: utf8 -*-

import os

from tornado.web import RequestHandler
from tornado.websocket import WebSocketHandler
from tornado.escape import json_decode


class BaseHandler(object):
    def get_current_user(self):
        user_json = self.get_secure_cookie("user")
        if not user_json:
            return {}
        user_data = json_decode(user_json)
        if user_data.get("v", 0) < 1:
            return {}
        return user_data


class WebRequest(BaseHandler, RequestHandler):
    def get_error_html(self, status_code, **kwargs):
        return open(os.path.join(os.path.dirname(__file__), "../static/404.html")).read()


class WebSocket(BaseHandler, WebSocketHandler):
    def check_origin(self, origin):
        return True