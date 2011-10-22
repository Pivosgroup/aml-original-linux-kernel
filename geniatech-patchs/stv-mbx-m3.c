/*
 * arch/arm/mach-meson/board-8726m-dvbc.c
 * arch/arm/mach-meson/board-8726m-refc03.c
 *  Copyright (C) 2010 AMLOGIC, INC.
 *
 * License terms: GNU General Public License (GPL) version 2
 * Platform machine definition.
 */
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/mm.h>
#include <linux/sched.h>
#include <linux/interrupt.h>
#include <linux/ioport.h>
#include <linux/platform_device.h>
#include <linux/io.h>
#include <linux/dma-mapping.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/nand.h>
#include <linux/mtd/nand_ecc.h>
#include <linux/mtd/partitions.h>
#include <linux/device.h>
#include <linux/spi/flash.h>
#include <mach/hardware.h>
#include <mach/platform.h>
#include <mach/memory.h>
#include <mach/clock.h>
#include <asm/mach-types.h>
#include <asm/mach/arch.h>
#include <asm/setup.h>
#include <mach/lm.h>
#include <asm/memory.h>
#include <asm/mach/map.h>
#include <mach/am_eth_pinmux.h>
#include <mach/nand.h>
#include <linux/i2c.h>
#include <linux/i2c-aml.h>
#include <mach/power_gate.h>
#include <linux/aml_bl.h>

#ifdef CONFIG_AM_UART_WITH_S_CORE 
#include <linux/uart-aml.h>
#endif
#include <mach/card_io.h>
#ifdef CONFIG_CACHE_L2X0
#include <asm/hardware/cache-l2x0.h>
#endif
#include <mach/pinmux.h>
#include <mach/gpio.h>
#include <linux/delay.h>
#include <mach/clk_set.h>
#include "stv-mbx-m3.h"


#ifdef CONFIG_ANDROID_PMEM
#include <linux/slab.h>
#include <linux/dma-mapping.h>
#include <linux/android_pmem.h>
#endif



#include <linux/power_supply.h>


#if defined(CONFIG_JPEGLOGO)
static struct resource jpeglogo_resources[] = {
    [0] = {
        .start = CONFIG_JPEGLOGO_ADDR,
        .end   = CONFIG_JPEGLOGO_ADDR + CONFIG_JPEGLOGO_SIZE - 1,
        .flags = IORESOURCE_MEM,
    },
    [1] = {
        .start = CODEC_ADDR_START,
        .end   = CODEC_ADDR_END,
        .flags = IORESOURCE_MEM,
    },
};

static struct platform_device jpeglogo_device = {
	.name = "jpeglogo-dev",
	.id   = 0,
    .num_resources = ARRAY_SIZE(jpeglogo_resources),
    .resource      = jpeglogo_resources,
};
#endif

#if defined(CONFIG_KEYPADS_AM)||defined(CONFIG_KEYPADS_AM_MODULE) 
static struct resource intput_resources[] = {
	{
		.start = 0x0,
		.end = 0x0,
		.name="8726",
		.flags = IORESOURCE_IO,
	},
};

static struct platform_device input_device = {
	.name = "m1-kp",
	.id = 0,
	.num_resources = ARRAY_SIZE(intput_resources),
	.resource = intput_resources,
	
};
#endif

#if defined(CONFIG_ADC_KEYPADS_AM)||defined(CONFIG_ADC_KEYPADS_AM_MODULE)
#include <linux/input.h>
#include <linux/adc_keypad.h>
#include <linux/saradc.h>

static struct adc_key adc_kp_key[] = {
    {KEY_DOWN,          "down",  CHAN_4, 0, 60},
    {KEY_UP,            "up",    CHAN_4, 267, 60},
    {KEY_LEFT,          "left",  CHAN_4, 397, 60},
    {KEY_ENTER,         "enter", CHAN_4, 521, 60},
    {KEY_RIGHT,         "right", CHAN_4, 648, 60},
    {KEY_MENU,          "menu",  CHAN_4, 772, 60},
};

