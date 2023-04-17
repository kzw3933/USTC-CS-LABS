module DataSort(input clk,rstn,          //冒泡法排序
                input [15:0] d,
                input bs,
                input pre,
                input nxt,
                input exe,
                output [6:0] seg,
                output  [7:0] an,
                output reg busy,
                output reg [15:0] delay
);
parameter s0 = 4'b0000,s0_1=4'b0001,s1= 4'b0010,s1_1=4'b0011,s1_2 = 4'b0100,s1_3 =4'b0101,s1_4 = 4'b0110,s1_5=4'b0111,s1_6=4'b1000,s1_7 = 4'b1001,s2 = 4'b1010;


reg [4:0] RA0,RA1;
wire [15:0] RD0,RD1;

reg [4:0] WA;
reg [15:0] WD;

wire DIo_we;

wire [4:0] DIo_RA,DIo_WA;
wire [15:0] DIo_WD;
wire [4:0] Count;

reg [3:0] CS,NS;
reg we;
reg [15:0] rd0,rd1;

// reg [15:0] ComIn0,ComIn1;
// wire [15:0] ComOut0,ComOut1;

register_file RF(.clk(clk),.ra0(RA0),.ra1(RA1),.rd0(RD0),.rd1(RD1),.wa(WA),.wd(WD),.we(we));
DIOx dio(clk,rstn,d,pre,bs,nxt,RD0,DIo_RA,DIo_WA,DIo_WD,DIo_we,an,seg,Count);
// CompareTwo comparetwo(ComIn0,ComIn1,ComOut0,ComOut1);

always @(posedge clk,negedge rstn) begin
    if(!rstn) CS <= s0;
    else CS <= NS;    
end

reg [4:0] Cnt1,Cnt0;

always @(*) begin
    case(CS)
    s0:begin
      if(exe) NS = s0_1;
      else NS = s0;
      end
    s0_1:begin
        NS = s1;
      end
    s1:begin
      if(Cnt0 == 0) NS = s2;
      else  NS = s1_1; 
      end
    s1_1:begin               
         NS = s1_2;             
    end
    s1_2:begin                     
        NS = s1_3;
    end
    s1_3:begin
    if(Cnt1 == Cnt0-1) begin
       NS = s1_7;
    end
    else   NS = s1_4;     
    end
    s1_4:begin
         NS = s1_5;
        end
    s1_5:begin
        NS = s1_6;
    end
    s1_6:begin
        NS = s1_3;
    end
    s1_7:begin
        NS = s1; 
      end
    s2:begin
      NS = s2; 
    end 
endcase
    
end

always @(posedge clk) begin
    case(NS)
        s0:begin               //输入数据阶段
            RA0 <= DIo_RA;
            WA <= DIo_WA;
            WD <= DIo_WD;
            we <= DIo_we;
            delay <= 0;
            busy <= 0;
           end 
        s0_1:begin
          Cnt0 <= Count;
          delay <= delay + 1;
        end
        s1:begin            //外层循环
          Cnt1 <= 0;   
          busy <= 1;
          we <=0;
          delay <= delay+1;
        end 
        s1_1:begin
          RA0 <= Cnt1;
          RA1 <= Cnt1+1;
           we <=0;
          delay <= delay+1;
        end
        s1_2:begin
          rd1 <= (RD1 > RD0)?RD0:RD1;      //rd1为较小数
          rd0 <= (RD0 > RD1)?RD0:RD1; 
           we <=0;
          delay <= delay+1;
        end
        s1_3:begin
             WA <= Cnt1;
            WD <= rd1;
            we <= 1;
          delay <= delay+1;
        end
        s1_4:begin
          Cnt1 <= Cnt1+1;
             we <=0;
          delay <= delay+1;
        end 
        s1_5:begin
           RA1 <= Cnt1+1;
             we <=0;
          delay <= delay+1;
        end
        s1_6:begin
            rd1 <= (RD1 > rd0)?rd0:RD1;   
            rd0 <= (rd0 > RD1)?rd0:RD1;
             we <=0;
            delay <= delay+1;
        end
        s1_7:begin
           Cnt0 <= Cnt0-1;
            we <=1;
          delay <= delay+1;
          WA <= Cnt0 ;
          WD <= rd0;
        end
        s2:begin
          busy <= 0;
          RA0 <= DIo_RA;
          WA <= DIo_WA;
          WD <= DIo_WD;
          we <= 0;
          end

    endcase
    
