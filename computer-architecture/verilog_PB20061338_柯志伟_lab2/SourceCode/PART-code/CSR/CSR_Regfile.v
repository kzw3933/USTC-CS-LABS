`timescale 1ns / 1ps
// 实验要求
    // 补全模块（阶段三�?

module CSR_Regfile(
    input wire clk,
    input wire rst,
    input wire CSR_write_en,
    input wire [11:0] CSR_write_addr,
    input wire [11:0] CSR_read_addr,
    input wire [31:0] CSR_data_write,
    output wire [31:0] CSR_data_read
    );

    reg [31:0] csr[4095:0];

    integer i;
    
    // TODO: Complete this module

    /* FIXME: Write your code here... */
    always @(posedge clk or posedge rst) begin
        if(rst)
            for(i=1;i<4096;i=i+1)
                csr[i] <= 32'h0;
        else if(CSR_write_en)
            csr[CSR_write_addr] = CSR_data_write;  
    end

    assign CSR_data_read = csr[CSR_read_addr];

endmodule 
