module CPU (
    input clk,rstn,
    input Up,Down,
    input Enter,
    input Back,
    input txd,                   //串口通信
    input ps2_clk,ps2_data,          //PS2接口
    input Play,
    output reg ja,
    output jc,                  //音频接口 
    output reg [11:0] prgb,            //VGA的色素值接口
    output reg hs,vs,                  //VGA的行同步与列同步接口
    output wire pwm 
);

wire up,down,enter,back;


//分频模块
wire pclk;
CLK50MHZ PCLK(.clk_in1(clk),.clk_out1(pclk));  

getEdge UpEdge(.x(Up),.pclk(pclk),.xEdge(up),.rstn(rstn));
getEdge DnEdge(.x(Down),.pclk(pclk),.xEdge(down),.rstn(rstn));
getEdge EnterEdge(.x(Enter),.pclk(pclk),.xEdge(enter),.rstn(rstn));
getEdge BackEdge(.x(Back),.pclk(pclk),.xEdge(back),.rstn(rstn));



wire [11:0] prgb1;

assign pwm=ja;



//视频显示模块
wire hs2,vs2;
reg en3;
wire [11:0] prgb2;
wire ja3;
video VDT(.clk(clk),.en(en3),.txd(txd),.hs(hs2),.vs(vs2),.prgb(prgb2),.ja(ja3)) ;

//音乐模块
reg en1;
wire soundUp,soundDn;
wire [1:0] sw;
wire ja1;
auto_Buzzer BUZZER(.clk(clk),.rstn(rstn),.en(en1),.sw(sw),.up(soundUp),.down(soundDn),.ja(ja1),.jc(jc));

//弹琴模块

// wire up,down,enter;
// reg back;
reg en2;
wire ja2;
piano  PIANO(.clk(clk),.rstn(rstn),.en(en2),.up(soundUp),.down(soundDn),.ps2_clk(ps2_clk),.ps2_data(ps2_data),.JA(ja2));


always @(*) begin
    if(en1) ja = ja1;
    else if(en2) ja = ja2;
    else if(en3) ja = ja3;
end


//界面及显示模块

wire hs1,vs1;

wire [10:0] x;
wire [9:0] y;

wire [14:0] addr;
wire [11:0] rgb1,rgb2,rgb3;
wire dot1,dot2,dot3;


reg [11:0] rgb;
reg arrowEn  = 1;
wire [7:0] Key;
wire Key_State;

wire [9:0]arrow;

wire [10:0] arrow_x;
wire [9:0] arrow_y;
assign arrow = arrow_y;
wire zone_arrow;
vdt_hp VDTHP(.pclk(pclk),.hs(hs1),.vs(vs1),.x(x),.y(y));

ROM1 rom1(.clka(clk),.addra(addr),.douta(dot1),.ena(1));
ROM2 rom2(.clka(clk),.addra(addr),.douta(dot2),.ena(1));
ROM3 rom3(.clka(clk),.addra(addr),.douta(dot3),.ena(1));

asciiArt ASCII1(.x(x),.y(y),.pclk(pclk),.dot(dot1),.prgb(rgb1));
asciiArt ASCII2(.x(x),.y(y),.pclk(pclk),.dot(dot2),.prgb(rgb2));
asciiArt ASCII3(.x(x),.y(y),.pclk(pclk),.dot(dot3),.prgb(rgb3));

ps2_keyboard_driver KeyBoard(.clk(pclk),.rstn(rstn),.ps2_clk(ps2_clk),.ps2_data(ps2_data),.ps2_asci(Key),.ps2_state(Key_State));
homepage HOMEPAGE(.up(up),.down(down),.rstn(rstn),.clk(pclk),.x(x),.y(y),.rgb(rgb),.prgb(prgb1),.addr(addr),.arrowEn(arrowEn),.arrow_x(arrow_x),.arrow_y(arrow_y),.zone_arrow(zone_arrow));        
Zone_Arrow ZONE_ARROW(.x(x),.y(y),.arrow_x(arrow_x),.arrow_y(arrow_y),.zone_arrow(zone_arrow));
// assign up = (Key == 8'h45)&&Key_State;

// assign down =  (Key == 8'h53)&&Key_State;

