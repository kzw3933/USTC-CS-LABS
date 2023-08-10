module tb_Synchron();
    reg x;
    reg clk;
    reg rstn;
    wire y;
    initial begin
        clk = 0;
        forever begin
            #5 clk = ~clk;
        end
    end
    initial begin
        rstn = 0;
        #8 rstn = ~rstn;
    end
    initial begin
        x = 0;
        forever begin
        #1 x = ~x;
        #2 x = ~x;
        #3 x = ~x;
        #4 x = ~x;
        #5 x = ~x;
        #6 x = ~x; 
        end
    end
    Synchron Syc(x,clk,rstn,y);

endmodule