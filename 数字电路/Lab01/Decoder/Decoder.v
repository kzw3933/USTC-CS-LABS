module CommonDecoder(              //4-10普通译码器
    input wire [3:0] In,
    output reg [9:0] Out
    );
    always @(*)
    begin
        case(In)
        4'b0000: Out = 10'b0000_0000_01;
        4'b0001: Out = 10'b0000_0000_10;
        4'b0010: Out = 10'b0000_0001_00;
        4'b0011: Out = 10'b0000_0010_00;
        4'b0100: Out = 10'b0000_0100_00;
        4'b0101: Out = 10'b0000_1000_00;
        4'b0110: Out = 10'b0001_0000_00;
        4'b0111: Out = 10'b0010_0000_00;
        4'b1000: Out = 10'b0100_0000_00;
        4'b1001: Out = 10'b1000_0000_00;
        default : Out = 10'b0000_0000_00;            //  无效输入
        endcase
    end       
endmodule



module Decoder7Seg(            //7段译码管
input wire [3:0] In,
output reg [6:0] Out
    );
always @ (*)
    begin
    case(In)
        4'b0000: Out = 7'b0000_001;
        4'b0001: Out = 7'b1001_111;
        4'b0010: Out = 7'b0010_010;
        4'b0011: Out = 7'b0000_110;
        4'b0100: Out = 7'b1001_100;
        4'b0101: Out = 7'b0100_100;
        4'b0110: Out = 7'b0100_000;
        4'b0111: Out = 7'b0001_111;
        4'b1000: Out = 7'b0000_000;
        4'b1001: Out = 7'b0000_100;
        default : Out = 7'b1111_111;
    endcase
    end       
endmodule