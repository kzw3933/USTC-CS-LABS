


module video(
  input clk,rstn,en,txd,
  input alphaEn,
  output reg hs,vs,
  output reg [11:0] prgb,
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
reg [14:0] raddr;
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

wire [11:0] prgb1,prgb2;
wire [14:0] raddr1,raddr2;
wire hs1,vs1,hs2,vs2;

dcu dcu(pclk,rdata,hs1,vs1,raddr1,prgb1);


always@(*)
begin
  if(rdata==12'b0)dot=0;
  else dot=1;
end



 wire [10:0] x;
 wire [9:0] y;

 always @(*) begin
   if(alphaEn)begin
     raddr = raddr2;
     prgb = prgb2;
     hs = hs2;
     vs = vs2; 
   end 
    else begin
      raddr = raddr1;
      prgb = prgb1;
      hs = hs1;
      vs = vs1;
    end
 end

 vdt_hp VDT(.pclk(pclk),.hs(hs2),.vs(vs2),.x(x),.y(y));

 asciiArt ASCIIART(.x(x),.y(y),.dot(dot),.prgb(prgb2),.raddr(raddr2));


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