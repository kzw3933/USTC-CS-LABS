module register(input clk,rst,en,              //寄存器
                input [3:0] in,
                output reg [3:0] out);
    always @(posedge clk) begin
        if(rst)  out <= 4'b0000;
        else if(en) out <= in;
    end
endmodule
