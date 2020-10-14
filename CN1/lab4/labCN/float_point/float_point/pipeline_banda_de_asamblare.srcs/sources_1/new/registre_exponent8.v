`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 05/07/2020 08:41:10 PM
// Design Name: 
// Module Name: registre_exponent8
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

module registre_exponent8(
    input clear,
    input load,
    input clk,
    input [7:0] exponent_in,
    output reg [7:0] exponent_out
    );
    
    always @(posedge clk)
    begin
        if (clear)
            exponent_out <= 0;
        if (load)
            exponent_out <= exponent_in;
    end     
endmodule
