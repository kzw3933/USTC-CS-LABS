
module PCU(input clk,rstn,
            input [3:0] dir,
            input [11:0] rgb,
            input draw,
            output reg [14:0] waddr,
            output  [11:0] wdata,
            output  we );                //绘画控制单元
always @(posedge clk,negedge rstn) begin
    if(!rstn) begin
      waddr <= 15'd14999;
    end
    else begin
      if((!dir[0])&&(!dir[1]&&(!dir[2])&&(dir[3]))) //左
            waddr <= waddr -1;
      else if((!dir[0])&&(!dir[1]&&(dir[2])&&(!dir[3]))) //下
            waddr <= waddr + 200;
      else if((!dir[0])&&(dir[1]&&(!dir[2])&&(!dir[3]))) //右
            waddr <= waddr +1;
      else if((!dir[0])&&(dir[1]&&(!dir[2])&&(!dir[3]))) //上
            waddr <= waddr - 200;
    end   
end
assign we = draw;
assign wdata = rgb;

endmodule