// assign enter = (Key == 8'h44)&&Key_State;
 assign soundUp = (Key == 8'h45)&&Key_State;
 assign soundDn = (Key == 8'h53)&&Key_State;


parameter X = 600,Y1 = 255,Y2 = 370,Y3 = 485;
reg [1:0] choice1 = 0;
reg [1:0] choice2 = 0;
assign sw = choice2;

// always @(*) begin
//     if(choice1 == 2'b11 ) back = (Key == 8'h59)&&Key_State;
//     else back =  (Key == 8'h41)&&Key_State;
// end

always @(posedge clk) begin
    if(back) choice1 <= 2'b00;
    else begin
        case(choice1) 
        2'b00:begin
            if(enter&&(arrow == Y1))
                 choice1 <= 2'b01;
            else if(enter&&(arrow == Y2))
                choice1 <= 2'b10;
            else if(enter&&(arrow == Y3))
                choice1 <= 2'b11;
            else choice1 <= 2'b00;
            choice2 <= 2'b00;

           
        end 
        2'b01:begin
            if(Play&&(arrow == Y1))
                 choice2 <= 2'b01;
            else if(Play&&(arrow == Y2))
                choice2 <= 2'b10;
            else if(Play&&(arrow == Y3))
                choice2 <= 2'b11;
            else choice2 <= 2'b00;
            choice1 <= 2'b01;
        end
        2'b10:begin
          choice1 = 2'b10;
          choice2 = 2'b00;
        end
        2'b11:begin
          choice1 = 2'b11;
          choice2 = 2'b00;
        end
        endcase
    end
end






always @(*) begin
    case(choice1)
        2'b00: begin           
            prgb = prgb1;
            rgb = rgb1;
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
             rgb = 12'h000;
            prgb = prgb2;
            hs = hs2;
            vs = vs2;
            arrowEn = 0;
        end
         2'b11: begin
           rgb = rgb3;
            prgb = prgb1;
            hs = hs1;
            vs = vs1;
            arrowEn = 0;
        end
    endcase
end


always @(*) begin
    case(choice1)
        2'b00:begin
            en1 = 0;
            en2 = 0;
            en3 = 0;
        end
        2'b01:begin
            en1 = 1;
            en2 = 0;
            en3 = 0;
        end
        2'b10:begin
            
            en1 = 0;
            en2 = 0;
            en3 = 1;
        end
        2'b11:begin
            en1 = 0;
            en2 = 1;
            en3 = 0;
        end
    endcase
end
endmodule


module Zone_Arrow (
  input [10:0] x,
  input [9:0] y,
  input [10:0] arrow_x,
  input [9:0] arrow_y,
  output zone_arrow
);

assign zone_arrow = ((x==arrow_x)&&(y==arrow_y)) || ((x==arrow_x+1)&&(y>=(arrow_y-1))&&(y<=(arrow_y+1))) || ((x==arrow_x+2)&&(y>=(arrow_y-2))&&(y<=(arrow_y+2))) || ((x==arrow_x+3)&&(y>=(arrow_y-3))&&(y<=(arrow_y+3))) || ((x==arrow_x+4)&&(y>=(arrow_y-4))&&(y<=(arrow_y+4))) || ((x==arrow_x+5)&&(y>=(arrow_y-5))&&(y<=(arrow_y+5))) || ((x==arrow_x+6)&&(y>=(arrow_y-6))&&(y<=(arrow_y+6)))  || ((x==arrow_x+7)&&(y>=(arrow_y-7))&&(y<=(arrow_y+7))) || ((x==arrow_x+8)&&(y>=(arrow_y-8))&&(y<=(arrow_y+8))) || ((x==arrow_x+9)&&(y>=(arrow_y-9))&&(y<=(arrow_y+9))) ||  ((x==arrow_x+10)&&(y>=(arrow_y-2))&&(y<=(arrow_y+2))) ||   ((x==arrow_x+11)&&(y>=(arrow_y-2))&&(y<=(arrow_y+2))) || ((x==arrow_x+12)&&(y>=(arrow_y-2))&&(y<=(arrow_y+2))) || ((x==arrow_x+13)&&(y>=(arrow_y-2))&&(y<=(arrow_y+2))) || ((x==arrow_x+14)&&(y>=(arrow_y-2))&&(y<=(arrow_y+2))) || ((x==arrow_x+15)&&(y>=(arrow_y-2))&&(y<=(arrow_y+2))) || ((x==arrow_x+16)&&(y>=(arrow_y-2))&&(y<=(arrow_y+2)))|| ((x==arrow_x+17)&&(y>=(arrow_y-2))&&(y<=(arrow_y+2)))|| ((x==arrow_x+18)&&(y>=(arrow_y-2))&&(y<=(arrow_y+2)))||((x==arrow_x+19)&&(y>=(arrow_y-2))&&(y<=(arrow_y+2)))||((x==arrow_x+20)&&(y>=(arrow_y-2))&&(y<=(arrow_y+2)))||((x==arrow_x+21)&&(y>=(arrow_y-2))&&(y<=(arrow_y+2)))||((x==arrow_x+22)&&(y>=(arrow_y-2))&&(y<=(arrow_y+2)))||((x==arrow_x+23)&&(y>=(arrow_y-2))&&(y<=(arrow_y+2)))||((x==arrow_x+24)&&(y>=(arrow_y-2))&&(y<=(arrow_y+2)))||((x==arrow_x+25)&&(y>=(arrow_y-2))&&(y<=(arrow_y+2)))||((x==arrow_x+26)&&(y>=(arrow_y-2))&&(y<=(arrow_y+2)))||((x==arrow_x+27)&&(y>=(arrow_y-2))&&(y<=(arrow_y+2)))||((x==arrow_x+28)&&(y>=(arrow_y-2))&&(y<=(arrow_y+2)))||((x==arrow_x+29)&&(y>=(arrow_y-2))&&(y<=(arrow_y+2)))||((x==arrow_x+30)&&(y>=(arrow_y-2))&&(y<=(arrow_y+2)));                             

  
endmodule


module homepage #(parameter UP_BOUND = 29,DOWN_BOUND = 628,LEFT_BOUND = 184,RIGHT_BOUND = 983,X = 600,Y1 = 255,Y2 = 370,Y3 = 485)(              //首页显示,将200*150转为800*600显示
    input up,input down,
    input clk,rstn,
    input [10:0] x, 
    input arrowEn,               
    input [9:0] y,
    input zone_arrow,
    input [11:0] rgb,
    output reg [11:0] prgb,
    output [14:0] addr,
    output reg [9:0] arrow_y = Y1,
    output reg [10:0] arrow_x = X
);
wire zone;


reg [7:0] quot_x;

reg [7:0] quot_y;

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

always @(posedge clk,negedge rstn) begin
    if(!rstn) arrow_y <= Y1;
    else begin
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
       
end
assign addr = (quot_y-1) * 200+quot_x; 

assign zone = ((x >= LEFT_BOUND)&&(x<=RIGHT_BOUND))&&((y >= UP_BOUND)&&(y <= DOWN_BOUND));





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



module asciiArt #(parameter UP_BOUND = 29-4,DOWN_BOUND = 628-4,LEFT_BOUND = 184-4,RIGHT_BOUND = 983-4)(                           //??????????????????????
    input [10:0] x,
    input [9:0] y,
    input pclk,
    input dot,
    output reg [11:0] prgb,
    output [14:0] raddr
);

