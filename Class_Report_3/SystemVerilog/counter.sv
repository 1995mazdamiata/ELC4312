`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 10/15/2025 02:54:54 PM
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


module counter(
    input logic clk,
    input logic rst,
    input logic en,
    input logic [31:0] val,
    output logic tic
    );
    
    logic [31:0] count, ncount;
    
    always_ff@(posedge(clk), posedge(rst))
        if(rst)
            count <= 0;     
        else
            if (en)
                count <= ncount;
            
    always_comb begin
        if (count < val - 1)
            ncount = count + 1;
        else
            ncount = 0;
    end 
    
    assign tic = (count == 1);
endmodule