end

endmodule







module DIO(input clk,rstn,
            input [15:0] d,
            input pre,
            input bs,
            input nxt,
            output [7:0] an,
            output [6:0] seg
);

//对输入去抖动并同步化
wire DS_bs,DS_pre,DS_nxt;
wire [15:0] DS_d;
wire [3:0] En_d;

reg [15:0] DR;
reg [4:0] AR;
reg [15:0] DReg;
reg [15:0] DNxt;

reg [7:0] Count;

reg [4:0] wa;

reg [15:0] wd;

reg we;
wire [15:0] RD;

reg [2:0] CS,NS;

reg [1:0] T;   // {nxt,pre}

parameter s0 = 3'b000,s1 = 3'b001,s2 = 3'b010,s3 = 3'b011,s4 = 3'b100,s5 = 3'b101;

deSyn desyn_bs(clk,rstn,bs,DS_bs);
deSyn desyn_pre(clk,rstn,pre,DS_pre);
deSyn desyn_nxt(clk,rstn,nxt,DS_nxt);

deSyn desyn_d0(clk,rstn,d[0],DS_d[0]);
deSyn desyn_d1(clk,rstn,d[1],DS_d[1]);
deSyn desyn_d2(clk,rstn,d[2],DS_d[2]);
deSyn desyn_d3(clk,rstn,d[3],DS_d[3]);
deSyn desyn_d4(clk,rstn,d[4],DS_d[4]);
deSyn desyn_d5(clk,rstn,d[5],DS_d[5]);
deSyn desyn_d6(clk,rstn,d[6],DS_d[6]);
deSyn desyn_d7(clk,rstn,d[7],DS_d[7]);
deSyn desyn_d8(clk,rstn,d[8],DS_d[8]);
deSyn desyn_d9(clk,rstn,d[9],DS_d[9]);
deSyn desyn_d10(clk,rstn,d[10],DS_d[10]);
deSyn desyn_d11(clk,rstn,d[11],DS_d[11]);
deSyn desyn_d12(clk,rstn,d[12],DS_d[12]);
deSyn desyn_d13(clk,rstn,d[13],DS_d[13]);
deSyn desyn_d14(clk,rstn,d[14],DS_d[14]);
deSyn desyn_d15(clk,rstn,d[15],DS_d[15]);




encode ECD(DNxt,En_d);

register_file RF(.clk(clk),.ra0(AR),.rd0(RD),.wa(AR),.wd(DR),.we(we));

dynimage DIS(clk,rstn,Count,DR,an,seg);

always @(posedge clk,negedge rstn) begin
    if(!rstn) 
        CS <= s0;
    else CS <= NS;
    
end

/*
s0:默认状态
s1:读出数字并显示
s2:输入数字
s3:保存数字
s4:退格
s5:显示上一个数字或下一个数字
*/

always @(*) begin
    case(CS)
    s0: NS = s1;
    s1: NS = s2;
    s2: begin
      if(DS_nxt || DS_pre)  NS = s5;
      else if(bs) NS = s4;
      else if(DNxt) NS = s3;
      else NS = s2;
    end
    s3: NS = s2;
    s4: NS = s2;
    s5: NS = s1;  
    endcase      
end


always @(posedge clk,negedge rstn) begin
    if(!rstn) begin
      DNxt <= 0;
      DReg <= 0;
      AR <= 0;
      DR <= 0;
      Count <= 0;
      we <= 0;
    end
    else begin
      DNxt <= DS_d - DReg;
      DReg <= DS_d;
      case(CS)
      s0:begin
         AR <= 0;
         DR <= 0; 
         we <= 0;
    end
      s1:begin
          DR <= RD;
          we <= 0;
      end
      s2:begin
         T <= {DS_nxt,DS_pre};
         we <= 0;
      end 
      s3:begin
          DR <= (DR<<4)+En_d;
          we <= 1;
      end 
      s4:begin
         DR <=  DR >> 4;
         we <= 1;
      end 
      s5:begin
        we = 0;
        if(T[1]) begin
          AR <= AR +1;
          Count <= Count +1;
        end 
        if(T[0]) begin
          AR <= AR -1;
          Count <= Count -1;
        end 
      end 
      endcase
    end
