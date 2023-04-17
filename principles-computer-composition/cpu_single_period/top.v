module top(
       input                clk,
       input                rstn,
       input                step,
       input                cont,
       input                chk,
       input                ent,
       input                del,
       input        [15:0]  hd, 
       output               pause,
       output       [15:0]  led,
       output       [7:0]   an,
       output       [6:0]   seg,
       output       [2:0]   seg_sel
);

wire                clk_cpu;
wire     [7:0]      io_addr;
wire     [31:0]     io_dout;
wire     [31:0]     io_din;
wire                io_we;
wire                io_rd;

wire     [7:0]      dm_rf_addr;
wire     [31:0]     rf_data;
wire     [31:0]     dm_data;
wire     [31:0]     pc;


wire                step_d,step_s,step_edge;
wire                cont_d,cont_s,cont_edge;
wire                chk_d,chk_s,chk_edge;
wire                ent_d,ent_s,ent_edge;
wire                del_d,del_s,del_edge;
wire    [15:0]      hd_d,hd_s,hd_edge;

reg                 step_r;
reg                 cont_r;
reg                 chk_r;
reg                 ent_r;
reg                 del_r;
reg     [15:0]      hd_r;
always@(posedge clk or negedge rstn)begin
    if(!rstn)begin
        step_r <= 0;
        cont_r <= 0;
        chk_r  <= 0;
        ent_r  <= 0;
        del_r  <= 0;
        hd_r   <= 0;
    end
    else begin
        step_r <= step_s;
        cont_r <= cont_s;
        chk_r  <= chk_s;
        ent_r  <= ent_s;
        del_r  <= del_s;
        hd_r   <= hd_s;

    end
end

assign      step_edge = !step_r & step_s;
assign      cont_edge = !cont_r & cont_s;
assign      chk_edge  = !chk_r & chk_s;
assign      ent_edge  = !ent_r & ent_s;
assign      del_edge  = !del_r & del_s;

