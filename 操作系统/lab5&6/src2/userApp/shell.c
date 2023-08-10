//shell.c --- malloc version
#include "../myOS/userInterface.h"


//获取终端用户输入并回显
int getCmdline(unsigned char *buf, int limit){
    unsigned char *ptr = buf;
    int n = 0;
    while (n < limit) {
        *ptr = uart_get_char();        
        if (*ptr == 0xd) {
            *ptr++ = '\n';
            *ptr = '\0';
            uart_put_char('\r');
            uart_put_char('\n');
            return n+2;
        }
        uart_put_char(*ptr);
        ptr++;
        n++;
    }       
    return n;
}
//计算字符串的长度
int strlen(unsigned char* str){
    int i=0;
    while(str[i]){
        i++;
    }
    return i;
}
// 定义命令的结构体
typedef struct cmd {
    unsigned char cmd[20+1]; //TODO: dynamic
    int (*func)(int argc, unsigned char **argv);
    void (*help_func)(void);
    unsigned char description[100+1]; //TODO: dynamic?   
    struct cmd * nextCmd;
} cmd;

#define cmd_size sizeof(struct cmd)

//在内存里维护命令的动态链表
struct cmd *ourCmds = 0;


//依据命令的动态链表打印当前所有命令
int listCmds(int argc, unsigned char **argv){
    struct cmd *tmpCmd = ourCmds;
    myPrintf(0x7, "list all registered commands:\n");
    myPrintf(0x7, "command name: description\n");

    while (tmpCmd != 0) {
        myPrintf(0x7,"% 12s: %s\n",tmpCmd->cmd, tmpCmd->description);
        tmpCmd = tmpCmd->nextCmd;
    }
    return 0;
}

// 向已有命令链表中动态添加一个命令
void addNewCmd(	unsigned char *cmd, 
		int (*func)(int argc, unsigned char **argv), 
		void (*help_func)(void), 
		unsigned char* description){
	//本函数需要实现！！！
    /*功能：增加命令
    1.使用malloc创建一个cmd的结构体，新增命令。
    2.同时还需要维护一个表头为ourCmds的链表。
    */
   
   struct cmd* new_cmd = (struct cmd*) malloc (cmd_size); 
   new_cmd->func = func;
   new_cmd->help_func = help_func;
   
   strncpy(cmd,new_cmd->cmd,strlen(cmd));
   new_cmd->cmd[strlen(cmd)] = 0;
   strncpy(description,new_cmd->description,strlen(description));
   new_cmd->description[strlen(description)] = 0;
   new_cmd->nextCmd = 0;
    
   if(help_func){
       new_cmd->help_func = help_func;
   }
    
   //维护命令的动态链表
   if(ourCmds == 0){
       ourCmds = new_cmd;
   }
   else {
       new_cmd->nextCmd = ourCmds->nextCmd;
       ourCmds->nextCmd = new_cmd;
   }
}

void help_help(void){
    myPrintf(0x7,"USAGE: help [cmd]\n\n");
}

int help(int argc, unsigned char **argv){
    int i;
    struct cmd *tmpCmd = ourCmds;
    if (argc==1) {
        help_help();
        return 0;
    }
    if (argc>2) return 1;
    
    while (tmpCmd != 0) {            
        if (strcmp(argv[1],tmpCmd->cmd)==0) {
            if (tmpCmd->help_func!=0)
                tmpCmd->help_func();
            else myPrintf(0x7,"%s\n",tmpCmd->description);
            break;
        }
        tmpCmd = tmpCmd->nextCmd;
    }
    return 0;
}

struct cmd *findCmd(unsigned char *cmd){
        struct cmd * tmpCmd = ourCmds;
	int found = 0;
        while (tmpCmd != 0) {  //at lease 2 cmds            
            if (strcmp(cmd,tmpCmd->cmd)==0){
		    found=1;
		    break;
	    }
            tmpCmd = tmpCmd->nextCmd;
        }
	return found?tmpCmd:0;
}

int split2Words(unsigned char *cmdline, unsigned char **argv, int limit){
    unsigned char c, *ptr = cmdline;
    int argc=0;    
    int inAWord=0;

    while ( c = *ptr ) {  // not '\0'
        if (argc >= limit) {
            myPrintf(0x7,"cmdline is tooooo long\n");
            break;
        }
        switch (c) {
            case ' ':  *ptr = '\0'; inAWord = 0; break; //skip white space
            case '\n': *ptr = '\0'; inAWord = 0; break; //end of cmdline?
            default:  //a word
             if (!inAWord) *(argv + argc++) = ptr;
             inAWord = 1;             
             break;
        }   
        ptr++;     
    }
    return argc;
}


void initShell(void){
    addNewCmd("cmd\0",listCmds,0,"list all registered commands\0");
    addNewCmd("help\0",help,help_help,"help [cmd]\0");
    //TODO: may be we can add a new command exit or quit
    addNewCmd("exit\0",0,0,"Exit shell\0");
}

unsigned char cmdline[100];
void startShell(void){    
    unsigned char *argv[10];  //max 10
    int argc;    
    struct cmd *tmpCmd;    
    dPartitionWalkByAddr(pMemHandler);
    char exit_or_not = 0;
    //实现有条件循环以提供退出功能
    while(!exit_or_not) {
        myPrintf(0x3,"Student >:");
        getCmdline(&cmdline[0],100);
        myPrintf(0x7,cmdline);
        argc = split2Words(cmdline,&argv[0],10); 
        if (argc == 0) continue;
	    tmpCmd = findCmd(argv[0]);
        if (tmpCmd) {
            if(! tmpCmd->func){
                exit_or_not = 1;
            }
            else {
                tmpCmd->func(argc, argv);
            }	        
        }            
	    else
            myPrintf(0x7,"UNKOWN command: %s\n",argv[0]);
    }
}