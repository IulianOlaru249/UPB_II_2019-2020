`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 05/07/2020 08:37:51 PM
// Design Name: 
// Module Name: parse
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

module parse(
    input [31:0] A,
    input [31:0] B,
    output reg [15:0] exponent,
    output reg [47:0] mantisa
    );
    
    always @(*)
    begin
        exponent = { A[30:23], B[30:23] };
        mantisa = { A[31], A[22:0], B[31], B[22:0] };
    end
endmodule