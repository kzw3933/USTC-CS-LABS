module GameOfLife (
    input clk,rstn,
    input draw,
    input evolu,
    input [11:0] rgb,
    input [2:0] Rate1,                       //存活率，8个中少于几个才能存活
    input [2:0] Rate2,                        //8个中多于几个才能才能存活
    output prgb,
    output hs,vs
);

wire pclk;

reg [7:0] addr_x,addr_y;
wire raddr1_x,raddr1_y,raddr2_x,raddr2_y,raddr3_x,raddr3_y,raddr4_x,raddr4_y,raddr5_x,raddr5_y,raddr6_x,raddr6_y,raddr7_x,raddr7_y,raddr8_x,raddr8_y;
assign raddr1_x = (addr_x+199)%200;
assign raddr2_x = (addr_x+199)%200;
assign raddr3_x = (addr_x+199)%200;
assign raddr4_x = addr_x;
assign raddr5_x = addr_x;
assign raddr6_x = (addr_x+1)%200;
assign raddr7_x = (addr_x+1)%200;
assign raddr8_x = (addr_x+1)%200;

assign raddr1_y = (addr_y+149)%150;
assign raddr2_y = addr_y;
assign raddr3_y = (addr_y+1)%150;
assign raddr4_y = (addr_y+149)%150;
assign raddr5_y = (addr_y+1)%150;
assign raddr6_y = (addr_y+149)%150;
assign raddr7_y = addr_y;
assign raddr8_y = (addr_y+1)%150;



clk_wiz_0 PCLK(.clk_in1(clk),.clk_out1(pclk));

blk_mem_gen_0 vram(.addra(waddr),.clka(clk),.dina(wdata_reg),.ena(1),.wea(we),.addrb(raddr1),.clkb(pclk),.doutb(rdata1),.enb(1));

blk_mem_gen_0 vram(.addra(waddr),.clka(clk),.dina(wdata_reg),.ena(1),.wea(we),.addrb(raddr2),.clkb(pclk),.doutb(rdata2),.enb(1));

blk_mem_gen_0 vram(.addra(waddr),.clka(clk),.dina(wdata_reg),.ena(1),.wea(we),.addrb(raddr3),.clkb(pclk),.doutb(rdata3),.enb(1));

blk_mem_gen_0 vram(.addra(waddr),.clka(clk),.dina(wdata_reg),.ena(1),.wea(we),.addrb(raddr4),.clkb(pclk),.doutb(rdata4),.enb(1));

blk_mem_gen_0 vram(.addra(waddr),.clka(clk),.dina(wdata_reg),.ena(1),.wea(we),.addrb(raddr5),.clkb(pclk),.doutb(rdata5),.enb(1));

blk_mem_gen_0 vram(.addra(waddr),.clka(clk),.dina(wdata_reg),.ena(1),.wea(we),.addrb(raddr6),.clkb(pclk),.doutb(rdata6),.enb(1));

blk_mem_gen_0 vram(.addra(waddr),.clka(clk),.dina(wdata_reg),.ena(1),.wea(we),.addrb(raddr7),.clkb(pclk),.doutb(rdata7),.enb(1));

blk_mem_gen_0 vram(.addra(waddr),.clka(clk),.dina(wdata_reg),.ena(1),.wea(we),.addrb(raddr8),.clkb(pclk),.doutb(rdata8),.enb(1));

blk_mem_gen_0 vram(.addra(waddr),.clka(clk),.dina(wdata_reg),.ena(1),.wea(we),.addrb(raddr8),.clkb(pclk),.doutb(rdata9),.enb(1));

// blk_mem_gen_0 vram(.addra(waddr),.clka(clk),.dina(wdata_reg),.ena(1),.wea(we),.addrb(raddr8),.clkb(pclk),.doutb(rdata10),.enb(1));


blk_mem_gen_1 rom(.addra(raddr2),.clka(pclk),.douta(rdata2),.ena(1));








    
endmodule