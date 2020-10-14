`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 05/07/2020 08:17:52 PM
// Design Name: 
// Module Name: changeExponent
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

module changeExponent(
    input [7:0] exponent_in,
    input [8:0] val1,
    input [8:0] val2,
    output reg [7:0] exponent_out
    );
    always @(*)
    begin
        if (val2[8] == 0)
            exponent_out = exponent_in - val2[7:0];
        else
            exponent_out = exponent_in + val2[7:0];
    end
endmodule
