module VDS( input pclk,
            input hen,ven,
            input [11:0] rdata,
            output reg [5:0] raddr,
            output reg [11:0] prgb);
    always @(posedge pclk) begin
        if(hen&&ven)begin
          raddr <= raddr+1;
          prgb <= rdata;
        end
        else if((!hen)&&(!ven))
            raddr <= 6'b000000;
        else begin
          raddr <= raddr;
          prgb <= 12'h000;
        end
    end

endmodule
