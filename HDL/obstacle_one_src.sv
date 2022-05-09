`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 05/06/2022 07:47:47 PM
// Design Name: 
// Module Name: obstacle_one_src
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


module obstacle_one_src
#(parameter CD = 12, ADDR = 10, KEY_COLOR = 0)(
    input  logic clk,
    input  logic [10:0] x, y,   // x-and  y-coordinate    
    input  logic [10:0] x0, y0, // origin of sprite 
    input  logic [4:0] ctrl,    // sprite control 
    //ram write 
    input  logic we ,
    input  logic [ADDR-1:0] addr_w,
    input  logic [1:0] pixel_in,
    // pixel output
    output logic [CD-1:0] sprite_rgb
    );

    // localparam declaration
   localparam H_SIZE = 16; // horizontal size of sprite
   localparam V_SIZE = 16; // vertical size of sprite
   // signal delaration
   logic signed [11:0] xr, yr;  // relative x/y position
   logic in_region;
   logic [ADDR-1:0] addr_r;
   logic [1:0] sid;             // sprite id   
   logic [1:0] plt_code;        
   logic frame_tick, ani_tick;
   logic [3:0] c_next;        
   logic [3:0] c_reg;        
   logic [1:0] ani_next;        
   logic [1:0] ani_reg;        
   logic [10:0] x_d1_reg;
   logic [CD-1:0]  out_rgb;
   logic [CD-1:0] full_rgb;
   logic [CD-1:0] out_rgb_d1_reg;
   logic [1:0] gc_color_sel;        
   logic [1:0] gc_id_sel;        
   logic auto; 
   
   //new//
   logic [CD-1:0] obstacle_one_rgb;
   
   // body 
   assign gc_color_sel = ctrl[4:3];
   assign gc_id_sel = ctrl[1:0];
   assign auto = ctrl[2];
   
   //******************************************************************
   // sprite RAM
   //******************************************************************
   // instantiate sprite RAM
   obstacle_one_ram_lut #(.ADDR_WIDTH(ADDR), .DATA_WIDTH(2)) ram_unit (
      .clk(clk), .we(we), .addr_w(addr_w), .din(pixel_in),
      .addr_r(addr_r), .dout(plt_code));
   assign addr_r = {sid, yr[3:0], xr[3:0]};
   
    
   //******************************************************************
   //color control
   //******************************************************************
   //color selection
   always_comb
      case (gc_color_sel)
         2'b00:   obstacle_one_rgb = 12'hf00;  
         2'b01:   obstacle_one_rgb = 12'h0f0;  
         2'b10:   obstacle_one_rgb = 12'h00f;  
         default: obstacle_one_rgb = 12'h0ff;   
      endcase   
   // palette table
   always_comb
      case (plt_code)
         2'b00:   full_rgb = 12'h000;   // chrome key
         2'b01:   full_rgb = 12'hfff;    
         2'b10:   full_rgb = obstacle_one_rgb; //body color
         default: full_rgb = 12'hfff;   
      endcase 
   
       
   //******************************************************************
   // in-region circuit
   //******************************************************************
   // relative coordinate calculation
   assign xr = $signed({1'b0, x}) - $signed({1'b0, x0});
   assign yr = $signed({1'b0, y}) - $signed({1'b0, y0});
   // in-region comparison and multiplexing 
   assign in_region = ((0<= xr) && (xr<H_SIZE) && (0<=yr) && (yr<V_SIZE));
   assign out_rgb = in_region ? full_rgb : KEY_COLOR; 
   

    //******************************************************************
   // delay line (one clock) 
   //******************************************************************
   // output with a-stage delay line
   always_ff @(posedge clk) 
      out_rgb_d1_reg <= out_rgb;
   assign sprite_rgb = out_rgb_d1_reg;
endmodule
