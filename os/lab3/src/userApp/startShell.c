#include "io.h"
#include "myPrintk.h"
#include "uart.h"
#include "vga.h"
#include "i8253.h"
#include "i8259A.h"
#include "tick.h"
#include "wallClock.h"

#define MaxCmdNum 20

typedef struct myCommand {
    char name[80];
    char help_content[200];
    int (*func)(int argc, char (*argv)[8]);
} myCommand; 

typedef struct cmdList {
    myCommand cmds[MaxCmdNum];
    int cmd_num;
} cmdList;

int func_cmd(int argc, char (*argv)[8]);
int func_help(int argc, char (*argv)[8]);
int func_exit(int argc, char (*argv)[8]);
int func_divZero(int argc, char (*argv)[8]);


cmdList cmd_list;
myCommand cmd={"cmd\0","List all command\n\0",func_cmd};
myCommand help={"help\0","Usage: help [command]\n\0Display info about [command]\n\0",func_help};
myCommand exit = {"exit\0","exit shell\n\0",func_exit};
myCommand divZero = {"divZero\0","test divide 0 interrupt\n\0",func_divZero};

int func_cmd(int argc, char (*argv)[8]){
    for(int i=0;i<cmd_list.cmd_num;i++){
        myPrintk(0x07,(cmd_list.cmds)[i].name);
        myPrintk(0x07," ");
    }
    myPrintk(0x07,"\n");
    return 0;
} 

int func_help(int argc, char (*argv)[8]){
    if(argc > 2)
        myPrintk(0x07,"The format is wrong,please check again!\n");
    else if(argc == 1)
        myPrintk(0x07,(cmd_list.cmds)[1].help_content);
    else {
        int i = get_cmd_index(cmd_list,argv[1]);
        myPrintk(0x07,(cmd_list.cmds)[i].help_content);
    }
     return 0;   
}

int func_exit(int argc,char (*argv)[8]){
    return 0;
}

int func_divZero(int argc,char (*argv)[8]){
    int a = 1/0;
    return 0;
}

void startShell(void){
//我们通过串口来实现数据的输入
    char BUF[256]; //输入缓存区
    int BUF_len=0;	//输入缓存区的长度

    (cmd_list.cmds)[0] = cmd;
    (cmd_list.cmds)[1] = help;
    (cmd_list.cmds)[2] = exit;
    (cmd_list.cmds)[3] = divZero;
    cmd_list.cmd_num = 4;
        
	int argc;
    char argv[8][8];
    int cont = 1;

    do{
        BUF_len=0; 
        myPrintk(0x07,"Student>>\0");
        while((BUF[BUF_len]=uart_get_char())!='\r'){
            uart_put_char(BUF[BUF_len]);//将串口输入的数存入BUF数组中
            BUF_len++;  //BUF数组的长度加
        }
        BUF[BUF_len] = 0; 
        // myPrintk(0x07,BUF);
        uart_put_chars(" -pseudo_terminal\0");
        append2screen(BUF,0x07);
        append2screen(" -pseudo_terminal\0",0x07);
        myPrintk(0x07,"\n");                 
        parse_command_from_buf(BUF,&argc,argv);
        // myPrintk(0x07,BUF);
        cont = execute_command(cmd_list,argc,argv);

        //OK,助教已经帮助你们实现了“从串口中读取数据存储到BUF数组中”的任务，接下来你们要做
        //的就是对BUF数组中存储的数据进行处理(也即，从BUF数组中提取相应的argc和argv参
        //数)，再根据argc和argv，寻找相应的myCommand ***实例，进行***.func(argc,argv)函数
        //调用。

        //比如BUF中的内容为 “help cmd”
        //那么此时的argc为2 argv[0]为help argv[1]为cmd
        //接下来就是 help.func(argc, argv)进行函数调用即可

    }while(cont);

}

void parse_command_from_buf(char* buf,int *argc,char (*argv)[8]){
    curtail(buf);
    int i,j;
    // myPrintk(0x07,buf);
    if(buf[0] == 0){
        *argc = 0;
        argv[0][0] = 0;
    }
    else {
        *argc = 0;
        int index = 0;
        for(i=0;buf[index]!=0;i++){
            for(j=0;buf[index]!=' ' && buf[index] != 0;j++){
                argv[i][j] = buf[index];
                index++;
            }
            if(buf[index]==' ')
                index++;
            (*argc) ++;
            argv[i][j] = 0;
        }
    }
}

void curtail(char* buf){   
    int i = 0;
    while(buf[i]==' ' || buf[i] == '\r' || buf[i] == '\n' || buf[i] == '\t' ) 
        i++;
    int j = 0;
    while(buf[i+j]!=0){
        buf[j] = buf[i+j];
        j++;
    }
}

int execute_command(cmdList cmd_list,int argc,char (*argv)[8]){
    int i = get_cmd_index(cmd_list,argv[0]);
    // myPrintk(0x07,argv[0]);
    // myPrintk(0x07,"\n");
    // myPrintk(0x07,"%d\n",i);
    if(i<0) 
        myPrintk(0x07,"command not found!\n");
    else
        ((cmd_list.cmds)[i]).func(argc,argv);
    if(i == 2)
        return 0;
    else 
        return 1;
}

int get_cmd_index(cmdList cmd_list,char* cmd_name){
    int i = 0;
    // myPrintk(0x07,cmd_name);
    while(i < cmd_list.cmd_num){       
        if(compare_str(((cmd_list.cmds)[i]).name,cmd_name)==0)
            break;
        else 
            i++;
    }
    // myPrintk(0x07,"%d\n",i);
    if(i==cmd_list.cmd_num)
        return -1;
    else 
        return i;
}

int compare_str(char* str1,char* str2){
    int i = 0;
    if(str1[0]==0 && str2[0] == 0) return 0;
    else if(str1[0] == 0 || str2[0] == 0) return -1;
    else {
        for(;str1[i]!=0;i++){
            if(str1[i] != str2[i])
                break;
        }
        return str1[i] - str2[i];
    }
}
