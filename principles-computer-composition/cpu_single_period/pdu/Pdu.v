`include "../parameter.v"

module Pdu(
       //From device interface
       input                clk,
       input                rstn,
       input                step,
       input                cont,
       input                chk,
       input                ent,
       input                del,
       input        [15:0]  hd, 
       output               clk_cpu,
       output               pause,
       output       [15:0]  led,
       output       [7:0]   an,
       output       [6:0]   seg,
       //To and From cpu interface
       //IO_BUS
       input        [7:0]   io_addr,
       input        [31:0]  io_dout,
       input                io_we,
       output       [31:0]  io_din,
       input                io_rd,
       //Debug_BUS
       output       [7:0]   dm_rf_addr,
       input        [31:0]  rf_data,
       input        [31:0]  dm_data,
       input        [31:0]  pc
    );

    reg            [1:0]    CPU_MODE;
    wire                    CPU_BREAK_POINT;
    reg            [31:0]   CPU_BRK_ADDR;

    wire           [31:0]   chk_addr;
    reg                     chk_pc;
    reg                     chk_rf;
    reg                     chk_dm;

    wire           [31:0]   io_data;


    wire                    io_to_led;
    wire                    io_from_sw;
    wire                    io_to_pol;
    wire                    io_from_pol_in_vld;
    wire                    io_from_pol;
    wire                    io_from_pol_out_vld;
    
    reg                     pol_din_vld;
    reg                     pol_dout_vld;
    reg            [31:0]   pol_din;
    wire           [31:0]   seg_data;
    wire                    KeyEvent;
    reg            [15:0]   hd_r;
    reg            [31:0]   tmp;

    assign                  KeyEvent = del | (hd_r != hd);
    


    assign                  CPU_BREAK_POINT = (pc == CPU_BRK_ADDR);
    assign                  pause = (CPU_MODE == `CPU_PAUSE);

    assign                  dm_rf_addr = chk_addr[7:0];

    always@(posedge clk or negedge rstn)begin
        if(!rstn)begin
            hd_r <= 0;
        end
        else begin
            hd_r <=  hd;
        end
                    
    end 

    IO IO(.clk(clk),.del(del),.hd(hd),.io_din(io_data),.rstn(rstn));

    assign                  chk_addr = io_data;

    always@(posedge clk or negedge rstn)begin
        if(!rstn) begin
            CPU_MODE <= `CPU_PAUSE;
        end 
        else if(CPU_MODE == `CPU_PAUSE)begin
            if(step)begin
                CPU_MODE <= `CPU_STEP;
            end
            else if(cont)begin
                CPU_MODE <= `CPU_CONT;
            end
        end
        else if(CPU_MODE == `CPU_STEP)begin
            if(CPU_BREAK_POINT)begin
                CPU_MODE <= `CPU_PAUSE;
            end
            else if(cont)begin
                CPU_MODE <= `CPU_CONT;
            end
            else begin
                CPU_MODE <= `CPU_PAUSE;
            end
        end
        else if(CPU_MODE == `CPU_CONT)begin
            if(CPU_BREAK_POINT)begin
                CPU_MODE <= `CPU_PAUSE;
            end
        end
    end  

    always@(posedge clk or negedge rstn)begin
        if(!rstn)begin
            CPU_BRK_ADDR <= 32'hFFFFFFFF;
        end
        else begin
            if(CPU_MODE == `CPU_PAUSE) begin
                if(ent) begin
                    CPU_BRK_ADDR <= io_data;
                end
            end
        end
    end  

    assign clk_cpu = (!rstn) ? 0 :
                     (CPU_MODE == `CPU_PAUSE) ? 0 :
                     (CPU_MODE == `CPU_STEP) ? 1 :
                     (CPU_MODE == `CPU_CONT) ? 1 :
                     0;

    always@(posedge clk or negedge rstn)begin
        if(!rstn)begin

            chk_pc <= 0;
            chk_rf <= 0;
            chk_dm <= 0;
        end
        else if(CPU_MODE == `CPU_PAUSE)begin
            if(chk)begin
                chk_pc  <=  (chk_addr[15:12] == `CHK_PC);
                chk_rf  <=  (chk_addr[15:12] == `CHK_RF); 
                chk_dm  <=  (chk_addr[15:12] == `CHK_DM);
            end 
        end
    end

    assign    io_to_led           =  ( io_addr == `IO_LED );
    assign    io_from_sw          =  ( io_addr == `IO_SW ); 
    assign    io_to_pol           =  ( io_addr == `IO_POL_OUT );
    assign    io_from_pol_in_vld  =  ( io_addr == `IO_POL_IN_VLD );
    assign    io_from_pol         =  ( io_addr == `IO_POL_IN );
    assign    io_from_pol_out_vld =  ( io_addr == `IO_POL_OUT_VLD );


    always@(posedge clk or negedge rstn)begin
        if(!rstn)begin
            pol_din_vld  <= 0;
        end
        else begin
            if(ent&(!pol_din_vld))begin
                pol_din_vld <= 1;
            end
            else if(io_rd)begin
                pol_din_vld <= 0;
            end
        end
    end   

    always@(posedge clk or negedge rstn)begin
        if(!rstn)begin
            pol_dout_vld  <= 0;
        end
        else begin
            if(io_we)begin
                pol_dout_vld <= 1;
            end
            else if(KeyEvent)begin
                pol_dout_vld <= 0;
            end
        end
    end  

    always@(posedge clk or negedge rstn)begin
        if(!rstn)begin
            tmp <= 0;
        end
        else begin
            if(ent&(!pol_din_vld))begin
                 pol_din <= tmp;
                 tmp     <= 0;
            end
            else begin
                 tmp     <= io_data;
            end
           
        end
    end

    assign    seg_data = (chk_pc ) ? pc :
                         (chk_rf ) ? rf_data :
                         (chk_dm ) ? dm_data :
                         (pol_dout_vld & io_to_pol) ?  io_dout :
                         io_data;                  

    DynDis DynDis(
       .clk(clk),
       .rstn(rstn),
       .in(seg_data),
       .seg(seg),
       .an(an)
    );

    assign      led    =  (CPU_MODE == `CPU_PAUSE) ? io_data[15:0] :
                         (io_to_led) ? io_dout[15:0] :
                         0;

    assign      io_din = io_from_sw  ? hd :
                         io_from_pol ? pol_din:
                         io_from_pol_in_vld ?  pol_din_vld :
                         io_from_pol_out_vld ? pol_dout_vld :
                         0;





endmodule
