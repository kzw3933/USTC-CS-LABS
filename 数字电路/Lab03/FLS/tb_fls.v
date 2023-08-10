module tb_tls();
reg clk;
wire [ 7:0] fn;
wire error;
parameter  Clock_Period = 10 ;

reg rstn;
reg [7:0] d;
initial begin
    clk = 0;
    rstn = 1;
    d = 8'b0000_0001;
    #4 rstn = ~rstn;
    #4 rstn = ~rstn;
    #4 rstn = ~rstn;
    #4 rstn = ~rstn;
    forever 
        #(Clock_Period/2) clk = ~clk;
end
FLS fls(.clk(clk),.en(1),.rstn(rstn),.d(d),.f(fn),.err(error));
endmodule