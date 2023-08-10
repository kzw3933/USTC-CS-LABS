from tkinter import Tk
from tkinter import Label,Entry,Frame,Button,LEFT,END
from config import *


class LoginWindow(Tk):
    def __init__(self):
        super().__init__()
        ## 初始化窗口属性
        self.window_init()
        ## 填充控件
        self.add_widgets()

    def window_init(self):
        """初始化窗口属性"""

        ## 设置窗口名称
        self.title('login')

        ## 设置窗口不能拉伸
        self.resizable(False,False)
        ## 设置窗口大小和位置
        screen_width = self.winfo_screenwidth()
        screen_height = self.winfo_screenheight()
        pos_x = (screen_width-LOGIN_WINDOW_WIDTH)/2
        pos_y = (screen_height-LOGIN_WINDOW_HEIGHT)/2
        self.geometry('%dx%d+%d+%d'%(LOGIN_WINDOW_WIDTH,LOGIN_WINDOW_HEIGHT,pos_x,pos_y))
 
    def add_widgets(self):

        ## 用户名
        username_label = Label(self,font=("Monaco",15))
        username_label['text'] = 'username:'
        username_label.grid(row=0,column=0,rowspan=2,padx=10)
        username_entry = Entry(self,name='username_entry',bd=2,font=("Monaco",15))
        username_entry['width'] = 25
        username_entry.grid(row=0,column=1,rowspan=2,padx=0,pady=15)

        ## 密码
        password_label = Label(self,font=("Monaco",15))
        password_label['text'] = 'password:'
        password_label.grid(row=2,column=0,rowspan=2,padx=10)
        password_entry = Entry(self,name='password_entry',bd=2,font=("Monaco",15))
        password_entry['width'] = 25
        password_entry['show'] = '*'
        password_entry.grid(row=2,column=1,rowspan=2,padx=0,pady=15)

        ## 按钮区
        button_frame = Frame(self,name='button_frame')

        reset_button = Button(button_frame,name='reset_button',font=("Monaco",15))
        reset_button['text'] = 'reset'
        reset_button.pack(side=LEFT,padx=20)

        login_button = Button(button_frame,name='login_button',font=("Monaco",15))
        login_button['text'] = 'login'
        login_button.pack(side=LEFT)

        button_frame.grid(row=6,columnspan=2,pady=10)

    def on_login_button_click(self,command):
        login_button = self.children['button_frame'].children['login_button']
        login_button['command'] = command

    def on_reset_button_click(self,command):
        reset_button = self.children['button_frame'].children['reset_button']
        reset_button['command'] = command

    def get_username(self):

        return self.children['username_entry'].get()

    def get_password(self):
        
        return self.children['password_entry'].get()


    def clear_username(self):
        
        self.children['username_entry'].delete(0,END)

    def clear_password(self):
        
        self.children['password_entry'].delete(0,END)

    def on_window_close(self,command):

        self.protocol('WM_DELETE_WINDOW',command)

if __name__ == '__main__':
    window = LoginWindow()
    window.mainloop()


    


