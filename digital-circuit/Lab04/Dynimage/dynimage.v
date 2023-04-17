module dynimage(input [3:0] d0,d1,d2,d3,
                input clk,rstn,
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
          DIn = d0;  
          an = 4'b1110;
          seg = DOut; 
          NS = s1;
      end
      s1 : begin
        DIn = d1;
        seg = DOut;
        NS = s2;
        if((d1==0)&&(d3==0)&&(d2==0)) begin
          an = 4'b1111;
        end
        else an = 4'b1101;
      end
      s2 : begin
        DIn = d2;
        seg = DOut;
        NS = s3;
        if((d3==0)&&(d2==0)) begin
          an = 4'b1111;
        end
        else  an = 4'b1011;  
        
      end
      s3 : begin
        DIn = d3;
        seg = DOut;
        NS = s0;
         if(d3==0) begin
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