`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2021/11/06 14:23:04
// Design Name: 
// Module Name: sir
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module fls(//两段式
    input  clk, rstn, en,
    input  [7:0]  d,
    output reg [7:0]  f, 
    output reg err
    );
    reg [1:0]state,next_state;
    reg [7:0] rf1,rf2;
    reg [7:0] mid;
//    reg error;

    parameter s0 =2'd0;
    parameter s1 =2'd1;
    parameter s2 =2'd2;
    parameter s3 =2'd3;
//    parameter s4 =3'd4;


    always@(posedge clk or negedge rstn)
    begin
        if(!rstn)  begin
          state<=s0;
        end 
        else if(en||state==s0)state<=next_state;
    end

    always@(*)
    begin
    //    if(error)next_state=s4;
    //    else
        begin
            case(state)
            s0:next_state=s1;
            s1:next_state=s2;
            s2:next_state=s3;
            s3:next_state=s3;
            endcase
        end    
    end

  /*  always@(*)
    begin
        case(state)
        s0:control=c0;
        s1:control=c1;
        s2:control=c2;
        s3:control=c3;
        s4:control=c4;
        endcase
    end
*/
    always@(posedge clk)
    begin
        if(en||state==s0)
        begin
            if((rf1[7:0]+rf2[7:0]>=rf2[7:0])&&(rf1[7:0]+rf2[7:0]>=rf1[7:0])||state==s0)
                begin
                    case(state)
                    s0:begin
                    rf1[7:0]=8'b0000_0000;
                    rf2[7:0]=8'b0000_0000;
                    err=0;
                    end
                    s1:begin
                        rf1[7:0]=d[7:0];
                    end
                    s2:begin
                        rf2[7:0]=rf1[7:0];
                        rf1[7:0]=d[7:0];
                    end
                    s3:begin
                        mid=rf2[7:0];
                        rf2[7:0]=rf1[7:0];
                        rf1[7:0]=rf1[7:0]+mid[7:0];
                    end
                    endcase
                    f[7:0]=rf1[7:0];
                    end
                else    err=1;
         end 
    end

endmodule




module fls_3(//三段式
    input  clk, rstn, en,
    input  [7:0]  d,
    output reg [7:0]  f, 
    output reg err
    );
    reg [1:0]state,next_state;
    reg [7:0] rf1,rf2;
    reg [1:0]control;
    reg [7:0] mid;

    parameter s0 =2'd0;
    parameter s1 =2'd1;
    parameter s2 =2'd2;
    parameter s3 =2'd3;

    parameter Reset =2'd0;
    parameter Input1 =2'd1;
    parameter Input2 =2'd2;
    parameter Plus =2'd3;


    always@(posedge clk or negedge rstn)
    begin
        if(!rstn)  begin
          state<=s0;
        end 
        else if(en)state<=next_state;
    end

    always@(*)
    begin

        begin
            case(state)
            s0:next_state=s1;
            s1:next_state=s2;
            s2:next_state=s3;
            s3:next_state=s3;
            endcase
        end    
    end

    always @(posedge clk or negedge rstn)
    begin
        if(!rstn)control = Reset; 
        else if(en||next_state==s1) begin
            case(next_state)
                    s1:control=Input1;
                    s2:control=Input2;
                    s3:control=Plus;
                    endcase
        end
    end

    always@(posedge clk)
    begin
        if(en||control==Reset)
        begin
            if((control==Reset||(control!=Reset&&(rf1[7:0]+rf2[7:0]>=rf2[7:0])&&(rf1[7:0]+rf2[7:0]>=rf1[7:0]))))
                begin
                    case(control)
                    Reset:begin
                        rf1[7:0]=8'b0000_0000;
                        rf2[7:0]=8'b0000_0000;
                        err=0;
                    end
                    Input1:begin
                        rf1[7:0]=d[7:0];
                    end
                    Input2:begin
                        rf2[7:0]=rf1[7:0];
                        rf1[7:0]=d[7:0];
                    end
                    Plus:begin
                        mid=rf2[7:0];
                        rf2[7:0]=rf1[7:0];
                        rf1[7:0]=rf1[7:0]+mid[7:0];
                    end
                    endcase
                    f[7:0]=rf1[7:0];
                    end
                else    err=1;
         end 
    end

endmodule