assign      hd_edge[0] = !hd_r[0] & hd_s[0];
assign      hd_edge[1] = !hd_r[1] & hd_s[1];
assign      hd_edge[2] = !hd_r[2] & hd_s[2];
assign      hd_edge[3] = !hd_r[3] & hd_s[3];
assign      hd_edge[4] = !hd_r[4] & hd_s[4];
assign      hd_edge[5] = !hd_r[5] & hd_s[5];
assign      hd_edge[6] = !hd_r[6] & hd_s[6];
assign      hd_edge[7] = !hd_r[7] & hd_s[7];
assign      hd_edge[8] = !hd_r[8] & hd_s[8];
assign      hd_edge[9] = !hd_r[9] & hd_s[9];
assign      hd_edge[10] = !hd_r[10] & hd_s[10];
assign      hd_edge[11] = !hd_r[11] & hd_s[11];
assign      hd_edge[12] = !hd_r[12] & hd_s[12];
assign      hd_edge[13] = !hd_r[13] & hd_s[13];
assign      hd_edge[14] = !hd_r[14] & hd_s[14];
assign      hd_edge[15] = !hd_r[15] & hd_s[15];



                  

 Debounce        Debounce_step(.clk(clk),.rstn(rstn),.in(step),.out(step_d));
 Synchron        Synchron_step(.clk(clk),.rstn(rstn),.in(step_d),.out(step_s));

 Debounce        Debounce_cont(.clk(clk),.rstn(rstn),.in(cont),.out(cont_d));
 Synchron        Synchron_cont(.clk(clk),.rstn(rstn),.in(cont_d),.out(cont_s));

 Debounce        Debounce_chk(.clk(clk),.rstn(rstn),.in(chk),.out(chk_d));
 Synchron        Synchron_chk(.clk(clk),.rstn(rstn),.in(chk_d),.out(chk_s));

 Debounce        Debounce_ent(.clk(clk),.rstn(rstn),.in(ent),.out(ent_d));
 Synchron        Synchron_ent(.clk(clk),.rstn(rstn),.in(ent_d),.out(ent_s));

 Debounce        Debounce_del(.clk(clk),.rstn(rstn),.in(del),.out(del_d));
 Synchron        Synchron_del(.clk(clk),.rstn(rstn),.in(del_d),.out(del_s));

 Debounce        Debounce_hd0(.clk(clk),.rstn(rstn),.in(hd[0]),.out(hd_d[0]));
 Synchron        Synchron_hd0(.clk(clk),.rstn(rstn),.in(hd[0]),.out(hd_s[0]));
 Debounce        Debounce_hd1(.clk(clk),.rstn(rstn),.in(hd[1]),.out(hd_d[1]));
 Synchron        Synchron_hd1(.clk(clk),.rstn(rstn),.in(hd[1]),.out(hd_s[1]));
 Debounce        Debounce_hd2(.clk(clk),.rstn(rstn),.in(hd[2]),.out(hd_d[2]));
 Synchron        Synchron_hd2(.clk(clk),.rstn(rstn),.in(hd[2]),.out(hd_s[2]));
 Debounce        Debounce_hd3(.clk(clk),.rstn(rstn),.in(hd[3]),.out(hd_d[3]));
 Synchron        Synchron_hd3(.clk(clk),.rstn(rstn),.in(hd[3]),.out(hd_s[3]));
 Debounce        Debounce_hd4(.clk(clk),.rstn(rstn),.in(hd[4]),.out(hd_d[4]));
 Synchron        Synchron_hd4(.clk(clk),.rstn(rstn),.in(hd[4]),.out(hd_s[4]));
 Debounce        Debounce_hd5(.clk(clk),.rstn(rstn),.in(hd[5]),.out(hd_d[5]));
 Synchron        Synchron_hd5(.clk(clk),.rstn(rstn),.in(hd[5]),.out(hd_s[5]));
 Debounce        Debounce_hd6(.clk(clk),.rstn(rstn),.in(hd[6]),.out(hd_d[6]));
 Synchron        Synchron_hd6(.clk(clk),.rstn(rstn),.in(hd[6]),.out(hd_s[6]));
 Debounce        Debounce_hd7(.clk(clk),.rstn(rstn),.in(hd[7]),.out(hd_d[7]));
 Synchron        Synchron_hd7(.clk(clk),.rstn(rstn),.in(hd[7]),.out(hd_s[7]));
 Debounce        Debounce_hd8(.clk(clk),.rstn(rstn),.in(hd[8]),.out(hd_d[8]));
 Synchron        Synchron_hd8(.clk(clk),.rstn(rstn),.in(hd[8]),.out(hd_s[8]));
 Debounce        Debounce_hd9(.clk(clk),.rstn(rstn),.in(hd[9]),.out(hd_d[9]));
 Synchron        Synchron_hd9(.clk(clk),.rstn(rstn),.in(hd[9]),.out(hd_s[9]));
 Debounce        Debounce_hd10(.clk(clk),.rstn(rstn),.in(hd[10]),.out(hd_d[10]));
 Synchron        Synchron_hd10(.clk(clk),.rstn(rstn),.in(hd[10]),.out(hd_s[10]));
 Debounce        Debounce_hd11(.clk(clk),.rstn(rstn),.in(hd[11]),.out(hd_d[11]));
 Synchron        Synchron_hd11(.clk(clk),.rstn(rstn),.in(hd[11]),.out(hd_s[11]));
 Debounce        Debounce_hd12(.clk(clk),.rstn(rstn),.in(hd[12]),.out(hd_d[12]));
 Synchron        Synchron_hd12(.clk(clk),.rstn(rstn),.in(hd[12]),.out(hd_s[12]));
 Debounce        Debounce_hd13(.clk(clk),.rstn(rstn),.in(hd[13]),.out(hd_d[13]));
 Synchron        Synchron_hd13(.clk(clk),.rstn(rstn),.in(hd[13]),.out(hd_s[13]));
 Debounce        Debounce_hd14(.clk(clk),.rstn(rstn),.in(hd[14]),.out(hd_d[14]));
 Synchron        Synchron_hd14(.clk(clk),.rstn(rstn),.in(hd[14]),.out(hd_s[14]));
 Debounce        Debounce_hd15(.clk(clk),.rstn(rstn),.in(hd[15]),.out(hd_d[15]));
 Synchron        Synchron_hd15(.clk(clk),.rstn(rstn),.in(hd[15]),.out(hd_s[15]));


Pdu Pdu(
    .clk(clk),
    .rstn(rstn),
    .step(step_edge),
    .cont(cont_edge),
    .chk(chk_edge),
    .ent(ent_edge),
    .del(del_edge),
    .hd(hd_edge),
    .clk_cpu(clk_cpu),
    .pause(pause),
    .led(led),
    .an(an),
    .seg(seg),
    .seg_sel(seg_sel),
    .io_addr(io_addr),
    .io_dout(io_dout),
    .io_we(io_we),
    .io_din(io_din),
    .io_rd(io_rd),
    .dm_rf_addr(dm_rf_addr),
    .rf_data(rf_data),
    .dm_data(dm_data),
    .pc(pc)
);
Cpu Cpu(
    .clk(clk_cpu),
    .rstn(rstn),
    .io_addr(io_addr),
    .io_dout(io_dout),
    .io_we(io_we),
    .io_rd(io_rd),
    .io_din(io_din),
    .debug_pc(pc),
    .debug_dm_rf_addr(dm_rf_addr),
    .debug_rf_data(rf_data),
    .debug_dm_data(dm_data)
);





endmodule