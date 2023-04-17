module tb_counter();
reg clk;
reg pe,ce,rstn;
reg [15:0] In;
wire [15:0] Out;
initial begin
     clk = 0;
     forever begin
       #5  clk = ~clk;
     end
end
initial begin
    rstn = 0;
    #8 rstn = ~rstn;
end
initial begin
    In = 0;
    pe = 0;
    ce = 1;
end
counter cnt(clk,rstn,pe,ce,In,Out);


endmodule
