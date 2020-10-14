`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 05/07/2020 08:21:02 PM
// Design Name: 
// Module Name: correctExponent
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


module correctExponent(
    input [15:0] exponent_in,
    output reg [7:0] exponent_out
    );
    
    always @(*)
    begin
        exponent_out = exponent_in[15:8];
    end
endmodule
