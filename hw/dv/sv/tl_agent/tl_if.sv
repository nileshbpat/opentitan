// Copyright lowRISC contributors (OpenTitan project).
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
// ---------------------------------------------
// TileLink interface
// ---------------------------------------------
interface tl_if(input clk, input rst_n);

  wire tlul_pkg::tl_h2d_t h2d; // req
  wire tlul_pkg::tl_d2h_t d2h; // rsp

  wire axi_struct_pkg::axi_wr_req_t  axi_wr_req;
  wire axi_struct_pkg::axi_wr_rsp_t  axi_wr_rsp;
  wire axi_struct_pkg::axi_rd_req_t  axi_rd_req;
  wire axi_struct_pkg::axi_rd_rsp_t  axi_rd_rsp;

  tlul_pkg::tl_h2d_t h2d_int; // req (internal)
  tlul_pkg::tl_d2h_t d2h_int; // rsp (internal)

  dv_utils_pkg::if_mode_e if_mode; // interface mode - Host or Device

  modport dut_host_mp(output h2d_int, input d2h_int);
  modport dut_device_mp(input h2d_int, output d2h_int);

  clocking host_cb @(posedge clk);
    input  rst_n;
    output h2d_int;
    input  d2h;
  endclocking
  modport host_mp(clocking host_cb);

  clocking device_cb @(posedge clk);
    input  rst_n;
    input  h2d;
    output d2h_int;
  endclocking
  modport device_mp(clocking device_cb);

  clocking mon_cb @(posedge clk);
    input  rst_n;
    input  h2d;
    input  d2h;
  endclocking
  modport mon_mp(clocking mon_cb);

  clocking wr_cb @(posedge clk);
    input axi_wr_req;
    output axi_wr_rsp;
  endclocking
  modport wr_mp(clocking wr_cb);

  clocking rd_cb @(posedge clk);
    input axi_rd_req;
    output axi_rd_rsp;
  endclocking
  modport rd_mp(clocking rd_cb);

  assign h2d = (if_mode == dv_utils_pkg::Host) ? h2d_int : 'z;
  assign d2h = (if_mode == dv_utils_pkg::Device) ? d2h_int : 'z;


  // always @(posedge clk) begin
  //   if (!rst_n) begin
  //      d2h_int.d_opcode <= tlul_pkg::AccessAckData;
  //      d2h_int.d_data   <= axi_rd.rdata;
  //      d2h_int.d_param  <= 0;
  //      d2h_int.d_size   <= axi_rd.arsize;
  //      d2h_int.d_source <= h2d_int.a_source;
  //      d2h_int.d_error  <= 0;
  //      d2h_int.d_sink   <= 0;
  //      d2h_int.d_valid  <= axi_rd.rvalid;
  //      h2d_int.d_ready  <= axi_rd.rready;
  //      d2h_int.d_user   <= axi_rd.aruser;
  //   end
  // end


  // // -- FIX ME
  // //-- Write interface signals assignment - AXI

  // assign axi_wr.awaddr = 'b0;
  // assign axi_wr.awburst = 'b0;
  // assign axi_wr.awsize = 'b0;
  // assign axi_wr.awlen = 'b0;
  // assign axi_wr.awuser = 'b0;
  // assign axi_wr.awid = 'b0;
  // assign axi_wr.awlock = 'b0;
  // assign axi_wr.awvalid = 'b0;
  // // assign axi_wr.awready = 'b0;
  
  // assign axi_wr.wdata = 'b0;
  // assign axi_wr.wstrb = 'b0;
  // assign axi_wr.wlast = 'b0;
  // assign axi_wr.wvalid = 'b0;
  // // assign axi_wr.wready = 'b0;
  
  // // assign axi_wr.bresp = 'b0;
  // // assign axi_wr.bid = 'b0;
  // // assign axi_wr.bvalid = 'b0;
  // assign axi_wr.bready = 'b0;


  // //-- Read interface signals assignment - AXI

  // assign axi_rd.araddr = 'b0;      // 32-bit address
  // assign axi_rd.arsize = 'b0;      // 3-bit burst size
  // assign axi_rd.arlen = 'b0;       // 12-bit burst length
  // assign axi_rd.arid = 'b0;        // 4-bit read transaction ID
  // assign axi_rd.arlock = 'b0;      // 1-bit lock signal
  // assign axi_rd.arvalid = 'b0;     // 1-bit read address valid
  // // assign axi_rd.arready = 'b0;     // 1-bit read address ready

  // // assign axi_rd.rdata = 'b0;       // 32-bit read data
  // // assign axi_rd.rresp = 'b0;       // 2-bit read response
  // // assign axi_rd.rid = 'b0;         // 4-bit read ID
  // // assign axi_rd.rlast = 'b0;       // 1-bit last read indicator
  // // assign axi_rd.rvalid = 'b0;      // 1-bit read data valid
  // assign axi_rd.rready = 'b0;      // 1-bit read data ready
  
endinterface : tl_if

