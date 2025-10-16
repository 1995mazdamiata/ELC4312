`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 10/15/2025 04:59:07 PM
// Design Name: 
// Module Name: Blinking_LED_Core
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


module Blinking_LED_Core(
    input  logic clk,
    input  logic reset,
    // slot interface
    input  logic cs,
    input  logic read,
    input  logic write,
    input  logic [4:0] addr,
    input  logic [31:0] wr_data,
    output logic [31:0] rd_data,
    // external port
    output logic [15:0] led
    );
    
    logic [3:0] tic;
    logic wr_led0, wr_led1, wr_led2, wr_led3;
    logic [15:0] led0_reg, led1_reg, led2_reg, led3_reg;
    
    counter count0(
    .clk(clk),
    .rst(reset),
    .en(1'b1),
    .val(100000*led0_reg),
    .tic(tic[0])
    );
    
    counter count1(
    .clk(clk),
    .rst(reset),
    .en(1'b1),
    .val(100000*led1_reg),
    .tic(tic[1])
    );
    
    counter count2(
    .clk(clk),
    .rst(reset),
    .en(1'b1),
    .val(100000*led2_reg),
    .tic(tic[2])
    );
    
    counter count3(
    .clk(clk),
    .rst(reset),
    .en(1'b1),
    .val(100000*led3_reg),
    .tic(tic[3])
    );
    
    led_driver drive0(
    .clk(clk),
    .rst(reset),
    .en(tic[0]),
    .dout(led[0])
    );
    
    led_driver drive1(
    .clk(clk),
    .rst(reset),
    .en(tic[1]),
    .dout(led[1])
    );
    
    led_driver drive2(
    .clk(clk),
    .rst(reset),
    .en(tic[2]),
    .dout(led[2])
    );
    
    led_driver drive3(
    .clk(clk),
    .rst(reset),
    .en(tic[3]),
    .dout(led[3])
    );

   // body
   always_ff @(posedge clk, posedge reset)
      if (reset)
         led0_reg <= 0;
      else   
         if (wr_led0)
            led0_reg <= wr_data[15:0];
            
   always_ff @(posedge clk, posedge reset)
      if (reset)
         led1_reg <= 0;
      else   
         if (wr_led1)
            led1_reg <= wr_data[15:0];
   
   always_ff @(posedge clk, posedge reset)
      if (reset)
         led2_reg <= 0;
      else   
         if (wr_led2)
            led2_reg <= wr_data[15:0];
            
   always_ff @(posedge clk, posedge reset)
      if (reset)
         led3_reg <= 0;
      else   
         if (wr_led3)
            led3_reg <= wr_data[15:0];
            
   // decoding logic 
   assign wr_led0 = (cs && write && (addr[1:0]==2'b00));
   assign wr_led1 = (cs && write && (addr[1:0]==2'b01));
   assign wr_led2 = (cs && write && (addr[1:0]==2'b10));
   assign wr_led3 = (cs && write && (addr[1:0]==2'b11));  
    
   // slot read interface
   assign rd_data =  0;
    
endmodule
