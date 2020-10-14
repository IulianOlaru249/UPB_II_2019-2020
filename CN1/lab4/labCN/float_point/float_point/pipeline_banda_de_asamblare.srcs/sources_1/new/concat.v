`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 05/07/2020 08:35:00 PM
// Design Name: 
// Module Name: concat
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

module concat(
    input [47:0] mantisa_in,
    input [8:0] val1,
    output reg [56:0] mantisa_out
    );
    
    always @(*)
    begin
        mantisa_out = {val1, mantisa_in};
    end
endmodule
