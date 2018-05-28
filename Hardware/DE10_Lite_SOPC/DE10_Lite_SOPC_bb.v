
module DE10_Lite_SOPC (
	clk_clk,
	clk_sdram_clk,
	lcd_reset_n_external_connection_export,
	lt24_controller_0_conduit_end_export_cs,
	lt24_controller_0_conduit_end_export_data,
	lt24_controller_0_conduit_end_export_rd,
	lt24_controller_0_conduit_end_export_rs,
	lt24_controller_0_conduit_end_export_wr,
	push_button_export,
	reset_reset_n,
	sdram_wire_addr,
	sdram_wire_ba,
	sdram_wire_cas_n,
	sdram_wire_cke,
	sdram_wire_cs_n,
	sdram_wire_dq,
	sdram_wire_dqm,
	sdram_wire_ras_n,
	sdram_wire_we_n,
	sliders_export,
	touch_panel_busy_external_connection_export,
	touch_panel_pen_irq_n_external_connection_export,
	touch_panel_spi_external_MISO,
	touch_panel_spi_external_MOSI,
	touch_panel_spi_external_SCLK,
	touch_panel_spi_external_SS_n);	

	input		clk_clk;
	output		clk_sdram_clk;
	output		lcd_reset_n_external_connection_export;
	output		lt24_controller_0_conduit_end_export_cs;
	output	[15:0]	lt24_controller_0_conduit_end_export_data;
	output		lt24_controller_0_conduit_end_export_rd;
	output		lt24_controller_0_conduit_end_export_rs;
	output		lt24_controller_0_conduit_end_export_wr;
	input	[1:0]	push_button_export;
	input		reset_reset_n;
	output	[12:0]	sdram_wire_addr;
	output	[1:0]	sdram_wire_ba;
	output		sdram_wire_cas_n;
	output		sdram_wire_cke;
	output		sdram_wire_cs_n;
	inout	[15:0]	sdram_wire_dq;
	output	[1:0]	sdram_wire_dqm;
	output		sdram_wire_ras_n;
	output		sdram_wire_we_n;
	input	[9:0]	sliders_export;
	input		touch_panel_busy_external_connection_export;
	input		touch_panel_pen_irq_n_external_connection_export;
	input		touch_panel_spi_external_MISO;
	output		touch_panel_spi_external_MOSI;
	output		touch_panel_spi_external_SCLK;
	output		touch_panel_spi_external_SS_n;
endmodule
