module ps2_test(
input clk,rst,ps2_clk,ps2_data,
output reg [15:0] led
 );
reg ps2_clk_r1,ps2_clk_r2;
wire ps2_clk_neg;
reg [3:0] ps2_clk_cnt;
always@(posedge clk or posedge rst)
begin
 if(rst)
 ps2_clk_r1 <= 1'b1;
 else
 ps2_clk_r1 <= ps2_clk; 
end
always@(posedge clk or posedge rst)
begin
 if(rst)
 ps2_clk_r2 <= 1'b1;
 else
 ps2_clk_r2 <= ps2_clk_r1; 
end
assign ps2_clk_neg = (ps2_clk_r1==1'b0)&&(ps2_clk_r2==1'b1); 
always@(posedge clk or posedge rst)
begin
 if(rst)
 ps2_clk_cnt <= 4'd0;
 else if(ps2_clk_neg)
 begin
 if(ps2_clk_cnt>=4'd10)
 ps2_clk_cnt <= 4'd0;
 else
 ps2_clk_cnt <= ps2_clk_cnt + 4'd1;
 end
end
always@(posedge clk or posedge rst)
begin
 if(rst)
 led <= 16'hFFFF;
 else if(ps2_clk_neg)
 begin
 if((ps2_clk_cnt>=1)&&(ps2_clk_cnt<=8))
 led <= { ps2_data,led[15:1]};
 end
end 
endmodule