/*
 * Amlogic osd
 * frame buffer driver
 *
 * Copyright (C) 2009 Amlogic, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the named License,
 * or any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA
 *
 * Author:  Tim Yao <timyao@amlogic.com>
 *
 */

#ifndef OSD_MAIN_H
#define OSD_MAIN_H
#include  <linux/list.h>
#include  <linux/vout/vout_notify.h>
#include  <linux/fb.h>




static struct fb_var_screeninfo mydef_var[] = {
{
	.xres            = 800,
	.yres            = 480,
	.xres_virtual    = 800,
	.yres_virtual    = 960,
	.xoffset         = 0,
	.yoffset         = 0,
	.bits_per_pixel  = 16,
	.grayscale       = 0,
	.red             = {0, 0, 0},
	.green           = {0, 0, 0},
	.blue            = {0, 0, 0},
	.transp          = {0, 0, 0},
	.nonstd          = 0,
	.activate        = FB_ACTIVATE_NOW,
	.height          = -1,
	.width           = -1,
	.accel_flags     = 0,
	.pixclock        = 0,
	.left_margin     = 0,
	.right_margin    = 0,
	.upper_margin    = 0,
	.lower_margin    = 0,
	.hsync_len       = 0,
	.vsync_len       = 0,
	.sync            = 0,
	.vmode           = FB_VMODE_NONINTERLACED,
	.rotate          = 0,
	
},
{
	.xres            = 720,
	.yres            = 576,
	.xres_virtual    = 720,
	.yres_virtual    = 1152,
	.xoffset         = 0,
	.yoffset         = 0,
	.bits_per_pixel  = 8,
	.grayscale       = 0,
	.red             = {0, 0, 0},  //leave as it is ,set by system.
	.green           = {0, 0, 0},
	.blue            = {0, 0, 0},
	.transp          = {0, 0, 0},
	.nonstd          = 0,
	.activate        = FB_ACTIVATE_NOW,
	.height          = -1,
	.width           = -1,
	.accel_flags     = 0,
	.pixclock        = 0,
	.left_margin     = 0,
	.right_margin    = 0,
	.upper_margin    = 0,
	.lower_margin    = 0,
	.hsync_len       = 0,
	.vsync_len       = 0,
	.sync            = 0,
	.vmode           = FB_VMODE_NONINTERLACED,
	.rotate          = 0,
}
};


static struct fb_fix_screeninfo mydef_fix = {
	.id		    = "OSD FB",
	.xpanstep 	= 1,
	.ypanstep 	= 1,
	.type		= FB_TYPE_PACKED_PIXELS,
	.visual		= FB_VISUAL_TRUECOLOR,
	.accel		= FB_ACCEL_NONE,
};

typedef  struct {
	int x ;
	int y ;
	int w ;
	int h ;
}disp_rect_t;
#define DRIVER_NAME "osdfb"
#define MODULE_NAME "osdfb"
#define  FBIOPUT_OSD_SRCCOLORKEY	0x46fb
#define  FBIOPUT_OSD_SRCKEY_ENABLE	0x46fa
#define  FBIOPUT_OSD_SET_GBL_ALPHA	0x4500
#define  FBIOGET_OSD_GET_GBL_ALPHA	0x4501
#define  FBIOPUT_OSD_2X_SCALE		0x4502	








#endif /* OSD_MAIN_H */
