module encoder_16_4(
            input       [15:0]  in,
            output      [3:0]   out
            );
assign  out = in[15]?4'hf:
              in[14]?4'he:
              in[13]?4'hd:
              in[12]?4'hc:
              in[11]?4'hb:
              in[10]?4'ha:
              in[9] ?4'h9:
              in[8] ?4'h8:
              in[7] ?4'h7:
              in[6] ?4'h6:
              in[5] ?4'h5:
              in[4] ?4'h4:
              in[3] ?4'h3:
              in[2] ?4'h2:  
              in[1] ?4'h1:
              in[0] ?4'h0:
              4'h0;
endmodule