static struct adc_kp_platform_data adc_kp_pdata = {
    .key = &adc_kp_key[0],
    .key_num = ARRAY_SIZE(adc_kp_key),
};

static struct platform_device adc_kp_device = {
    .name = "m1-adckp",
    .id = 0,
    .num_resources = 0,
    .resource = NULL,
    .dev = {
    .platform_data = &adc_kp_pdata,
    }
};
#endif

#if defined(CONFIG_KEY_INPUT_CUSTOM_AM) || defined(CONFIG_KEY_INPUT_CUSTOM_AM_MODULE)
#include <linux/input.h>
#include <linux/input/key_input.h>

int _key_code_list[] = {KEY_EXIT};

static inline int key_input_init_func(void)
{	
	//GPIOD_8 as input key.
    WRITE_CBUS_REG(0x2012, (READ_CBUS_REG(0x2012) &~(1<<6)));
    return 0;
}
static inline int key_scan(int *key_state_list)
{
    int ret = 0;
    key_state_list[0] = ((READ_CBUS_REG(0x2014) >> 6) & 1) ? 0 : 1;
    return ret;
}

static  struct key_input_platform_data  key_input_pdata = {
    .scan_period = 20,
    .fuzz_time = 60,
    .key_code_list = &_key_code_list[0],
    .key_num = ARRAY_SIZE(_key_code_list),
    .scan_func = key_scan,
    .init_func = key_input_init_func,
    .config = 0,
};

static struct platform_device input_device_key = {
    .name = "m1-keyinput",
    .id = 0,
    .num_resources = 0,
    .resource = NULL,
    .dev = {
        .platform_data = &key_input_pdata,
    }
};
#endif

#if defined(CONFIG_FB_AM)
static struct resource fb_device_resources[] = {
    [0] = {
        .start = OSD1_ADDR_START,
        .end   = OSD1_ADDR_END,
        .flags = IORESOURCE_MEM,
    },
#if defined(CONFIG_FB_OSD2_ENABLE)
    [1] = {
        .start = OSD2_ADDR_START,
        .end   = OSD2_ADDR_END,
        .flags = IORESOURCE_MEM,
    },
#endif

};

static struct platform_device fb_device = {
    .name       = "mesonfb",
    .id         = 0,
    .num_resources = ARRAY_SIZE(fb_device_resources),
    .resource      = fb_device_resources,
};
#endif


#ifdef CONFIG_USB_DWC_OTG_HCD
static void set_usb_a_vbus_power(char is_power_on)
{
#define USB_A_POW_GPIO_BIT	20
	if(is_power_on){
		printk(KERN_INFO "set usb port power on (board gpio %d)!\n",USB_A_POW_GPIO_BIT);
		set_gpio_mode(PREG_HGPIO,USB_A_POW_GPIO_BIT,GPIO_OUTPUT_MODE);
		set_gpio_val(PREG_HGPIO,USB_A_POW_GPIO_BIT,1);
	}
	else	{
		printk(KERN_INFO "set usb port power off (board gpio %d)!\n",USB_A_POW_GPIO_BIT);		
		set_gpio_mode(PREG_HGPIO,USB_A_POW_GPIO_BIT,GPIO_OUTPUT_MODE);
		set_gpio_val(PREG_HGPIO,USB_A_POW_GPIO_BIT,0);		
	}
}
//usb_a is OTG port
static struct lm_device usb_ld_a = {
	.type = LM_DEVICE_TYPE_USB,
	.id = 0,
	.irq = INT_USB_A,
	.resource.start = IO_USB_A_BASE,
	.resource.end = -1,
	.dma_mask_room = DMA_BIT_MASK(32),
	.port_type = USB_PORT_TYPE_HOST,
	.port_speed = USB_PORT_SPEED_DEFAULT,
	.dma_config = USB_DMA_BURST_SINGLE,
	.set_vbus_power = 0,
};
static struct lm_device usb_ld_b = {
	.type = LM_DEVICE_TYPE_USB,
	.id = 1,
	.irq = INT_USB_B,
	.resource.start = IO_USB_B_BASE,
	.resource.end = -1,
	.dma_mask_room = DMA_BIT_MASK(32),
	.port_type = USB_PORT_TYPE_HOST,
	.port_speed = USB_PORT_SPEED_DEFAULT,
	.dma_config = USB_DMA_BURST_SINGLE,
	.set_vbus_power = 0,
};
#endif
#ifdef CONFIG_SATA_DWC_AHCI
static struct lm_device sata_ld = {
	.type = LM_DEVICE_TYPE_SATA,
	.id = 2,
	.irq = INT_SATA,
	.dma_mask_room = DMA_BIT_MASK(32),
	.resource.start = IO_SATA_BASE,
	.resource.end = -1,
};
#endif

