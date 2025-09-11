`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 09/10/2025 05:41:17 PM
// Design Name: 
// Module Name: rotating_square_t
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


module rotating_square_t;
    parameter N = 1;
    logic clk;
    logic en;
    logic cw;
    logic rst;
    reg [6:0] CA;
    reg [7:0] AN;
    
    rotating_square #(.N(N)) dut(
    .clk(clk),
    .en(en),
    .cw(cw),
    .rst(rst),
    .CA(CA),
    .AN(AN)
    );
  
    initial begin
        clk = 0;
        forever
            #5 clk = ~clk;
    end
    
    initial begin
        rst = 0;
        #3 rst = 1;
        #3 rst = 0;
    end
    
    initial begin
        en = 1;
        cw = 0;
        #400 cw = 1;
        #200 en = 0;
        #40  en = 1;
        #300 cw = 0;
        #100 $finish;
    end
    
endmodule
