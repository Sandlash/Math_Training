	DE10_Lite_SOPC u0 (
		.clk_clk                                          (<connected-to-clk_clk>),                                          //                                       clk.clk
		.clk_sdram_clk                                    (<connected-to-clk_sdram_clk>),                                    //                                 clk_sdram.clk
		.lcd_reset_n_external_connection_export           (<connected-to-lcd_reset_n_external_connection_export>),           //           lcd_reset_n_external_connection.export
		.leds_export                                      (<connected-to-leds_export>),                                      //                                      leds.export
		.lt24_controller_0_conduit_end_export_cs          (<connected-to-lt24_controller_0_conduit_end_export_cs>),          //             lt24_controller_0_conduit_end.export_cs
		.lt24_controller_0_conduit_end_export_data        (<connected-to-lt24_controller_0_conduit_end_export_data>),        //                                          .export_data
		.lt24_controller_0_conduit_end_export_rd          (<connected-to-lt24_controller_0_conduit_end_export_rd>),          //                                          .export_rd
		.lt24_controller_0_conduit_end_export_rs          (<connected-to-lt24_controller_0_conduit_end_export_rs>),          //                                          .export_rs
		.lt24_controller_0_conduit_end_export_wr          (<connected-to-lt24_controller_0_conduit_end_export_wr>),          //                                          .export_wr
		.push_button_export                               (<connected-to-push_button_export>),                               //                               push_button.export
		.reset_reset_n                                    (<connected-to-reset_reset_n>),                                    //                                     reset.reset_n
		.sdram_wire_addr                                  (<connected-to-sdram_wire_addr>),                                  //                                sdram_wire.addr
		.sdram_wire_ba                                    (<connected-to-sdram_wire_ba>),                                    //                                          .ba
		.sdram_wire_cas_n                                 (<connected-to-sdram_wire_cas_n>),                                 //                                          .cas_n
		.sdram_wire_cke                                   (<connected-to-sdram_wire_cke>),                                   //                                          .cke
		.sdram_wire_cs_n                                  (<connected-to-sdram_wire_cs_n>),                                  //                                          .cs_n
		.sdram_wire_dq                                    (<connected-to-sdram_wire_dq>),                                    //                                          .dq
		.sdram_wire_dqm                                   (<connected-to-sdram_wire_dqm>),                                   //                                          .dqm
		.sdram_wire_ras_n                                 (<connected-to-sdram_wire_ras_n>),                                 //                                          .ras_n
		.sdram_wire_we_n                                  (<connected-to-sdram_wire_we_n>),                                  //                                          .we_n
		.sliders_export                                   (<connected-to-sliders_export>),                                   //                                   sliders.export
		.touch_panel_busy_external_connection_export      (<connected-to-touch_panel_busy_external_connection_export>),      //      touch_panel_busy_external_connection.export
		.touch_panel_pen_irq_n_external_connection_export (<connected-to-touch_panel_pen_irq_n_external_connection_export>), // touch_panel_pen_irq_n_external_connection.export
		.touch_panel_spi_external_MISO                    (<connected-to-touch_panel_spi_external_MISO>),                    //                  touch_panel_spi_external.MISO
		.touch_panel_spi_external_MOSI                    (<connected-to-touch_panel_spi_external_MOSI>),                    //                                          .MOSI
		.touch_panel_spi_external_SCLK                    (<connected-to-touch_panel_spi_external_SCLK>),                    //                                          .SCLK
		.touch_panel_spi_external_SS_n                    (<connected-to-touch_panel_spi_external_SS_n>),                    //                                          .SS_n
		.vga_clk_clk                                      (<connected-to-vga_clk_clk>)                                       //                                   vga_clk.clk
	);