#if defined(CONFIG_AM_STREAMING)
static struct resource codec_resources[] = {
    [0] = {
        .start =  CODEC_ADDR_START,
        .end   = CODEC_ADDR_END,
        .flags = IORESOURCE_MEM,
    },
    [1] = {
        .start = STREAMBUF_ADDR_START,
	 .end = STREAMBUF_ADDR_END,
	 .flags = IORESOURCE_MEM,
    },
};

static struct platform_device codec_device = {
    .name       = "amstream",
    .id         = 0,
    .num_resources = ARRAY_SIZE(codec_resources),
    .resource      = codec_resources,
};
#endif

#if defined(CONFIG_AM_VIDEO)
static struct resource deinterlace_resources[] = {
    [0] = {
        .start =  DI_ADDR_START,
        .end   = DI_ADDR_END,
        .flags = IORESOURCE_MEM,
    },
};

static struct platform_device deinterlace_device = {
    .name       = "deinterlace",
    .id         = 0,
    .num_resources = ARRAY_SIZE(deinterlace_resources),
    .resource      = deinterlace_resources,
};
#endif

#if defined(CONFIG_TVIN_VDIN)
static struct resource vdin_resources[] = {
    [0] = {
        .start =  VDIN_ADDR_START,		//pbufAddr
        .end   = VDIN_ADDR_END,				//pbufAddr + size
        .flags = IORESOURCE_MEM,
    },


};

static struct platform_device vdin_device = {
    .name       = "vdin",
    .id         = -1,
    .num_resources = ARRAY_SIZE(vdin_resources),
    .resource      = vdin_resources,
};

//add pin mux info for bt656 input
static struct resource bt656in_resources[] = {
    [0] = {
        .start =  VDIN_ADDR_START,		//pbufAddr
        .end   = VDIN_ADDR_END,				//pbufAddr + size
        .flags = IORESOURCE_MEM,
    },
    [1] = {		//bt656/camera/bt601 input resource pin mux setting
        .start =  0x3000,		//mask--mux gpioD 15 to bt656 clk;  mux gpioD 16:23 to be bt656 dt_in
        .end   = PERIPHS_PIN_MUX_5 + 0x3000,	 
        .flags = IORESOURCE_MEM,
    },

    [2] = {			//camera/bt601 input resource pin mux setting
        .start =  0x1c000,		//mask--mux gpioD 12 to bt601 FIQ; mux gpioD 13 to bt601HS; mux gpioD 14 to bt601 VS;
        .end   = PERIPHS_PIN_MUX_5 + 0x1c000,				
        .flags = IORESOURCE_MEM,
    },

    [3] = {			//bt601 input resource pin mux setting
        .start =  0x800,		//mask--mux gpioD 24 to bt601 IDQ;;
        .end   = PERIPHS_PIN_MUX_5 + 0x800,			
        .flags = IORESOURCE_MEM,
    },

};