wire zone;
wire [1:0] choice;

reg [7:0] quot_x,quot_y;
reg [1:0] rem_x,rem_y;
wire [11:0] rgb;
assign zone = ((x >= LEFT_BOUND)&&(x<=RIGHT_BOUND)&&(y >= UP_BOUND)&&(y <= DOWN_BOUND));

always @(*) begin
    if(zone)begin
        quot_x = (x-LEFT_BOUND)/4;
        rem_x = (x-LEFT_BOUND)%4;
        quot_y =  (y - UP_BOUND)/4;
        rem_y = (y - UP_BOUND)%4;
    end
    else begin
        quot_x = 0;
        quot_y = 0;
        rem_x = 0;
        rem_y = 0;
    end 
end



assign raddr = (quot_y-1) * 200+quot_x; 

random RAND(.x(quot_x),.y(quot_y),.choice(choice));

        
ascii ASCII(.x(rem_x),.y(rem_y),.choice(choice),.prgb(rgb));


always @(*) begin
    if(zone)begin
        if(!dot) prgb = rgb;
        else prgb = 12'hFFF;
    end
    else prgb = 12'h000;
end




    
endmodule


module ascii (
    input [1:0] x,
    input [1:0] y,
    input [1:0] choice ,
    output reg [11:0] prgb
);



