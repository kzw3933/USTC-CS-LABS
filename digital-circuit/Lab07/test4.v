module homepage #(parameter UP_BOUND = 29,DOWN_BOUND = 628,LEFT_BOUND = 184,RIGHT_BOUND = 983)(              //首页显示,将200*150转为800*600显示
    input up,input down,
    input clk,
    input [10:0] x, 
    input arrowEn,               
    input [9:0] y,
    input [11:0] rgb,
    output reg [11:0] prgb,
    output [14:0] addr
);
wire zone;
wire zone_arrow;

reg [7:0] quot_x;

reg [7:0] quot_y;

parameter X = 600,Y1 = 230,Y2 = 350,Y3 = 480;
reg [10:0] arrow_x = X;
reg [9:0]  arrow_y = Y1;

always @(*) begin
    if(zone)begin
        quot_x = (x-LEFT_BOUND)/4;
        quot_y =  (y - UP_BOUND)/4;
    end
    else begin
        quot_x = 0;
        quot_y = 0;
    end 
end

always @(posedge clk) begin
    if(up)begin
        if(arrow_y == Y1) arrow_y <= Y3;
        else if(arrow_y == Y2) arrow_y <= Y1;
        else if(arrow_y == Y3) arrow_y <= Y2;
    end

    else if(down)begin
        if(arrow_y == Y1) arrow_y <= Y2;
        else if(arrow_y == Y2) arrow_y <= Y3;
        else if(arrow_y == Y3) arrow_y <= Y1;
    end    
end
assign addr = (quot_y-1) * 200+quot_x; 

assign zone = ((x >= LEFT_BOUND)&&(x<=RIGHT_BOUND))&&((y >= UP_BOUND)&&(y <= DOWN_BOUND));

assign zone_arrow = ((x==arrow_x)&&(y==arrow_y)) || ((x==arrow_x+1)&&(y>=(arrow_y-1))&&(y<=(arrow_y+1))) || ((x==arrow_x+2)&&(y>=(arrow_y-2))&&(y<=(arrow_y+2))) || ((x==arrow_x+3)&&(y>=(arrow_y-3))&&(y<=(arrow_y+3))) || ((x==arrow_x+4)&&(y>=(arrow_y-4))&&(y<=(arrow_y+4))) || ((x==arrow_x+5)&&(y>=(arrow_y-5))&&(y<=(arrow_y+5))) || ((x==arrow_x+6)&&(y>=(arrow_y-6))&&(y<=(arrow_y+6)))  || ((x==arrow_x+7)&&(y>=(arrow_y-7))&&(y<=(arrow_y+7))) || ((x==arrow_x+8)&&(y>=(arrow_y-8))&&(y<=(arrow_y+8))) || ((x==arrow_x+9)&&(y>=(arrow_y-9))&&(y<=(arrow_y+9))) ||  ((x==arrow_x+10)&&(y>=(arrow_y-2))&&(y<=(arrow_y+2))) ||   ((x==arrow_x+11)&&(y>=(arrow_y-2))&&(y<=(arrow_y+2))) || ((x==arrow_x+12)&&(y>=(arrow_y-2))&&(y<=(arrow_y+2))) || ((x==arrow_x+13)&&(y>=(arrow_y-2))&&(y<=(arrow_y+2))) || ((x==arrow_x+14)&&(y>=(arrow_y-2))&&(y<=(arrow_y+2))) || ((x==arrow_x+15)&&(y>=(arrow_y-2))&&(y<=(arrow_y+2))) || ((x==arrow_x+16)&&(y>=(arrow_y-2))&&(y<=(arrow_y+2)))|| ((x==arrow_x+17)&&(y>=(arrow_y-2))&&(y<=(arrow_y+2)))|| ((x==arrow_x+18)&&(y>=(arrow_y-2))&&(y<=(arrow_y+2)))||((x==arrow_x+19)&&(y>=(arrow_y-2))&&(y<=(arrow_y+2)))||((x==arrow_x+20)&&(y>=(arrow_y-2))&&(y<=(arrow_y+2)));                             



always @(*) begin
    if(arrowEn)begin
        if(zone)begin
            if(zone_arrow)    prgb = 12'h000;
            else prgb = rgb;
        end
        else prgb = 12'h000;
    end
    else begin
        if(zone)begin
            prgb = rgb;
        end
        else prgb = 12'h000;
    end

