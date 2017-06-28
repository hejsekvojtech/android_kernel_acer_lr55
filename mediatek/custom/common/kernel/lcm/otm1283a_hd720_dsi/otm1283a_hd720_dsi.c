/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2008
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
*  RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
*****************************************************************************/
#ifndef BUILD_LK
#include <linux/string.h>
#endif
#include "lcm_drv.h"

#ifdef BUILD_LK
	#include <platform/mt_gpio.h>
#elif defined(BUILD_UBOOT)
	#include <asm/arch/mt_gpio.h>
#else
	#include <mach/mt_gpio.h>
#endif

#ifdef BUILD_LK
#define LCM_PRINT printf
#else
#if defined(BUILD_UBOOT)
#define LCM_PRINT printf
#else
#define LCM_PRINT printk
#endif
#endif

#define LCM_ID_OTM1283 (0x1283)

#define LCM_DBG(fmt, arg...) \
	LCM_PRINT ("[LCM-OTM1383A-DSI-VDO] %s (line:%d) :" fmt "\r\n", __func__, __LINE__, ## arg)

// ---------------------------------------------------------------------------
//  Local Constants
// ---------------------------------------------------------------------------
#define LCM_DSI_CMD_MODE									0

#define FRAME_WIDTH  										(720)
#define FRAME_HEIGHT 										(1280)

#define REGFLAG_DELAY             							0XFE
#define REGFLAG_END_OF_TABLE      							0xdd   // END OF REGISTERS MARKER


#ifndef TRUE
    #define TRUE 1
#endif

#ifndef FALSE
    #define FALSE 0
#endif

static unsigned int lcm_esd_test = FALSE;      ///only for ESD test

// ---------------------------------------------------------------------------
//  Local Variables
// ---------------------------------------------------------------------------

static LCM_UTIL_FUNCS lcm_util = {0};

#define SET_RESET_PIN(v)    								(lcm_util.set_reset_pin((v)))

#define UDELAY(n) 											(lcm_util.udelay(n))
#define MDELAY(n) 											(lcm_util.mdelay(n))


// ---------------------------------------------------------------------------
//  Local Functions
// ---------------------------------------------------------------------------

#define dsi_set_cmdq_V2(cmd, count, ppara, force_update)	        lcm_util.dsi_set_cmdq_V2(cmd, count, ppara, force_update)
#define dsi_set_cmdq(pdata, queue_size, force_update)		lcm_util.dsi_set_cmdq(pdata, queue_size, force_update)
#define wrtie_cmd(cmd)										lcm_util.dsi_write_cmd(cmd)
#define write_regs(addr, pdata, byte_nums)					lcm_util.dsi_write_regs(addr, pdata, byte_nums)
#define read_reg(cmd)											lcm_util.dsi_dcs_read_lcm_reg(cmd)
#define read_reg_v2(cmd, buffer, buffer_size)   				lcm_util.dsi_dcs_read_lcm_reg_v2(cmd, buffer, buffer_size)    

struct LCM_setting_table {
    unsigned char cmd;
    unsigned char count;
    unsigned char para_list[64];
};

extern void set_HS_read(void);
extern void restore_HS_read(void);


static struct LCM_setting_table lcm_initialization_setting[] = {

    {0x00,1,{0x00}},
    {0xff,3,{0x12,0x83,0x01}},	//EXTC=1
    {0x00,1,{0x80}},	            //Orise mode enable
    {0xff,2,{0x12,0x83}},

    {0x00,1,{0x92}},	            //Orise mode enable
    {0xff,2,{0x30,0x02}}, 

    {0x00,1,{0x80}},             //TCON Setting
    {0xc0,9,{0x00,0x64,0x00,0x0f,0x11,0x00,0x64,0x0f,0x11}},

    {0x00,1,{0x90}},             //Panel Timing Setting
    {0xc0,6,{0x00,0x55,0x00,0x01,0x00,0x04}},

    {0x00,1,{0xa4}},
    {0xc0,1,{0x00}}, 

    {0x00,1,{0xb3}},             //Interval Scan Frame: 0 frame, column inversion
    {0xc0,2,{0x00,0x50}},  //0x50=colume

    {0x00,1,{0x81}},             //frame rate:60Hz
    {0xc1,1,{0x66}},

    {0x00,1,{0x90}},
    {0xc4,1,{0x49}},

    {0x00,1,{0x90}},             //Mode-3
    {0xf5,4,{0x02,0x11,0x02,0x11}},

    {0x00,1,{0x90}},             //2xVPNL, 1.5*=00, 2*=50, 3*=a0
    {0xc5,1,{0x50}},

    {0x00,1,{0x94}},             //Frequency
    {0xc5,1,{0x66}},

    {0x00,1,{0xb2}},             //VGLO1 setting
    {0xf5,2,{0x00,0x00}},

    {0x00,1,{0xb4}},             //VGLO1_S setting
    {0xf5,1,{0x00,0x00}},

    {0x00,1,{0xb6}},             //VGLO2 setting
    {0xf5,2,{0x00,0x00}},

    {0x00,1,{0xb8}},             //VGLO2_S setting
    {0xf5,2,{0x00,0x00}},

    {0x00,1,{0x94}},             //VCL ON
    {0xf5,1,{0x02}},

    {0x00,1,{0xBA}},             //VSP ON
    {0xf5,1,{0x03}},


    {0x00,1,{0xb4}},             //VGLO Option
    {0xc5,1,{0xC0}},


    {0x00,1,{0xa0}},             //dcdc setting
    {0xc4,14,{0x05,0x10,0x06,0x02,0x05,0x15,0x10,0x05,0x10,0x07,0x02,0x05,0x15,0x10}},

    {0x00,1,{0xb0}},             //clamp voltage setting
    {0xc4,2,{0x00,0x00}},

    {0x00,1,{0x91}},             //VGH=13V, VGL=-12V, pump ratio:VGH=6x, VGL=-5x
    {0xc5,2,{0x19,0x50}},

    {0x00,1,{0x00}},             //GVDD=4.87V, NGVDD=-4.87V
    {0xd8,2,{0xa5,0xa5}},

    {0x00,1,{0x00}},             //VCOMDC=-1.1
    {0xd9,1,{0x72}},  //5d  6f

    {0x00,1,{0x00}},
    {0xE1,16,{0x08,0x10,0x16,0x0d,0x06,0x13,0x0c,0x0b,0x02,0x06,0x0a,0x07,0x0f,0x0e,0x08,0x00}},

    {0x00,1,{0x00}},
    {0xE2,16,{0x08,0x10,0x16,0x0d,0x06,0x13,0x0c,0x0b,0x02,0x06,0x0a,0x07,0x0f,0x0e,0x08,0x00}},

    {0x00,1,{0xb0}},             //VDD_18V=1.7V, LVDSVDD=1.55V
    {0xc5,2,{0x04,0xB8}},

    {0x00,1,{0xbb}},             //LVD voltage level setting
    {0xc5,1,{0x80}},

    {0x00,1,{0xb2}},             //LVD voltage level setting
    {0xc5,1,{0x40}},

    {0x00,1,{0x00}},             
    {0xd0,1,{0x40}},
    {0x00,1,{0x00}},             
    {0xd1,2,{0x00,0x00}},

    {0x00,1,{0x81}},             
    {0xc4,2,{0x84,0x02}},

    {0x00,1,{0xc6}},             
    {0xb0,1,{0x03}},


    {0x00,1,{0x80}},             //panel timing state control
    {0xcb,11,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},

    {0x00,1,{0x90}},             //panel timing state control
    {0xcb,15,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},

    {0x00,1,{0xa0}},             //panel timing state control
    {0xcb,15,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},

    {0x00,1,{0xb0}},             //panel timing state control
    {0xcb,15,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},

    {0x00,1,{0xc0}},             //panel timing state control
    {0xcb,15,{0x05,0x05,0x05,0x05,0x05,0x05,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},

    {0x00,1,{0xd0}},             //panel timing state control
    {0xcb,15,{0x00,0x00,0x00,0x00,0x00,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x00,0x00}},

    {0x00,1,{0xe0}},             //panel timing state control
    {0xcb,14,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x05,0x05}},

    {0x00,1,{0xf0}},             //panel timing state control
    {0xcb,11,{0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff}},

    {0x00,1,{0x80}},             //panel pad mapping control
    {0xcc,15,{0x09,0x0b,0x0d,0x0f,0x01,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},

    {0x00,1,{0x90}},             //panel pad mapping control
    {0xcc,15,{0x00,0x00,0x00,0x00,0x00,0x2e,0x2d,0x0a,0x0c,0x0e,0x10,0x02,0x04,0x00,0x00}},

    {0x00,1,{0xa0}},             //panel pad mapping control
    {0xcc,14,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x2e,0x2d}},

    {0x00,1,{0xb0}},             //panel pad mapping control
    {0xcc,15,{0x10,0x0e,0x0c,0x0a,0x04,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},

    {0x00,1,{0xc0}},             //panel pad mapping control
    {0xcc,15,{0x00,0x00,0x00,0x00,0x00,0x2d,0x2e,0x0f,0x0d,0x0b,0x09,0x03,0x01,0x00,0x00}},

    {0x00,1,{0xd0}},             //panel pad mapping control
    {0xcc,14,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x2d,0x2e}},

    {0x00,1,{0x80}},             //panel VST setting
    {0xce,12,{0x8b,0x03,0x00,0x8a,0x03,0x00,0x89,0x03,0x00,0x88,0x03,0x00}},

    {0x00,1,{0x90}},             //panel VEND setting
    {0xce,14,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},

    {0x00,1,{0xa0}},             //panel CLKA1/2 setting
    {0xce,14,{0x38,0x07,0x84,0xFC,0x8b,0x04,0x00,0x38,0x06,0x84,0xFD,0x8b,0x04,0x00}},

    {0x00,1,{0xb0}},             //panel CLKA3/4 setting
    {0xce,14,{0x38,0x05,0x84,0xFe,0x8b,0x04,0x00,0x38,0x04,0x84,0xFf,0x8b,0x04,0x00}},

    {0x00,1,{0xc0}},             //panel CLKb1/2 setting
    {0xce,14,{0x38,0x03,0x85,0x00,0x8b,0x04,0x00,0x38,0x02,0x85,0x01,0x8b,0x04,0x00}},

    {0x00,1,{0xd0}},             //panel CLKb3/4 setting
    {0xce,14,{0x38,0x01,0x85,0x02,0x8b,0x04,0x00,0x38,0x00,0x85,0x03,0x8b,0x04,0x00}},

    {0x00,1,{0x80}},             //panel CLKc1/2 setting
    {0xcf,14,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},

    {0x00,1,{0x90}},             //panel CLKc3/4 setting
    {0xcf,14,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},

    {0x00,1,{0xa0}},             //panel CLKd1/2 setting
    {0xcf,14,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},

    {0x00,1,{0xb0}},             //panel CLKd3/4 setting
    {0xcf,14,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},

    {0x00,1,{0xc0}},             //panel ECLK setting
    {0xcf,11,{0x01,0x01,0x20,0x20,0x00,0x00,0x01,0x01,0x00,0x02,0x02}},

    {0x00,1,{0xb5}},             //TCON_GOA_OUT Setting
    {0xc5,6,{0x33,0xf1,0xff,0x33,0xf1,0xff}},  //normal output with VGH/VGL

    {0x00,1,{0xa0}},
    {0xc1,1,{0x02}},

    {0x00,1,{0x87}},
    {0xc4,1,{0x08}},

    {0x00,1,{0x89}},
    {0xc4,1,{0x08}},

    //0tm1283  sumsung CE�\u0160��\u017d�

    {0x00, 1, {0xa0}},
    {0xD6, 12, {0x01,0x1a,0x01,0x1a,0x01,0xe6,0x01,0xb3,0x01,0xb3,0x01,0xb3}},
    {0x00, 1, {0xb0}},
    {0xD6, 12, {0x01,0xb3,0x01,0xCD,0x01,0xCD,0x01,0xCD,0x01,0xCD,0x01,0x66}},
    {0x00, 1, {0xc0}},
    {0xD6, 12, {0x66,0x11,0x11,0x9a,0x11,0x77,0x77,0x11,0x77,0x77,0x11,0x89}},
    {0x00, 1, {0xd0}},
    {0xD6, 6, {0x89,0x11,0x89,0x89,0x11,0x44}},
    {0x00, 1, {0xe0}},
    {0xD6, 12, {0x33,0x11,0x44,0x4d,0x11,0x3c,0x3c,0x11,0x3c,0x3c,0x11,0x44}},
    {0x00, 1, {0xf0}},
    {0xD6, 6, {0x44,0x11,0x44,0x44,0x11,0x22}},
    {0x00, 1, {0x00}},
    {0x55, 1, {0x90}}, //0xb0 -high   0x90-mid  //color effect

    {0x00,1,{0x00}},             //Orise mode disable
    {0xff,1,{0xff,0xff,0xff}},

    {0x11,0,{ 0x00 }},
    {REGFLAG_DELAY, 120, {}},

    {0x29,0,{ 0x00 }},
    {REGFLAG_DELAY, 50, {}},	
};


static struct LCM_setting_table lcm_sleep_out_setting[] = {
	//Normal mode on
	//{0x13, 1, {0x00}},
	//{REGFLAG_DELAY,20,{}},
    // Sleep Out
	{0x11, 0, {0x00}},
    {REGFLAG_DELAY, 120, {}},
    // Display ON
	{0x29, 0, {0x00}},
	{REGFLAG_END_OF_TABLE, 0x00, {}}
};


static struct LCM_setting_table lcm_deep_sleep_mode_in_setting[] = {
	//All pixel off
	//{0x22, 1, {0x00}},
	//{REGFLAG_DELAY,50,{}},
	// Display off sequence
	{0x28, 0, {0x00}},
	{REGFLAG_DELAY,20,{}},
    // Sleep Mode On
	{0x10, 0, {0x00}},
    {REGFLAG_DELAY,120,{}},
	{REGFLAG_END_OF_TABLE, 0x00, {}}
};

static struct LCM_setting_table lcm_compare_id_setting[] = {
	// Display off sequence
	{0xB9,	3,	{0xFF, 0x83, 0x69}},
	{REGFLAG_DELAY, 10, {}},

    // Sleep Mode On
//	{0xC3, 1, {0xFF}},

	{REGFLAG_END_OF_TABLE, 0x00, {}}
};

static struct LCM_setting_table lcm_backlight_level_setting[] = {
	{0x51, 1, {0xFF}},
	{REGFLAG_END_OF_TABLE, 0x00, {}}
};


static void push_table(struct LCM_setting_table *table, unsigned int count, unsigned char force_update)
{
	unsigned int i;
    for(i = 0; i < count; i++) {	
        unsigned cmd;
        cmd = table[i].cmd;
        switch (cmd) {
            case REGFLAG_DELAY :
                MDELAY(table[i].count);
                break;
            case REGFLAG_END_OF_TABLE :
                break;
            default:
		  dsi_set_cmdq_V2(cmd, table[i].count, table[i].para_list, force_update);
       	}
    }
	
}

// ---------------------------------------------------------------------------
//  LCM Driver Implementations
// ---------------------------------------------------------------------------

static void lcm_set_util_funcs(const LCM_UTIL_FUNCS *util)
{
    memcpy(&lcm_util, util, sizeof(LCM_UTIL_FUNCS));
}


static void lcm_get_params(LCM_PARAMS *params)
{

		memset(params, 0, sizeof(LCM_PARAMS));
	
		params->type   = LCM_TYPE_DSI;

		params->width  = FRAME_WIDTH;
		params->height = FRAME_HEIGHT;
	    params->physical_height = 110; //LCM xdpi ydpi setting
	    params->physical_width = 62;
        
		// enable tearing-free
		params->dbi.te_mode 				= LCM_DBI_TE_MODE_VSYNC_ONLY;
		params->dbi.te_edge_polarity		= LCM_POLARITY_RISING;

        #if (LCM_DSI_CMD_MODE)
		params->dsi.mode   = CMD_MODE;
        #else  
        params->dsi.mode   = SYNC_PULSE_VDO_MODE;
		//params->dsi.mode   = BURST_VDO_MODE;
        #endif
	
		// DSI
		/* Command mode setting */
		//1 Three lane or Four lane
		params->dsi.LANE_NUM				= LCM_FOUR_LANE;
		//The following defined the fomat for data coming from LCD engine.
		params->dsi.data_format.color_order = LCM_COLOR_ORDER_RGB;
		params->dsi.data_format.trans_seq   = LCM_DSI_TRANS_SEQ_MSB_FIRST;
		params->dsi.data_format.padding     = LCM_DSI_PADDING_ON_LSB;
		params->dsi.data_format.format      = LCM_DSI_FORMAT_RGB888;

		// Highly depends on LCD driver capability.
		// Not support in MT6573
		params->dsi.packet_size=256;

		// Video mode setting		
		params->dsi.intermediat_buffer_num = 0;//because DSI/DPI HW design change, this parameters should be 0 when video mode in MT658X; or memory leakage

		params->dsi.PS=LCM_PACKED_PS_24BIT_RGB888;
		params->dsi.word_count=720*3;	

        params->dsi.vertical_sync_active				= 4;	//2;
        params->dsi.vertical_backporch					= 16;//40;	//14;
        params->dsi.vertical_frontporch					= 20;//40;	//16;
        params->dsi.vertical_active_line				= FRAME_HEIGHT; 

        params->dsi.horizontal_sync_active				= 4;	//2;
        params->dsi.horizontal_backporch				= 50;	//60;	//42;
        params->dsi.horizontal_frontporch				= 50;	//60;	//44;
        params->dsi.horizontal_active_pixel				= FRAME_WIDTH;

        // Bit rate calculation
        //1 Every lane speed
        /*params->dsi.pll_div1=0;		// div1=0,1,2,3;div1_real=1,2,4,4 ----0: 546Mbps  1:273Mbps
        params->dsi.pll_div2=1;		// div2=0,1,2,3;div1_real=1,2,4,4	
        params->dsi.fbk_div =16; */   // fref=26MHz, fvco=fref*(fbk_div+1)*2/(div1_real*div2_real)	

        params->dsi.PLL_CLOCK = 208;//312		(h+hsyc+hfp+hbp)*(w+vsync+vfp+vhp)*24*60/lanes/2
        // Spread Spectrum Clocking setting (MT6582 new added)
        params->dsi.ssc_disable = 0; // 0:enable, 1:disable
        params->dsi.ssc_range   = 3; // Range: 1..8 (default: 5)

}

static unsigned int lcm_compare_id(void)
{
    unsigned int id=0, id1 = 0,id2 = 0;
    unsigned int check_esd = 0;
    unsigned int array[16]; 
    unsigned char buffer[5];
    unsigned char buffer1[5];

    array[0] = 0x00023700;// read id return 4 bytes,version and id
    dsi_set_cmdq(array, 1, 1);
    MDELAY(10);	
    read_reg_v2(0x0A, buffer1, 2); 
    printk("lcm_compare_id 0x0A read id= 0x%x\n",buffer1[0]);

    array[0] = 0x00043700;// read id return 4 bytes,version and id
    dsi_set_cmdq(array, 1, 1);
    MDELAY(10);	
    read_reg_v2(0xA1, buffer, 4);
    id1 = buffer[2];
    id2 = buffer[3];
    id = (id1<<8 | id2);
    printk("lcm_compare_id read id0=0x%x, id1=0x%x, id2=0x%x id3=0x%x\n" ,buffer[0], buffer[1],buffer[2],buffer[3]);

    return (LCM_ID_OTM1283 == id)?1:0;
}                                     

static void lcm_init(void)
{
    SET_RESET_PIN(1);
     MDELAY(1);
    SET_RESET_PIN(0);
    MDELAY(1);
    SET_RESET_PIN(1);
    MDELAY(120);
    //MDELAY(20);
    push_table(lcm_initialization_setting, sizeof(lcm_initialization_setting) / sizeof(struct LCM_setting_table), 1);
    
}


static void lcm_suspend(void)
{
	LCM_DBG("lcm_suspend");
    
	push_table(lcm_deep_sleep_mode_in_setting, sizeof(lcm_deep_sleep_mode_in_setting) / sizeof(struct LCM_setting_table), 1);
	
	/*SET_RESET_PIN(1);
    SET_RESET_PIN(0);
    MDELAY(10);
    SET_RESET_PIN(1);
    MDELAY(120);*/
}


static void lcm_resume(void)
{
	lcm_init();

	//push_table(lcm_sleep_out_setting, sizeof(lcm_sleep_out_setting) / sizeof(struct LCM_setting_table), 1);
}


static void lcm_update(unsigned int x, unsigned int y,
                       unsigned int width, unsigned int height)
{
	unsigned int x0 = x;
	unsigned int y0 = y;
	unsigned int x1 = x0 + width - 1;
	unsigned int y1 = y0 + height - 1;

	unsigned char x0_MSB = ((x0>>8)&0xFF);
	unsigned char x0_LSB = (x0&0xFF);
	unsigned char x1_MSB = ((x1>>8)&0xFF);
	unsigned char x1_LSB = (x1&0xFF);
	unsigned char y0_MSB = ((y0>>8)&0xFF);
	unsigned char y0_LSB = (y0&0xFF);
	unsigned char y1_MSB = ((y1>>8)&0xFF);
	unsigned char y1_LSB = (y1&0xFF);

	unsigned int data_array[16];

	data_array[0]= 0x00053902;
	data_array[1]= (x1_MSB<<24)|(x0_LSB<<16)|(x0_MSB<<8)|0x2a;
	data_array[2]= (x1_LSB);
	data_array[3]= 0x00053902;
	data_array[4]= (y1_MSB<<24)|(y0_LSB<<16)|(y0_MSB<<8)|0x2b;
	data_array[5]= (y1_LSB);
	data_array[6]= 0x002c3909;

	dsi_set_cmdq(data_array, 7, 0);

}


static void lcm_setbacklight(unsigned int level)
{
	unsigned int default_level = 145;
	unsigned int mapped_level = 0;

	//for LGE backlight IC mapping table
	if(level > 255) 
			level = 255;

	if(level >0) 
			mapped_level = default_level+(level)*(255-default_level)/(255);
	else
			mapped_level=0;

	// Refresh value of backlight level.
	lcm_backlight_level_setting[0].para_list[0] = mapped_level;

	push_table(lcm_backlight_level_setting, sizeof(lcm_backlight_level_setting) / sizeof(struct LCM_setting_table), 1);
}

static unsigned int lcm_esd_check(void)
{
	unsigned int ret=FALSE;
#ifndef BUILD_LK	
	char  buffer[6];
	int   array[4];
	char esd1,esd2,esd3,esd4;
        if(lcm_esd_test)
        {
            lcm_esd_test = FALSE;
            return TRUE;
        }
        
    set_HS_read();
    
	array[0] = 0x00023700;
	dsi_set_cmdq(array, 1, 1);

	read_reg_v2(0x0A, buffer, 2);
	esd1=buffer[0];
	
	array[0] = 0x00023700;
	dsi_set_cmdq(array, 1, 1);
	read_reg_v2(0x0B, buffer, 2);
	esd2=buffer[0];

    array[0] = 0x00023700;
	dsi_set_cmdq(array, 1, 1);
	read_reg_v2(0x0C, buffer, 2);
	esd3=buffer[0];

	array[0] = 0x00023700;
	dsi_set_cmdq(array, 1, 1);	
	read_reg_v2(0x0D, buffer, 2);
	esd4=buffer[0];
    
    restore_HS_read();
	LCM_DBG("lcm_esd_check esd1(%x) esd2(%x) esd3(%x) esd4(%x)",esd1,esd2,esd3,esd4);

	/*array[0] = 0x00023700;
	dsi_set_cmdq(array, 1, 1);	
	read_reg_v2(0x0E, buffer, 2);
	esd4=buffer[0];*/
	if(esd1==0x9C && esd2==0 && esd3==0x07 && esd4==0)
	{
		ret=FALSE;
	}
	else
	{			 
		ret=TRUE;
	}
#endif
	return ret;
}

static unsigned int lcm_esd_recover(void)
{
    SET_RESET_PIN(1);
    MDELAY(10);
    SET_RESET_PIN(0);
    MDELAY(10);
    SET_RESET_PIN(1);
    MDELAY(120);
	push_table(lcm_initialization_setting, sizeof(lcm_initialization_setting) / sizeof(struct LCM_setting_table), 1);
    MDELAY(10);

    LCM_DBG("lcm_esd_recover enter");
    return TRUE;
}

// ---------------------------------------------------------------------------
//  Get LCM Driver Hooks
// ---------------------------------------------------------------------------
LCM_DRIVER otm1283a_hd720_dsi_lcm_drv = 
{
    .name			= "otm1283a_hd720_dsi",
	.set_util_funcs = lcm_set_util_funcs,
	.get_params     = lcm_get_params,
	.init           = lcm_init,
	.suspend        = lcm_suspend,
	.resume         = lcm_resume,
	//.compare_id    = lcm_compare_id,
	.esd_check   = lcm_esd_check,
	.esd_recover   = lcm_esd_recover,
#if (LCM_DSI_CMD_MODE)
	.update         = lcm_update,
//.set_backlight	= lcm_setbacklight,
//	.set_pwm        = lcm_setpwm,
//	.get_pwm        = lcm_getpwm,
#endif
};
