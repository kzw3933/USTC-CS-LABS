module  painter (
    input clk, rstn,		//时钟, 复位 (低电平有�?)
    input [3:0] dir,
    input [11:0] rgb, //画笔颜色
    input draw,		//绘画�?关：1-绘画
    output reg [11:0] prgb,
    output hs,		//行同�?
    output vs		//场同�?
);
reg [3:0] dir_reg;
wire  [3:0] Dir;
wire we;
always @(posedge clk) begin
  dir_reg <= dir;
end
assign Dir[3] = (!dir_reg[3])&&dir[3];
assign Dir[2] = (!dir_reg[2])&&dir[2];
assign Dir[1] = (!dir_reg[1])&&dir[1];
assign Dir[0] = (!dir_reg[0])&&dir[0];
wire [14:0] raddr,waddr;
wire [11:0] rdata,wdata;
wire [11:0] prgb_t;
wire pclk;
wire hen,ven;
//获取50MHz的时�?
clk_wiz_0 PCLK(.clk_in1(clk),.clk_out1(pclk)); 

PCU pcu(.clk(clk),.rstn(rstn),.dir(Dir),.rgb(rgb),.draw(draw),.waddr(waddr),.wdata(wdata),.we(we));
blk_mem_gen_0 RAM(.addra(waddr),.clka(clk),.dina(wdata),.ena(1),.wea(we),.addrb(raddr),.clkb(pclk),.doutb(rdata),.enb(1));
VDT vdt(.pclk(pclk),.hs(hs),.vs(vs),.hen(hen),.ven(ven));
VDS vds(.pclk(pclk),.hen(hen),.ven(ven),.rdata(rdata),.raddr(raddr),.prgb(prgb_t));
   always@(*)begin
        if(raddr == waddr)
            prgb = rgb;
        else prgb = prgb_t;
   end

endmodule


module PCU(input clk,rstn,
            input [3:0] dir,
            input [11:0] rgb,
            input draw,
            output reg [14:0] waddr,
            output  [11:0] wdata,
            output  we );                //绘画控制单元
always @(posedge clk,negedge rstn) begin

    if(!rstn) begin
      waddr <= 15'd14899;
    end
    else begin
      case (dir)
          4'b0001: waddr <= waddr -200;
          4'b0010:  waddr <= waddr -1;
          4'b0100: waddr <= waddr +1;
          4'b1000: waddr <= waddr +200;
      endcase
       
   
    end   
end
assign wdata = rgb;
assign we = draw;

endmodule

module VDS( input pclk,
            input hen,ven,
            input [11:0] rdata,
            output reg [14:0] raddr,
            output reg [11:0] prgb);
    parameter HSize = 4,VSize = 4;
    reg [1:0] hscnt,vscnt;
    reg hen_reg;
    wire nextline;

    assign nextline = hen_reg&&(!hen);
    always @(posedge pclk) begin
      hen_reg <= hen;
    end 

   always @(posedge pclk) begin
     if(hen)  hscnt <= (hscnt==(HSize-1))?0:(hscnt+1);
     else  hscnt <=0;  
    end

    always @(posedge pclk) begin
      if(ven) begin  
        if(nextline) vscnt <= (vscnt==(VSize-1))?0:(vscnt+1);
      end 
      else vscnt <= 0;
    end

    always @(posedge pclk) begin
        if(hen&&ven)begin
          if(hscnt==(HSize-1)) raddr <= raddr+1;
        end
        else  begin
          if(nextline)begin
            if(vscnt<(VSize-1)) raddr <= raddr -200;
          end 
          if((!hen)&&(!ven)) raddr <= 15'b0000_0000_0000_000;  
        end
        end
        
    always @(posedge pclk) begin
          if(hen&&ven)begin
            prgb <= rdata;
          end
          else prgb <= 12'h000;
            
        end
endmodule

module VDT (input pclk,
            output  hs,vs,
            output reg hen,ven
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

    always @(posedge pclk) begin
        if(hcnt==henMax)begin
            hcnt <= 0;
            vcnt <=(vcnt==venMax)?0:(vcnt+1);
        end
        else hcnt <= hcnt +1;   
    end

    always @(posedge pclk) begin
        hen <= (hcnt>=LEFT_BOUND)&&(hcnt <=RIGHT_BOUND);
        ven <= (vcnt>=UP_BOUND)&&(vcnt <=DOWN_BOUND);
    end
    assign hs = (hcnt>(HSW-1))?0:1;
    assign vs = (vcnt>(VSW-1))?0:1;
endmodule


