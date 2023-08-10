module ForwardingUnit (
    input              reg_w_en_mem,
    input              reg_w_en_wb,
    input      [4:0]   reg_dest_mem,
    input      [4:0]   reg_src1_exe,
    input      [4:0]   reg_src2_exe,
    input      [4:0]   reg_dest_wb,
    output reg [1:0]   rs_value_from,
    output reg [1:0]   rt_value_from
);

always@(*)begin
    if((reg_src1_exe == reg_dest_mem)&&reg_w_en_mem &&(reg_dest_mem !=0))
        rs_value_from = 2'b01;
    else if((reg_src1_exe == reg_dest_wb)&&reg_w_en_wb &&(reg_dest_wb !=0))
        rs_value_from = 2'b10;
    else 
        rs_value_from = 2'b00;

end

always@(*)begin
    if((reg_src2_exe == reg_dest_mem)&&reg_w_en_mem &&(reg_dest_mem !=0))
        rt_value_from = 2'b01;
    else if((reg_src2_exe == reg_dest_wb)&&reg_w_en_wb &&(reg_dest_wb !=0))
        rt_value_from = 2'b10;
    else 
        rt_value_from = 2'b00;

end





endmodule