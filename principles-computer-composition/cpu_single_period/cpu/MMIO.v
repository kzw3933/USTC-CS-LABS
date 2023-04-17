module MMIO(
    input   [7:0]   dm_addr,
    input           dm_r_en,
    input   [31:0]  dm_r_data_raw,
    input   [31:0]  dm_r_data_dev,
    output  [31:0]  dm_r_data,
    input           dm_w_en,
    output          dm_w_dev_en,
    output          dm_r_dev_en
);

assign       dm_w_dev_en   = dm_w_en &(dm_addr >= 32'h00007f00);
assign       dm_r_dev_en   = dm_r_en &(dm_addr >= 32'h00007f00);

assign       dm_r_data  =  dm_r_en ? (dm_r_dev_en ? dm_r_data_dev : dm_r_data_raw):
                                    32'h0;
endmodule