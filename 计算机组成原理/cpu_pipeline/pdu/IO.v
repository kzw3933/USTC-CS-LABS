module  IO(
        input               clk,
        input               rstn,
        input               del,
        input       [15:0]  hd,
        output      [31:0]  io_din
            );
            
wire        [3:0]   num;
reg         [31:0]  data;

wire                next;

assign     next =    hd[15] |
                     hd[14] |  
                     hd[13] |
                     hd[12] |
                     hd[11] |
                     hd[10] |
                     hd[ 9] |
                     hd[ 8] |
                     hd[ 7] |
                     hd[ 6] |
                     hd[ 5] |
                     hd[ 4] |
                     hd[ 3] |
                     hd[ 2] |
                     hd[ 1] |
                     hd[ 0] ;

encoder_16_4 encoder_16_4(.in(hd),.out(num));

    
always@(posedge clk or negedge rstn)begin
    if(!rstn)begin
        data <= 0;
    end
    else if(del) begin
        data  <= (data >> 4);
        end
    else if(next)begin
        data <= ((data << 4) + num );
    end
end     

assign      io_din = data;
          
endmodule