module counter #(parameter W = 16,RST_VLU = 0)(input clk,rstn,pe,ce
,input [W-1:0] d,output reg [W-1:0] q);    //pe 同步置数使能 ；ce 计数使能 ；clk 时钟 ；rstn 复位
always @(posedge clk) begin
    if(!rstn) q <= RST_VLU;     //同步复位
    else if(pe) q <= d;
    else if(ce) q <= q+1;       //16位 
end
endmodule