static struct platform_device bt656in_device = {
    .name       = "amvdec_656in",
    .id         = -1,
    .num_resources = ARRAY_SIZE(bt656in_resources),
    .resource      = bt656in_resources,
};
#endif

#if defined(CONFIG_CARDREADER)
static struct resource amlogic_card_resource[]  = {
	[0] = {
		.start = 0x1200230,   //physical address
		.end   = 0x120024c,
		.flags = 0x200,
	}
};

static struct aml_card_info  amlogic_card_info[] = {
	[0] = {
		.name = "sd_card",
		.work_mode = CARD_HW_MODE,
		.io_pad_type = SDIO_GPIOA_9_14,
#ifdef CONFIG_M3V11   // Enable here For M3 V1.1 PCBA SDCARD Detect differce.
		.card_ins_en_reg = EGPIO_GPIOD_ENABLE,
		.card_ins_en_mask = PREG_IO_11_MASK,
		.card_ins_input_reg = EGPIO_GPIOD_INPUT,
		.card_ins_input_mask = PREG_IO_11_MASK,
		.card_power_en_reg = 0,
		.card_power_en_mask = 0,
		.card_power_output_reg = 0,
		.card_power_output_mask = 0,
		.card_power_en_lev = 0,
		.card_wp_en_reg = EGPIO_GPIOA_ENABLE,
		.card_wp_en_mask = PREG_IO_11_MASK,
		.card_wp_input_reg = EGPIO_GPIOA_INPUT,
		.card_wp_input_mask = PREG_IO_11_MASK,
		.card_extern_init = 0,
#else
		.card_ins_en_reg = EGPIO_GPIOC_ENABLE,
		.card_ins_en_mask = PREG_IO_25_MASK,
		.card_ins_input_reg = EGPIO_GPIOC_INPUT,
		.card_ins_input_mask = PREG_IO_25_MASK,
		.card_power_en_reg = EGPIO_GPIOC_ENABLE,
		.card_power_en_mask = PREG_IO_26_MASK,
		.card_power_output_reg = EGPIO_GPIOC_OUTPUT,
		.card_power_output_mask = PREG_IO_26_MASK,
		.card_power_en_lev = 0,
		.card_wp_en_reg = 0,
		.card_wp_en_mask = 0,
		.card_wp_input_reg = 0,
		.card_wp_input_mask = 0,
		.card_extern_init = 0,
#endif
	},
};

static struct aml_card_platform amlogic_card_platform = {
	.card_num = ARRAY_SIZE(amlogic_card_info),
	.card_info = amlogic_card_info,
};

static struct platform_device amlogic_card_device = { 
	.name = "AMLOGIC_CARD", 
	.id    = -1,
	.num_resources = ARRAY_SIZE(amlogic_card_resource),
	.resource = amlogic_card_resource,
	.dev = {
		.platform_data = &amlogic_card_platform,
	},
};
#endif

#if defined(CONFIG_AML_AUDIO_DSP)
static struct resource audiodsp_resources[] = {
    [0] = {
        .start = AUDIODSP_ADDR_START,
        .end   = AUDIODSP_ADDR_END,
        .flags = IORESOURCE_MEM,
    },
};

static struct platform_device audiodsp_device = {
    .name       = "audiodsp",
    .id         = 0,
    .num_resources = ARRAY_SIZE(audiodsp_resources),
    .resource      = audiodsp_resources,
};
#endif

static struct resource aml_m1_audio_resource[]={
		[0]	=	{
				.start 	=	0,
				.end		=	0,
				.flags	=	IORESOURCE_MEM,
		},
};
static struct platform_device aml_sound_card={
		.name 				= "aml_m1_audio",//"aml_m1_audio_wm8900",
		.id 					= -1,
		.resource 		=	aml_m1_audio_resource,
		.num_resources	=	ARRAY_SIZE(aml_m1_audio_resource),
};

