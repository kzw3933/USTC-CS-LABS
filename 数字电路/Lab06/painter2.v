

module  painter (
    input clk, rstn,		//时钟, 复位 (低电平有�?)
    input [3:0] dir,
    input [11:0] rgb,
    input erase,
//    input [3:0] red,  	//画笔颜色 (rgb)：红
//    input [3:0] green, 	//画笔颜色 (rgb)：绿
//    input [3:0] blue, 	//画笔颜色 (rgb)：蓝
    input draw,		//绘画�?关：1-绘画
    output  [11:0] prgb,
//    output [3:0] pred,	//像素颜色 (prgb)：红
//    output [3:0] pgreen, 	//像素颜色 (prgb)：绿 
//    output [3:0] pblue, 	//像素颜色 (prgb)：蓝
    output hs,		//行同�?
    output vs		//场同�?
);
reg [11:0] mdata,mprgb;
wire [14:0] waddr,raddr;
reg [11:0] wdata_reg;
wire [11:0] wdata,rdata;
wire [14:0] raddr2;
wire [11:0] rdata2;
wire we;
reg delay;
always @(posedge clk) begin
  if((raddr == (waddr +1))||(raddr == (waddr -1))||(raddr == (waddr +200))||(raddr == (waddr -200))||(raddr == waddr))
    delay <= 1;
  else delay <= 0;
end
always @(*) begin
    begin
      if(delay)
        mdata = 12'h000;
      else if(rdata == 12'h000) mdata = rdata2;      
      else mdata = rdata;
  end
end

always @(posedge clk) begin
  mprgb <= mdata;
end

always @(posedge clk) begin
  if(erase) wdata_reg <= 12'h000;
  else wdata_reg <= wdata;
end

clk_wiz_0 clkmaker(.clk_in1(clk),.clk_out1(pclk));
PCU pcu(pclk,rstn,draw,dir,rgb,we,waddr,wdata);

DCU dcu(pclk,mprgb,hs,vs,raddr,prgb);
assign raddr2 = raddr;
blk_mem_gen_0 vram(.addra(waddr),.clka(clk),.dina(wdata_reg),.ena(1),.wea(we),.addrb(raddr),.clkb(pclk),.doutb(rdata),.enb(1));
blk_mem_gen_1 rom(.addra(raddr2),.clka(pclk),.douta(rdata2),.ena(1));

endmodule


module PCU (
    input clk,rstn,draw,
    input [3:0] dir,//依次为上下左�?
    input [11:0] rgb,
    output wire we,
    output reg [14:0] waddr,
    output wire [11:0] wdata
);
reg [3:0] last_dir;

    always@(posedge clk)
    begin
      last_dir<=dir;
    end

    always @(posedge clk or negedge rstn)
    begin
      if(!rstn)waddr <= 15'd14899;
      else if((!last_dir[3]&&dir[3])||(!last_dir[2]&&dir[2])||(!last_dir[1]&&dir[1])||(!last_dir[0]&&dir[0]))
        case(dir)
        4'b1000:waddr <= waddr - 200;//�?
        4'b0100:waddr <= waddr + 200;//�?
        4'b0010:waddr <= waddr - 1;//�?
        4'b0001:waddr <= waddr + 1;//�?
        default :waddr <= waddr;
      endcase
      else waddr <= waddr;
    end

    assign we = draw;
    assign wdata = rgb;
      
    
endmodule





module VDS#(parameter TIMES = 4 )(
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
always@*
begin
  if(hen&&ven)
  prgb=rdata;
  else prgb=0;
end


endmodule


module DCU(
  input pclk,
  input [11:0] rdata,
  output hs,vs,
  output [14:0] raddr,
  output [11:0] prgb
);
wire hen,ven;

VDS vds(pclk,hen,ven,rdata,raddr,prgb);

VDT vdt(pclk,hs,vs,hen,ven);

endmodule


module VDT (input pclk,
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
        if((hcnt>=LEFT_BOUND)&&(hcnt<=RIGHT_BOUND)) 
            hen <= 1;
        else hen<=0;
        if((vcnt>=UP_BOUND)&&(vcnt<=DOWN_BOUND)) 
            ven <= 1;
        else ven<= 0;
  end

endmodule














