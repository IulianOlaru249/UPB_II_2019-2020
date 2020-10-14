`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 04/10/2020 07:38:21 PM
// Design Name: 
// Module Name: carry_look_ahead_16bits_sim
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


module carry_look_ahead_16bits_sim();
    reg [15:0] a,b;
    reg cin;
    wire [15:0] sum;
    wire cout;
 
    carry_look_ahead_16bits module1(a, b, cin, sum, cout);
 
    initial begin
        a=0; b=0; cin=0;
        //0 + 0 + cin = 1 
        #10 a=16'd0; b=16'd0; cin=1'd1;
        
        //1 + 1 = 2
        #10 a=16'd1; b=16'd1; cin=1'd0;
        
        // 20 + 30 + 1 = 51
        #10 a=16'd14; b=16'd1; cin=1'd1;
    
        //999 + 0 + cin = 1000
        #10 a=16'd999; b=16'd0; cin=1'd1;
        
        //65535 + 2 = 1 + cout
        #10 a=16'd65535; b=16'd2; cin=1'd0;
    
    end
 
endmodule
