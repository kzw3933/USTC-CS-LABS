`ifndef CONST_VALUES
`define CONST_VALUES
//Imm-Type
    `define R_TYPE 3'h0
    `define I_TYPE 3'h1
    `define B_TYPE 3'h2
    `define J_TYPE 3'h3
    `define S_TYPE 3'h4
    `define U_TYPE 3'h5
//Alu-Op
    `define ALU_ADD 2'h0
    `define ALU_SUB 2'h1
    `define ALU_OP1 2'h2
    `define ALU_OP2 2'h3
//Cpu-Mode
    `define CPU_PAUSE 2'h0
    `define CPU_CONT  2'h1
    `define CPU_STEP  2'h2
//Chk-Category
    `define CHK_PC    4'h4
    `define CHK_RF    4'h8
    `define CHK_DM    4'hC
//IO-Category
    `define  IO_LED             8'h0
    `define  IO_SW              8'h4
    `define  IO_POL_OUT         8'h8
    `define  IO_POL_OUT_VLD     8'hC
    `define  IO_POL_IN          8'h10
    `define  IO_POL_IN_VLD      8'h14
`endif