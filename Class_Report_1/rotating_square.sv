`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 09/10/2025 05:41:17 PM
// Design Name: 
// Module Name: rotating_square
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


module rotating_square#(parameter N = 26)(
    input logic clk,
    input logic en,
    input logic cw,
    input logic rst,
    output logic [6:0] CA,
    output logic [7:0] AN
    );
    
    // general variables and signals
    parameter TOP = 7'b0011100;
    parameter BTM = 7'b0100011;
    logic tic;
    logic [3:0] pos, npos;
    logic [6:0] ca, nca;
    logic [7:0] an, nan;
    
    // counter for timing
    counter# (.N(N)) count(
        .clk(clk),
        .tic(tic)
    );
    
    // next state register
    always_ff @(posedge(tic), posedge(rst)) begin
        if (rst) begin
            ca = BTM;
            an = 8'b011111111;
            pos = 0;
        end else begin
            ca <= nca;
            an <= nan;
            pos <= npos;
        end
    end
    
    // next state logic
    always_comb begin
        if (en) begin
            // seven segment controls
            if (pos < 8) begin
                nca = BTM;
                nan = 8'b11111111;
                nan[7-pos] = 1'b0;
            end else begin
                nca = TOP;
                nan = 8'b11111111;
                nan[pos-8] = 1'b0;
            end
            
            // direction control
            if (cw) begin
                npos = (pos == 0) ? 15 : pos - 1;
            end else begin
                npos = (pos == 15) ? 0 : pos + 1;
            end 
               
        end else begin
            nca = ca;
            nan = an;
            npos = pos;
        end
    end
    
    assign CA = ca;
    assign AN = an;
    
endmodule