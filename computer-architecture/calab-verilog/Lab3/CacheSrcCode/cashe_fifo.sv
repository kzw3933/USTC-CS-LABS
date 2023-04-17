// n路组相联,FIFO

module cache_fifo #(
    parameter LINE_ADDR_LEN = 3,    // line内地�??长度
    parameter SET_ADDR_LEN  = 3,    // 组地�??长度，决定组�??
    parameter TAG_ADDR_LEN  = 6,    // tag长度
    parameter WAY_CNT       = 3     // 组相联度
)(
    input               clk,rst,
    output              miss,
    input   [31:0]      addr,
    input               rd_req,
    output  reg [31:0]  rd_data,
    input               wr_req,
    input   [31:0]      wr_data
);

localparam  MEM_ADDR_LEN    = TAG_ADDR_LEN + SET_ADDR_LEN ;  //主存地址长度,数据块数
localparam  UNUSED_ADDR_LEN = 32 - TAG_ADDR_LEN - SET_ADDR_LEN - LINE_ADDR_LEN -2;

localparam  LINE_SIZE       = 1 << LINE_ADDR_LEN;
localparam  SET_SIZE        = 1 << SET_ADDR_LEN;

reg     [31:0]               cache_mem   [SET_SIZE][WAY_CNT][LINE_SIZE];
reg     [TAG_ADDR_LEN-1:0]   cache_tags  [SET_SIZE][WAY_CNT];
reg                          valid       [SET_SIZE][WAY_CNT];
reg                          dirty       [SET_SIZE][WAY_CNT];

wire    [2-1:0]               word_addr;
wire    [LINE_ADDR_LEN-1:0]   line_addr;
wire    [SET_ADDR_LEN-1:0]    set_addr;
wire    [TAG_ADDR_LEN-1:0]    tag_addr;
wire    [UNUSED_ADDR_LEN-1:0] unused_addr;

enum    {IDLE,SWAP_OUT,SWAP_IN,SWAP_IN_OK} cache_stat;

reg     [SET_ADDR_LEN-1:0]    mem_rd_set_addr = 0;
reg     [TAG_ADDR_LEN-1:0]    mem_rd_tag_addr = 0;
wire    [MEM_ADDR_LEN-1:0]    mem_rd_addr     = {mem_rd_tag_addr,mem_rd_set_addr};
reg     [MEM_ADDR_LEN-1:0]    mem_wr_addr     = 0;

reg     [31:0]                mem_wr_line[LINE_SIZE];
wire    [31:0]                mem_rd_line[LINE_SIZE];
 
wire    [WAY_CNT-1:0]         cache_hits;
wire                          cache_hit;

int                           next_way;
int                           fifo_way;
int                           way_hit;

reg     [31:0]                way_hit_reg = 0;

wire                          mem_gnt;        // 主存响应读写的握手信�??

wire  mem_rd_req = (cache_stat == SWAP_IN);
wire  mem_wr_req = (cache_stat == SWAP_OUT);
wire  [MEM_ADDR_LEN-1:0] mem_addr ;


reg     [31:0]                use_fifo[SET_SIZE];

assign {unused_addr, tag_addr, set_addr, line_addr, word_addr} = addr;  // 拆分 32bit ADDR

assign miss = (rd_req | wr_req) & ~(cache_hit && cache_stat == IDLE);

assign cache_hit = |cache_hits;

assign way_hit  = cache_hit ? $clog2 (cache_hits) : (next_way == WAY_CNT)?fifo_way : next_way;

assign mem_addr = mem_rd_req ? mem_rd_addr :(mem_wr_req ? mem_wr_addr:0);

assign fifo_way = use_fifo[set_addr];


// cache  hit判定
genvar k;
generate 
    for(k=0;k<WAY_CNT;k++)begin
       assign  cache_hits[k] = (valid[set_addr][k]) & (cache_tags[set_addr][k] == tag_addr);
    end
endgenerate


// next_way
always@(posedge clk)begin
    next_way = WAY_CNT;
    for(integer i=0;i<SET_SIZE;i++)begin
        if(!valid[set_addr][i] && (i<next_way))
            next_way = i;
    end
end

// cache state transfer
always@(posedge clk or posedge rst)begin
    if(rst) begin
        cache_stat <= IDLE;
        for(integer i=0;i<SET_SIZE;i++)begin            
            for(integer j=0;j<WAY_CNT;j++)begin
                dirty[i][j] = 1'b0;
                valid[i][j] = 1'b0;
            end
            use_fifo[i] <= 0;
        end
        for(integer k=0;k<LINE_SIZE;k++)begin
            mem_wr_line[k] <= 0;
        mem_wr_addr <= 0;
        {mem_rd_tag_addr,mem_rd_set_addr} <= 0;
        rd_data <= 0;
        end
    end
    else begin
        case(cache_stat)
        IDLE: begin
            way_hit_reg = way_hit;
            if(cache_hit)begin
                if(rd_req)begin
                    rd_data <= cache_mem[set_addr][way_hit_reg][line_addr];
                end
                else if(wr_req)begin
                    cache_mem[set_addr][way_hit_reg][line_addr] <= wr_data;
                    dirty[set_addr][way_hit_reg] <= 1'b1;
                end
            end
            else begin
                if(wr_req | rd_req)begin
                    if(valid[set_addr][way_hit_reg] & dirty[set_addr][way_hit_reg])begin
                        cache_stat  <= SWAP_OUT;
                        mem_wr_addr <= {cache_tags[set_addr][way_hit_reg],set_addr};
                        mem_wr_line <= cache_mem[set_addr][way_hit_reg];
                    end
                    else begin
                        cache_stat <= SWAP_IN;
                    end
                    {mem_rd_tag_addr,mem_rd_set_addr} <= {tag_addr,set_addr};
                end
            end
        end
        SWAP_OUT:begin
            if(mem_gnt)begin
                cache_stat <= SWAP_IN;
            end
        end
        SWAP_IN:begin
            use_fifo[set_addr] <= (use_fifo[set_addr]+1)%WAY_CNT;
            if(mem_gnt)begin
                cache_stat <= SWAP_IN_OK;
            end
        end
        SWAP_IN_OK:begin
            for(integer i=0;i<LINE_SIZE;i++) 
                cache_mem[mem_rd_set_addr][way_hit_reg][i] <= mem_rd_line[i];
            cache_tags[mem_rd_set_addr][way_hit_reg] <= mem_rd_tag_addr;
            valid[mem_rd_set_addr][way_hit_reg]     <= 1'b1;
            dirty[mem_rd_set_addr][way_hit_reg]     <= 1'b0;
            cache_stat <= IDLE;           
        end
        endcase
    end
end







main_mem #(
    .LINE_ADDR_LEN (LINE_ADDR_LEN),
    .ADDR_LEN(MEM_ADDR_LEN)
) main_mem_instance (
    .clk(clk),
    .rst(rst),
    .gnt(mem_gnt),
    .addr(mem_addr),
    .rd_req(mem_rd_req),
    .rd_line(mem_rd_line),
    .wr_req(mem_wr_req),
    .wr_line(mem_wr_line)
);

endmodule