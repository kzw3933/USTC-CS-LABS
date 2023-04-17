module top_tb();

    reg clk;
    reg rstn;

    reg [15:0]  hd;
    reg         ent;
    reg         chk;
    reg         cont;
    reg         del;


top top(.clk(clk),.hd(hd),.ent(ent),.chk(chk),.cont(cont),.rstn(rstn),.del(del));

always #1 clk = ~clk;
initial begin
$display("initialing....");
clk = 0;
rstn = 1;
hd   = 0;
chk  = 0;
cont = 0;
del = 0;
#10 
rstn = 0;
#10 
rstn = 1;
hd   = 16'b0000_0000_0001_0000;
#2
hd   = 16'b0000_0000_0000_0000;
#8
hd   = 16'b0000_0000_0000_0001;
#2 
hd   = 16'b0000_0000_0000_0000;
#8
hd   = 16'b0000_0000_0000_0001;
#2 
hd   = 16'b0000_0000_0000_0000;
#8
hd   = 16'b0000_0000_0000_0001;
chk  = 1;
#10 
$display("Start Executing ...");
cont = 1;
# 100
hd   = 16'b0000_0000_0000_0001;
#2
hd   = 16'b0000_0000_0000_0000;
#8
hd   = 16'b0000_0000_0000_0001;
#2 
hd   = 16'b0000_0000_0000_0000;
#8
hd   = 16'b0000_0000_0000_0001;
#2 
hd   = 16'b0000_0000_0000_0000;
#8
hd   = 16'b0000_0000_0000_0001;
#2 
hd   = 16'b0000_0000_0000_0000;
#8
hd   = 16'b0000_0000_0000_0010;
#2 
hd   = 16'b0000_0000_0000_0000;
#8
hd   = 16'b0000_0000_0000_0100;
#2 
hd   = 16'b0000_0000_0000_0000;
#2
ent = 1;
#100000
del = 1;
#4
del = 0;
#4
del = 1;
#4 
del = 0;
#4
del = 1;
#4 
del = 0;
#4
del = 1;
#4 
del = 0;
#4
del = 1;
#4 
del = 0;
#4
del = 1;
#4 
del = 0;
#4
del = 1;
#4
del = 0;
#4
del = 1;
#4 
del = 0;
#4
del = 1;
#4 
del = 0;
#4
del = 1;
#4 
del = 0;
#4
del = 1;
#4 
del = 0;
#4
del = 1;
#4 
del = 0;
#4
del = 1;
#4 
del = 0;

$display("Simulation Over");
$stop();
end









endmodule