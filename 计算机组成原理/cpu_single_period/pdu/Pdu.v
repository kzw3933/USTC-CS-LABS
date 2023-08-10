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
       output       [2:0]   seg_sel,
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

    wire           [31:0]     io_in_data;
    reg            [31:0]     io_out_data;


    wire                     io_to_led;
    wire                     io_from_sw;
    wire                     io_to_pol;
    wire                     io_from_pol_in_vld;
    wire                     io_from_pol;
    wire                     io_from_pol_out_vld;
    reg                      io_to_pol_r;
    reg                      io_to_led_r;
    
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

    IO IO(.clk(clk),.del(del),.hd(hd),.io_din(io_in_data),.rstn(rstn));

    assign                  chk_addr = io_in_data;

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
                    CPU_BRK_ADDR <= io_in_data;
                end
            end
        end
    end  

    assign clk_cpu = (!rstn) ? 0 :
                     (CPU_MODE == `CPU_PAUSE) ? 0 :
                     (CPU_MODE == `CPU_STEP) ? 1 :
                     (CPU_MODE == `CPU_CONT) ? clk :
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

    assign   io_from_sw            =   ( io_addr == `IO_SW )&io_rd;
    assign   io_from_pol_in_vld    =   ( io_addr == `IO_POL_IN_VLD)&io_rd ;
    assign   io_from_pol           =    (io_addr == `IO_POL_IN) & io_rd;
    assign   io_from_pol_out_vld   =    (io_addr == `IO_POL_OUT_VLD)&io_rd;
    assign   io_to_pol             =    (io_addr == `IO_POL_OUT)&io_we ;
    assign   io_to_led             =    (io_addr == `IO_LED)&io_we ;



    always@(posedge clk or negedge rstn)begin
        if(!rstn)begin
           io_to_led_r  <= 0; 
           io_to_pol_r <= 0;
        end    
        else if(CPU_MODE == `CPU_CONT)begin
            if(io_we)begin
                io_to_led_r <= io_to_led;
                io_to_pol_r <= io_to_pol;
            end

        end 
        else begin
                io_to_led_r <= 0;
                io_to_pol_r <= 0;
        end
    end

    
    always@(posedge clk or negedge rstn)begin
        if(!rstn)begin
            pol_din_vld  <= 0;
        end
        else begin
            if(CPU_MODE == `CPU_CONT)begin
                if(io_from_pol&&(!io_from_pol_in_vld))begin
                    pol_din_vld <= 0;
                end
                else if(ent&(!pol_din_vld))begin
                    pol_din_vld <= 1;
                end
            end
            else begin
                pol_din_vld <= 0;
            end
        end

    end   

    always@(posedge clk or negedge rstn)begin
        if(!rstn)begin
            pol_dout_vld  <= 0;
            io_out_data <= 0;
        end
        else begin
            if(CPU_MODE == `CPU_CONT)begin
                if(io_to_pol)begin
                io_out_data <= io_dout;
                pol_dout_vld <= 1;
                end
                else if(KeyEvent)begin
                    pol_dout_vld <= 0;
                end

            end
            else begin
                pol_dout_vld <= 0;
            end
        end
    end  

    always@(posedge clk or negedge rstn)begin
        if(!rstn)begin
            tmp <= 0;
        end
        else begin
            if(CPU_MODE == `CPU_CONT)begin
                if(ent&(!pol_din_vld))begin
                    pol_din <= tmp;
                    tmp     <= 0;
                end
                else begin
                    tmp     <= io_in_data;
                end
            end  
            else begin
                tmp <= 0;
            end 
        end
    end

    // assign  seg_data =   (io_to_pol_r) ?  io_out_data :
    //                      (chk_pc ) ? pc :
    //                      (chk_rf ) ? rf_data :
    //                      (chk_dm ) ? dm_data :   
    //                      io_in_data;     
    assign seg_data = io_in_data;             

    DynDis DynDis(
       .clk(clk),
       .rstn(rstn),
       .in(seg_data),
       .seg(seg),
       .an(an)
    );

    assign      led    =    (io_to_led_r) ? io_out_data[15:0] :
                            io_in_data[15:0] ;
                            

    assign      io_din = io_from_sw  ? {{16{1'b0}},hd} :
                         io_from_pol ? pol_din:
                         io_from_pol_in_vld ?  {{31{1'b0}},pol_din_vld} :
                         io_from_pol_out_vld ? {{31{1'b0}},pol_dout_vld} :
                         0;

// assign         seg_data = tmp ; 


    assign      seg_sel = (io_to_pol_r) ? 3'b001 :
                          (chk_pc | chk_rf | chk_dm)? 3'b100 :
                          3'b010;
//assign      seg_sel = (CPU_MODE == `CPU_PAUSE) ? 3'b100:
//                          ((CPU_MODE == `CPU_CONT)) ? 3'b010 :
//                          ((CPU_MODE == `CPU_STEP)) ? 3'b001 :
//                          3'b000;



endmodule
