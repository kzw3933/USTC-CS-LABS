module alu(
  input         clk,
  input         reset,
  input  [15:0] alu_op,
  input  [31:0] alu_src1,
  input  [31:0] alu_src2,
  output [63:0] alu_result,
//  output [63:0] multi_result,
//  output [63:0] divi_result,
  output        m_axis_dout_tvalid,
  output        div_block,
  input         overflow_en,
  output        overflow
);

wire op_add;   //�ӷ�����
wire op_sub;   //��������
wire op_slt;   //�з��űȽϣ�С����λ
wire op_sltu;  //�޷��űȽϣ�С����λ
wire op_and;   //��λ��
wire op_nor;   //��λ���?
wire op_or;    //��λ��
wire op_xor;   //��λ���?
wire op_sll;   //�߼�����
wire op_srl;   //�߼�����
wire op_sra;   //��������
wire op_lui;   //���������ڸ߰벿��
wire op_mul;
wire op_mulu;
wire op_div;
wire op_divu;

reg [31:0] dividend;
reg [31:0] divisor;

// control code decomposition
assign op_add  = alu_op[ 0];
assign op_sub  = alu_op[ 1];
assign op_slt  = alu_op[ 2];
assign op_sltu = alu_op[ 3];
assign op_and  = alu_op[ 4];
assign op_nor  = alu_op[ 5];
assign op_or   = alu_op[ 6];
assign op_xor  = alu_op[ 7];
assign op_sll  = alu_op[ 8];
assign op_srl  = alu_op[ 9];
assign op_sra  = alu_op[10];
assign op_lui  = alu_op[11];
assign op_mul  = alu_op[12];
assign op_mulu = alu_op[13];
assign op_div  = alu_op[14];
assign op_divu = alu_op[15];


wire [31:0] add_sub_result; 
wire [31:0] slt_result; 
wire [31:0] sltu_result;
wire [31:0] and_result;
wire [31:0] nor_result;
wire [31:0] or_result;
wire [31:0] xor_result;
wire [31:0] lui_result;
wire [31:0] sll_result; 
wire [63:0] sr64_result; 
wire [31:0] sr_result; 
wire [31:0] srl_result;
wire [31:0] sra_result;
wire [63:0] mul_result;
wire [63:0] mulu_result;
wire [63:0] div_result;
wire [63:0] divu_result;

// 32-bit adder
wire [31:0] adder_a;
wire [31:0] adder_b;
wire        adder_cin;
wire [31:0] adder_result;
wire        adder_cout;

assign adder_a   = alu_src1;
assign adder_b   = (op_sub | op_slt | op_sltu) ? ~alu_src2 : alu_src2;
assign adder_cin = (op_sub | op_slt | op_sltu) ? 1'b1      : 1'b0;
assign {adder_cout, adder_result} = adder_a + adder_b + adder_cin;
assign overflow = overflow_en & ((adder_a[31] & adder_b[31] & !adder_result[31]) | (!adder_a[31] & !adder_b[31] & adder_result[31]));

// ADD, SUB result
assign add_sub_result = adder_result;

// SLT result
assign slt_result[31:1] = 31'b0;
assign slt_result[0]    = (alu_src1[31] & ~alu_src2[31])
                        | ((alu_src1[31] ~^ alu_src2[31]) & adder_result[31]);

// SLTU result
assign sltu_result[31:1] = 31'b0;
assign sltu_result[0]    = ~adder_cout;

