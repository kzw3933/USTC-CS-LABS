module tb_dedynimage();
    reg clk1,clk2;
    reg rstn;
    wire [3:0] an;
    wire [6:0] seg;
    dedynimage DN(clk2,rstn,clk1,an,seg);
    initial begin
        rstn = 0;
        #5 rstn = ~rstn;
        #5 rstn = ~rstn;
        #5 rstn = ~rstn;

    end
    initial begin
        clk1 = 0;
        forever begin
            #1000000 clk1 = ~clk1;
        end
    end
    initial begin
        clk2 = 0;
        forever begin
            #1 clk2 = ~clk2;
        end
    end
endmodule