#ifdef CONFIG_AM_NAND
static struct mtd_partition partition_info[] = 
{
	{
		.name = "U-BOOT",
		.offset = 0,
		.size=4*1024*1024,
	//	.set_flags=0,
	//	.dual_partnum=0,
	},
	{
		.name = "Boot Para",
		.offset = 4*1024*1024,
		.size=4*1024*1024,
	//	.set_flags=0,
	//	.dual_partnum=0,
	},
	{
		.name = "recovery",
		.offset = 8*1024*1024,
		.size = 4 * 1024*1024,
	//	.set_flags=0,
	//	.dual_partnum=0,
	},
	{
		.name = "boot",
		.offset = 12*1024*1024,
		.size = 4 * 1024*1024,
	//	.set_flags=0,
	//	.dual_partnum=0,
	},
	{
		.name = "system",
		.offset = 16*1024*1024,
		.size = 128 * 1024*1024,
	//	.set_flags=0,
	//	.dual_partnum=0,
	},
	{
		.name = "userdata",
		.offset=MTDPART_OFS_APPEND,
		.size=MTDPART_SIZ_FULL,
	//	.set_flags=0,
	//	.dual_partnum=0,
	},
//	{	.name="FTL_Part",
//		.offset=MTDPART_OFS_APPEND,
//		.size=MTDPART_SIZ_FULL,
//	//	.set_flags=MTD_AVNFTL,
//	//	.dual_partnum=1,
//	}
};
/*
static struct aml_m1_nand_platform aml_2kpage128kblocknand_platform = {
	.page_size = 2048,
	.spare_size=64,
	.erase_size= 128*1024,
	.bch_mode=1,			//BCH8
	.encode_size=528,
	.timing_mode=5,
	.ce_num=1,
	.onfi_mode=0,
	.partitions = partition_info,
	.nr_partitions = ARRAY_SIZE(partition_info),
};
*/

static struct aml_m1_nand_platform aml_Micron4GBABAnand_platform = 
{
	.page_size = 2048*2,
	.spare_size= 224,		//for micron ABA 4GB
	.erase_size=1024*1024,
	.bch_mode=	  3,		//BCH16
	.encode_size=540,				
	.timing_mode=5,
	.onfi_mode=1,
	.ce_num=1,
	.partitions = partition_info,
	.nr_partitions = ARRAY_SIZE(partition_info),
};

static struct resource aml_nand_resources[] = {
	{
		.start = 0xc1108600,
		.end = 0xc1108624,
		.flags = IORESOURCE_MEM,
	},
};

static struct platform_device aml_nand_device = {
	.name = "aml_m1_nand",
	.id = 0,
	.num_resources = ARRAY_SIZE(aml_nand_resources),
	.resource = aml_nand_resources,
	.dev = {
		.platform_data = &aml_Micron4GBABAnand_platform,
	},
};
#endif

#if defined(CONFIG_I2C_SW_AML)

static struct aml_sw_i2c_platform aml_sw_i2c_plat = {
	.sw_pins = {
		.scl_reg_out		= MESON_I2C_PREG_GPIOA_OUTLVL,
		.scl_reg_in		= MESON_I2C_PREG_GPIOA_INLVL,
		.scl_bit			= 19,	/*MESON_GPIOA_11 + 4*/
		.scl_oe			= MESON_I2C_PREG_GPIOA_OE,
		.sda_reg_out		= MESON_I2C_PREG_GPIOA_OUTLVL,
		.sda_reg_in		= MESON_I2C_PREG_GPIOA_INLVL,
		.sda_bit			= 20,	/*MESON_GPIOA_12 + 4*/
		.sda_oe			= MESON_I2C_PREG_GPIOA_OE,

	},	
	.udelay			= 2,
	.timeout			= 100,
};

static struct platform_device aml_sw_i2c_device = {
	.name		  = "aml-sw-i2c",
	.id		  = -1,
	.dev = {
		.platform_data = &aml_sw_i2c_plat,
	},
};

