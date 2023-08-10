module HazardUnit (
    input           br_taken_exe,
    input           dm_r_en_exe,
    input           alu_src1_is_pc_id,
    input           alu_src2_is_4_id,
    input           alu_src2_is_imm_id,
    input    [4:0]  rs_id,
    input    [4:0]  rt_id,
    input    [4:0]  rd_exe,    
    output   reg    flushF,
    output   reg    bubbleF,
    output   reg    flushD,
    output   reg    bubbleD,
    output   reg    flushE,
    output   reg    bubbleE,
    output          flushM,
    output          bubbleM,
    output          flushW,
    output          bubbleW
);

always@(*)begin
    flushF = 0;
    bubbleF = 0;
    if(dm_r_en_exe && ((rs_id == rd_exe)||(rt_id == rd_exe)))begin
        flushF = 0;
        bubbleF = 1;
    end
    else begin
        flushF = 0;
        bubbleF = 0;
    end  
end

always@(*)begin
    flushD = 0;
    bubbleD = 0;
    if(br_taken_exe)begin
        flushD = 1;
        bubbleD = 0;
    end
    else if(dm_r_en_exe &&  ((rs_id == rd_exe)||(rt_id == rd_exe))) begin
        flushD = 0;
        bubbleD = 1;
    end 
end


always@(*)begin
    if(br_taken_exe)begin
        flushE = 1;
        bubbleE = 0;
    end
    else if(dm_r_en_exe && ((rs_id == rd_exe)||(rt_id == rd_exe)))begin
        flushE = 1;
        bubbleE = 0;
    end
    else begin
        flushE = 0;
        bubbleE = 0;
    end
end

assign  bubbleW = 0;
assign  flushW = 0;

assign  flushM = 0;
assign  bubbleM = 0;

 
endmodule