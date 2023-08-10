module FreqDivision #(parameter N = 200000)(
                      input         clk,
                      input         rstn, 
                      output  reg   clkd
                      );  //分频�? N = 100000 ~ 2000000
wire [19:0] cnt ;
wire       reset;

always@(posedge clk or negedge rstn)begin
  if(!rstn) begin
        clkd <= 0;
  end
  else begin
        if(cnt == N-1) clkd <= 1;
        else           clkd <= 0;
  end
end

assign reset = (!rstn)|(cnt == N-1);
Counter Counter(.clk(clk),.rst(reset),.cnt(cnt));

endmodule