`ifndef MYCPU_H
    `define MYCPU_H

    `define BR_BUS_WD       34
    `define FS_TO_DS_BUS_WD 71
    `define DS_TO_ES_BUS_WD 186
    `define ES_TO_MS_BUS_WD 84+1
    `define MS_TO_WS_BUS_WD 74
    `define WS_TO_RF_BUS_WD 41
    
    `define CR_STATUS 8'b01100_000
    `define CR_CAUSE 8'b01101_000
    `define CR_EPC 8'b01110_000
    `define CR_COUNT 8'b01001_000
    `define CR_COMPARE 8'b01011_000
    `define CR_BADVADDR 8'b01000_000
    
    `define EXC_INT 8'h00
    `define EXC_ADEL 8'h04
    `define EXC_ADES 8'h05
    `define EXC_SYS 8'h08
    `define EXC_BP 8'h09
    `define EXC_RI 8'h0a
    `define EXC_OV 8'h0c
    
    
`endif
