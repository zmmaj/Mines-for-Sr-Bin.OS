/*
 * Copyright (c) 2020 Jiri Svoboda
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * - Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 * - The name of the author may not be used to endorse or promote products
 *   derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/** @addtogroup mines
 * @{
 */
/**
 * @file Hello world (in UI)
 */

#ifndef MINES_H
#define MINES_H

#include <display.h>
#include <ui/fixed.h>
#include <ui/label.h>
#include <ui/ui.h>
#include <ui/window.h>
#include <gfx/coord.h>
#include <ui/entry.h>
#include <ui/menu.h>
#include <ui/menubar.h>
#include <ui/menudd.h>
#include <ui/menuentry.h>
#include <ui/pbutton.h>
#include <ui/label.h>
#include <ui/resource.h>

/** Dimensions. Most of this should not be needed with auto layout */
typedef struct {
	gfx_rect_t menubar_rect;
	gfx_rect_t entry_rect;
	gfx_coord2_t btn_orig;
	gfx_coord2_t btn_stride;
	gfx_coord2_t btn_dim;
} mines_geom_t;

/** Hello world UI application */
typedef struct {
	ui_t *ui;
	ui_window_t *window;
	ui_fixed_t *fixed;
	ui_label_t *label;
	ui_resource_t *ui_res;
	ui_pbutton_t *btn_0; ui_pbutton_t *btn_1; ui_pbutton_t *btn_2; ui_pbutton_t *btn_3;  ui_pbutton_t *btn_4;  ui_pbutton_t *btn_5;
	ui_pbutton_t *btn_6; ui_pbutton_t *btn_7; ui_pbutton_t *btn_8; ui_pbutton_t *btn_9; ui_pbutton_t *btn_10; ui_pbutton_t *btn_11;
    ui_pbutton_t *btn_12;ui_pbutton_t *btn_13; ui_pbutton_t *btn_14;ui_pbutton_t *btn_15;ui_pbutton_t *btn_16; ui_pbutton_t *btn_17;
    ui_pbutton_t *btn_18; ui_pbutton_t *btn_19; ui_pbutton_t *btn_20; ui_pbutton_t *btn_21;ui_pbutton_t *btn_22;ui_pbutton_t *btn_23; 
	ui_pbutton_t *btn_24; ui_pbutton_t *btn_25; ui_pbutton_t *btn_26; ui_pbutton_t *btn_27; ui_pbutton_t *btn_28;ui_pbutton_t *btn_29;
	ui_pbutton_t *btn_30; ui_pbutton_t *btn_31; ui_pbutton_t *btn_32; ui_pbutton_t *btn_33; ui_pbutton_t *btn_34; ui_pbutton_t *btn_35; 
	ui_menu_bar_t *menubar;
	mines_geom_t geom;
} mines_t;

#endif

/** @}
 */
