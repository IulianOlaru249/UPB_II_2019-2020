`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 03/27/2020 09:00:52 PM
// Design Name: 
// Module Name: cafea
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

//S0=000 -initial
//S1=001 -1ron A
//S2=010 -1ron B
//S3=011 -1ron C
//S4=100 -5ron
//S5=101 -10ron
module cafea(
    input clk, reset, I, V, X,
    output B, R1, R5
    );
    
    reg B, R1, R5;
    reg[1:0] state, nextState;
    
    always @ (posedge clk)
        if (reset)
            state <= 0;
        else
            state <= nextState;
        
        always @ (state & I & V & X)
            case({state, I, V, X})
                6'b001000: begin
                            if (state == 3'b000)
                                nextState = 3'b101;B=0;R1=0;R5=0;
                            if (state == 3'b011)
                                nextState = 3'b101;B=0;R1=0;R5=0;
                            if (state == 3'b011)
                                nextState = 3'b101;B=0;R1=0;R5=0;
                           end
                6'b010000: begin
                            if (state == 3'b000)
                                nextState = 3'b100;B=0;R1=0;R5=0;
                            if (state == 3'b001)
                                nextState = 3'b100;B=0;R1=0;R5=0;
                           end
                6'b100000: begin
                            if (state == 3'b000)
                                nextState = 3'b001;B=0;R1=0;R5=0;
                            if (state == 3'b001)
                                nextState = 3'b011;B=0;R1=0;R5=0;
                           end
                6'b00100: begin
                            if (state == 3'b010)
                                nextState = 3'b000;B=1;R1=0;R5=0;
                            if (state == 3'b001)
                                nextState = 3'b000;B=1;R1=0;R5=0;
                           end        
                6'b00001: begin
                                nextState = 3'b100;B=0;R1=0;R5=1;
                           end
                6'b00010: begin
                             if (state == 3'b100)
                                nextState = 3'b001;B=0;R1=1;R5=0;
                             if (state == 3'b011)
                                nextState = 3'b001;B=0;R1=1;R5=0;
                             if (state == 3'b001)
                                nextState = 3'b010;B=0;R1=1;R5=0;
                             //if (state == 3'b010)
                             //   nextState = 3'b001;B=0;R1=1;R5=0;                                
                           end                                                                                      
                default:   begin
                           state=3'b000;B=0;R1=0;R5=0;
                           end
            endcase
endmodule
