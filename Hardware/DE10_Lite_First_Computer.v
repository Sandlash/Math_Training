module DE10_Lite_First_Computer(

	//////////// CLOCK //////////
	input 		          		ADC_CLK_10,
	input 		          		MAX10_CLK1_50,
	input 		          		MAX10_CLK2_50,

	//////////// SDRAM //////////
	output		    [12:0]		DRAM_ADDR,
	output		     [1:0]		DRAM_BA,
	output		          		DRAM_CAS_N,
	output		          		DRAM_CKE,
	output		          		DRAM_CLK,
	output		          		DRAM_CS_N,
	inout 		    [15:0]		DRAM_DQ,
	output		          		DRAM_LDQM,
	output		          		DRAM_RAS_N,
	output		          		DRAM_UDQM,
	output		          		DRAM_WE_N,


	//////////// KEY //////////
	input 		     [1:0]		KEY,


	//////////// SW //////////
	input 		     [9:0]		SW,


	//////////// GPIO, GPIO connect to LT24 - 2.4" LCD and Touch //////////
	input 		          		LT24_ADC_BUSY,
	output		          		LT24_ADC_CS_N,
	output		          		LT24_ADC_DCLK,
	output		          		LT24_ADC_DIN,
	input 		          		LT24_ADC_DOUT,
	input 		          		LT24_ADC_PENIRQ_N,
	output		          		LT24_CS_N,
	output		    [15:0]		LT24_D,
	output		          		LT24_LCD_ON,
	output		          		LT24_RD_N,
	output		          		LT24_RESET_N,
	output		          		LT24_RS,
	output		          		LT24_WR_N
);



//=======================================================
//  REG/WIRE declarations
//=======================================================




//=======================================================
//  Structural coding
//=======================================================


assign reset_n				= KEY[1];
assign LT24_LCD_ON 		= 1'b1;	//default on

    DE10_Lite_SOPC u0 (
        .clk_clk                                          (MAX10_CLK1_50),                                          //                                       clk.clk
        .clk_sdram_clk                                    (DRAM_CLK),                                    //                                 clk_sdram.clk
	     .reset_reset_n                                    (reset_n),                                    //                                     reset.reset_n

		  .push_button_export                               (KEY),                               //                               push_button.export
		  .sliders_export                                   (SW),                                   //                                   sliders.export

        .lcd_reset_n_external_connection_export           (LT24_RESET_N),           //           lcd_reset_n_external_connection.export
        .lt24_controller_0_conduit_end_export_cs          (LT24_CS_N),          //             lt24_controller_0_conduit_end.export_cs
        .lt24_controller_0_conduit_end_export_data        (LT24_D),        //                                          .export_data
        .lt24_controller_0_conduit_end_export_rd          (LT24_RD_N),          //                                          .export_rd
        .lt24_controller_0_conduit_end_export_rs          (LT24_RS),          //                                          .export_rs
        .lt24_controller_0_conduit_end_export_wr          (LT24_WR_N),          //                                          .export_wr
        
		  .sdram_wire_addr                                  (DRAM_ADDR),                                  //                                sdram_wire.addr
        .sdram_wire_ba                                    (DRAM_BA),                                    //                                          .ba
        .sdram_wire_cas_n                                 (DRAM_CAS_N),                                 //                                          .cas_n
        .sdram_wire_cke                                   (DRAM_CKE),                                   //                                          .cke
        .sdram_wire_cs_n                                  (DRAM_CS_N),                                  //                                          .cs_n
        .sdram_wire_dq                                    (DRAM_DQ),                                    //                                          .dq
        .sdram_wire_dqm                                   ({DRAM_UDQM, DRAM_LDQM}),                                   //                                          .dqm
        .sdram_wire_ras_n                                 (DRAM_RAS_N),                                 //                                          .ras_n
        .sdram_wire_we_n                                  (DRAM_WE_N),                                  //                                          .we_n
        
        
		  .touch_panel_busy_external_connection_export      (LT24_ADC_BUSY),      //      touch_panel_busy_external_connection.export
        .touch_panel_pen_irq_n_external_connection_export (LT24_ADC_PENIRQ_N), // touch_panel_pen_irq_n_external_connection.export
        .touch_panel_spi_external_MISO                    (LT24_ADC_DOUT),                    //                  touch_panel_spi_external.MISO
        .touch_panel_spi_external_MOSI                    (LT24_ADC_DIN),                    //                                          .MOSI
        .touch_panel_spi_external_SCLK                    (LT24_ADC_DCLK),                    //                                          .SCLK
        .touch_panel_spi_external_SS_n                    (LT24_ADC_CS_N)                    //                                          .SS_n

	
	
);


endmodule
