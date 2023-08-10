 module UART(
     input              clk,
     input              rstn,
     //rx
     input              rx,
     output  reg        rx_vld,
     output  reg  [7:0] rx_data,
     //tx
     output  reg        tx,
     input              tx_ready,
     output  reg        tx_rd,
     input        [7:0] tx_data  
 );

 UART_RX UART_RX(
     .clk(clk),
     .rstn(rstn),
     .rx(rx),
     .rx_vld(rx_vld),
     .rx_data(rx_data)
 );
 UART_TX UART_TX(
     .clk(clk),
     .rstn(rstn),
     .tx(tx),
     .tx_ready(tx_ready),
     .tx_rd(tx_rd),
     .tx_data(tx_data)
 );


 endmodule



module UART_RX(
    input               clk,
    input               rstn,
    input               rx,
    output  reg         rx_vld,
    output  reg   [7:0] rx_data
);

parameter   C_IDLE = 1'b0,C_RX = 1'b1;
parameter   HDIV_CNT = 443,DIV_CNT = 867,RX_CNT = 8;

reg             CS,NS;
reg    [9:0]    div_cnt;
reg    [3:0]    rx_cnt;
wire            rx_pulse;
reg    [7:0]    rx_reg;
 
always@(posedge clk or negedge rstn)begin
    if(!rstn)
        CS <= C_IDLE;
    else 
        CS <= NS;
end

always@(*)begin
    case(CS)
        C_IDLE:begin
            if(div_cnt == HDIV_CNT)
                NS = C_RX;
            else 
                NS = C_IDLE;
        end
        C_RX:begin
            if((div_cnt == DIV_CNT)&&(rx_cnt >= RX_CNT))
                NS = C_IDLE;
            else 
                NS = C_RX; 
        end
    endcase
end

always@(posedge clk or negedge rstn)begin
    if(!rstn)
        div_cnt <= 10'h0;
    else if(CS == C_IDLE) begin
        if(rx == 1'b1)
            div_cnt <= 10'h0;
        else if(div_cnt < HDIV_CNT)
            div_cnt <= div_cnt + 10'h1;
        else 
            div_cnt <= 10'h0;
    end
    else if(CS == C_RX)begin
        if(div_cnt >= DIV_CNT)
            div_cnt <= 10'h0;
        else 
            div_cnt <= div_cnt + 10'h1;
    end
end

always@(posedge clk or negedge rstn)begin
    if(!rstn)begin
        rx_cnt <= 4'h0;
    end
    else if(CS == C_IDLE)
        rx_cnt <= 4'h0;
    else if((div_cnt == DIV_CNT)&&(rx_cnt<4'hF))
        rx_cnt <= rx_cnt + 1'b1;

end

assign  rx_pulse = (CS==C_RX)&&(div_cnt==DIV_CNT);

always@(posedge clk)begin
    if(rx_pulse)begin
        case(rx_cnt)
            4'h0:rx_reg[0] <= rx;
            4'h1:rx_reg[1] <= rx;
            4'h2:rx_reg[2] <= rx;
            4'h3:rx_reg[3] <= rx;
            4'h4:rx_reg[4] <= rx;
            4'h5:rx_reg[5] <= rx;
            4'h6:rx_reg[6] <= rx;
            4'h7:rx_reg[7] <= rx;
        endcase

    end

end

always@(posedge clk or negedge rstn)begin
    if(!rstn)begin
        rx_vld <= 0;
        rx_data <= 8'h55;
    end
    else if((CS==C_RX)&&(NS==C_IDLE))
    begin
        rx_vld <= 1'b1;
        rx_data <= rx_reg;
    end
    else 
        rx_vld <= 1'b0;
end

endmodule


module UART_TX(
    input           clk,
    input           rstn,
    output   reg    tx,
    input           tx_ready,
    output   reg    tx_rd,
    input           tx_data
);
parameter DIV_CNT = 10'd867;
parameter HDIV_CNT = 10'd433;
parameter TX_CNT = 4'h9;
parameter C_IDLE = 1'b0,C_TX = 1'b1;

reg         CS,NS;

reg     [9:0]   div_cnt;
reg     [4:0]   tx_cnt;
reg     [7:0]   tx_reg;

always@(posedge clk or negedge rstn)begin
    if(!rstn)begin
        CS <= C_IDLE;
    end
    else begin
        CS <= NS;
    end

end

always@(*)begin
    case(CS)
        C_IDLE:begin
            if(tx_ready==1'b1)
                NS = C_TX;
            else 
                NS = C_IDLE;
        end
        C_TX:begin
            if((div_cnt == DIV_CNT)&&(tx_cnt >= TX_CNT))
                NS = C_IDLE;
            else 
                NS = C_TX;
        end

    endcase
end

always@(posedge clk or negedge rstn)begin
    if(!rstn)
        div_cnt <= 10'h0;
    else if(CS == C_TX)begin
        if(div_cnt >= DIV_CNT)
            div_cnt <= 10'h0;
        else
            div_cnt <= div_cnt + 10'h1;
    end
    else 
        div_cnt <= 10'h0;

end

always@(posedge clk or negedge rstn)begin
    if(!rstn)
        tx_cnt <= 4'h0;
    else if(CS == C_TX)begin
        if(div_cnt == DIV_CNT)
            tx_cnt <= tx_cnt + 1'b1;
    end
    else 
        tx_cnt <= 4'h0;

end

always@(posedge clk or negedge rstn)begin
    if(!rstn)
        tx_rd <= 1'b0;
    else if((CS == C_IDLE)&&(tx_ready==1))
        tx_rd <= 1'b1;
    else 
        tx_rd <= 1'b0;
end

always@(posedge clk or negedge rstn)begin
    if(!rstn)
        tx_reg <= 0;
    else if((CS == C_IDLE)&&(tx_ready == 1'b1))
        tx_reg <= tx_data;

end

always@(posedge clk or negedge rstn)begin
    if(!rstn)
        tx <= 1'b1;
    else if(CS == C_IDLE)
        tx <= 1'b1;
    else if(div_cnt == 10'h0) begin
        case(tx_cnt)
            4'h0:   tx <= 1'b0;
            4'h1:   tx <= tx_reg[0];
            4'h2:   tx <= tx_reg[1];
            4'h3:   tx <= tx_reg[2];
            4'h4:   tx <= tx_reg[3];
            4'h5:   tx <= tx_reg[4];
            4'h6:   tx <= tx_reg[5];
            4'h7:   tx <= tx_reg[6];
            4'h8:   tx <= tx_reg[7];
            4'h9:   tx <= 1'b1;
        endcase

    end


end



endmodule