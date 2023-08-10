#include "vsprintf.h"

/* 将 printf 格式串转化成可直接输出的字符串，存储在 buf 中 
 * 可以移植或自编，自编至少需要支持 %d （建议移植）
 */
typedef enum bool{false,true} bool ;

int vsprintf(char *buf, const char *fmt, va_list args) {
	char chs[2];
	int buf_index =0;
	int fmt_index = 0;
	
	Buffer buffer;
	bool new = true;
	
	for(;fmt[fmt_index];fmt_index++){
		if(new) {
			chs[0] = fmt[fmt_index];
			new = false;			
		}
		else {
			if(chs[0]=='%') {
				if(fmt[fmt_index]=='d') {
					int num = va_arg(args,int);
		     			buffer.char_num = 0;
		     	  		if(num == 0){
		     	  			buffer.char_buf[buffer.char_num] = '0';
		     	  			buffer.char_num++;
		     	  		}
		     	  		else {
		     	  			int rem;
		     	  			while(num){
		     	  				rem = num % 10;
							num = num /10;
							buffer.char_buf[buffer.char_num] = rem+48;
		     	  				buffer.char_num++;
  							}						     	  
		     	  		}
		     	  		for(int i=0;i<buffer.char_num;i++){		     	  
		     	  			buf[buf_index] = buffer.char_buf[buffer.char_num-i-1];
		     	  			buf_index++;		     	  
		     	  			}
		     	  		
				}
				else {
					chs[1] = fmt[fmt_index];
		     			buf[buf_index] = chs[0];
		     			buf[buf_index+1] = chs[1];
		     			buf_index += 2;				
				}
				new = true;			
			}
			else if(fmt[fmt_index]=='%') {
				buf[buf_index] = chs[0];
				buf_index += 1;
				chs[0] = '%';
			}
			else {
				chs[1] = fmt[fmt_index];
		     		buf[buf_index] = chs[0];
		     		buf[buf_index+1] = chs[1];
		     		buf_index += 2;
		     		new = true;
			}
		     

		}	
	}
	if(!new){
		buf[buf_index] = chs[0];
		buf_index += 1;	
	}
	buf[buf_index] = 0;
	if (buf_index) return buf_index;
	else return -1; 
}
