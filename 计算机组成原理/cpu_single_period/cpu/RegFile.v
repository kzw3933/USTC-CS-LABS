module RegFile(
    input               clk,
    input       [4:0]   raddr1,
    input       [4:0]   raddr2,
    output      [31:0]  rdata1,
    output      [31:0]  rdata2,
    input       [4:0]   waddr,
    input       [31:0]  wdata,
    input               w_en,
    input       [4:0]   debug_addr,
    output      [31:0]  debug_data
    );
    
    reg [31:0] regfile[31:1];
    
    assign rdata1       = (raddr1 == 0) ? 0 : regfile[raddr1];
    assign rdata2       = (raddr2 == 0) ? 0 : regfile[raddr2];
    assign debug_data   = (debug_addr == 0) ? 0 : regfile[debug_addr];
    
    always@(posedge clk)begin
        if(w_en) begin
            if(waddr != 0)begin
                regfile[waddr] = wdata;
            end
        end
    end   
endmodule