module Decoder7Seg(            //7段译码管
                  input wire [3:0] In,
                  output reg [6:0] Out
                   );
always @ (*)
    begin
        case(In)
        4'b0000: Out = 7'b000_0001;
        4'b0001: Out = 7'b100_1111;
        4'b0010: Out = 7'b001_0010;
        4'b0011: Out = 7'b000_0110;
        4'b0100: Out = 7'b100_1100;
        4'b0101: Out = 7'b010_0100;
        4'b0110: Out = 7'b010_0000;
        4'b0111: Out = 7'b000_1111;
        4'b1000: Out = 7'b000_0000;
        4'b1001: Out = 7'b000_0100;
        4'b1010: Out = 7'b000_1000;  //A
        4'b1011: Out = 7'b110_0000;  //b
        4'b1100: Out = 7'b011_0001;  //C
        4'b1101: Out = 7'b100_0010;  //d
        4'b1110: Out = 7'b011_0000;  //E
        4'b1111: Out = 7'b011_1000;  //F
        endcase
    end       
endmodule