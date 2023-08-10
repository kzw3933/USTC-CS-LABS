from config import *

class RequestProcotol(object):
    @staticmethod
    def request_login_result(username,password):

        return DELIMITER.join([REQUEST_LOGIN,username,password])

    @staticmethod
    def request_chat(username,messages):

        return DELIMITER.join([REQUEST_CHAT,username,messages])

