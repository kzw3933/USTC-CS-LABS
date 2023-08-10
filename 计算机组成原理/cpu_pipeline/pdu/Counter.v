// W              计数器位�?
// RST_VLU        计数器重置置�?
// CNT_VLU        计数器的模数

module Counter #(parameter W = 20 ,RST_VLU = 0 )(
                            input               clk,
                            input               rst,
                            output reg   [W-1:0] cnt
                            );
    always@(posedge clk )begin
        if(rst)begin
            cnt <= RST_VLU;
        end
        else begin
            cnt <= cnt + 1;
        end
    end

endmodule