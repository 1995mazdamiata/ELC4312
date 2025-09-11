`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 09/10/2025 05:41:17 PM
// Design Name: 
// Module Name: rotating_square_top
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


module rotating_square_top(
    input logic CLK100MHZ,
    input logic [1:0] SW,
    output logic [6:0] CA,
    output logic [7:0] AN
    );
    
    rotating_square# (.N(26)) rs(
        .clk(CLK100MHZ),
        .en(SW[0]),
        .cw(SW[1]),
        .CA(CA),
        .AN(AN)
    );
endmodule