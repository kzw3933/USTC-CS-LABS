from threading import Thread
from config import *
from response_for_protocol import ResponseProtocol
from server_socket import ServerSocket, SocketWrapper
from db import DB


class Server(object):

    def __init__(self):
        ## 创建服务器套接字
        self.server_socket = ServerSocket()

        ## 创建请求id及其对应的处理函数
        self.request_handle_functions = {}
        self.register(REQUEST_LOGIN, self.request_login_handle)
        self.register(REQUEST_CHAT, self.request_chat_handle)

        ## 创建保存当前登录用户的字典
        self.clients = {}

        ## 创建数据库对象
        self.db = DB()

    def register(self, request_id, handle_function):
        ## 注册消息类型及处理函数
        self.request_handle_functions[request_id] = handle_function

    def startup(self):

        while True:
            print("Server is listening on port "+str(SERVER_PORT))
            soc, addr = self.server_socket.accept()
            client_ip,client_port = soc.getpeername()
            print("\033[31m Accept connect from Ip: ("+str(client_ip)+") Port: ("+str(client_port)+")\033[0m")
            client_soc = SocketWrapper(soc)

            Thread(target=lambda: self.request_handle(client_soc)).start()

    def request_handle(self, client_soc):
        client_ip,client_port = client_soc.soc.getpeername()

        while True:

            ## 接收客户端请求
            recv_data = client_soc.recv_data()

            if not recv_data:
                self.remove_offline_user(client_soc)
                client_soc.close()
                break


            print("\033[32m Recv data from Ip: ("+str(client_ip)+") Port: ("+str(client_port)+") Message: ("+recv_data.strip("\n")+")\033[0m")

            ## 解析客户端请求
            parse_data = self.parse_request_data(recv_data)

            ## 判断请求类型
            handle_function = self.request_handle_functions.get(parse_data['request_id'])

            if handle_function:
                handle_function(client_soc, parse_data)

                ## 调用请求对应的处理函数

    def remove_offline_user(self, client_soc):
        client_ip,client_port = client_soc.soc.getpeername()
        print("\033[31m Client Ip: ("+str(client_ip)+") Port: ("+str(client_port)+") is offline\033[0m")
        for username, info in self.clients.items():
            if info['sock'] == client_soc:
                del self.clients[username]
                break

    @staticmethod
    def parse_request_data(text):
        """
            登录信息格式  0001|username|password
            聊天信息格式  0002|username|messages
        """

        request_list = text.split(DELIMITER)

        request_data = {}

        request_data['request_id'] = request_list[0]

        if request_data['request_id'] == REQUEST_LOGIN:
            request_data['username'] = request_list[1]
            request_data['password'] = request_list[2]
        elif request_data['request_id'] == REQUEST_CHAT:
            request_data['username'] = request_list[1]
            request_data['messages'] = request_list[2]

        return request_data

    def request_login_handle(self, client_sock, request_data):

        username = request_data['username']
        password = request_data['password']

        ret, nickname, username = self.check_user_login(username, password)

        if ret == '1':
            self.clients[username] = {'sock': client_sock, 'nickname': nickname}

        response_text = ResponseProtocol.response_login_result(ret, nickname, username)

        client_sock.send_data(response_text)

    def request_chat_handle(self, client_soc, request_data):
        ## 获取消息内容
        username = request_data['username']
        messages = request_data['messages']
        nickname = self.clients[username]['nickname']
        ## 拼接消息文本
        msg = ResponseProtocol.response_chat(nickname, messages)
        ## 转发消息给在线用户
        for u_name, info in self.clients.items():
            if username == u_name:
                continue
            info['sock'].send_data(msg)

    def check_user_login(self, username, password):
        result = self.db.get_user(username)

        if not result:
            return '0', '', username

        if password != result['password']:
            return '0', '', username

        return '1', result['nickname'], username


if __name__ == '__main__':
    Server().startup()
