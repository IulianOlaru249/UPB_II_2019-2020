`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 05/07/2020 08:58:43 PM
// Design Name: 
// Module Name: simulation
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

module simulation(
    );
    
    reg Clear;
	reg Load;
	reg clk;
	reg [31:0] A = 0;
	reg [31:0] B = 0;
	wire [31:0] Rez;
    pipeline pipe(Clear, Load, clk, A, B, Rez);

	initial begin
		
		Clear = 0;
		Load = 0;
		clk = 0;
		
		#50
		Load = 1;
		A = 32'b00110010100010100011110101110001; 
		B = 32'b00001111000000000000000000000000; 
		
		#50
		Load = 1;
		A = 32'b00111111000110011111100110011010; 
		B = 32'b101111011100110011110110011001101; 
		
		#50
		A = 32'b11111111011001100110011001100110; 
		B = 32'b11001111011101100110011001101101; 
 
	end
	always 
	
		 #25 clk = ~clk;
	  
endmodule

