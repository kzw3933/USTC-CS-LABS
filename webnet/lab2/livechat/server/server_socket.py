import socket
from config import * 

class ServerSocket(socket.socket):

    def __init__(self):
        super().__init__(socket.AF_INET,socket.SOCK_STREAM)   ## 使用ipv4和tcp协议
        self.bind((SERVER_IP,SERVER_PORT))  ## 绑定ip地址与端口号
        self.listen(128)                    ## 设置监听模式



class SocketWrapper(object):

    def __init__(self,soc):
        self.soc = soc

    def recv_data(self):
        try:
            return self.soc.recv(512).decode('utf-8')
        except:
            return ""

    def send_data(self,messages):

        return self.soc.send(messages.encode('utf-8'))

    def close(self):
        self.soc.close()





    
        