#endif

#if defined(CONFIG_I2C_AML)
static struct aml_i2c_platform aml_i2c_plat = {
	.wait_count		= 1000000,
	.wait_ack_interval	= 5,
	.wait_read_interval	= 5,
	.wait_xfer_interval	= 5,
	.master_no		= AML_I2C_MASTER_B,
	.use_pio			= 0,
	.master_i2c_speed	= AML_I2C_SPPED_100K,

	.master_b_pinmux = {
		.scl_reg	= MESON_I2C_MASTER_B_GPIOB_0_REG,
		.scl_bit	= MESON_I2C_MASTER_B_GPIOB_0_BIT,
		.sda_reg	= MESON_I2C_MASTER_B_GPIOB_1_REG,
		.sda_bit	= MESON_I2C_MASTER_B_GPIOB_1_BIT,
	}
};

static struct resource aml_i2c_resource[] = {
	[0] = {/*master a*/
		.start = 	MESON_I2C_MASTER_A_START,
		.end   = 	MESON_I2C_MASTER_A_END,
		.flags = 	IORESOURCE_MEM,
	},
	[1] = {/*master b*/
		.start = 	MESON_I2C_MASTER_B_START,
		.end   = 	MESON_I2C_MASTER_B_END,
		.flags = 	IORESOURCE_MEM,
	},
	[2] = {/*slave*/
		.start = 	MESON_I2C_SLAVE_START,
		.end   = 	MESON_I2C_SLAVE_END,
		.flags = 	IORESOURCE_MEM,
	},
};

static struct platform_device aml_i2c_device = {
	.name		  = "aml-i2c",
	.id		  = -1,
	.num_resources	  = ARRAY_SIZE(aml_i2c_resource),
	.resource	  = aml_i2c_resource,
	.dev = {
		.platform_data = &aml_i2c_plat,
	},
};
#endif

#if defined(CONFIG_AM_UART_WITH_S_CORE)

#if defined(CONFIG_AM_UART0_SET_PORT_A)
#define UART_0_PORT		UART_A
#define UART_1_PORT		UART_B
#elif defined(CONFIG_AM_UART0_SET_PORT_B)
#define UART_0_PORT		UART_B
#define UART_1_PORT		UART_A
#endif

static struct aml_uart_platform aml_uart_plat = {
    .uart_line[0]		=	UART_0_PORT,
    .uart_line[1]		=	UART_1_PORT
};

static struct platform_device aml_uart_device = {	
    .name         = "am_uart",  
    .id       = -1, 
    .num_resources    = 0,  
    .resource     = NULL,   
    .dev = {        
                .platform_data = &aml_uart_plat,  
           },
};
#endif

#ifdef CONFIG_ANDROID_PMEM
static struct android_pmem_platform_data pmem_data =
{
	.name = "pmem",
	.start = PMEM_START,
	.size = PMEM_SIZE,
	.no_allocator = 1,
	.cached = 0,
};

static struct platform_device android_pmem_device =
{
	.name = "android_pmem",
	.id = 0,
	.dev = {
		.platform_data = &pmem_data,
	},
};
#endif

#if defined(CONFIG_AML_RTC)
static	struct platform_device aml_rtc_device = {
      		.name            = "aml_rtc",
      		.id               = -1,
	};
#endif


