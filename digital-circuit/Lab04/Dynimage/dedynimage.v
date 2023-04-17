module dedynimage(input x,rstn,
                    input clk,
                    output [3:0] an,
                    output [6:0] seg);
                    wire db_x;
                    wire Syn_x;
                    wire [15:0] Cnt_Out;
debounceX DB(clk,rstn,x,db_x);
Synchron PS(clk,rstn,db_x,Syn_x);
counter cnt(clk,rstn,0,Syn_x,0,Cnt_Out);
dynimage DIS(clk,rstn,Cnt_Out,an,seg);

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
module debounceX(input clk,rstn,                //其实可以例化一个计数器模块
                input x,
                output reg y);
parameter  Cnt0 = 3'b000, Cnt1 = 3'b001,Assign0 = 3'b010,Assign1=3'b011,Cnt1x=4'b0100,Assign0x=3'b110, Default = 3'b111;   //刚开始进入默认状态，两个计数状态，两个赋值状态 
reg [2:0] CS,NS;
reg [31:0] Count;
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
    Cnt1x: Count <=Count+1;
    Assign0x: Count <= 0;
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
       if(Count == 1000000) begin
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
       if(Count == 1000000) begin          //去抖动
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
      if(x == 1) NS = Cnt1x;
      else NS = Default;                  //只有极个别才在此刻为1
    end
    Cnt1x:begin
      y = 1;
        if(Count == 2000000000 ) 
        NS = Assign0x;
      else if(x==0)
        NS = Cnt1x;
      else NS = Default;
    end
    Assign0x:begin           //将连续的1信号划分为多个01上升沿
      y = 0;
      if(Count == 2000000000) NS = Assign1;
      else if(x == 1) NS = Assign0x;
      else NS = Default;
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


module counter #(parameter W = 16,RST_VLU = 0)(input clk,rstn,pe,ce
,input [W-1:0] d,output reg [W-1:0] q);    //pe 同步置数使能 ；ce 计数使能 ；clk 时钟 ；rstn 复位
always @(posedge clk) begin
    if(!rstn) q <= RST_VLU;     //同步复位
    else if(pe) q <= d;
    else if(ce) q <= q+1;       //16位 
end
endmodule



module dynimage(
                input clk,rstn,
                input [15:0] d,
                output reg [3:0] an,
                output reg [6:0] seg);

                wire clkd;        //分频时钟
                reg [3:0] DIn;
                wire [6:0] DOut;
        frequdivision  FreDivClk(clk,~rstn,clkd);
        Decoder7Seg decoder(DIn,DOut);

parameter s0 = 2'b00, s1 = 2'b01,s2 = 2'b10,s3 = 2'b11;
reg [1:0] CS,NS;


always @(posedge clkd,negedge rstn) begin  //异步初始化
    if(~rstn) CS <= s0;
    else CS <= NS;
end

always @(*) begin
      case(CS)
      s0 : begin 
          DIn = d[3:0];  
          an = 4'b1110;
          seg = DOut; 
          NS = s1;
      end
      s1 : begin
        DIn = d[7:4];
        seg = DOut;
        NS = s2;
        if(( d[15:12]==0)&&(d[11:8]==0)&&( d[7:4]==0)) begin
          an = 4'b1111;
        end
        else an = 4'b1101;
      end
      s2 : begin
        DIn = d[11:8];
        seg = DOut;
        NS = s3;
        if((d[15:12]==0)&&( d[11:8]==0)) begin
          an = 4'b1111;
        end
        else  an = 4'b1011;  
        
      end
      s3 : begin
        DIn = d[15:12];
        seg = DOut;
        NS = s0;
         if(d[15:12]==0) begin
           an = 4'b1111;
         end
         else  an = 4'b0111;
      end
      endcase
end
    

endmodule





module frequdivision #(parameter N = 200000,RST_VLU = 0)(input clk,rst, 
output reg out);  //分频器 N = 100000 ~ 2000000
reg [19:0] cnt ;
always @(posedge clk) begin
   if(rst)  cnt <= RST_VLU;
   else if(cnt == (N-1)) cnt <= 0;
   else cnt <= cnt + 1;
end
always @(posedge clk) begin
    if(rst) out <= 0;
    else if(cnt == (N-2)) out <= 1;
    else out <= 0;
end
endmodule

module Decoder7Seg(            //7段译码管
input wire [3:0] In,
output reg [6:0] Out
    );
always @ (*)
    begin
    case(In)
        4'b0000: Out = 7'b000_0001;
        4'b0001: Out = 7'b100_1111;
        4'b0010: Out = 7'b001_0010;
        4'b0011: Out = 7'b000_0110;
        4'b0100: Out = 7'b100_1100;
        4'b0101: Out = 7'b010_0100;
        4'b0110: Out = 7'b010_0000;
        4'b0111: Out = 7'b000_1111;
        4'b1000: Out = 7'b000_0000;
        4'b1001: Out = 7'b000_0100;
        4'b1010: Out = 7'b000_1000;  //A
        4'b1011: Out = 7'b110_0000;  //b
        4'b1100: Out = 7'b011_0001;  //C
        4'b1101: Out = 7'b100_0010;  //d
        4'b1110: Out = 7'b011_0000;  //E
        4'b1111: Out = 7'b011_1000;  //F
    endcase
    end       
endmodule