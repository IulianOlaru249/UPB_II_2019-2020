`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 03/28/2020 11:30:23 AM
// Design Name: 
// Module Name: cafea_sim
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


module cafea_sim(
    );
    reg clk, reset, I, V, X;
    wire B, R1, R5;
    cafea a(clk, reset, I, V , X, R1, R5, B);
    initial
    begin
        clk = 0;
        I = 0; V = 0; X = 0;
        reset = 1;
        #20 reset = 0;
        #20 I = 1; V = 0; X = 0;
        #20 I = 0; V = 1; X = 0;
        #20 I = 0; V = 0; X = 1;
    end
    initial
    begin
        forever
        #20 clk = !clk;
    end;
endmodule
