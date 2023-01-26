from config import *


## 定义用于响应拼接的类
class ResponseProtocol(object):

    @staticmethod
    def response_login_result(result,nickname,username):
        
        return DELIMITER.join([RESPONSE_LOGIN_RESULT,result,nickname,username])

    @staticmethod
    def response_chat(nickname,messages):

        return DELIMITER.join([RESPONSE_CHAT,nickname,messages])

