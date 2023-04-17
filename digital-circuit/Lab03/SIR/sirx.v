`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2021/11/04 16:58:16
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


module sir_binary(//二进制编码状态
    input x,rstn,clk,
    output reg y,
    output reg [7:0] rx,ry
    );
    reg [3:0] state,next_state;
    reg [7:0] rtx,rty;
    parameter s0 =4'd0 ;
    parameter s1 =4'd1 ;
    parameter s2 =4'd2 ;
    parameter s3 =4'd3 ;
    parameter s4 =4'd4 ;
    parameter s5 =4'd5 ;
    parameter s6 =4'd6 ;
    parameter s7 =4'd7 ;
    parameter s8 =4'd8 ;
    parameter s9 =4'd9 ;
    parameter s10 =4'd10 ;
    parameter s11 =4'd11 ;
    /*
        编码：
        s0：上次输出y为0，且上次输入x为0
        s1：上次输出y为0，且已输入1个1
        s2：上次输出y为0，且已输入2个1
        s3：上次输出y为0，且已输入3个1
        s4：上次输出y为0，且已输入4个1
        s5：上次输出y为0，且已输入5个1
        s6：上次输出y为1，且上次输入x为1
        s7：上次输出y为1，且已输入1个0
        s8：上次输出y为1，且已输入2个0
        s9：上次输出y为1，且已输入3个0
        s10：上次输出y为1，且已输入4个0
        s11：上次输出y为1，且已输入5个0
    */

    always@(posedge clk)//状态寄存器
    begin
        if(rstn==1)
        begin
            state[3:0]<=s0;
        end
        else  
        begin
            state[3:0]<=next_state[3:0];
        end
    end

    always@(state[3:0])//组合逻辑电路
        begin
            case(state[3:0])
            s0:begin
                if(x==1) next_state=s1;
                else  next_state=s0; 
                y=0;
                end
            s1:begin
                if(x==1) next_state=s2;
                else next_state=s0;
                y=0;
                end  
            s2:begin
                if(x==1)next_state=s3;
                else next_state=s0;
                y=0;  
                end
            s3:begin
                if(x==1)next_state=s4;
                else next_state=s0;
                y=0;   
                end
            s4:begin
                if(x==1)next_state=s5;
                else next_state=s0;
                y=0;   
                end
            s5:begin
                if(x==1)
                    begin
                        next_state=s6;
                        y=1; 
                    end
                else 
                    begin
                        next_state=s0;
                        y=0;
                    end
                end
            s6:begin
                    if(x==0)next_state=s7;
                    else next_state=s6;
                    y=1; 
                end
            s7:begin
                    if(x==0)next_state[3:0]=s8;
                    else next_state[3:0]=s6;
                    y=1;    
                end
            s8:begin
                    if(x==0)next_state[3:0]=s9;
                    else next_state[3:0]=s6;
                y=1;  
                end
            s9:begin
                    if(x==0)next_state[3:0]=s10;
                    else next_state[3:0]=s6;
                y=1;  
                end
            s10:begin
                    if(x==0)next_state[3:0]=s11;
                    else next_state[3:0]=s6;
                y=1;   
                end
            s11:begin
                if(x==0)
                    begin
                        next_state[3:0]=s0;
                        y=0;
                    end
                else 
                    begin
                        next_state[3:0]=s6;
                        y=1;
                    end
                end
        endcase
        end        
        
        always@(posedge clk)//数据通路
        begin
            if(rstn)
                begin
                rx[7:0]=0;
                ry[7:0]=0;
                end
            else
                begin
                    rx=rx<<1;
                    ry=ry<<1;
                    rx[0]<=x;
                    ry[0]<=y;
                end
        end
        
endmodule




module sir_onehot(//独热码编码状态
    input x,rstn,clk,
    output reg y,
    output reg [7:0] rx,ry
    );
    reg [11:0] state,next_state;
    parameter s0 =12'b0000_0000_0001;
    parameter s1 =12'b0000_0000_0010;
    parameter s2 =12'b0000_0000_0100;
    parameter s3 =12'b0000_0000_1000;
    parameter s4 =12'b0000_0001_0000;
    parameter s5 =12'b0000_0010_0000;
    parameter s6 =12'b0000_0100_0000;
    parameter s7 =12'b0000_1000_0000;
    parameter s8 =12'b0001_0000_0000;
    parameter s9 =12'b0010_0000_0000;
    parameter s10 =12'b0100_0000_0000;
    parameter s11 =12'b1000_0000_0000;
    /*
        编码：
        s0：上次输出y为0，且上次输入x为0
        s1：上次输出y为0，且已输入1个1
        s2：上次输出y为0，且已输入2个1
        s3：上次输出y为0，且已输入3个1
        s4：上次输出y为0，且已输入4个1
        s5：上次输出y为0，且已输入5个1
        s6：上次输出y为1，且上次输入x为1
        s7：上次输出y为1，且已输入1个0
        s8：上次输出y为1，且已输入2个0
        s9：上次输出y为1，且已输入3个0
        s10：上次输出y为1，且已输入4个0
        s11：上次输出y为1，且已输入5个0
    */

    always@(posedge clk)
    begin
        if(rstn==1)
        begin
            state[11:0]<=s0;
        end
        else  
        begin
            state[11:0]<=next_state[11:0];
        end
    end

    always@(state)
        begin
            
            case(state[11:0])
            s0:begin
                if(x==1) next_state=s1;
                else  next_state=s0; 
                y=0;
                end
            s1:begin
                if(x==1) next_state=s2;
                else next_state=s0;
                y=0;
                end  
            s2:begin
                if(x==1)next_state=s3;
                else next_state=s0;
                y=0;  
                end
            s3:begin
                if(x==1)next_state=s4;
                else next_state=s0;
                y=0;   
                end
            s4:begin
                if(x==1)next_state=s5;
                else next_state=s0;
                y=0;   
                end
            s5:begin
                if(x==1)
                    begin
                        next_state=s6;
                        y=1; 
                    end
                else 
                    begin
                        next_state=s0;
                        y=0;
                    end
                end
            s6:begin
                    if(x==0)next_state=s7;
                    else next_state=s6;
                    y=1; 
                end
            s7:begin
                    if(x==0)next_state=s8;
                    else next_state=s6;
                    y=1;    
                end
            s8:begin
                    if(x==0)next_state=s9;
                    else next_state=s6;
                y=1;  
                end
            s9:begin
                    if(x==0)next_state=s10;
                    else next_state=s6;
                y=1;  
                end
            s10:begin
                    if(x==0)next_state=s11;
                    else next_state=s6;
                y=1;   
                end
            s11:begin
                if(x==0)
                    begin
                        next_state=s0;
                        y=0;
                    end
                else 
                    begin
                        next_state=s6;
                        y=1;
                    end
                end

        endcase
        end

        always@(posedge clk)//数据通路
        begin
            if(rstn)
                begin
                rx[7:0]=0;
                ry[7:0]=0;
                end
            else
                begin
                    rx=rx<<1;
                    ry=ry<<1;
                    rx[0]<=x;
                    ry[0]<=y;
                end
        end
  
endmodule