module BTB #(
    parameter index_len = 12,
    parameter tag_addr_len = 32-index_len-2
    )(
    input               clk,
    input               rst,
    //read    
    input   [31:0]      r_br_pc,    
    output              br_out,
    output  [31:0]      br_target,
    output  reg         hit,
    // write
    input               br_in,
    input               w_en,
    input   [31:0]      w_br_pc,
    input   [31:0]      w_br_target
);

//取指的地址为4的倍数
parameter  index_size = 1<< index_len;

reg  [tag_addr_len-1:0]   tag_buffer[index_size];
reg  [31:0]               pred_pc_buffer[index_size];
reg                       br_buffer[index_size];



wire [index_len-1:0]        index_addr;
wire [tag_addr_len-1:0]     tag_addr;

wire [index_len-1:0]        w_index_addr;
wire [tag_addr_len-1:0]     w_tag_addr;

assign tag_addr = r_br_pc[31:31-tag_addr_len+1];
assign index_addr = r_br_pc[31-tag_addr_len:2];
assign w_tag_addr = w_br_pc[31:31-tag_addr_len+1];
assign w_index_addr = w_br_pc[31-tag_addr_len:2];

always@(*)begin
    if(br_buffer[index_addr] && tag_buffer[index_addr] == tag_addr)
        hit = 1;
    else 
        hit = 0;
end

always@(posedge clk or posedge rst)begin
    if(rst)begin
        for(integer i=0;i<index_size;i++)begin
            tag_buffer[i] <= 0;
            pred_pc_buffer[i] <= 0;
            br_buffer[i] <= 0;
        end 
    end         
    else begin
        if(w_en)begin
            br_buffer[w_index_addr] <= br_in;
            pred_pc_buffer[w_index_addr] <= w_br_target;
            tag_buffer[w_index_addr] <= w_tag_addr;
        end
    end
end

assign br_out = hit ? br_buffer[index_addr] : 1'b0;

assign br_target = pred_pc_buffer[index_addr];


endmodule