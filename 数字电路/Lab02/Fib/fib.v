module register(input clk,rst,en,              //寄存器
                input [3:0] in,
                output reg [3:0] out);
    always @(posedge clk) begin
        if(rst)  out <= 4'b0000;
        else if(en) out <= in;
    end
endmodule

`timescale 1ns/100ps

module addof1(input  a,
            input b,
            input ci,             //一位全加器
            output  y,
            output co);
            assign y=a^b^ci;
            assign co = a*b+(a^b)*ci;
endmodule 

module addof4(input [3:0] a,
              input [3:0] b,
              output  [3:0] y,
               output wire cf,
              output reg of,
              output reg zf);   
    wire [2:0] temp;
    addof1   addof1_0(a[0],b[0],1'b0,y[0],temp[0]) ;
    addof1   addof1_1(a[1],b[1],temp[0],y[1],temp[1]) ;
    addof1   addof1_2(a[2],b[2],temp[1],y[2],temp[2]) ;
    addof1   addof1_3(a[3],b[3],temp[2],y[3],cf) ;  
    always @(*) begin
        if(cf^temp[2])  of = 1;
        else of = 0;
        if(y ==0) zf =1;
        else zf =0 ;        
    end 
endmodule

module subtractof4(input [3:0] a,
              input [3:0] b,
              output  [3:0] y,
               output reg cf,
              output reg of,
              output reg zf);   
    wire [3:0] temp;
    addof1   addof1_0(a[0],~b[0],1'b1,y[0],temp[0]) ;
    addof1   addof1_1(a[1],~b[1],temp[0],y[1],temp[1]) ;
    addof1   addof1_2(a[2],~b[2],temp[1],y[2],temp[2]) ;
    addof1   addof1_3(a[3],~b[3],temp[2],y[3],temp[3]) ;  
    always @(*) begin
        if(cf^temp[2])  of = 1;
        else of = 0;
        if(y ==0) zf =1;
        else zf =0 ; 
        cf = ~temp[3];
    end
endmodule


module alu(input wire [3:0] a,b,          //两个操作数
           input wire [2:0] s,         //操作选择
           output reg [3:0] y,         //运算结果
           output reg cf,             //  进、借位标志
           output reg of,               // 溢出标志
           output reg zf);           //  零标志
reg [3:0] Adda,Addb,Suba,Subb;
wire [3:0] Addy,Suby;
wire Addcf,Addof,Addzf,Subcf,Subof,Subzf;
addof4 Add(Adda,Addb,Addy,Addcf,Addof,Addzf);
subtractof4 Subtract(Suba,Subb,Suby,Subcf,Subof,Subzf);
always @(*) begin
    case(s)
        3'b000:
            begin
              Adda = a;
              Addb = b;
              y = Addy;
              cf = Addcf;
              zf = Addzf;
              of = Addof;
            end
        3'b001:
        begin
              Suba = a;
              Subb = b;
              y = Suby;
              zf = Subzf;
              of = Subof;
              cf = Subcf;
           // if(y[3]==1) cf = 1;
            //else cf = 0;  
            end
        3'b100:
        begin
              y = a&b;
              cf = 0;
              zf = y?0:1;
              of = 0;
            end
        3'b101:
        begin
              y = a|b;
              cf = 0;
              zf = y?0:1;
              of = 0;
            end
        3'b110:
        begin
              y = a^b;
               cf = 0;
              zf = y?0:1;
              of = 0;
            end
        3'b111:
        begin
              y = ~a;
               cf = 0;
              zf = y?0:1;
              of = 0;
            end
      default:
        begin
              y = 0;
               cf = 0;
              zf = 1;
              of = 0;
            end
    endcase
end
endmodule



module fib(input clk,rst,en,
            output reg [3:0] fn);
wire [3:0] R1out,R2out;
wire [3:0] AluOut;
reg [3:0]  R1in,R2in;
alu ALU(.s(3'b000),.a(R1out),.b(R2out),.y(AluOut));
register R1(.clk(clk),.rst(0),.en(en),.in(R1in),.out(R1out));
register R2(.clk(clk),.rst(0),.en(en),.in(R2in),.out(R2out));
always @(*) begin
      if(rst)  begin
        R1in <= 0;
        R2in <= 1;
        fn <= R2out;
      end
      else begin
        R1in <= R2out;
        R2in <= AluOut;
        fn <= R2out;
      end
end
endmodule


//Verilog的执行流程，数据流的流动顺序