`include "../../parameter.v"
module WB (
    input                                   clk,
    input                                   rstn,
    input                                   flushW,
    input                                   bubbleW,
    input     [`mem_to_wb_bus_w-1:0]        mem_to_wb_bus,
    output    [`wb_bus_w-1:0]               wb_bus,
    output    [4:0]                         reg_dest_wb,
    output                                  reg_w_en_wb,
    output    [31:0]                        wb_result
);

reg           [`mem_to_wb_bus_w-1:0]        mem_to_wb_bus_r;
reg           [`mem_to_wb_bus_w-1:0]        mem_to_wb_bus_old;
wire          [`mem_to_wb_bus_w-1:0]        mem_to_wb_bus_now;
reg                                         flushW_r;
reg                                         bubbleW_r;

wire          [31:0]         pc_wb;
wire          [4:0]          wb_addr;
wire          [31:0]         dm_r_data;
wire          [31:0]         alu_out;
wire                         mem_to_reg;
wire                         wb_en;

wire          [31:0]         wb_data;

assign                       wb_result = wb_data;
assign                       reg_dest_wb = wb_addr;

assign                       reg_w_en_wb = wb_en;

assign    wb_data = mem_to_reg ? dm_r_data : alu_out;

assign    {
            pc_wb,
            wb_addr,
            dm_r_data,
            alu_out,
            mem_to_reg,
            wb_en
}  = mem_to_wb_bus_now;

assign mem_to_wb_bus_now = bubbleW_r ? mem_to_wb_bus_old :
                           flushW_r ? 0 : mem_to_wb_bus_r;


always@(posedge clk or negedge rstn)begin
    if(!rstn)begin
        mem_to_wb_bus_old <= 0;
        mem_to_wb_bus_r <= 0;
        flushW_r <= 0;
        bubbleW_r <= 0;
    end
    else begin
        mem_to_wb_bus_r <= mem_to_wb_bus;
        flushW_r <= flushW;
        bubbleW_r <= bubbleW;
    end
end

assign  wb_bus = {
                wb_data,
                wb_en,
                wb_addr
                };
    
endmodule