always @(*) begin
case(choice)
        2'b00:begin
                case(x)
                2'b00:begin
                    prgb = 12'hFFF;
                end
                2'b01:begin
                     if((y==2'b01)||(y==2'b10))
                        prgb = 12'hFFF;
                    else prgb = 12'h000;
                end
                2'b10:begin
                    if((y==2'b01)||(y==2'b10))
                        prgb = 12'hFFF;
                    else prgb = 12'h000;
                end
                2'b11:begin
                    prgb = 12'hFFF;
                end
                endcase
        end



        2'b01:begin
                case(x)
                2'b00:begin
                    if(y==2'b01)
                        prgb = 12'h000;
                    else prgb = 12'hFFF;
                       
                end
                2'b01:begin
                     if(y==2'b01)
                        prgb = 12'h000;
                    else prgb = 12'hFFF;
                end
                2'b10:begin
                     prgb = 12'h000;
                end
                2'b11:begin
                     if(y==2'b01)
                        prgb = 12'h000;
                    else prgb = 12'hFFF;
                end
                endcase
            
        end



        2'b10:begin
            case(x)
                2'b00:begin
                    if((y==2'b11) || (y==2'b00)) prgb = 12'h000;
                        else prgb = 12'hFFF;
                       
                end
                2'b01:begin
                    if((y==2'b10) || (y==2'b01)) prgb = 12'h000;
                        else prgb = 12'hFFF;
                end
                2'b10:begin
                      if((y==2'b10) || (y==2'b01)) prgb = 12'h000;
                        else prgb = 12'hFFF;
                end
                2'b11:begin
                   if((y==2'b11) || (y==2'b00)) prgb = 12'h000;
                        else prgb = 12'hFFF;
                end
                endcase
            
        end
        2'b11:begin
               case(x)
                2'b00:begin
                    if((y==2'b11) || (y==2'b00)) prgb = 12'h000;
                        else prgb = 12'hFFF;
                       
                end
                2'b01:begin
                    prgb = 12'hFFF;
                end
                2'b10:begin
                     prgb = 12'hFFF;
                end
                2'b11:begin
                   if((y==2'b11) || (y==2'b00)) prgb = 12'h000;
                        else prgb = 12'hFFF;
                end
                endcase
  
        end
    endcase


end
  
endmodule





module random (
    input [7:0] x,
    input [7:0] y,
    output [1:0] choice
);
reg [1:0] out;
assign choice = out;
always @(*) begin
    out = x*x+y*y;
end
    
endmodule



module getEdge (
    input x,
    input rstn,
    input pclk,
    output xEdge
);
wire x_de;
debounce DEBOUNCE(.clk(pclk),.rstn(rstn),.x(x),.y(x_de));
Synchron SYNC(.clk(pclk),.x(x_de),.y(xEdge),.rstn(rstn));

    
endmodule

module debounce(input clk,rstn,                //其实可以例化一个计数器模块
                input x,
                output reg y);
parameter  Cnt0 = 3'b000, Cnt1 = 3'b001,Assign0 = 3'b010,Assign1=3'b011,Default = 3'b111;   //刚开始进入默认状态，两个计数状态，两个赋值状态    
reg [2:0] CS,NS;
reg [19:0] Count;
reg y_reg;
always @(posedge clk,negedge rstn) begin          //异步复位
    if(!rstn)begin
      CS <= Default;
      y_reg <= 0;
    end 
    else begin
      CS <= NS;
    y_reg <= y;
    end 
end

always @(posedge clk,negedge rstn) begin
  if(!rstn) Count <=0;
  else begin
    case(CS)
       Default: Count <= 0;
    Cnt0 :begin
      Count <= Count+1;
    end 
    Cnt1: Count <= Count+1;
    Assign0: Count <= 0;
    Assign1: Count <= 0;

  endcase
  end

  
end

always @(*) begin
    y = y_reg;
    NS = Default;
    case (CS) 
    Default: begin
      y = 0;
      if(x == 1) NS = Cnt1;
      else NS = Cnt0;
    end
    Cnt0:begin
       if(Count == 100000) begin
         NS = Assign0;
       end 
       else if(x == 0)begin
         NS = Cnt0;
       end    
       else begin
         NS = Default;
       end      
    end
    Cnt1:begin
       if(Count == 100000) begin
         NS = Assign1;
       end 
       else if(x == 1)begin
         NS = Cnt1;
       end    
       else begin
         NS = Default;
       end      
    end
    Assign0: begin
           y = 0;
      if(x == 0)
        NS = Assign0;                
      else 
         NS = Default;              //只有极个别才在此刻为1
    end
    Assign1:begin
      y = 1;
      if(x == 1) NS = Assign1;
      else NS = Default;                  //只有极个别才在此刻为1
    end
    endcase
end
endmodule


module Synchron(input clk,rstn,input x,
                output y);
reg s1,s2;
reg s;

always @(posedge clk) begin
    if(~rstn ) begin
      s1 <= 0;
      s2 <= 0;
      s <= 0;
    end
    else begin
      s1 <= x;
      s2 <= s1;
      s <= s2;
    end
end
assign y = (!s)&s2;
endmodule































































































module video(
  input clk,rstn,en,txd,
  output hs,vs,
  output [11:0] prgb,
  output ja
);

wire [7:0] txd_data;
wire w_en;
reg w_en_reg;
reg next_pic;
reg read;
wire pclk;//50Hz时钟
wire [11:0] rgb;
reg [14:0] waddr,midaddr;
wire [14:0] raddr;
wire [11:0] wdata,rdata,middata;



reg [14:0] midaddr_reg;
reg dot;

assign wdata=rgb;

uart_txd uart(clk,rstn,txd,w_en,txd_data);
//串口通信驱动，每个w_en（持续一个时钟周期的高电平）到来时，说明受到了一个新数据，更新一次txd_data。
//txd_data为收到的八位数据

trans trans(txd_data,rgb);
//将收到的八位数据译码成12位rgb值

VRAM0 vram0(.clka(clk),.addra(waddr),.dina(wdata),.ena(1),.wea(w_en_reg),.clkb(clk),.addrb(midaddr),.doutb(middata),.enb(1));
//0号存储器，存储由串口发来的信息。
//在每个w_en到来时向存储器写入一个12位rgb数据
//写到尾后，从头继续写。即将写到尾时，发送一个“换帧”信号。

VRAM1 vram1(.clka(clk),.addra(midaddr_reg),.dina(middata),.ena(1),.wea(read),.clkb(clk),.addrb(raddr),.doutb(rdata),.enb(1));
//1号存储器，直接与dcu相连。
//每收到一个换帧信号，将0号存储器中所有信息依次写入1号存储器

CLK50MHZ clk_50Hz(.clk_in1(clk),.clk_out1(pclk));

// dcu dcu(pclk,rdata,hs,vs,raddr,prgb);


always@(*)
begin
  if(rdata==12'b0)dot=0;
  else dot=1;
end

 wire [10:0] x;
 wire [9:0] y;

 vdt_hp VDT(.pclk(pclk),.hs(hs),.vs(vs),.x(x),.y(y));

 asciiArt ASCIIART(.x(x),.y(y),.dot(dot),.prgb(prgb),.raddr(raddr));


always@(posedge clk)
begin
  midaddr_reg<=midaddr;
end

always@(posedge clk)
begin
  w_en_reg<=w_en;
end

always@(posedge clk)
begin
  if(w_en_reg)
    begin
    if(waddr==15'd29999)
      waddr<=0;
    else  waddr<=waddr+1;
    
    if(waddr==15'd29997)
      next_pic<=1;
    else next_pic<=0;
    
    end

end

always@(posedge clk)//受到vram0即将写完一帧的信号后，通知vram1开始读vram0中数据
begin
  if(next_pic)
    read<=1;
  else if(midaddr==15'd29999)
    read<=0;
end

always@(posedge clk)//vram1读取vram0中数据
begin
  
    if(midaddr==15'd29999)
      midaddr<=0;
    else if(read)
      midaddr<=midaddr+1;
    else midaddr<=0;
end

//音乐
wire [5:0] rdata_mic;
reg [10:0] raddr_mic;
wire [28:0] en_1;
reg start=0;
wire clkd;
reg clkd_reg;
wire JA;

always@(posedge clk)
begin
  if(!txd)start<=1;//收到视频，开始播放音频
end

fd_PWM#(12500000) auto_play(clk,rstn,clkd);//按照既定速度读取音符，即0.125秒位一个最小发音时长单位

BADAPPLE_m badapple(.clka(clk),.douta(rdata_mic),.addra(raddr_mic),.ena(1));

note_decoder de_1(rdata_mic,en_1);

piano_key piano_key_1(clk,rstn,en_1,3'd5,JA);

    always@(posedge clk)
    begin
      clkd_reg<=clkd;
    end

    always@(posedge clk)
    begin
        if(en&&start)
        begin
            if(!clkd_reg&&clkd)
                raddr_mic<=raddr_mic+1;
        end
        
    end

assign ja=JA&&en&&start;

endmodule








module fd_PWM #(//分频器
    parameter N=50)(//取N=200000，周期为2ms
    input clk,rstn,
    output reg out
);
    reg [31:0] cnt;


    always @(posedge clk) begin
        if (cnt == 0)  begin 
            cnt <= N/2 - 1;
            out <= ~ out;
        end
        else  cnt <= cnt - 1;
    end
endmodule





module fp_verilog #(parameter N=4)(
    input clk,rstn,en,
    input [6:0] sound,
    output reg JA);
    reg[6:0] cn;
    wire clkd;
    
        fd_PWM#(N) fd(clk,rstn,clkd);

        always@(posedge clkd)
        if(en)
            begin
            cn<=cn+1'b1;
            if (cn==7'd101-sound)
                begin
              //      cn<=7'd0;
                    JA<=0;
                end
          else if (cn==7'd100)
                begin
                    cn<=7'd0;
                    JA<=1;
                end
        end
    
endmodule





















module note_decoder(//乐谱文件译码器
    input [5:0] rdata,
    output reg [28:0] en
);
always@*
    case(rdata)
    6'd0:en=29'b1_0000000_0000000_0000000_0000000;
    
    6'd1:en=29'b0_0000000_1000000_0000000_0000000;
    6'd2:en=29'b0_0000000_0100000_0000000_0000000;
    6'd3:en=29'b0_0000000_0010000_0000000_0000000;
    6'd4:en=29'b0_0000000_0001000_0000000_0000000;
    6'd5:en=29'b0_0000000_0000100_0000000_0000000;
    6'd6:en=29'b0_0000000_0000010_0000000_0000000;
    6'd7:en=29'b0_0000000_0000001_0000000_0000000;

    6'd11:en=29'b0_0000000_0000000_1000000_0000000;
    6'd12:en=29'b0_0000000_0000000_0100000_0000000;
    6'd13:en=29'b0_0000000_0000000_0010000_0000000;
    6'd14:en=29'b0_0000000_0000000_0001000_0000000;
    6'd15:en=29'b0_0000000_0000000_0000100_0000000;
    6'd16:en=29'b0_0000000_0000000_0000010_0000000;
    6'd17:en=29'b0_0000000_0000000_0000001_0000000;

    6'd21:en=29'b0_0000000_0000000_0000000_1000000;
    6'd22:en=29'b0_0000000_0000000_0000000_0100000;
    6'd23:en=29'b0_0000000_0000000_0000000_0010000;
    6'd24:en=29'b0_0000000_0000000_0000000_0001000;
    6'd25:en=29'b0_0000000_0000000_0000000_0000100;
    6'd26:en=29'b0_0000000_0000000_0000000_0000010;
    6'd27:en=29'b0_0000000_0000000_0000000_0000001;

    6'd31:en=29'b0_1000000_0000000_0000000_0000000;
    6'd32:en=29'b0_0100000_0000000_0000000_0000000;
    6'd33:en=29'b0_0010000_0000000_0000000_0000000;
    6'd34:en=29'b0_0001000_0000000_0000000_0000000;
    6'd35:en=29'b0_0000100_0000000_0000000_0000000;
    6'd36:en=29'b0_0000010_0000000_0000000_0000000;
    6'd37:en=29'b0_0000001_0000000_0000000_0000000;

    default:en=29'b1_0000000_0000000_0000000_0000000;
    endcase

endmodule













module piano_key(//发音模块
    input clk,rstn,
    input [28:0] en,
    input [6:0] sound,
    output wire out
);
wire [27:0] pwm;
wire out_1;

    fp_verilog#(7634) low_low_1(clk,rstn,en[27],sound,pwm[27]);
    fp_verilog#(6803) low_low_2(clk,rstn,en[26],sound,pwm[26]);
    fp_verilog#(6061) low_low_3(clk,rstn,en[25],sound,pwm[25]);
    fp_verilog#(5714) low_low_4(clk,rstn,en[24],sound,pwm[24]);
    fp_verilog#(5102) low_low_5(clk,rstn,en[23],sound,pwm[23]);
    fp_verilog#(4545) low_low_6(clk,rstn,en[22],sound,pwm[22]);
    fp_verilog#(4049) low_low_7(clk,rstn,en[21],sound,pwm[21]);


    fp_verilog#(3817) low_1(clk,rstn,en[20],sound,pwm[20]);
    fp_verilog#(3401) low_2(clk,rstn,en[19],sound,pwm[19]);
    fp_verilog#(3030) low_3(clk,rstn,en[18],sound,pwm[18]);
    fp_verilog#(2865) low_4(clk,rstn,en[17],sound,pwm[17]);
    fp_verilog#(2551) low_5(clk,rstn,en[16],sound,pwm[16]);
    fp_verilog#(2273) low_6(clk,rstn,en[15],sound,pwm[15]);
    fp_verilog#(2024) low_7(clk,rstn,en[14],sound,pwm[14]);

    fp_verilog#(1912) mid_1(clk,rstn,en[13],sound,pwm[13]);
    fp_verilog#(1704) mid_2(clk,rstn,en[12],sound,pwm[12]);
    fp_verilog#(1517) mid_3(clk,rstn,en[11],sound,pwm[11]);
    fp_verilog#(1433) mid_4(clk,rstn,en[10],sound,pwm[10]);
    fp_verilog#(1276) mid_5(clk,rstn,en[9],sound,pwm[9]);
    fp_verilog#(1136) mid_6(clk,rstn,en[8],sound,pwm[8]);
    fp_verilog#(1012) mid_7(clk,rstn,en[7],sound,pwm[7]);
    
    fp_verilog#(956) high_1(clk,rstn,en[6],sound,pwm[6]);
    fp_verilog#(851) high_2(clk,rstn,en[5],sound,pwm[5]);
    fp_verilog#(759) high_3(clk,rstn,en[4],sound,pwm[4]);
    fp_verilog#(716) high_4(clk,rstn,en[3],sound,pwm[3]);
    fp_verilog#(638) high_5(clk,rstn,en[2],sound,pwm[2]);
    fp_verilog#(568) high_6(clk,rstn,en[1],sound,pwm[1]);
    fp_verilog#(506) high_7(clk,rstn,en[0],sound,pwm[0]);



 assign out_1=pwm[0]+pwm[1]+pwm[2]+pwm[3]+pwm[4]+pwm[5]+pwm[6]+pwm[7]+pwm[8]+pwm[9]+pwm[10]+pwm[11]+pwm[12]+pwm[13]+pwm[14]+pwm[15]+pwm[16]+pwm[17]+pwm[18]+pwm[19]+pwm[20]+pwm[21]+pwm[22]+pwm[23]+pwm[24]+pwm[25]+pwm[26]+pwm[27];
assign out=out_1&&!en[28];

endmodule













module trans(//把串口通信数据译码成12位rgb
  input [7:0] data,
  output reg [11:0] prgb
);
always@*
begin
  case(data[5:4])
  2'b00:prgb[11:8]=4'b0000;
  2'b01:prgb[11:8]=4'b0101;
  2'b10:prgb[11:8]=4'b1010;
  2'b11:prgb[11:8]=4'b1111;
endcase

  case(data[3:2])
  2'b00:prgb[7:4]=4'b0000;
  2'b01:prgb[7:4]=4'b0101;
  2'b10:prgb[7:4]=4'b1010;
  2'b11:prgb[7:4]=4'b1111;
endcase

  case(data[1:0])
  2'b00:prgb[3:0]=4'b0000;
  2'b01:prgb[3:0]=4'b0101;
  2'b10:prgb[3:0]=4'b1010;
  2'b11:prgb[3:0]=4'b1111;
endcase
end
endmodule




//串口通信模块

module uart_txd(//uarl接受模块
  input clk,rstn,txd,
  output wire out_en,//out_en=1时，说明接受完8位数据，可以输出。
  output reg [7:0] data
);

wire receive;//receive=1时，接受一位数据
wire bps_en;//控制bps计数
wire data_en;//data_en为1时，可以接受数据。


reg [7:0] data_reg;//用来寄存data，在接受数据中不输出data，接受完8位数据后一并输出。

always@(posedge clk)//读数据
begin
  if(data_en&&receive)
      data_reg<={txd,data_reg[7:1]};
  if(out_en)
      data<=data_reg;
  else if(!bps_en)
      data_reg<=0;
end

uart_control control(clk,rstn,txd,receive,bps_en,data_en,out_en);

uart_bps#(3000000) bps_count(clk,rstn,bps_en,receive);

endmodule




module uart_bps#(parameter bps=1382400)(
  input clk,rstn,
  input en, //数据到来
  output wire receive //接受数据
);
parameter cycle=100000000/bps;
reg [12:0] cnt;

always@(posedge clk)
begin
  if(!rstn)cnt<=0;
  else if(!en)cnt<=0;
  else if(cnt==cycle)cnt<=0;
  else if(en)cnt<=cnt+1;
end

assign receive=(cnt==cycle/2);//每到半个周期时，获取一次数据。

endmodule




module uart_control(
  input clk,rstn,txd,receive,
  output reg bps_en,data_en,out_en
);
reg txd_reg;
reg [3:0] bit_cnt;

always@(posedge clk)
begin
  txd_reg<=txd;
end

  always@(posedge clk)//位计数
  begin
    if(!rstn)bit_cnt<=0;
    else if(bit_cnt==4'd10)
      bit_cnt<=0;
    else if(receive)
      bit_cnt<=bit_cnt+1;  
  end


  always@*//控制bps计数和移位读txd数据
  begin
    if(bit_cnt>4'd0&&bit_cnt<4'd9)data_en=1;
    else data_en=0;
    if(!txd&&txd_reg)bps_en=1;
    else if(bit_cnt==4'd10)bps_en=0;
    if(bit_cnt==4'd10)out_en=1;
    else out_en=0;
  end

endmodule


module uart_try(
  input clk,rstn,txd,
  output wire [7:0] led
);
wire out_en;
uart_txd txd_t(clk,rstn,txd,out_en,led);

endmodule




















//dcu显示模块

module vds#(parameter TIMES = 4 )(
            input pclk,
            input hen,ven,
            input [11:0] rdata,
            output reg [14:0] raddr=0,
            output reg [11:0] prgb
);
reg [10:0] xcnt=0,ycnt=0;
reg reg_hen;

//记录换行
always@(posedge pclk)
begin
  reg_hen<=hen;
end

//将一个像素点扩大为TIMES^2个，cnt计数
  always@(posedge pclk)
  begin
    if(ven)
    begin
      if(hen)
        begin
        if(xcnt==TIMES-1)
          xcnt<=0;
        else
          xcnt<=xcnt+1;
        end
      
      if(!hen&&reg_hen)
        begin
        if(ycnt==TIMES-1)
        ycnt<=0;
        else ycnt<=ycnt+1;
        end
    end
  end

//放大
always@(posedge pclk)
begin
  if(ven)
  begin
        if(!hen&&reg_hen&&ycnt!=TIMES-1)raddr<=raddr-800/TIMES;
        else if(hen&&xcnt==TIMES-1)raddr<=raddr+1;  
  end
  if(!ven)
  begin
    raddr<=0;
  end
end

//输出prgb
reg hen_reg;

always@(posedge pclk)
begin
  hen_reg<=hen;
end

always@*
begin
  if(hen_reg&&ven)
  prgb=rdata;
  else prgb=0;
end

endmodule



module dcu(
  input pclk,
  input [11:0] rdata,
  output hs,vs,
  output [14:0] raddr,
  output [11:0] prgb
);
wire hen,ven;

vds vds(pclk,hen,ven,rdata,raddr,prgb);

vdt vdt(pclk,hs,vs,hen,ven);

endmodule



module vdt (input pclk,
            output wire hs,vs,
            output reg hen,ven
);
  parameter hmax=1040 ,vmax=666;  
  parameter UP_BOUND = 29;
	parameter DOWN_BOUND = 628;
	parameter LEFT_BOUND = 184;
	parameter RIGHT_BOUND = 983;
  parameter HSW = 120,VSW=6;

  reg [11:0] hcnt=0;
  reg [11:0] vcnt=0;

  always@(posedge pclk)
  begin
      if(hcnt==hmax-1)
          begin
                hcnt=0;
                if(vcnt==vmax-1)vcnt<=0;
                else vcnt=vcnt+1;
          end
      else  hcnt=hcnt+1;

  end

  assign hs=(hcnt<HSW);
  assign vs=(vcnt<VSW);


  always@(posedge pclk)
  begin
        if((hcnt>=LEFT_BOUND-3)&&(hcnt<=RIGHT_BOUND-3)) 
            hen <= 1;
        else hen<=0;
        if((vcnt>=UP_BOUND)&&(vcnt<=DOWN_BOUND)) 
            ven <= 1;
        else ven<= 0;
  end

endmodule








module counter #(
    parameter N=50)(//取N=200000，周期为2ms
    input clk,en,
    output wire out
);
    reg [31:0] cnt;

    always@(posedge clk)
    begin
      if(en)
      begin
        if(cnt==N-1)cnt<=0;
        else cnt<=cnt+1;
      end
    end

    assign out=(cnt==N-1);

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
reg ps2_clk_r0,ps2_clk_r1,ps2_clk_r2;	//ps2_clk状态寄存器
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
































// 自动演奏模块

module auto_Buzzer(
    input clk,rstn,en,up,down,
    input [1:0] sw,
    output  ja,jc,
    output wire pwm
    );
    wire [28:0] en_1,en_2;
    reg last_up,last_down;
    reg [6:0] sound=5;//音量
    wire JA,JC;

    wire [5:0] rdata_1,rdata_2,rdata_3,rdata_4;//从乐谱中读到的数据,
    reg [5:0] data_1,data_2;//最终用于演奏的信号
    reg [10:0] raddr=0;
    wire clkd;
    reg clkd_reg;

    assign pwm=ja;
    
    //音量控制模块
    always@(posedge clk)
    begin
      last_up<=up;
      last_down<=down;
    end

    always@(posedge clk)
    begin
      if(!last_up&&up)sound<=sound+1;
      if(!last_down&&down)sound<=sound-1;
    end


    fd_PWM#(12500000) auto_play(clk,rstn,clkd);//按照既定速度读取音符，即0.125秒位一个最小发音时长单位

    NEWS1  news_1(.clka(clk),.douta(rdata_1),.addra(raddr),.ena(1));
    NEWS2  news_2(.clka(clk),.douta(rdata_2),.addra(raddr),.ena(1));

    BADAPPLE_m badapple(.clka(clk),.douta(rdata_3),.addra(raddr),.ena(1));

    QINGHUACI qinghuaci(.clka(clk),.douta(rdata_4),.addra(raddr),.ena(1));


    note_decoder de_1(data_1,en_1);
    note_decoder de_2(data_2,en_2);


    piano_key piano_key_1(clk,rstn,en_1,sound,JA);
    piano_key piano_key_2(clk,rstn,en_2,sound-5'd3,JC);

    always@(*)//根据外部信号，选择乐曲
    begin
      case(sw)
      2'b00:begin
          data_1=0;data_2=0;
            end
      2'b01:begin
          data_1=rdata_3;data_2=0;
            end
      2'b10:begin
          data_1=rdata_1;data_2=rdata_2;
            end
      2'b11:begin
          data_1=rdata_4;data_2=0;
            end        
      endcase
    end

    always@(posedge clk)
    begin
      clkd_reg<=clkd;
    end

    always@(posedge clk)
    begin
        if(en)
        begin
            if(!clkd_reg&&clkd)
                raddr<=raddr+1;
        end
        
    end

assign ja=JA&&en;
assign jc=JC&&en;

endmodule




module ps2_Buzzer(
    input clk,rstn,up,down,
    input [7:0] in,
    output  JA
    );
    wire [21:0] pwm;
    reg [21:0] en;
    reg last_up,last_down;
    reg [6:0] sound=5;

    
    always@(posedge clk)
    begin
      last_up<=up;
      last_down<=down;
    end

    always@(posedge clk)
    begin
      if(!last_up&&up&&sound)sound<=sound+1;
      if(!last_down&&down&&sound>=0)sound<=sound-1;
    end



    fp_verilog#(999999) low_0(clk,rstn,en[21],sound,pwm[21]);

    fp_verilog#(3817) low_1(clk,rstn,en[20],sound,pwm[20]);
    fp_verilog#(3401) low_2(clk,rstn,en[19],sound,pwm[19]);
    fp_verilog#(3030) low_3(clk,rstn,en[18],sound,pwm[18]);
    fp_verilog#(2865) low_4(clk,rstn,en[17],sound,pwm[17]);
    fp_verilog#(2551) low_5(clk,rstn,en[16],sound,pwm[16]);
    fp_verilog#(2273) low_6(clk,rstn,en[15],sound,pwm[15]);
    fp_verilog#(2024) low_7(clk,rstn,en[14],sound,pwm[14]);

    fp_verilog#(1912) mid_1(clk,rstn,en[13],sound,pwm[13]);
    fp_verilog#(1704) mid_2(clk,rstn,en[12],sound,pwm[12]);
    fp_verilog#(1517) mid_3(clk,rstn,en[11],sound,pwm[11]);
    fp_verilog#(1433) mid_4(clk,rstn,en[10],sound,pwm[10]);
    fp_verilog#(1276) mid_5(clk,rstn,en[9],sound,pwm[9]);
    fp_verilog#(1136) mid_6(clk,rstn,en[8],sound,pwm[8]);
    fp_verilog#(1012) mid_7(clk,rstn,en[7],sound,pwm[7]);
    
    fp_verilog#(956) high_1(clk,rstn,en[6],sound,pwm[6]);
    fp_verilog#(851) high_2(clk,rstn,en[5],sound,pwm[5]);
    fp_verilog#(759) high_3(clk,rstn,en[4],sound,pwm[4]);
    fp_verilog#(716) high_4(clk,rstn,en[3],sound,pwm[3]);
    fp_verilog#(638) high_5(clk,rstn,en[2],sound,pwm[2]);
    fp_verilog#(568) high_6(clk,rstn,en[1],sound,pwm[1]);
    fp_verilog#(506) high_7(clk,rstn,en[0],sound,pwm[0]);


    assign JA=pwm[0]+pwm[1]+pwm[2]+pwm[3]+pwm[4]+pwm[5]+pwm[6]+pwm[7]+pwm[8]+pwm[9]+pwm[10]+pwm[11]+pwm[12]+pwm[13]+pwm[14]+pwm[15]+pwm[16]+pwm[17]+pwm[18]+pwm[19]+pwm[20]+pwm[21];


always@*
    case(in)
    7'h34:en=22'b10000000_0000000_0000000;//静音  G
    7'h5a:en=22'b01000000_0000000_0000000;//低音1  Z
    7'h58:en=22'b00100000_0000000_0000000;//低音2  X
    7'h43:en=22'b00010000_0000000_0000000;//低音3  C
    7'h56:en=22'b00001000_0000000_0000000;//低音4  V
    7'h42:en=22'b00000100_0000000_0000000;//低音5  B
    7'h4e:en=22'b00000010_0000000_0000000;//低音6  N
    7'h4d:en=22'b00000001_0000000_0000000;//低音7  M

    7'h41:en=22'b00000000_1000000_0000000;//中音1  A
    7'h53:en=22'b00000000_0100000_0000000;//中音2  S
    7'h44:en=22'b00000000_0010000_0000000;//中音3  D
    7'h46:en=22'b00000000_0001000_0000000;//中音4  F
    7'h4a:en=22'b00000000_0000100_0000000;//中音5  J
    7'h4b:en=22'b00000000_0000010_0000000;//中音6  K
    7'h4c:en=22'b00000000_0000001_0000000;//中音7  L

    7'h51:en=22'b00000000_0000000_1000000;//高音1  Q
    7'h57:en=22'b00000000_0000000_0100000;//高音2  W
    7'h45:en=22'b00000000_0000000_0010000;//高音3  E
    7'h52:en=22'b00000000_0000000_0001000;//高音4  R
    7'h49:en=22'b00000000_0000000_0000100;//高音5  I
    7'h4f:en=22'b00000000_0000000_0000010;//高音6  O
    7'h50:en=22'b00000000_0000000_0000001;//高音7  P

    default:en=22'b10000000_0000000_0000000;
    endcase

endmodule


module piano(
    input clk,rstn,up,down,en,
    input ps2_clk,	//PS2接口时钟信号
    input ps2_data,		//PS2接口数据信号
    output JA,
    output wire pwm
);
wire [7:0] asci;
wire ja;

ps2_keyboard_driver keyboard(clk,rstn,ps2_clk,ps2_data,asci);

ps2_Buzzer PS2_BUZ(clk,rstn,up,down,asci,ja);

assign JA=en&&ja;
assign pwm=JA;

endmodule










