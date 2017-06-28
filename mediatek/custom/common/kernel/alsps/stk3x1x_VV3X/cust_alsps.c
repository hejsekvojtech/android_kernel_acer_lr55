/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */
/* MediaTek Inc. (C) 2010. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
 * AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
 * SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 * The following software/firmware and/or related documentation ("MediaTek Software")
 * have been modified by MediaTek Inc. All revisions are subject to any receiver's
 * applicable license agreements with MediaTek Inc.
 */

#include <linux/types.h>
#include <cust_alsps.h>
#ifdef MT6573
#include <mach/mt6573_pll.h>
#endif
#ifdef MT6575
#include <mach/mt6575_pm_ldo.h>
#endif
#ifdef MT6577
#include <mach/mt6577_pm_ldo.h>
#endif
#ifdef MT6589
#include <mach/mt_pm_ldo.h>
#endif
//VV38 porting for STK3310 start
#include <mach/mt_pm_ldo.h>
//VV38 porting for STK3310 end
static struct alsps_hw cust_alsps_hw = {
    .i2c_num    = 2,		/* i2c bus number, for mt657x, default=0. For mt6589, default=3 */
	//.polling_mode =1,
	.polling_mode_ps =0,//0: interrupt mode
	.polling_mode_als =1,
    .power_id   = MT65XX_POWER_NONE,    /*LDO is not used*/
    .power_vol  = VOL_DEFAULT,          /*LDO is not used*/
    .i2c_addr   = {0x90, 0x00, 0x00, 0x00},	/*STK3x1x*/
	//.als_level  = {5,  9, 36, 59, 82, 132, 205, 273, 500, 845, 1136, 1545, 2364, 4655, 6982},	/* als_code */
    //.als_value  = {0, 10, 40, 65, 90, 145, 225, 300, 550, 930, 1250, 1700, 2600, 5120, 7680, 10240},    /* lux */
	//STK add for CCI start 20130113
    //.als_level  = {10, 40, 65, 90, 145, 225, 300, 550, 930, 1250, 1700, 2600, 5120, 7680, 10240},	// als_code for CCI VV01 TP transmittance = 10%
	//.als_value  = {0, 10, 40, 65, 90, 145, 225, 300, 550, 930, 1250, 1700, 2600, 5120, 7680, 10240},    /* lux */
	    .als_level  = {18, 81, 315, 990, 3150, 5040, 6030, 7830},	//for VV38, set table as same as C38 doc, als_level - 10%
	.als_value  = {0, 20, 90, 350, 1100, 3500, 5600, 6700, 8700},    //for VV38, set table as same as C38 doc
	//STK add for CCI end 20130113
   	.state_val = 0x0,		/* disable all */
	.psctrl_val = 0x33, //0x73,//VV01, CCI=0x73		/* ps_persistance=4, ps_gain=64X, PS_IT=0.391ms */
	.alsctrl_val = 0x39,	/* als_persistance=1, als_gain=64X, ALS_IT=100ms */
	.ledctrl_val = 0xFF,	/* 50mA IRDR, 64/64 LED duty */
	.wait_val = 0x9,		/* 50 ms */
    .ps_high_thd_val = 1500,
    .ps_low_thd_val = 1300,
};
struct alsps_hw *get_cust_alsps_hw(void) {
    return &cust_alsps_hw;
}
