module  DynDis (
                input           clk,
                input           rstn,
                input   [31:0]  in,
                output  [6:0]   seg,
                output  [7:0]   an
                );

parameter s0 = 3'b000, s1 = 3'b001,s2 = 3'b010,s3 = 3'b011,s4 = 3'b100
                 ,s5 = 3'b101,s6 = 3'b110 ,s7 = 3'b111;
                 
wire                    clkd; 
reg     [3:0]           DIn;
wire    [6:0]           DOut;
reg     [2:0]           CS;
reg     [2:0]           NS;

assign              seg = DOut;

Decoder7Seg decoder(DIn,DOut); 

FreqDivision#(200000)  FreDivision(.clk(clk),.rstn(rstn),.clkd(clkd)); 

always @(posedge clkd or negedge rstn) begin  //异步初始�?
    if(!rstn)begin
        CS <= s0;
    end 
    else begin
        CS <= NS;
    end
end

assign an = (CS == s0) ? 8'b1111_1110 :
            (CS == s1) ? 8'b1111_1101 :
            (CS == s2) ? 8'b1111_1011 :
            (CS == s3) ? 8'b1111_0111 :
            (CS == s4) ? 8'b1110_1111 :
            (CS == s5) ? 8'b1101_1111 :
            (CS == s6) ? 8'b1011_1111 :
            (CS == s7) ? 8'b0111_1111 :
                        8'b1111_1111;

always @(*) begin
      case(CS)
      s0 : begin 
        DIn = in[3:0];  
        NS = s1;
      end
      s1 : begin
        DIn = in[7:4];
        NS = s2;
      end
      s2 : begin
        DIn = in[11:8];
        NS = s3;
      end
      s3 : begin
        DIn = in[15:12];
        NS = s4;
      end
      s4:begin
        DIn = in[19:16];
        NS = s5;
      end
      s5:begin
        DIn = in[23:20];
        NS = s6;
      end
      s6:begin
        DIn = in[27:24];
        NS = s7;
      end
      s7:begin
        DIn = in[31:28];
        NS = s0;
      end
      endcase
end
 
endmodule 