end

endmodule

module DIOx(input clk,rstn,
            input [15:0] d,
            input pre,
            input bs,
            input nxt,
            input  [15:0] RD,
            output reg [4:0] RA,WA,
            output reg [15:0] WD,
            output reg we,
            output  [7:0] an,
            output  [6:0] seg,
            output  [4:0] Countx
 );
//对输入去抖动并同步化
wire DS_bs,DS_pre,DS_nxt;
wire [15:0] DS_d;
wire [3:0] En_d;

reg [7:0] Count;

reg [15:0] DReg,DNxt;

reg [2:0] CS,NS;

reg [1:0] T;

parameter s0 = 3'b000,s1 = 3'b001,s2 = 3'b010,s3 = 3'b011,s4 = 3'b100,s5 = 3'b101;

deSyn desyn_bs(clk,rstn,bs,DS_bs);
deSyn desyn_pre(clk,rstn,pre,DS_pre);
deSyn desyn_nxt(clk,rstn,nxt,DS_nxt);

deSyn desyn_d0(clk,rstn,d[0],DS_d[0]);
deSyn desyn_d1(clk,rstn,d[1],DS_d[1]);
deSyn desyn_d2(clk,rstn,d[2],DS_d[2]);
deSyn desyn_d3(clk,rstn,d[3],DS_d[3]);
deSyn desyn_d4(clk,rstn,d[4],DS_d[4]);
deSyn desyn_d5(clk,rstn,d[5],DS_d[5]);
deSyn desyn_d6(clk,rstn,d[6],DS_d[6]);
deSyn desyn_d7(clk,rstn,d[7],DS_d[7]);
deSyn desyn_d8(clk,rstn,d[8],DS_d[8]);
deSyn desyn_d9(clk,rstn,d[9],DS_d[9]);
deSyn desyn_d10(clk,rstn,d[10],DS_d[10]);
deSyn desyn_d11(clk,rstn,d[11],DS_d[11]);
deSyn desyn_d12(clk,rstn,d[12],DS_d[12]);
deSyn desyn_d13(clk,rstn,d[13],DS_d[13]);
deSyn desyn_d14(clk,rstn,d[14],DS_d[14]);
deSyn desyn_d15(clk,rstn,d[15],DS_d[15]);

assign Countx = Count[4:0];

encode ECD(DNxt,En_d);

dynimage DIS(clk,rstn,Count,WD,an,seg);

always @(posedge clk,negedge rstn) begin
    if(!rstn) 
        CS <= s0;
    else CS <= NS;
    
end

always @(*) begin
    case(CS)
    s0: NS = s1;
    s1: NS = s2;
    s2: begin
      if(T[1]||T[0])  NS = s5;
      else if(DS_bs) NS = s4;
      else if(DNxt) NS = s3;
      else NS = s2;
    end
    s3: NS = s2;
    s4: NS = s2;
    s5: NS = s1;  
    endcase      
end

always @(posedge clk,negedge rstn) begin
    if(!rstn) begin
      DNxt <= 0;
      DReg <= 0;
      RA <= 0;
      Count <= 0;
      we <= 0;
    end
    else begin
      DNxt <= DS_d - DReg;
      DReg <= DS_d;
      WA <= RA; 
      case(NS)
      s1:begin
          we <= 0;
      end
      s2:begin
         WD <= RD;
         T <= {DS_nxt,DS_pre};
         we <= 0;
      end 
      s3:begin
          WD <= (WD<<4)+En_d;
          we <= 1;
      end 
      s4:begin
         WD <=  WD >> 4;
         we <= 1;
      end 
      s5:begin
         we = 0;
        if(T[1]) begin
          RA <= RA +1;
          Count <= Count +1;
        end 
        if(T[0]) begin
          RA <= RA -1;
          Count <= Count -1;
        end 
      end 
      endcase
    end
