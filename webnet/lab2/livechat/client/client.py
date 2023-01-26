from config import *
from login_window import LoginWindow
from chat_window import ChatWindow
from request_for_protocol import RequestProcotol
from client_socket import ClientSocket
from threading import Thread
from tkinter.messagebox import showinfo
import sys

from config import DELIMITER, RESPONSE_CHAT

class Client(object):

    def __init__(self):

        ## 初始化登录窗口
        self.window = LoginWindow() 
        self.window.on_reset_button_click(self.clear_inputs)
        self.window.on_login_button_click(self.send_login_data)
        self.window.on_window_close(self.exit)

        self.mycolor =  MYCOLOR
        self.othercolor = OTHERCOLOR

        ## 初始化聊天窗口
        self.window_chat = ChatWindow()
        self.window_chat.withdraw() ## 隐藏窗口
        self.window_chat.on_send_button_click(self.send_chat_data)
        self.window_chat.on_window_close(self.exit)

        self.response_handle_functions = {}
        self.register(RESPONSE_LOGIN_RESULT,self.response_login_handle)
        self.register(RESPONSE_CHAT,self.response_chat_handle)

        self.conn = ClientSocket()

        self.is_running = True

    def register(self,response_id,handle_function):
        self.response_handle_functions[response_id] = handle_function

    def startup(self):
        self.conn.connect()
        self.t = Thread(target=self.response_handle)
        self.t.start()
        self.window.mainloop()

    def clear_inputs(self):

        self.window.clear_username()
        self.window.clear_password()

    def send_login_data(self):
        username = self.window.get_username()
        password = self.window.get_password()

        request_text = RequestProcotol.request_login_result(username,password)
        self.conn.send_data(request_text)


    def send_chat_data(self):
        messages = self.window_chat.get_inputs()
        self.window_chat.clear_inputs()
        request_text = RequestProcotol.request_chat(self.username,messages)
        self.window_chat.append_message(self.nickname+"(myself)",messages,self.mycolor)
        self.conn.send_data(request_text)

    def response_handle(self):

        while self.is_running:

            ## 获取服务器的消息
            recv_data = self.conn.recv_data()

            ## 解析消息内容
            parse_data = self.parse_request_data(recv_data)

            handle_function = self.response_handle_functions.get(parse_data['response_id'])
            if handle_function:
                handle_function(parse_data) 
        sys.exit(0)


    @staticmethod
    def parse_request_data(recv_data):
        """
            登录响应格式  1001|成功/失败|nickname|username
            聊天响应格式  1002|nickname|messages
        """
        response_list = recv_data.split(DELIMITER)

        response_data = {}
        response_data['response_id'] = response_list[0]

        if response_data['response_id'] == RESPONSE_LOGIN_RESULT:
            response_data['result'] = response_list[1]
            response_data['nickname'] = response_list[2]
            response_data['username'] = response_list[3]
        elif response_data['response_id'] == RESPONSE_CHAT:
            response_data['nickname'] = response_list[1]
            response_data['messages'] = response_list[2]

        return response_data


    def response_login_handle(self,response_data):
        result = response_data['result']
        if result == '0':
            showinfo('info','login failed,username or password is wrong!')
            return 
        self.nickname = response_data['nickname']
        self.username = response_data['username']

        ## 显示聊天窗口，隐藏登录窗口
        self.window_chat.set_title(self.nickname)
        self.window_chat.update()
        self.window_chat.deiconify()

        self.window.withdraw()


    def response_chat_handle(self,response_data):
        messages = response_data['messages']
        sender = response_data['nickname']
        self.window_chat.append_message(sender,messages,self.othercolor)

    def exit(self):
        self.is_running = False 
        self.conn.close()
        self.window.destroy()
        sys.exit(0)
        


if __name__ == '__main__':
    Client().startup()




