`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 03/10/2020 09:31:48 AM
// Design Name: 
// Module Name: divizor
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


module divizor(
    input clk_in, reset,
    output clk_out_led, clk_out_seg
    );
 reg[32:0] counter;
 
 always @(posedge clk_in)
    if(!reset)
        counter=0;
        else
        counter <= counter + 1;
        
        assign clk_out_let=counter[26];
        assign clk_out_seg=counter[17];   
    
endmodule
