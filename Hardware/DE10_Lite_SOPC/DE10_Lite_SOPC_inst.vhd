	component DE10_Lite_SOPC is
		port (
			clk_clk                                          : in    std_logic                     := 'X';             -- clk
			clk_sdram_clk                                    : out   std_logic;                                        -- clk
			lcd_reset_n_external_connection_export           : out   std_logic;                                        -- export
			lt24_controller_0_conduit_end_export_cs          : out   std_logic;                                        -- export_cs
			lt24_controller_0_conduit_end_export_data        : out   std_logic_vector(15 downto 0);                    -- export_data
			lt24_controller_0_conduit_end_export_rd          : out   std_logic;                                        -- export_rd
			lt24_controller_0_conduit_end_export_rs          : out   std_logic;                                        -- export_rs
			lt24_controller_0_conduit_end_export_wr          : out   std_logic;                                        -- export_wr
			push_button_export                               : in    std_logic_vector(1 downto 0)  := (others => 'X'); -- export
			reset_reset_n                                    : in    std_logic                     := 'X';             -- reset_n
			sdram_wire_addr                                  : out   std_logic_vector(12 downto 0);                    -- addr
			sdram_wire_ba                                    : out   std_logic_vector(1 downto 0);                     -- ba
			sdram_wire_cas_n                                 : out   std_logic;                                        -- cas_n
			sdram_wire_cke                                   : out   std_logic;                                        -- cke
			sdram_wire_cs_n                                  : out   std_logic;                                        -- cs_n
			sdram_wire_dq                                    : inout std_logic_vector(15 downto 0) := (others => 'X'); -- dq
			sdram_wire_dqm                                   : out   std_logic_vector(1 downto 0);                     -- dqm
			sdram_wire_ras_n                                 : out   std_logic;                                        -- ras_n
			sdram_wire_we_n                                  : out   std_logic;                                        -- we_n
			sliders_export                                   : in    std_logic_vector(9 downto 0)  := (others => 'X'); -- export
			touch_panel_busy_external_connection_export      : in    std_logic                     := 'X';             -- export
			touch_panel_pen_irq_n_external_connection_export : in    std_logic                     := 'X';             -- export
			touch_panel_spi_external_MISO                    : in    std_logic                     := 'X';             -- MISO
			touch_panel_spi_external_MOSI                    : out   std_logic;                                        -- MOSI
			touch_panel_spi_external_SCLK                    : out   std_logic;                                        -- SCLK
			touch_panel_spi_external_SS_n                    : out   std_logic                                         -- SS_n
		);
	end component DE10_Lite_SOPC;

	u0 : component DE10_Lite_SOPC
		port map (
			clk_clk                                          => CONNECTED_TO_clk_clk,                                          --                                       clk.clk
			clk_sdram_clk                                    => CONNECTED_TO_clk_sdram_clk,                                    --                                 clk_sdram.clk
			lcd_reset_n_external_connection_export           => CONNECTED_TO_lcd_reset_n_external_connection_export,           --           lcd_reset_n_external_connection.export
			lt24_controller_0_conduit_end_export_cs          => CONNECTED_TO_lt24_controller_0_conduit_end_export_cs,          --             lt24_controller_0_conduit_end.export_cs
			lt24_controller_0_conduit_end_export_data        => CONNECTED_TO_lt24_controller_0_conduit_end_export_data,        --                                          .export_data
			lt24_controller_0_conduit_end_export_rd          => CONNECTED_TO_lt24_controller_0_conduit_end_export_rd,          --                                          .export_rd
			lt24_controller_0_conduit_end_export_rs          => CONNECTED_TO_lt24_controller_0_conduit_end_export_rs,          --                                          .export_rs
			lt24_controller_0_conduit_end_export_wr          => CONNECTED_TO_lt24_controller_0_conduit_end_export_wr,          --                                          .export_wr
			push_button_export                               => CONNECTED_TO_push_button_export,                               --                               push_button.export
			reset_reset_n                                    => CONNECTED_TO_reset_reset_n,                                    --                                     reset.reset_n
			sdram_wire_addr                                  => CONNECTED_TO_sdram_wire_addr,                                  --                                sdram_wire.addr
			sdram_wire_ba                                    => CONNECTED_TO_sdram_wire_ba,                                    --                                          .ba
			sdram_wire_cas_n                                 => CONNECTED_TO_sdram_wire_cas_n,                                 --                                          .cas_n
			sdram_wire_cke                                   => CONNECTED_TO_sdram_wire_cke,                                   --                                          .cke
			sdram_wire_cs_n                                  => CONNECTED_TO_sdram_wire_cs_n,                                  --                                          .cs_n
			sdram_wire_dq                                    => CONNECTED_TO_sdram_wire_dq,                                    --                                          .dq
			sdram_wire_dqm                                   => CONNECTED_TO_sdram_wire_dqm,                                   --                                          .dqm
			sdram_wire_ras_n                                 => CONNECTED_TO_sdram_wire_ras_n,                                 --                                          .ras_n
			sdram_wire_we_n                                  => CONNECTED_TO_sdram_wire_we_n,                                  --                                          .we_n
			sliders_export                                   => CONNECTED_TO_sliders_export,                                   --                                   sliders.export
			touch_panel_busy_external_connection_export      => CONNECTED_TO_touch_panel_busy_external_connection_export,      --      touch_panel_busy_external_connection.export
			touch_panel_pen_irq_n_external_connection_export => CONNECTED_TO_touch_panel_pen_irq_n_external_connection_export, -- touch_panel_pen_irq_n_external_connection.export
			touch_panel_spi_external_MISO                    => CONNECTED_TO_touch_panel_spi_external_MISO,                    --                  touch_panel_spi_external.MISO
			touch_panel_spi_external_MOSI                    => CONNECTED_TO_touch_panel_spi_external_MOSI,                    --                                          .MOSI
			touch_panel_spi_external_SCLK                    => CONNECTED_TO_touch_panel_spi_external_SCLK,                    --                                          .SCLK
			touch_panel_spi_external_SS_n                    => CONNECTED_TO_touch_panel_spi_external_SS_n                     --                                          .SS_n
		);

