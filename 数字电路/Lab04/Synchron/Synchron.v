module Synchron(input x,input clk,rstn,
                output y);
reg s1,s2;
reg s;

always @(posedge clk) begin
    if(~rstn ) begin
      s1 <= 0;
      s2 <= 0;
      s <= 0;
    end
    else begin
      s1 <= x;
      s2 <= s1;
      s <= s2;
    end
end
assign y = (!s)&s2;
endmodule









