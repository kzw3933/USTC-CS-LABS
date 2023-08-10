module tb_fredivision();
    reg clk;
    reg rst;
    wire Out;
    genvar i;
    initial begin
        rst = 1;
        #8 rst = ~rst;

    end
    initial begin
        clk = 0;
        forever begin
          #5  clk = ~clk;
        end
    end
    frequdivision fredivision(clk,rst,Out);

endmodule