end

  
endmodule


module vdt_hp (input pclk,
            output  hs,vs,
            output [10:0] x,
            output [9:0] y
            );
    parameter henMax=1039 ,venMax=665;
    //HBP =64  ,HFP = 56,VBP = 23  ,VFP= 37 ;
    parameter UP_BOUND = 29;
	parameter DOWN_BOUND = 628;
	parameter LEFT_BOUND = 184;
	parameter RIGHT_BOUND = 983;
    parameter HSW = 120,VSW=6;
    reg [10:0] hcnt=0;
    reg [9:0] vcnt=0;

    assign x = hcnt;
    assign y = vcnt;

    always @(posedge pclk) begin
        if(hcnt==henMax)begin
            hcnt <= 0;
            vcnt <=(vcnt==venMax)?0:(vcnt+1);
        end
        else hcnt <= hcnt +1;   
    end


    assign hs = (hcnt>(HSW-1))?0:1;
    assign vs = (vcnt>(VSW-1))?0:1;
endmodule

module top (
    input clk,
    input up,
    input down,
    output hs,vs,
    output [11:0] prgb
);

wire [10:0] x;
wire [9:0] y;
wire [14:0] addr;
wire pclk;
wire [11:0] rgb;

clk_wiz_0 PCLK(.clk_in1(clk),.clk_out1(pclk));   
vdt_hp VDT(.pclk(pclk),.hs(hs),.vs(vs),.x(x),.y(y));
blk_mem_gen_0 ROM(.clka(clk),.addra(addr),.douta(rgb),.ena(1));
homepage hp(.x(x),.clk(clk),.up(up),.down(down),.rgb(rgb),.y(y),.prgb(prgb),.addr(addr));
    
endmodule









module CPU (
    input clk,//rstn
    // input enter,
    // input up,
    // input down,
    // input back,
    input txd,                   //串口通信
    input ps2_clk,ps2_data,          //PS2接口
    output ja,jc,                  //音频接口 
    output reg [11:0] prgb,            //VGA的色素值接口
    output reg hs,vs                  //VGA的行同步与列同步接口

);


wire [11:0] prgb1;

//?????
wire pclk;
clk_wiz_0 PCLK(.clk_in1(clk),.clk_out1(pclk));  


//??????????
// wire hs2,vs2;
reg [11:0] prgb2 = 12'hFFF;
// video VDT(.clk(pclk),.rstn(rstn),.txd(txd),.hs(hs2),.vs(vs2),.prgb(prgb2)) ;

//??????????
reg en1;
// auto_Buzzer BUZZER(.clk(pclk),.rstn(rstn),.en(en1),.sw(sw),.ja(ja),.jc(jc));


//??????????
reg en2;
wire [1:0] sw;
// paino  PIANO(.clk(clk),.rstn(rstn),.en(en2),.ps2_clk(ps2_clk),.ps2_data(ps2_data),.ja(ja));



//???????????
wire hs1,vs1;
wire [10:0] x;
wire [9:0] y;

wire Up,Down,Enter,Back;
wire [14:0] addr;
wire [11:0] rgb1,rgb2,rgb3;
reg [11:0] rgb;
reg arrowEn  = 1;




vdt_hp VDTHP(.pclk(pclk),.hs(hs1),.vs(vs1),.x(x),.y(y));
blk_mem_gen_1 ROM1(.clka(clk),.addra(addr),.douta(rgb1),.ena(1));
blk_mem_gen_2 ROM2(.clka(clk),.addra(addr),.douta(rgb2),.ena(1));
blk_mem_gen_3 ROM3(.clka(clk),.addra(addr),.douta(rgb3),.ena(1));
homepage HOMEPAGE(.up(up),.down(down),.clk(clk),.x(x),.y(y),.rgb(rgb),.prgb(prgb1),.addr(addr),.arrowEn(arrowEn));        


parameter X = 600,Y1 = 150,Y2 = 340,Y3 = 470;
reg [1:0] choice1 = 0;
reg [1:0] choice2 = 0;
assign sw = choice2;


reg hs2 = 0;
reg vs2 = 0;



