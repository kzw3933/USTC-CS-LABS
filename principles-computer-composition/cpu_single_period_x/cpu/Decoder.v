module decoder_7_128(
    input    [6:0]      in,
    output   [127:0]    out
    );

genvar  i;
generate 
    for (i=0; i<128; i=i+1) begin : gen_for_dec_7_128
        assign out[i] = (in == i);
    end 
endgenerate

endmodule

module decoder_3_8(
    input    [2:0]      in,
    output   [7:0]    out
    );

genvar  i;
generate 
    for (i=0; i<8; i=i+1) begin : gen_for_dec_3_8
        assign out[i] = (in == i);
    end 
endgenerate

endmodule


