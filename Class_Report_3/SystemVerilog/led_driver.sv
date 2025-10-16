`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 10/15/2025 03:33:45 PM
// Design Name: 
// Module Name: led_driver
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


module led_driver(
    input logic clk,
    input logic rst,
    input logic en,
    output logic dout
    );
    
    logic out, nout;
    
    always_ff @(posedge(clk), posedge(rst))
        if(rst)
            out <= 0;
        else
            out <= nout;
            
    assign nout = en ? ~out : out;
    
    assign dout = out;
endmodule