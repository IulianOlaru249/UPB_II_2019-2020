`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 03/10/2020 09:35:53 AM
// Design Name: 
// Module Name: bin2bcd_sim
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


module bin2bcd_sim();

    reg [5:0] bin_val;
    wire [3:0] bcd0, bcd1;
    
    bin2bcd b2d(bin_val, bcd0. bcd1);
    
    initial begin
    
    #50 bin_val = 10111;
    
    #50 bin_val = 110000;
    
    #50 $stop;
    #50 $finish;
    end
endmodule
