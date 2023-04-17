`include "../../parameter.v"

module MEM (
    input                                   clk,
    input                                   rstn,
    input                                   flushM,
    input                                   bubbleM,
    input  [`exe_to_mem_bus_w-1:0]          exe_to_mem_bus,
    output [`mem_to_wb_bus_w-1:0]           mem_to_wb_bus,
    output [4:0]                            reg_dest_mem,
    output                                  reg_w_en_mem,
    output [31:0]                           mem_result,
    output [7:0]                            io_addr,
    output [31:0]                           io_dout,
    output                                  io_we,
    output                                  io_rd,
    input  [31:0]                           io_din,
    input  [7:0]                            debug_dm_addr,
    output [31:0]                           debug_dm_data
);

reg        [`exe_to_mem_bus_w-1:0]          exe_to_mem_bus_r;
reg        [`exe_to_mem_bus_w-1:0]          exe_to_mem_bus_old;
wire       [`exe_to_mem_bus_w-1:0]          exe_to_mem_bus_now;
reg                                         bubbleM_r;
reg                                         flushM_r;

wire       [31:0]                           pc_mem;
wire       [31:0]                           alu_out;
wire       [4:0]                            rd;
wire       [31:0]                           dm_w_data;
wire                                        mem_to_reg;
wire                                        dm_r_en;
wire                                        dm_w_en;
wire                                        reg_w_en;

wire       [31:0]                           dm_r_data_raw;
wire       [31:0]                           dm_r_data_dev;
wire                                        dm_w_dev_en;
wire                                        dm_r_dev_en;
wire       [31:0]                           dm_r_data;

assign                                      io_rd = dm_r_dev_en;
assign                                      io_we = dm_w_dev_en;
assign                                      io_addr = alu_out[7:0];
assign                                      io_dout = dm_w_data;
assign                                      dm_r_data_dev = io_din;
            
assign        reg_dest_mem = rd;
assign        reg_w_en_mem = reg_w_en;
assign        mem_result = alu_out;

assign   {
        pc_mem,
        rd,
        dm_w_data,
        alu_out,
        mem_to_reg,
        reg_w_en,
        dm_r_en,
        dm_w_en    
} = exe_to_mem_bus_now; 

assign  exe_to_mem_bus_now = bubbleM_r ? exe_to_mem_bus_old :
                                flushM_r ? 0 : exe_to_mem_bus_r;

always@(posedge clk or negedge rstn)begin
    if(!rstn)begin
        exe_to_mem_bus_r <= 0;
        exe_to_mem_bus_old <= 0;
        flushM_r <= 0;
        bubbleM_r <= 0;
    end      
    else begin
        exe_to_mem_bus_r <= exe_to_mem_bus;
        exe_to_mem_bus_old <= exe_to_mem_bus_now;
        flushM_r <= flushM;
        bubbleM_r <= bubbleM;
    end
        
end



DataMemory  DataMemory(
    .a(alu_out[7:0]),
    .d(dm_w_data),
    .dpra(debug_dm_addr),
    .dpo(debug_dm_data),
    .clk(clk),
    .we(dm_w_en&(!dm_w_dev_en)),
    .spo(dm_r_data_raw)
    );

MMIO   MMIO(
    .dm_addr(alu_out[31:0]),
    .dm_r_en(dm_r_en),
    .dm_r_data_raw(dm_r_data_raw),
    .dm_r_data_dev(dm_r_data_dev),
    .dm_w_dev_en(dm_w_dev_en),
    .dm_r_data(dm_r_data),
    .dm_w_en(dm_w_en),
    .dm_r_dev_en(dm_r_dev_en)
);




assign    mem_to_wb_bus = {
                        pc_mem,
                        rd,
                        dm_r_data,
                        alu_out,
                        mem_to_reg,
                        reg_w_en
                        };

    
endmodule