end


endmodule




module deSyn(input clk,rstn,input x,output y);
wire de_x;
debounce DB(clk,rstn,x,de_x);
Synchron SYN(de_x,clk,rstn,y);
endmodule

module debounce(input clk,rstn,               
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





module Synchron(input x,input clk,rstn,
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





module  register_file # (
    parameter AW = 5,		//地址宽度
    parameter DW = 16		//数据宽度
)(
    input clk,			//时钟
    input [AW-1:0] ra0, ra1,		//读地址
    output [DW-1:0] rd0, rd1,	//读数据
    input [AW-1:0] wa, 		//写地址
    input [DW-1:0] wd,		//写数据
    input we			//写使能
);
    reg [DW-1:0] rf [0: (1<<AW)-1];		//寄存器堆
    assign rd0 = rf[ra0], rd1 = rf[ra1];	//读操作
    
    always  @ (posedge  clk)
        if (we)  rf[wa]  <=  wd;		//写操作
endmodule

module encode(input [15:0] In,output reg [3:0] Out );
    always @(*) begin
        case(In)
        16'b0000_0000_0000_0001: Out = 4'b0000;
        16'b0000_0000_0000_0010: Out = 4'b0001;
        16'b0000_0000_0000_0100: Out = 4'b0010;
        16'b0000_0000_0000_1000: Out = 4'b0011;
        16'b0000_0000_0001_0000: Out = 4'b0100;
        16'b0000_0000_0010_0000: Out = 4'b0101;
        16'b0000_0000_0100_0000: Out = 4'b0110;
        16'b0000_0000_1000_0000: Out = 4'b0111;
        16'b0000_0001_0000_0000: Out = 4'b1000;
        16'b0000_0010_0000_0000: Out = 4'b1001;
        16'b0000_0100_0000_0000: Out = 4'b1010;
        16'b0000_1000_0000_0000: Out = 4'b1011;
        16'b0001_0000_0000_0000: Out = 4'b1100;
        16'b0010_0000_0000_0000: Out = 4'b1101;
        16'b0100_0000_0000_0000: Out = 4'b1110;
        16'b1000_0000_0000_0000: Out = 4'b1111;
        endcase       
    end

endmodule

module dynimage(
                input clk,rstn,
                input [7:0] Cnt,
                input [15:0] d,
                output reg [7:0] an,
                output reg [6:0] seg);

                wire clkd;        //分频时钟
                reg [3:0] DIn;
                wire [6:0] DOut;
        frequdivision  FreDivClk(clk,~rstn,clkd);
        Decoder7Seg decoder(DIn,DOut);

parameter s0 = 3'b000, s1 = 3'b001,s2 = 3'b010,s3 = 3'b011,s4 = 3'b100,s5 = 3'b101;
reg [2:0] CS,NS;


always @(posedge clkd,negedge rstn) begin  //异步初始化
    if(~rstn) CS <= s0;
    else CS <= NS;
end

always @(*) begin
      case(CS)
      s0 : begin 
          DIn = d[3:0];  
          an = 8'b1111_1110;
          seg = DOut; 
          NS = s1;
      end
      s1 : begin
        DIn = d[7:4];
        seg = DOut;
        NS = s2;
        an = 8'b1111_1101;
      end
      s2 : begin
        DIn = d[11:8];
        seg = DOut;
        NS = s3;
        an = 8'b1111_1011;  
        
      end
      s3 : begin
        DIn = d[15:12];
        seg = DOut;
        NS = s4;
        an = 8'b1111_0111;
      end
      s4 :begin
        DIn = Cnt[3:0];
        seg = DOut;
        NS = s5;
        an = 8'b1011_1111;
      end
      s5 :begin
        DIn = Cnt[7:4];
        seg = DOut;
        NS = s0;
        an = 8'b0111_1111;
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


// module CompareTwo(input [15:0] In0,In1,
//                   output reg [15:0] Out0,Out1  
//                     );
// always @(*) begin
//           if(In0 > In1) begin
//             Out0 = In0;
//             Out1 = Out1;
//           end
//           else begin
//             Out0 = In1;
//             Out1 = In0;
//           end
//         end

// endmodule