static struct platform_device __initdata *platform_devs[] = {
    #if defined(CONFIG_AM_UART_WITH_S_CORE)
        &aml_uart_device,
    #endif
    #if defined(CONFIG_JPEGLOGO)
		&jpeglogo_device,
	#endif	
    #if defined(CONFIG_FB_AM)
    	&fb_device,
    #endif		
    #if defined(CONFIG_AM_STREAMING)
		&codec_device,
    #endif
    #if defined(CONFIG_AM_VIDEO)
		&deinterlace_device,
    #endif
    #if defined(CONFIG_TVIN_VDIN)
        &vdin_device,
		&bt656in_device,
    #endif
	#if defined(CONFIG_AML_AUDIO_DSP)
		&audiodsp_device,
	#endif
		&aml_sound_card,
	#if defined(CONFIG_CARDREADER)
    	&amlogic_card_device,
    #endif
    #if defined(CONFIG_KEYPADS_AM)||defined(CONFIG_VIRTUAL_REMOTE)||defined(CONFIG_KEYPADS_AM_MODULE)
		&input_device,
    #endif	
    #if defined(CONFIG_ADC_KEYPADS_AM)||defined(CONFIG_ADC_KEYPADS_AM_MODULE)
        &adc_kp_device,
    #endif
    #if defined(CONFIG_KEY_INPUT_CUSTOM_AM) || defined(CONFIG_KEY_INPUT_CUSTOM_AM_MODULE)
		&input_device_key,  //changed by Elvis
    #endif
    #ifdef CONFIG_AM_NAND
		&aml_nand_device,
    #endif		
    #if defined(CONFIG_I2C_SW_AML)
		&aml_sw_i2c_device,
    #endif
    #if defined(CONFIG_I2C_AML)
		&aml_i2c_device,
    #endif

    #if defined(CONFIG_ANDROID_PMEM)
		&android_pmem_device,
    #endif
    #if defined(CONFIG_AML_RTC)
		&aml_rtc_device,
    #endif
};



static void __init eth_pinmux_init(void)
{
	printk("eth pinmux init\n");
	eth_set_pinmux(ETH_BANK2_GPIOD15_D23,ETH_CLK_OUT_GPIOD24_REG5_1,0);		
		
	CLEAR_CBUS_REG_MASK(PREG_ETHERNET_ADDR0, 1);
	SET_CBUS_REG_MASK(PREG_ETHERNET_ADDR0, (1 << 1));
	SET_CBUS_REG_MASK(PREG_ETHERNET_ADDR0, 1);
	udelay(100);
	/*reset*/
	set_gpio_mode(GPIOD_bank_bit2_24(14),GPIOD_bit_bit2_24(14),GPIO_OUTPUT_MODE);
	set_gpio_val(GPIOD_bank_bit2_24(14),GPIOD_bit_bit2_24(14),0);
	udelay(100);	
	set_gpio_val(GPIOD_bank_bit2_24(14),GPIOD_bit_bit2_24(14),1);
}

#ifdef CONFIG_STV
static void __init led_pinmux_init(void)
{

	printk("led pinmux init\n");
#if 1 
	set_gpio_val(PREG_FGPIO,13,0);
	set_gpio_mode(PREG_FGPIO,13,GPIO_OUTPUT_MODE);
	set_gpio_val(PREG_FGPIO,14,1);
	set_gpio_mode(PREG_FGPIO,14,GPIO_OUTPUT_MODE);
	set_gpio_val(PREG_FGPIO,15,1);
	set_gpio_mode(PREG_FGPIO,15,GPIO_OUTPUT_MODE);
#else	
	CLEAR_CBUS_REG_MASK(PREG_FGPIO_EN_N, 1<<14);
	SET_CBUS_REG_MASK(PREG_FGPIO_O, 1<<14);
	CLEAR_CBUS_REG_MASK(PREG_FGPIO_EN_N, 1<<15);
	CLEAR_CBUS_REG_MASK(PREG_FGPIO_O, 1<<15);
#endif
}
#endif

static void __init device_pinmux_init(void )
{
	clearall_pinmux();
#ifdef CONFIG_STV
	led_pinmux_init();
#endif
	/*other deivce power on*/
	/*GPIOA_200e_bit4..usb/eth/YUV power on*/
//	set_gpio_mode(PREG_EGPIO,1<<4,GPIO_OUTPUT_MODE);
//	set_gpio_val(PREG_EGPIO,1<<4,1);
	/* uart port A */
	uart_set_pinmux(UART_PORT_A,UART_A_GPIO_B2_B3);
	/*pinmux of eth*/
	eth_pinmux_init();

	/* IR decoder pinmux */
	set_mio_mux(5, 1<<31);

#ifdef CONFIG_I2C_SW_AML   /*for multak*/
	/* SmartCard pinmux */
	set_mio_mux(2, 0xF<<20);
#endif

	set_audio_pinmux(SPDIF_OUT_GPIOA); //for spdif
}

