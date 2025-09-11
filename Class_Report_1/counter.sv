`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 09/10/2025 06:35:20 PM
// Design Name: 
// Module Name: counter
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


module counter#(parameter N = 10)(
    input clk,
    output tic
    );
    
    logic [N-1:0] count = 0;
    logic [N-1:0] ncount = 0;
    
    always_ff@(posedge(clk))
        count <= ncount;
            
    always_comb begin
        ncount = (count < 2**N - 1) ? count + 1 : 0;
    end 
    
    assign tic = (count == 1);
    
endmodule
