module Synchron #(parameter W=1)(
                input               clk,
                input               rstn,
                input     [W-1:0]   in,
                output    [W-1:0]   out
                );

reg     [W-1:0]        in_r1;
reg     [W-1:0]        in_r2;

always@(posedge clk or negedge rstn)begin
    if(!rstn)begin
        in_r1 <= 0;
        in_r2 <= 0;
    end
    else begin
        in_r1 <= in;
        in_r2 <= in_r1;
    end
    
end

assign out = (~in_r2)&in;

endmodule