#include "io.h"
#include "vga.h"

#define VGA_BASE 0xB8000 // vga 显存起始地址
#define VGA_SCREEN_WIDTH 80 // vga 屏幕宽度（可容纳字符数）
#define VGA_SCREEN_HEIGHT 25 // vga 屏幕高度

#define CURSOR_LINE_REG 0xE // 行号寄存器
#define CURSOR_COL_REG 0xF // 列号寄存器
#define CURSOR_INDEX_PORT 0x3D4 // 光标行列索引端口号
#define CURSOR_DATA_PORT 0x3D5 // 光标数据端口号

/* ========= 以下函数仅供参考，可以根据自己的需求进行修改，甚至删除 ========= */

/* 将光标设定到特定位置
 * 提示：使用 outb */
void set_cursor_pos(cursorPos pos) {
    /* todo */
    outb(CURSOR_INDEX_PORT,CURSOR_LINE_REG );
    outb(CURSOR_DATA_PORT ,pos.line);
    outb(CURSOR_INDEX_PORT,CURSOR_COL_REG );
    outb(CURSOR_DATA_PORT,pos.col);
    
}

/* 获取光标当前所在位置
 * 提示：使用 inb */ 
cursorPos get_cursor_pos(void) {
    /* todo */
    cursorPos pos;
    outb(CURSOR_INDEX_PORT,CURSOR_LINE_REG);
    pos.line = inb(CURSOR_DATA_PORT);
    outb(CURSOR_INDEX_PORT,CURSOR_COL_REG);
    pos.col = inb(CURSOR_DATA_PORT);
    return pos;
}

/* 滚屏，vga 屏幕满时使用。丢弃第一行内容，将剩余行整体向上滚动一行
 * 提示：使用指针修改显存 */
void scroll_screen(void) {
    /* todo */
    unsigned char* ptr = VGA_BASE;
    int i=0;
    for(;i<80*24;i+=1){
    	*(ptr+2*i) = *(ptr+2*i+80*2);
    	*(ptr+2*i+1) = *(ptr+2*i+80*2+1);
    }
     for(;i<80*25;i+=1){
    	*(ptr+2*i) = 0;
    	*(ptr+2*i+1) = 0x2;
    }
    
}

/* 向 vga 的特定光标位置 pos 输出一个字符
 * 提示：使用指针修改显存 */
 
// 只处理常规输出
void put_char2pos(unsigned char c, int color, cursorPos pos) {
    /* todo */
    unsigned char* ptr = VGA_BASE + pos.line*80*2+pos.col*2;
    *ptr = c;
    *(ptr+1) = color;
}

/* ========= 以下函数接口禁止修改 ========= */

/* 清除屏幕上所有字符，并将光标位置重置到顶格
 * 提示：使用指针修改显存 */
void clear_screen(void) {
    /* todo */
    unsigned char* ptr = VGA_BASE;
    cursorPos pos = {0,0};
    *(ptr) = 0;
    *(ptr+1) = 0x2;
    for(int i=1;i<80*25;i+=1){
    	*(ptr+2*i) = 0;
    	*(ptr+2*i+1) = 0;
    }
    set_cursor_pos(pos);   
}

/* 向 vga 的当前光标位置输出一个字符串，并移动光标位置到串末尾字符的下一位
 * 如果超出了屏幕范围，则需要滚屏
 * 需要能够处理转义字符 \n */
void append2screen(char *str, int color) { 
    /* todo */
    cursorPos pos = get_cursor_pos();
    char ch;
    for(int i=0;str[i]!=0;i++){
    	ch = str[i];
    	
    	if(ch == '\n') {
    		if(pos.line == 24) {
    			scroll_screen();
    			pos.line = 24;
    			pos.col = 0;
    		}
    		else {
    			pos.col = 0;
    			pos.line += 1;
    		}
    		
    	}
    	else {   	
    		put_char2pos(ch,color,pos);
    		if(pos.col == 79){
    			if(pos.line == 24) {
    				scroll_screen();
    			}
    			else {
    			     pos.line += 1;
    			     pos.col = 0;
    			}
    		}
    		else pos.col += 1;
    	}  
    	
    }
    set_cursor_pos(pos);
    
}
