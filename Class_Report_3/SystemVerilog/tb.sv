`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 10/15/2025 02:59:54 PM
// Design Name: 
// Module Name: tb
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


module tb;
    logic clk;
    logic reset;    
    // slot interface
    logic cs;
    logic read;
    logic write;
    logic [4:0] addr;
    logic [31:0] wr_data;
    logic [31:0] rd_data;
    logic [3:0] led;
    logic [11:0] other_led;
    
    Blinking_LED_Core dut(
    .clk(clk),
    .reset(reset),
    // slot interface
    .cs(cs),
    .read(read),
    .write(write),
    .addr(addr),
    .wr_data(wr_data),
    .rd_data(rd_data),
    .led({other_led, led})
    );
    
    initial begin
        clk = 0;
        forever 
            #5 clk=~clk;
    end
    
    initial begin
        reset = 0;
        #3 reset = 1;
        #3 reset = 0;
    end
    
    initial begin
        write = 1'b1;
        cs = 1'b1;
        #20 addr = 2'b00;
        wr_data = 32'd5;
        #10 addr = 2'b01;
        wr_data = 32'd10;
        #10 addr = 2'b10;
        wr_data = 32'd15;
        #10 addr = 2'b11;
        wr_data = 32'd20;
        #10 write = 1'b0;
        cs = 1'b0;
        #500 write = 1'b1;
        cs = 1'b1;
        addr = 2'b10;
        wr_data = 32'd3;
        #10 write = 1'b0;
        cs = 1'b0;
        #300$finish;
    end
    
endmodule
