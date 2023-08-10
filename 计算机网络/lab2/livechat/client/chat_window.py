from time import strftime,time,localtime
from tkinter import END, UNITS, Toplevel
from tkinter.scrolledtext import ScrolledText
from tkinter import Text
from tkinter import Button
from config import *

from config import CHAT_WINDOW_HEIGHT


class ChatWindow(Toplevel):
    def __init__(self):
        super().__init__()

        ## 设置窗口大小
        self.geometry("%dx%d"%(CHAT_WINDOW_WIDTH,CHAT_WINDOW_HEIGHT))
        ## 设置窗口不可修改
        self.resizable(False,False)

        ## 添加组件
        self.add_widget()



    def add_widget(self):
        
        ## 聊天区
        chat_text_area = ScrolledText(self,font=("Monaco",15))
        chat_text_area['width'] = 75
        chat_text_area['height'] = 18
        chat_text_area.grid(row=0,column=0,columnspan=2)

        ## 添加标签
        chat_text_area.tag_config(MYCOLOR,foreground='#0000ff')
        chat_text_area.tag_config(OTHERCOLOR,foreground='#804000')
        chat_text_area.tag_config('black',foreground='#000000')
        self.children['chat_text_area'] = chat_text_area
        

        ## 输入区
        chat_input_area = Text(self,name='chat_input_area',font=("Monaco",15))
        chat_input_area['width'] = 65
        chat_input_area['height'] = 5
        chat_input_area.grid(row=1,column=0,pady=10)

        ## 发送按钮
        send_button = Button(self,name='send_button',font=("Monaco",15))
        send_button['text'] = "send"
        send_button['width'] = 5
        send_button['height'] = 2

        send_button.grid(row=1,column=1)

    def set_title(self,name):
        self.title('welcome to livechat, %s '%(name))

    def on_send_button_click(self,command):
        self.children['send_button']['command'] = command

    def get_inputs(self):

        return self.children['chat_input_area'].get(0.0,END)

    def clear_inputs(self):
        self.children['chat_input_area'].delete(0.0,END)

    def append_message(self,sender,messages,color):
        send_time = strftime('%Y-%m-%d %H:%M:%S',localtime(time()))
        send_info = '%s: %s\n'%(sender,send_time)
        self.children['chat_text_area'].insert(END,send_info,color)
        self.children['chat_text_area'].insert(END,' '+messages+'\n','black')

        self.children['chat_text_area'].yview_scroll(3,UNITS)

    def on_window_close(self,command):

        self.protocol('WM_DELETE_WINDOW',command)

if __name__ == '__main__':
    window = ChatWindow()
    window.mainloop()
    