always @(posedge clk) begin
    if(back) choice1 <= 2'b00;
    else begin
        case(choice1) 
        2'b00:begin
            if((x==X)&&enter)begin
                if(y == Y1)
                    choice1 <= 2'b01;
                else if(y == Y2)
                    choice1 <= 2'b10;
                else if(y == Y3)
                    choice1 <= 2'b11;

            end
        end 
        2'b01:begin
              if((x==X)&&enter)begin
                if(y == Y1)
                    choice2 <= 2'b01;
                else if(y == Y2)
                    choice2 <= 2'b10;
                else if(y == Y3)
                    choice2 <= 2'b11;

            end

        end
        endcase
    end
end

always @(*) begin
    case(choice1)
        2'b00: begin
            rgb = rgb1;
            prgb = prgb1;
            arrowEn = 1;
            hs = hs1;
            vs = vs1;
        end
         2'b01: begin
            rgb = rgb2;
            prgb = prgb1;
            hs = hs1;
            vs = vs1;
            arrowEn = 1;
        end
         2'b10: begin
            rgb = rgb3;
            prgb = prgb1;
            hs = hs1;
            vs = vs1;
            arrowEn = 0;
        end
         2'b11: begin
            rgb = 12'h000;
            prgb = prgb2;
            hs = hs2;
            vs = vs2;
            arrowEn = 0;
        end
    endcase
end


always @(*) begin
    case(choice1)
        2'b00:begin
            en1 = 0;
            en2 = 0;
        end
        2'b01:begin
            en1 = 1;
            en2 = 0;
        end
        2'b10:begin
            
            en1 = 0;
            en2 = 1;
        end
        2'b11:begin
            en1 = 0;
            en2 = 0;
        end
    endcase
end
endmodule


module cpu_tb();


reg clk;


wire enter,up,down,back;

assign enter = 0;
assign up = 0;
assign down = 0;
assign back = 0;

wire hs,vs;


wire [11:0] prgb;


CPU cpu(.clk(clk),.enter(enter),.up(up),.down(down),.back(back),.prgb(prgb),.hs(hs),.vs(vs));

initial begin
    clk = 1;
    forever #1 clk = ~clk;
end
endmodule





module ps2_keyboard_driver(
input clk,		//50M时钟信号
input rstn,	//复位信号
input ps2_clk,	//PS2接口时钟信号
input ps2_data,		//PS2接口数据信号
output reg [7:0] ps2_asci,	// 1byte键值，只做简单的按键扫描
output ps2_state		//键盘当前状态，ps2_state=1表示有键被按下 
);
//------------------------------------------
wire [7:0] ps2_byte;
reg ps2_clk_r0,ps2_clk_r1,ps2_clk_r2;	//2_clk状态寄存器



//wire pos_ps2_clk; 	// ps2_clk上升沿标志位
wire neg_ps2_clk;	// ps2_clk下降沿标志位
//设备发送向主机的数据在下降沿有效，首先检测ps2_clk的下降沿
//利用上面逻辑赋值语句可以提取得下降沿，neg_ps2_clk为高电平时表示数据可以被采集
always @ (posedge clk or negedge rstn) begin
	if(!rstn) begin
			ps2_clk_r0 <= 1'b0;
			ps2_clk_r1 <= 1'b0;
			ps2_clk_r2 <= 1'b0;
		end
	else begin								//锁存状态，进行滤波
			ps2_clk_r0 <= ps2_clk;
			ps2_clk_r1 <= ps2_clk_r0;
			ps2_clk_r2 <= ps2_clk_r1;
		end
end
 
assign neg_ps2_clk = ~ps2_clk_r1 & ps2_clk_r2;	//下降沿
 
//-----------------数据采集-------------------------
	/*
	帧结构：设备发往主机数据帧为11比特，（主机发送数据包为12bit） 
			1bit start bit ,This is always 0,
			 8bit data bits, 
			 1 parity bit,(odd parity)校验位，奇校验，
			 data bits 为偶数个1时该位为1，
			 data bits 为奇数个1时该位为0.
	         1bit stop bit ,this is always 1.
				num 范围为 'h00,'h0A;
	*/
reg[7:0] ps2_byte_r;		//PC接收来自PS2的一个字节数据存储器
reg[7:0] temp_data;			//当前接收数据寄存器
reg[3:0] num;				//计数寄存器
 
