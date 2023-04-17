module frequdivision #(parameter N = 16,RST_VLU = 0)(input clk,rst, 
output reg out);  //分频器,将频率降低N倍
reg [3:0] cnt;
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