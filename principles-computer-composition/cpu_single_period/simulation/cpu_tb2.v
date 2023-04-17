module cpu_tb2();

    reg clk;
    reg rstn;

    reg [15:0]  hd;
    reg         ent;
    reg         chk;
    reg         step;


top top(.clk(clk),.hd(hd),.ent(ent),.chk(chk),.step(step),.rstn(rstn));

always #1 clk = ~clk;
initial begin
$display("initialing....");
clk = 0;
rstn = 1;
hd   = 0;
chk  = 0;
step = 0;
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
step = 1;
#2
step = 0;
#2
step = 1;
#2 
step = 0;

#100000
$display("Simulation Over");
$stop();
end









endmodule