always @ (posedge clk or negedge rstn) begin
	if(!rstn) begin
			num <= 4'd0;
			temp_data <= 8'd0;
		end
	else if(neg_ps2_clk) begin	//检测到ps2_clk的下降沿
			case (num)
			 /*
		帧结构中数据位为一个字节，且低位在前，高位在后，
		这里要定义一个buf,size is one Byte.
	 */   
				4'd0:	num <= num+1'b1;
				4'd1:	begin
							num <= num+1'b1;
							temp_data[0] <= ps2_data;	//bit0
						end
				4'd2:	begin
							num <= num+1'b1;
							temp_data[1] <= ps2_data;	//bit1
						end
				4'd3:	begin
							num <= num+1'b1;
							temp_data[2] <= ps2_data;	//bit2
						end
				4'd4:	begin
							num <= num+1'b1;
							temp_data[3] <= ps2_data;	//bit3
						end
				4'd5:	begin
							num <= num+1'b1;
							temp_data[4] <= ps2_data;	//bit4
						end
				4'd6:	begin
							num <= num+1'b1;
							temp_data[5] <= ps2_data;	//bit5
						end
				4'd7:	begin
							num <= num+1'b1;
							temp_data[6] <= ps2_data;	//bit6
						end
				4'd8:	begin
							num <= num+1'b1;
							temp_data[7] <= ps2_data;	//bit7
						end
				4'd9:	begin
							num <= num+1'b1;	//奇偶校验位，不做处理
						end
				4'd10: begin
							num <= 4'd0;	// num清零
						end
				default: ;
				endcase
		end	
end
 
reg key_f0;		//松键标志位，置1表示接收到数据8'hf0，再接收到下一个数据后清零
reg ps2_state_r;	//键盘当前状态，ps2_state_r=1表示有键被按下 
//+++++++++++++++数据处理开始++++++++++++++++=============
always @ (posedge clk or negedge rstn) begin	//接收数据的相应处理，这里只对1byte的键值进行处理
	if(!rstn) begin
			key_f0 <= 1'b0;
			ps2_state_r <= 1'b0;
		end
	else if(num==4'd10) ///一帧数据是否采集完。
			begin	//刚传送完一个字节数据
					if(temp_data == 8'hf0) key_f0 <= 1'b1;//判断该接收数据是否为断码
				else
					begin
					//========================理解困难==================================
						if(!key_f0) 
								begin	//说明有键按下
									ps2_state_r <= 1'b1;
									ps2_byte_r <= temp_data;	//锁存当前键值
								end
						else 
								begin
									ps2_state_r <= 1'b0;
									key_f0 <= 1'b0;
								end
					//=====================================================
					end
			end
end

assign ps2_byte = ps2_asci;	 
assign ps2_state = ps2_state_r; 

always @ (ps2_byte_r) begin
	case (ps2_byte_r)		//键值转换为ASCII码，这里做的比较简单，只处理字母
		8'h15: ps2_asci <= 8'h51;	//Q
		8'h1d: ps2_asci <= 8'h57;	//W
		8'h24: ps2_asci <= 8'h45;	//E
		8'h2d: ps2_asci <= 8'h52;	//R
		8'h2c: ps2_asci <= 8'h54;	//T
		8'h35: ps2_asci <= 8'h59;	//Y
		8'h3c: ps2_asci <= 8'h55;	//U
		8'h43: ps2_asci <= 8'h49;	//I
		8'h44: ps2_asci <= 8'h4f;	//O
		8'h4d: ps2_asci <= 8'h50;	//P				  	
		8'h1c: ps2_asci <= 8'h41;	//A
		8'h1b: ps2_asci <= 8'h53;	//S
		8'h23: ps2_asci <= 8'h44;	//D
		8'h2b: ps2_asci <= 8'h46;	//F
		8'h34: ps2_asci <= 8'h47;	//G
		8'h33: ps2_asci <= 8'h48;	//H
		8'h3b: ps2_asci <= 8'h4a;	//J
		8'h42: ps2_asci <= 8'h4b;	//K
		8'h4b: ps2_asci <= 8'h4c;	//L
		8'h1a: ps2_asci <= 8'h5a;	//Z
		8'h22: ps2_asci <= 8'h58;	//X
		8'h21: ps2_asci <= 8'h43;	//C
		8'h2a: ps2_asci <= 8'h56;	//V
		8'h32: ps2_asci <= 8'h42;	//B
		8'h31: ps2_asci <= 8'h4e;	//N
		8'h3a: ps2_asci <= 8'h4d;	//M
		default: ;
		endcase
end
 
endmodule
