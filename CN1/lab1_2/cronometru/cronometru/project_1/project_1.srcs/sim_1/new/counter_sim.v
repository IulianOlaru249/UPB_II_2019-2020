`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 03/10/2020 08:50:43 AM
// Design Name: 
// Module Name: counter_sim
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


module counter_sim();
    reg clk, reset, pause;
    wire carry_out;
    wire [5:0] up_counter;
    
    Numarator num(clk, reset, pauza, valoare_bin, carry_out);
    
    initial begin
        
        #50 clk = 0; reset = 1; pause = 0; //prima varianta de reset e bine sa ife cel ce opreste tot (reset = 1)
        #100 reset = 0;
   
    end
    always 
        #50 clk = !clk;
endmodule