static void __init  device_clk_setting(void)
{
	/*Demod CLK for eth and sata*/
	demod_apll_setting(0,1200*CLK_1M);
	/*eth clk*/
    	eth_clk_set(ETH_CLKSRC_APLL_CLK,400*CLK_1M,50*CLK_1M);
}
static __init void m1_init_machine(void)
{
#ifdef CONFIG_CACHE_L2X0
		/* 128kb (16KB/way), 8-way associativity, evmon/parity/share disabled
		 * Bits:  .... .... .000 0010 0000 .... .... .... */
		l2x0_init((void __iomem *)IO_PL310_BASE, 0x00020000, 0xff800fff);
#endif
	device_clk_setting();
	device_pinmux_init();
	platform_add_devices(platform_devs, ARRAY_SIZE(platform_devs));

#ifdef CONFIG_USB_DWC_OTG_HCD
	set_usb_phy_clk(USB_PHY_CLOCK_SEL_XTAL_DIV2);
	lm_device_register(&usb_ld_a);
	lm_device_register(&usb_ld_b);
#endif
#ifdef CONFIG_SATA_DWC_AHCI
	set_sata_phy_clk(SATA_PHY_CLOCK_SEL_DEMOD_PLL);
	lm_device_register(&sata_ld);
#endif
}

/*VIDEO MEMORY MAPING*/
static __initdata struct map_desc meson_video_mem_desc[] = {
	{
		.virtual	= PAGE_ALIGN(__phys_to_virt(RESERVED_MEM_START)),
		.pfn		= __phys_to_pfn(RESERVED_MEM_START),
		.length		= RESERVED_MEM_END-RESERVED_MEM_START+1,
		.type		= MT_DEVICE,
	},
};

static __init void m1_map_io(void)
{
	meson_map_io();
	iotable_init(meson_video_mem_desc, ARRAY_SIZE(meson_video_mem_desc));
}

static __init void m1_irq_init(void)
{
	meson_init_irq();
}

static __init void m1_fixup(struct machine_desc *mach, struct tag *tag, char **cmdline, struct meminfo *m)
{
	struct membank *pbank;
	m->nr_banks = 0;
	pbank=&m->bank[m->nr_banks];
	pbank->start = PAGE_ALIGN(PHYS_MEM_START);
	pbank->size  = SZ_64M & PAGE_MASK;
	pbank->node  = PHYS_TO_NID(PHYS_MEM_START);
	m->nr_banks++;
	pbank=&m->bank[m->nr_banks];
	pbank->start = PAGE_ALIGN(RESERVED_MEM_END+1);
	pbank->size  = (PHYS_MEM_END-RESERVED_MEM_END) & PAGE_MASK;
	pbank->node  = PHYS_TO_NID(RESERVED_MEM_END+1);
	m->nr_banks++;
}

MACHINE_START(MESON_8726M, "AMLOGIC MESON-M1 8726M SZ")
	.phys_io		= MESON_PERIPHS1_PHYS_BASE,
	.io_pg_offst	= (MESON_PERIPHS1_PHYS_BASE >> 18) & 0xfffc,
	.boot_params	= BOOT_PARAMS_OFFSET,
	.map_io			= m1_map_io,
	.init_irq		= m1_irq_init,
	.timer			= &meson_sys_timer,
	.init_machine	= m1_init_machine,
	.fixup			= m1_fixup,
	.video_start	= RESERVED_MEM_START,
	.video_end		= RESERVED_MEM_END,
MACHINE_END