// bitwise operation
assign and_result = alu_src1 & alu_src2;
assign or_result  = alu_src1 | alu_src2;
assign nor_result = ~or_result;
assign xor_result = alu_src1 ^ alu_src2;
assign lui_result = {alu_src2[15:0], 16'b0};

// SLL result 


// SRL, SRA result
assign sr64_result = {{32{op_sra & alu_src2[31]}}, alu_src2[31:0]} >> alu_src1[4:0];

assign sr_result   = sr64_result[30:0];
// assign srl_result = alu_src2[31:0] >> alu_src1[4:0];
// assign sra_result = $signed(alu_src2[31:0]) >>> alu_src1[4:0];

wire [31:0] shift_src;
wire [31:0] shift_result;
wire [31:0] mask;
assign shift_src = op_sll ? {
  alu_src2[0] ,alu_src2[1] ,alu_src2[2] ,alu_src2[3] ,alu_src2[4] ,alu_src2[5] ,alu_src2[6] ,alu_src2[7] ,
  alu_src2[8] ,alu_src2[9] ,alu_src2[10] ,alu_src2[11] ,alu_src2[12] ,alu_src2[13] ,alu_src2[14] ,alu_src2[15] ,
  alu_src2[16] ,alu_src2[17] ,alu_src2[18] ,alu_src2[19] ,alu_src2[20] ,alu_src2[21] ,alu_src2[22] ,alu_src2[23] ,
  alu_src2[24] ,alu_src2[25] ,alu_src2[26] ,alu_src2[27] ,alu_src2[28] ,alu_src2[29] ,alu_src2[30] ,alu_src2[31]
} : alu_src2[31:0];
assign shift_result = shift_src >> alu_src1[4:0];
assign srl_result = shift_result;
assign sll_result = {
  shift_result[0] ,shift_result[1] ,shift_result[2] ,shift_result[3] ,shift_result[4] ,shift_result[5] ,shift_result[6] ,shift_result[7] ,
  shift_result[8] ,shift_result[9] ,shift_result[10] ,shift_result[11] ,shift_result[12] ,shift_result[13] ,shift_result[14] ,shift_result[15] ,
  shift_result[16] ,shift_result[17] ,shift_result[18] ,shift_result[19] ,shift_result[20] ,shift_result[21] ,shift_result[22] ,shift_result[23] ,
  shift_result[24] ,shift_result[25] ,shift_result[26] ,shift_result[27] ,shift_result[28] ,shift_result[29] ,shift_result[30] ,shift_result[31]
};
assign mask = ~(32'hffffffff >> alu_src1[4:0]);
assign sra_result = (mask & {32{alu_src2[31]}}) | shift_result;

//MUL result
assign mulu_result = alu_src1 * alu_src2;
assign mul_result = $signed(alu_src1) * $signed(alu_src2);


//DIV result
wire [31:0] s_axis_divisor_tdata;
wire s_axis_divisor_tready;
wire us_axis_divisor_tready;
wire us_axis_divisor_tvalid;
wire ss_axis_divisor_tready;
wire ss_axis_divisor_tvalid;

wire [31:0] s_axis_dividend_tdata;
wire s_axis_dividend_tready;
wire us_axis_dividend_tready;
wire us_axis_dividend_tvalid;
wire ss_axis_dividend_tready;
wire ss_axis_dividend_tvalid;

wire [63:0] um_axis_dout_tdata;
wire [63:0] sm_axis_dout_tdata;
wire um_axis_dout_tvalid;
wire sm_axis_dout_tvalid;

reg [1:0] state;
reg [1:0] next_state;
parameter still = 0;
parameter send = 1;
parameter compute = 2;
parameter s3 = 3;

assign s_axis_divisor_tready = (us_axis_divisor_tready & op_divu) | (ss_axis_divisor_tready & op_div);
assign s_axis_dividend_tready = (us_axis_dividend_tready & op_divu) | (ss_axis_dividend_tready & op_div);
assign m_axis_dout_tvalid = (um_axis_dout_tvalid & op_divu) | (sm_axis_dout_tvalid & op_div);

assign us_axis_dividend_tvalid = op_divu & (state == still | state == send);
assign us_axis_divisor_tvalid = op_divu & (state == still | state == send);
assign ss_axis_dividend_tvalid = op_div & (state == still | state == send);
assign ss_axis_divisor_tvalid = op_div & (state == still | state == send);

assign s_axis_dividend_tdata = (state == still) ? alu_src1 : dividend;
assign s_axis_divisor_tdata = (state == still) ? alu_src2 : divisor;


assign div_block = (next_state != still);

always@(posedge clk)
begin
  if(reset)
    state <= still;
  else
    state <= next_state;
end

always@(posedge clk)
begin
    if(reset)
        begin
            dividend = 32'b0;
            divisor = 32'b0;
        end
    else if(state == still & next_state == send)
        begin
            dividend = alu_src1;
            divisor = alu_src2;
        end
end

always@*
begin
  case(state)
  still:
    begin
      if(op_div | op_divu) next_state = send;
      else                 next_state = still;
    end
  
  send:
    begin
      if(s_axis_divisor_tready & s_axis_divisor_tready) next_state = compute;
      else next_state = send;
    end
  
  compute:
    begin
      if(m_axis_dout_tvalid) next_state = still;
      else next_state = compute;
    end
  
  default: next_state = still;
  endcase
end

divider_signed divider_signed(
  .aclk                    (clk),
  .s_axis_divisor_tdata    (s_axis_divisor_tdata),
  .s_axis_divisor_tready   (ss_axis_divisor_tready),
  .s_axis_divisor_tvalid   (ss_axis_divisor_tvalid),

  .s_axis_dividend_tdata    (s_axis_dividend_tdata),
  .s_axis_dividend_tready   (ss_axis_dividend_tready),
  .s_axis_dividend_tvalid   (ss_axis_dividend_tvalid),

  .m_axis_dout_tdata    (sm_axis_dout_tdata),
  .m_axis_dout_tvalid   (sm_axis_dout_tvalid)
);

divider_unsigned divider_unsigned(
  .aclk                    (clk),
  .s_axis_divisor_tdata    (s_axis_divisor_tdata),
  .s_axis_divisor_tready   (us_axis_divisor_tready),
  .s_axis_divisor_tvalid   (us_axis_divisor_tvalid),

  .s_axis_dividend_tdata    (s_axis_dividend_tdata),
  .s_axis_dividend_tready   (us_axis_dividend_tready),
  .s_axis_dividend_tvalid   (us_axis_dividend_tvalid),

  .m_axis_dout_tdata    (um_axis_dout_tdata),
  .m_axis_dout_tvalid   (um_axis_dout_tvalid)
);

// final result mux
assign alu_result = ({64{op_add|op_sub}} & {32'b0,add_sub_result})
                  | ({64{op_slt       }} & {32'b0,slt_result})
                  | ({64{op_sltu      }} & {32'b0,sltu_result})
                  | ({64{op_and       }} & {32'b0,and_result})
                  | ({64{op_nor       }} & {32'b0,nor_result})
                  | ({64{op_or        }} & {32'b0,or_result})
                  | ({64{op_xor       }} & {32'b0,xor_result})
                  | ({64{op_lui       }} & {32'b0,lui_result})
                  | ({64{op_sll       }} & {32'b0,sll_result})
                  | ({64{op_srl       }} & {32'b0,srl_result})
                  | ({64{op_sra       }} & {32'b0,sra_result})
                  | ({64{op_mul       }} & mul_result)
                  | ({64{op_mulu      }} & mulu_result)
                  | ({64{op_div       }} & sm_axis_dout_tdata)
                  | ({64{op_divu      }} & um_axis_dout_tdata);

endmodule
