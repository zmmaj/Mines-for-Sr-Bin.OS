/*
 * Copyright (c) 2024 Zmaj from ZmajSoft
 * This  is A GAME, nothing else,
 * written for SR-Bin OS.
 * Sr-Bin Is rewriten Branch of Helen.OS
 * many commands are on Serbian, since is olmost whole SR-Bin.OS on Serbian.
 * Kernel is whole from HELEN.OS, so this game you can start and there.  

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
/** @file  Simple game
 *
 * Inspired by the old C code for Mines
 *
 */

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <str.h>
#include <ui/entry.h>
#include <ui/fixed.h>
#include <ui/menu.h>
#include <ui/menubar.h>
#include <ui/menudd.h>
#include <ui/menuentry.h>
#include <ui/pbutton.h>
#include <ui/promptdialog.h>
#include <ui/msgdialog.h>
#include <ui/ui.h>
#include <ui/window.h>
// In future I have intention to put and some sound effect.. I will see.. So, get and This below.
#include <hound/client.h>

extern void ui_msg_dialog_params_init(ui_msg_dialog_params_t *);
extern errno_t ui_msg_dialog_create(ui_t *, ui_msg_dialog_params_t *, ui_msg_dialog_t **);
extern void ui_msg_dialog_set_cb(ui_msg_dialog_t *, ui_msg_dialog_cb_t *, void *);
extern void ui_msg_dialog_destroy(ui_msg_dialog_t *);

#define NAME  "mines"
#define NULL_DISPLAY  "0"


/** Dimensions. Most of this should not be needed with auto layout */
typedef struct {
	gfx_rect_t menubar_rect;
	gfx_rect_t entry_rect;
	gfx_coord2_t btn_orig;
	gfx_coord2_t btn_stride;
	gfx_coord2_t btn_dim;
} mine_geom_t;

typedef struct {
	ui_t *ui;
	ui_resource_t *ui_res;
	ui_entry_t *entry;
	ui_window_t *window;
	ui_pbutton_t *btn_0;ui_pbutton_t *btn_1;ui_pbutton_t *btn_2;ui_pbutton_t *btn_3;ui_pbutton_t *btn_4;
	ui_pbutton_t *btn_5;ui_pbutton_t *btn_6;ui_pbutton_t *btn_7;ui_pbutton_t *btn_8;ui_pbutton_t *btn_9;
	ui_pbutton_t *btn_10;ui_pbutton_t *btn_11;ui_pbutton_t *btn_12;ui_pbutton_t *btn_13;ui_pbutton_t *btn_14;
	ui_pbutton_t *btn_15;ui_pbutton_t *btn_16;ui_pbutton_t *btn_17;ui_pbutton_t *btn_18;ui_pbutton_t *btn_19;
	ui_pbutton_t *btn_20;ui_pbutton_t *btn_21;ui_pbutton_t *btn_22;ui_pbutton_t *btn_23;ui_pbutton_t *btn_24;
	ui_pbutton_t *btn_25;ui_pbutton_t *btn_26;ui_pbutton_t *btn_27;ui_pbutton_t *btn_28;ui_pbutton_t *btn_29;
	ui_pbutton_t *btn_30;ui_pbutton_t *btn_31;ui_pbutton_t *btn_32;ui_pbutton_t *btn_33;ui_pbutton_t *btn_34;
	ui_pbutton_t *btn_35;ui_pbutton_t *lose;
	ui_pbutton_t *end;
	ui_menu_bar_t *menubar;
	mine_geom_t geom;
} mine_t;

  ui_fixed_t *fixed;
  mine_t mine;
  static ui_entry_t *display;
  ui_t *ui = NULL;
  ui_window_t *window;
 
static void mine_pb_clicked(ui_pbutton_t *, void *);
static void mine_pb_released(ui_pbutton_t *, void *);
static ui_pbutton_cb_t glupi_pbutton_cb = {};
void skloni_dugmad(void);  // remove buttons
void vrati_dugmad(void);  // get back buttons
// DEO ZA MINEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE / Part for Mines
void beginner( void );
void minefield_generator( void );
int total_mines = 0;
int mines = 0;
int minefield[30][30];                                      //This 2-D array contains all of the mines, numbers and blank spaces
int blank_minefield[30][30];                                //This contains the minefield full of '|-|' characters
int final_minefield[30][30];
int mesta_dugmica_startX[36]; // startX button position
int mesta_dugmica_startY[36]; // startX button position
int P0x[1], P0y[1], P1x[1], P1y[1] ; // storage for button positions
int WinMx[1], WinMy[1];              // storage for button positions
int E0x[1], E0y[1], E1x[1], E1y[1] ; // storage for button positions
int EndMx[1], EndMy[1];              // storage for button positions

static void check_minefield(ui_pbutton_t *pbutton, void *arg);
void end_game( void );
int x, y, START , prazno;
int M, N;

void beginner( void )                                       //Gives the minefield the 'beginner' grid and mines
{
    M = 6; //MAX horizontal buttons
    N = 6; // MAX Vertikal buttons
	prazno =0; //Init Coounting mistakes
    total_mines = 10;
    minefield_generator();  // This call puts random mines on the 6x6 buttons field
}

void minefield_generator( void )                            //Function that generates the minefield
{
    int i = 0, j = 0;
    srand( time( NULL ) );                                  //Starts the random no. generator
    while( j < N )                                          //Nested loop for making the blank minefield and final minefield
    {
        while( i < M)
        {
            minefield[i][j] = ' ';
            blank_minefield[i][j] = minefield[i][j];        
            final_minefield[i][j] = minefield[i][j];        
            i++;
        }
        i = 0;
        j++;
    }
    mines = 0;
    while( mines < total_mines )                            //Randomly generates the mines into the minefield
    {
        i = rand()%(M);
        j = rand()%(N);
        if( minefield[i][j] != '*')                         //If statement that checks if there is a mine already there and doesn't place a mine if there is
        {
            minefield[i][j] = '*';
            final_minefield[i][j] = minefield[i][j];
            mines++;
        }
    }
    i = 0;
    j = 0;

    while( j < N )                                          //While loop that generates the numbers for any adjacent mines
    {
        while( i < M)
        {
            if( minefield[i][j] != '*')
            {
                minefield[i][j] = 0;
            }   
            if((minefield[i-1][j-1] == '*') && (minefield[i][j] != '*'))
            {
                minefield[i][j]++;
            }
            if((minefield[i-1][j] == '*') && (minefield[i][j] != '*'))
            {
                minefield[i][j]++;
            }
            if((minefield[i][j-1] == '*') && (minefield[i][j] != '*'))
            {
                minefield[i][j]++;
            }
            if((minefield[i-1][j+1] == '*') && (minefield[i][j] != '*'))
            {
                minefield[i][j]++;
            }
            if((minefield[i+1][j-1] == '*') && (minefield[i][j] != '*'))
            {
                minefield[i][j]++;
            }
            if((minefield[i+1][j] == '*') && (minefield[i][j] != '*'))
            {
                minefield[i][j]++;
            }
            if((minefield[i][j+1] == '*') && (minefield[i][j] != '*'))
            {
                minefield[i][j]++;
            }
            if((minefield[i+1][j+1] == '*') && (minefield[i][j] != '*'))
            {
                minefield[i][j]++;
            }
            i++;
        }
        i = 0;
        j++;
    }
    i = 0;
    j = 0;
}

//END PARTS FOR CREATING MINE FIELD*******************************

static ui_pbutton_cb_t mine_pbutton_cb = {
	.clicked = mine_pb_clicked,
	.up = mine_pb_released
};

static void wnd_close(ui_window_t *, void *);

static ui_window_cb_t window_cb = {
	.close = wnd_close,
	
};

static ui_entry_t *display;

/** Window close request
 *
 * @param window Window
 * @param arg Argument (mine_t *)
 */
static void wnd_close(ui_window_t *window, void *arg)
{
	mine_t *mine = (mine_t *) arg;

	ui_quit(mine->ui);
}

/** File / Exit menu entry selected.
 *
 * @param mentry Menu entry
 * @param arg Argument (mine_t *)
 */
static void mine_file_exit(ui_menu_entry_t *mentry, void *arg)
{
	mine_t *mine = (mine_t *) arg;

	ui_quit(mine->ui);
} 

static void mine_pb_clicked(ui_pbutton_t *pbutton, void *arg)
{
// ui_entry_set_text(display, " ");
//return;
}

static void mine_pb_released(ui_pbutton_t *pbutton, void *arg)
{
	//const char *B = (const char *) "B";
	const char *subexpr = (const char *) arg;  
	const char *O = (const char *) "-";
	const char *K = (const char *) "/";
	const char *BU = (const char *) "*";
	const char *END = (const char *) "+";

    if ( (str_cmp(subexpr,O)==0) || (str_cmp(subexpr,BU)==0) || (str_cmp(subexpr,K)==0) )
       check_minefield(pbutton,arg); 
	else if (str_cmp(subexpr,END)==0) end_game();
	else return;
}

static errno_t mine_button_create(mine_t *mine, ui_fixed_t *fixed,
    int x, int y, const char *text, ui_pbutton_cb_t *cb, void *arg,
    ui_pbutton_t **rbutton)
{
	ui_pbutton_t *pb;
	gfx_rect_t rect;
	errno_t rc;

	rc = ui_pbutton_create(mine->ui_res, text, &pb);
	if (rc != EOK) {
		printf("Greska pri kreiranju dugmeta.\n"); // error create button
		return rc;
	}

	ui_pbutton_set_cb(pb, cb, arg);

	rect.p0.x = mine->geom.btn_orig.x + mine->geom.btn_stride.x * x;
	rect.p0.y = mine->geom.btn_orig.y + mine->geom.btn_stride.y * y;
	rect.p1.x = rect.p0.x + mine->geom.btn_dim.x;
	rect.p1.y = rect.p0.y + mine->geom.btn_dim.y;
	ui_pbutton_set_rect(pb, &rect);
	
   
	rc = ui_fixed_add(fixed, ui_pbutton_ctl(pb));
	if (rc != EOK) {
		printf("Neuspelo dodavanje kontrole izgledu.\n");  // error create button
		return rc;
	}

if (rbutton != NULL)
		*rbutton = pb;
	return EOK;
	}

static errno_t mine_button_lose(mine_t *mine, ui_fixed_t *fixed,int x, int y, const char *text, ui_pbutton_cb_t *cb, void *arg, ui_pbutton_t **rbutton)
{        
	ui_pbutton_t *pb;
	gfx_rect_t rect;
	errno_t rc;
	rc = ui_pbutton_create(mine->ui_res, text, &pb);
	if (rc != EOK) {
		printf("Greska pri kreiranju dugmeta.\n");
		return rc;
	}
	ui_pbutton_set_cb(pb, cb, arg);

	rect.p0.x = mine->geom.btn_orig.x + mine->geom.btn_stride.x * x;
	rect.p0.y = mine->geom.btn_orig.y + mine->geom.btn_stride.y * y;
	rect.p1.x = rect.p0.x + mine->geom.btn_dim.x;
	rect.p1.y = rect.p0.y + mine->geom.btn_dim.y;
	P0x[0]=rect.p0.x;
	P0y[0]=rect.p0.y;
	P1x[0]=rect.p1.x;
	P1y[0]=rect.p1.y;
	ui_pbutton_set_rect(pb, &rect);

	rc = ui_fixed_add(fixed, ui_pbutton_ctl(pb));
	if (rc != EOK) {
		printf("Neuspelo dodavanje kontrole izgledu.\n");
		return rc;
	}

if (rbutton != NULL)
		*rbutton = pb;
	return EOK;
}

static errno_t mine_button_end(mine_t *mine, ui_fixed_t *fixed,int x, int y, const char *text, ui_pbutton_cb_t *cb, void *arg, ui_pbutton_t **rbutton)
{        
	ui_pbutton_t *pb;
	gfx_rect_t tect;
	errno_t rc;
	rc = ui_pbutton_create(mine->ui_res, text, &pb);
	if (rc != EOK) {
		printf("Greska pri kreiranju dugmeta.\n");
		return rc;
	}
	ui_pbutton_set_cb(pb, cb, arg);

	tect.p0.x = mine->geom.btn_orig.x + mine->geom.btn_stride.x * x;
	tect.p0.y = mine->geom.btn_orig.y + mine->geom.btn_stride.y * y;
	tect.p1.x = tect.p0.x + mine->geom.btn_dim.x;
	tect.p1.y = tect.p0.y + mine->geom.btn_dim.y;
	E0x[0]=tect.p0.x;
	E0y[0]=tect.p0.y;
	E1x[0]=tect.p1.x;
	E1y[0]=tect.p1.y;
	ui_pbutton_set_rect(pb, &tect);

	rc = ui_fixed_add(fixed, ui_pbutton_ctl(pb));
	if (rc != EOK) {
		printf("Neuspelo dodavanje kontrole izgledu.\n");
		return rc;
	}
if (rbutton != NULL)
		*rbutton = pb;
	return EOK;
}

static void print_syntax(void)
{
	printf("Sintaksa: %s [-d <displej-spec>]\n", NAME);
}

void end_game(){
	ui_window_destroy(window);
	ui_destroy(ui);
}


static void check_minefield(ui_pbutton_t *pbutton, void *arg)                                  // This function check the minefield
{  
	const char *subexpr = (const char *) arg;
	//const char *G =(const char *)"To Restart game Click Button, to Quit chose 'EXIT' from MENU";
   const char *O =(const char *)"You MISS the MINE, try again :) ";
   const char *R =(const char *)"You Find THE MINE!";
    const char *W =(const char *)"WAUUU... YOU WIN !!!";
	const char *BU = (const char *) "*";
	const char *Minus = (const char *) "-";
    const char *K = (const char *) "/";    
     /// Yeah you WIN GAME                    
           if (prazno == 26) {   // Since we have 36 fields with 10 mines, 26 fields are empty
	       // YOU WIN
			(void) ui_entry_set_text(display, W);	
		   ui_pbutton_set_cb(mine.lose, &mine_pbutton_cb, (void *)"/");
		   skloni_dugmad();    // Move buttons from screen , Where? To Right, out from visible screen !
           gfx_rect_t rect;
	       rect.p0.x = 80;
	       rect.p0.y = 100;
	       rect.p1.x = 300;
	       rect.p1.y = 150;
	       ui_pbutton_set_rect(mine.lose, &rect);	// get right dimension for RESTART GAME button
           rect.p0.x = 80;
	       rect.p0.y = 170;
	       rect.p1.x = 300;
	       rect.p1.y = 220;
	       ui_pbutton_set_rect(mine.end, &rect);	// get right dimension for RESTART GAME button		
			ui_window_paint(window);
			ui_entry_paint(display);
			return;
    }      
	  // if clicked RESTART BUTTON  hide Restart and QUIT button (move it out, right)
           if (str_cmp(subexpr,K)==0) {
           gfx_rect_t rect;
	       rect.p0.x = 800;
	       rect.p0.y = 0;
	       rect.p1.x = 810;
	       rect.p1.y = 10;
	       ui_pbutton_set_rect(mine.lose, &rect);
		   rect.p0.x = 800;
	       rect.p0.y = 20;
	       rect.p1.x = 810;
	       rect.p1.y = 40;
	       ui_pbutton_set_rect(mine.end, &rect);
          vrati_dugmad();    // When remove Restart and QUIT button, let, return back minefield buttons
		                     // command vrati_dugmad(), starts again beginner() and minefield_generator() for new game.
	     prazno=0;
		 ui_window_paint(window);
			return;
	}
  if (str_cmp(subexpr,Minus)==0) 
    {       
		    ui_pbutton_set_cb(pbutton, &glupi_pbutton_cb, NULL); // This is for mistakes counter. Disable any 
			                                                     // clicks from once clicked button
            ui_pbutton_set_caption(pbutton , Minus);  // Visuale mark clicked button with (-)
			ui_entry_set_text(display, O);   // set text 
			ui_entry_paint(display);
			prazno++;     //Count mistakes.
			return;
    }
     if (str_cmp(subexpr,BU)==0)
	{     
	      ui_entry_set_text(display, R);	// Print message on the Display " You find the mine :)"
		 skloni_dugmad();    // Move buttons from screen , Where? To Right, out from visible screen !
           gfx_rect_t rect;   // get back RESTART and QUIT button
	       rect.p0.x = 80;
	       rect.p0.y = 130;
	       rect.p1.x = 300;
	       rect.p1.y = 180;
	       ui_pbutton_set_rect(mine.lose, &rect);	// get right dimension for RESTART GAME button
           rect.p0.x = 80;
	       rect.p0.y = 200;
	       rect.p1.x = 300;
	       rect.p1.y = 250;
	       ui_pbutton_set_rect(mine.end, &rect);	// get right dimension for QUIT GAME button
            // re-paint window, to see these 2 buttons
			ui_window_paint(window);
			  ui_entry_paint(display); // Paint message
			prazno=0;
     		return;	
    }  
}

void skloni_dugmad(void){
    gfx_rect_t rect;
	rect.p0.x = 800;
	rect.p0.y = 0;
	rect.p1.x = 810;
	rect.p1.y = 10;
	       ui_pbutton_set_rect(mine.btn_0, &rect);  ui_pbutton_set_rect(mine.btn_1, &rect);
		   ui_pbutton_set_rect(mine.btn_2, &rect);  ui_pbutton_set_rect(mine.btn_3, &rect);
		   ui_pbutton_set_rect(mine.btn_4, &rect);  ui_pbutton_set_rect(mine.btn_5, &rect);
		   ui_pbutton_set_rect(mine.btn_6, &rect);  ui_pbutton_set_rect(mine.btn_7, &rect);
		   ui_pbutton_set_rect(mine.btn_8, &rect);  ui_pbutton_set_rect(mine.btn_9, &rect);
		   ui_pbutton_set_rect(mine.btn_10, &rect); ui_pbutton_set_rect(mine.btn_11, &rect);
		   ui_pbutton_set_rect(mine.btn_12, &rect); ui_pbutton_set_rect(mine.btn_13, &rect);
		   ui_pbutton_set_rect(mine.btn_14, &rect); ui_pbutton_set_rect(mine.btn_15, &rect);
		   ui_pbutton_set_rect(mine.btn_16, &rect); ui_pbutton_set_rect(mine.btn_17, &rect);
		   ui_pbutton_set_rect(mine.btn_18, &rect); ui_pbutton_set_rect(mine.btn_19, &rect);
		   ui_pbutton_set_rect(mine.btn_20, &rect); ui_pbutton_set_rect(mine.btn_21, &rect);
		   ui_pbutton_set_rect(mine.btn_22, &rect); ui_pbutton_set_rect(mine.btn_23, &rect);
		   ui_pbutton_set_rect(mine.btn_24, &rect); ui_pbutton_set_rect(mine.btn_25, &rect);
		   ui_pbutton_set_rect(mine.btn_26, &rect); ui_pbutton_set_rect(mine.btn_27, &rect);
		   ui_pbutton_set_rect(mine.btn_28, &rect); ui_pbutton_set_rect(mine.btn_29, &rect);
		   ui_pbutton_set_rect(mine.btn_30, &rect); ui_pbutton_set_rect(mine.btn_31, &rect);
		   ui_pbutton_set_rect(mine.btn_32, &rect); ui_pbutton_set_rect(mine.btn_33, &rect);
		   ui_pbutton_set_rect(mine.btn_34, &rect); ui_pbutton_set_rect(mine.btn_35, &rect);
		   return;
}

void vrati_dugmad(void){
	beginner();  // Get start minefield values
	minefield_generator();  // make random minefield
           gfx_rect_t rect;
// I am GameMaker Expert, no and #C expert..
// I did'nt know how to put buttons in switch from btn_0 to btn_35,
// So, if you wana change something, do that.. :)
int i=0;
while (i < 36){
	rect.p0.x = mine.geom.btn_orig.x + mine.geom.btn_stride.x * mesta_dugmica_startX[i];
	rect.p0.y = mine.geom.btn_orig.y + mine.geom.btn_stride.y * mesta_dugmica_startY[i];
	rect.p1.x = rect.p0.x + mine.geom.btn_dim.x;
	rect.p1.y = rect.p0.y + mine.geom.btn_dim.y;
   switch(i){
case 0:ui_pbutton_set_rect(mine.btn_0, &rect);
      if (final_minefield[0][0] != '*' )  ui_pbutton_set_cb(mine.btn_0, &mine_pbutton_cb,(void *)"-");
	  else ui_pbutton_set_cb(mine.btn_0, &mine_pbutton_cb,(void *)"*");
	  ui_pbutton_set_caption(mine.btn_0 , " ");
    break;
 
case 1:ui_pbutton_set_rect(mine.btn_1, &rect);
      if (final_minefield[1][0] != '*' )  ui_pbutton_set_cb(mine.btn_1, &mine_pbutton_cb, (void *)"-");
	  else  ui_pbutton_set_cb(mine.btn_1, &mine_pbutton_cb, (void *)"*");
	  ui_pbutton_set_caption(mine.btn_1 , " ");
    break; 
case 2:ui_pbutton_set_rect(mine.btn_2, &rect);
      if (final_minefield[2][0] != '*' )  ui_pbutton_set_cb(mine.btn_2, &mine_pbutton_cb, (void *)"-");
	  else ui_pbutton_set_cb(mine.btn_2, &mine_pbutton_cb, (void *)"*");
	  ui_pbutton_set_caption(mine.btn_2 , " ");
	break;
case 3:ui_pbutton_set_rect(mine.btn_3, &rect);
      if (final_minefield[3][0] != '*' )  ui_pbutton_set_cb(mine.btn_3, &mine_pbutton_cb, (void *)"-");
	  else ui_pbutton_set_cb(mine.btn_3, &mine_pbutton_cb, (void *)"*");
	  ui_pbutton_set_caption(mine.btn_3 , " ");
	break;
case 4:ui_pbutton_set_rect(mine.btn_4, &rect);
      if (final_minefield[4][0] != '*' )  ui_pbutton_set_cb(mine.btn_4, &mine_pbutton_cb, (void *)"-");
	  else ui_pbutton_set_cb(mine.btn_4, &mine_pbutton_cb, (void *)"*");
	  ui_pbutton_set_caption(mine.btn_4 , " ");
	break;
case 5:ui_pbutton_set_rect(mine.btn_5, &rect);
      if (final_minefield[5][0] != '*' )  ui_pbutton_set_cb(mine.btn_5, &mine_pbutton_cb, (void *)"-");
	  else ui_pbutton_set_cb(mine.btn_5, &mine_pbutton_cb, (void *)"*");
	  ui_pbutton_set_caption(mine.btn_5 , " ");
	break;
case 6:ui_pbutton_set_rect(mine.btn_6, &rect);
      if (final_minefield[0][1] != '*' )  ui_pbutton_set_cb(mine.btn_6, &mine_pbutton_cb, (void *)"-");
	  else ui_pbutton_set_cb(mine.btn_6, &mine_pbutton_cb, (void *)"*");
	  ui_pbutton_set_caption(mine.btn_6 , " ");
	break;
case 7:ui_pbutton_set_rect(mine.btn_7, &rect);
      if (final_minefield[1][1] != '*' )  ui_pbutton_set_cb(mine.btn_7, &mine_pbutton_cb, (void *)"-");
	  else ui_pbutton_set_cb(mine.btn_7, &mine_pbutton_cb, (void *)"*");
	  ui_pbutton_set_caption(mine.btn_7 , " ");
	break;
case 8:ui_pbutton_set_rect(mine.btn_8, &rect); 
      if (final_minefield[2][1] != '*' )  ui_pbutton_set_cb(mine.btn_8, &mine_pbutton_cb, (void *)"-");
	  else ui_pbutton_set_cb(mine.btn_8, &mine_pbutton_cb, (void *)"*");
	  ui_pbutton_set_caption(mine.btn_8 , " ");
	break;
case 9:ui_pbutton_set_rect(mine.btn_9, &rect);
      if (final_minefield[3][1] != '*' )  ui_pbutton_set_cb(mine.btn_9, &mine_pbutton_cb, (void *)"-");
	  else ui_pbutton_set_cb(mine.btn_9, &mine_pbutton_cb, (void *)"*");
	  ui_pbutton_set_caption(mine.btn_9 , " ");
	break;
case 10:ui_pbutton_set_rect(mine.btn_10, &rect);
      if (final_minefield[4][1] != '*' )  ui_pbutton_set_cb(mine.btn_10, &mine_pbutton_cb, (void *)"-");
	  else ui_pbutton_set_cb(mine.btn_10, &mine_pbutton_cb, (void *)"*");
	  ui_pbutton_set_caption(mine.btn_10 , " ");
	break;
case 11:ui_pbutton_set_rect(mine.btn_11, &rect);
      if (final_minefield[5][1] != '*' )  ui_pbutton_set_cb(mine.btn_11, &mine_pbutton_cb, (void *)"-");
	  else ui_pbutton_set_cb(mine.btn_11, &mine_pbutton_cb, (void *)"*");
	  ui_pbutton_set_caption(mine.btn_11 , " ");
	break;
case 12:ui_pbutton_set_rect(mine.btn_12, &rect);
      if (final_minefield[0][2] != '*' )  ui_pbutton_set_cb(mine.btn_12, &mine_pbutton_cb, (void *)"-");
	  else ui_pbutton_set_cb(mine.btn_12, &mine_pbutton_cb, (void *)"*");
	  ui_pbutton_set_caption(mine.btn_12 , " ");
	break;
case 13:ui_pbutton_set_rect(mine.btn_13, &rect);
      if (final_minefield[1][2] != '*' )  ui_pbutton_set_cb(mine.btn_13, &mine_pbutton_cb, (void *)"-");
	  else ui_pbutton_set_cb(mine.btn_13, &mine_pbutton_cb, (void *)"*");
	  ui_pbutton_set_caption(mine.btn_13 , " ");
	break;
case 14:ui_pbutton_set_rect(mine.btn_14, &rect);
      if (final_minefield[2][2] != '*' )  ui_pbutton_set_cb(mine.btn_14, &mine_pbutton_cb, (void *)"-");
	  else ui_pbutton_set_cb(mine.btn_14, &mine_pbutton_cb, (void *)"*");
	  ui_pbutton_set_caption(mine.btn_14 , " ");
	break;
case 15:ui_pbutton_set_rect(mine.btn_15, &rect);
      if (final_minefield[3][2] != '*' )  ui_pbutton_set_cb(mine.btn_15, &mine_pbutton_cb, (void *)"-");
	  else ui_pbutton_set_cb(mine.btn_15, &mine_pbutton_cb, (void *)"*");
	  ui_pbutton_set_caption(mine.btn_15 , " ");
	break;
case 16:ui_pbutton_set_rect(mine.btn_16, &rect);
	  if (final_minefield[4][2] != '*' )  ui_pbutton_set_cb(mine.btn_16, &mine_pbutton_cb, (void *)"-");
	  else ui_pbutton_set_cb(mine.btn_16, &mine_pbutton_cb, (void *)"*");
	  ui_pbutton_set_caption(mine.btn_16 , " ");
	break;
case 17:ui_pbutton_set_rect(mine.btn_17, &rect);
      if (final_minefield[5][2] != '*' )  ui_pbutton_set_cb(mine.btn_17, &mine_pbutton_cb, (void *)"-");
	  else ui_pbutton_set_cb(mine.btn_17, &mine_pbutton_cb, (void *)"*");
	  ui_pbutton_set_caption(mine.btn_17 , " ");
	break;
case 18:ui_pbutton_set_rect(mine.btn_18, &rect);
      if (final_minefield[0][3] != '*' )  ui_pbutton_set_cb(mine.btn_18, &mine_pbutton_cb, (void *)"-");
	  else ui_pbutton_set_cb(mine.btn_18, &mine_pbutton_cb, (void *)"*");
	  ui_pbutton_set_caption(mine.btn_18 , " ");
	break;
case 19:ui_pbutton_set_rect(mine.btn_19, &rect);
      if (final_minefield[1][3] != '*' )  ui_pbutton_set_cb(mine.btn_19, &mine_pbutton_cb, (void *)"-");
	  else ui_pbutton_set_cb(mine.btn_19, &mine_pbutton_cb, (void *)"*");
	  ui_pbutton_set_caption(mine.btn_19 , " ");
	break;
case 20:ui_pbutton_set_rect(mine.btn_20, &rect);
      if (final_minefield[2][3] != '*' )  ui_pbutton_set_cb(mine.btn_20, &mine_pbutton_cb, (void *)"-");
	  else ui_pbutton_set_cb(mine.btn_20, &mine_pbutton_cb, (void *)"*");
	  ui_pbutton_set_caption(mine.btn_20 , " ");
	break;
case 21:ui_pbutton_set_rect(mine.btn_21, &rect);
      if (final_minefield[3][3] != '*' )  ui_pbutton_set_cb(mine.btn_21, &mine_pbutton_cb, (void *)"-");
	  else ui_pbutton_set_cb(mine.btn_21, &mine_pbutton_cb, (void *)"*");
	  ui_pbutton_set_caption(mine.btn_21 , " ");
	break;
case 22:ui_pbutton_set_rect(mine.btn_22, &rect);
      if (final_minefield[4][3] != '*' )  ui_pbutton_set_cb(mine.btn_22, &mine_pbutton_cb, (void *)"-");
	  else ui_pbutton_set_cb(mine.btn_22, &mine_pbutton_cb, (void *)"*");
	  ui_pbutton_set_caption(mine.btn_22 , " ");
	break;
case 23:ui_pbutton_set_rect(mine.btn_23, &rect);
      if (final_minefield[5][3] != '*' )  ui_pbutton_set_cb(mine.btn_23, &mine_pbutton_cb, (void *)"-");
	  else ui_pbutton_set_cb(mine.btn_23, &mine_pbutton_cb, (void *)"*");
	  ui_pbutton_set_caption(mine.btn_23 , " ");
	break;
case 24:ui_pbutton_set_rect(mine.btn_24, &rect);
      if (final_minefield[0][4] != '*' )  ui_pbutton_set_cb(mine.btn_24, &mine_pbutton_cb, (void *)"-");
	  else ui_pbutton_set_cb(mine.btn_24, &mine_pbutton_cb, (void *)"*");
	  ui_pbutton_set_caption(mine.btn_24 , " ");
	break;
case 25:ui_pbutton_set_rect(mine.btn_25, &rect);
      if (final_minefield[1][4] != '*' )  ui_pbutton_set_cb(mine.btn_25, &mine_pbutton_cb, (void *)"-");
	  else ui_pbutton_set_cb(mine.btn_25, &mine_pbutton_cb, (void *)"*");
	  ui_pbutton_set_caption(mine.btn_25 , " ");
	break;
case 26:ui_pbutton_set_rect(mine.btn_26, &rect);
      if (final_minefield[2][4] != '*' )  ui_pbutton_set_cb(mine.btn_26, &mine_pbutton_cb, (void *)"-");
	  else ui_pbutton_set_cb(mine.btn_26, &mine_pbutton_cb, (void *)"*");
	  ui_pbutton_set_caption(mine.btn_26 , " ");
	break;
case 27:ui_pbutton_set_rect(mine.btn_27, &rect);
      if (final_minefield[3][4] != '*' )  ui_pbutton_set_cb(mine.btn_27, &mine_pbutton_cb, (void *)"-");
	  else ui_pbutton_set_cb(mine.btn_27, &mine_pbutton_cb, (void *)"*");
	  ui_pbutton_set_caption(mine.btn_27 , " ");
	break;
case 28:ui_pbutton_set_rect(mine.btn_28, &rect);
      if (final_minefield[4][4] != '*' )  ui_pbutton_set_cb(mine.btn_28, &mine_pbutton_cb, (void *)"-");
	  else ui_pbutton_set_cb(mine.btn_28, &mine_pbutton_cb, (void *)"*");
	  ui_pbutton_set_caption(mine.btn_28 , " ");
	break;
case 29:ui_pbutton_set_rect(mine.btn_29, &rect);
      if (final_minefield[5][4] != '*' )  ui_pbutton_set_cb(mine.btn_29, &mine_pbutton_cb, (void *)"-");
	  else ui_pbutton_set_cb(mine.btn_29, &mine_pbutton_cb, (void *)"*");
	  ui_pbutton_set_caption(mine.btn_29 , " ");
	break;
case 30:ui_pbutton_set_rect(mine.btn_30, &rect);
      if (final_minefield[0][5] != '*' )  ui_pbutton_set_cb(mine.btn_30, &mine_pbutton_cb, (void *)"-");
	  else ui_pbutton_set_cb(mine.btn_30, &mine_pbutton_cb, (void *)"*");
	  ui_pbutton_set_caption(mine.btn_30 , " ");
	break;
case 31:ui_pbutton_set_rect(mine.btn_31, &rect);
      if (final_minefield[1][5] != '*' )  ui_pbutton_set_cb(mine.btn_31, &mine_pbutton_cb, (void *)"-");
	  else ui_pbutton_set_cb(mine.btn_31, &mine_pbutton_cb, (void *)"*");
	  ui_pbutton_set_caption(mine.btn_31 , " ");
	break;
case 32:ui_pbutton_set_rect(mine.btn_32, &rect);
      if (final_minefield[2][5] != '*' )  ui_pbutton_set_cb(mine.btn_32, &mine_pbutton_cb, (void *)"-");
	  else ui_pbutton_set_cb(mine.btn_32, &mine_pbutton_cb, (void *)"*");
	  ui_pbutton_set_caption(mine.btn_32 , " ");
	break;
case 33:ui_pbutton_set_rect(mine.btn_33, &rect);
      if (final_minefield[3][5] != '*' )  ui_pbutton_set_cb(mine.btn_33, &mine_pbutton_cb, (void *)"-");
	  else ui_pbutton_set_cb(mine.btn_33, &mine_pbutton_cb, (void *)"*");
	  ui_pbutton_set_caption(mine.btn_33 , " ");
	break;
case 34:ui_pbutton_set_rect(mine.btn_34, &rect);
      if (final_minefield[4][5] != '*' )  ui_pbutton_set_cb(mine.btn_34, &mine_pbutton_cb, (void *)"-");
	  else ui_pbutton_set_cb(mine.btn_34, &mine_pbutton_cb, (void *)"*");
	  ui_pbutton_set_caption(mine.btn_34 , " ");
	break;
case 35:ui_pbutton_set_rect(mine.btn_35, &rect);
      if (final_minefield[5][5] != '*' )  ui_pbutton_set_cb(mine.btn_35, &mine_pbutton_cb, (void *)"-");
	  else ui_pbutton_set_cb(mine.btn_35, &mine_pbutton_cb, (void *)"*");
	  ui_pbutton_set_caption(mine.btn_35 , " ");
	break;
default: break;
   }
   i++;
}
ui_pbutton_set_cb(mine.lose, &mine_pbutton_cb, (void *)"/");
ui_pbutton_set_cb(mine.end, &mine_pbutton_cb, (void *)"+");
return;
}	   
		

int main(int argc, char *argv[])
{
	const char *display_spec = UI_ANY_DEFAULT;
	ui_resource_t *ui_res;
	ui_wnd_params_t params;
	
	ui_menu_t *mfile;
	ui_menu_entry_t *mexit;
	errno_t rc;
	int i;
  beginner();

	i = 1;
	while (i < argc) {
		if (str_cmp(argv[i], "-d") == 0) {
			++i;
			if (i >= argc) {
				printf("Nedostaje argument.\n");
				print_syntax();
				return 1;
			}

			display_spec = argv[i++];
		} else {
			printf("Neispravna opcija '%s'.\n", argv[i]);
			print_syntax();
			return 1;
		}
	}

	rc = ui_create(display_spec, &ui);
	if (rc != EOK) {
		printf("Greska pri kreiranju UI na displeju %s.\n", display_spec);
		return rc;
	}

	ui_wnd_params_init(&params);
	params.caption = "Minisweeper game by ZmajSoft";
	params.rect.p0.x = 0;
	params.rect.p0.y = 0;

	if (ui_is_textmode(ui)) {
		params.rect.p1.x = 38;
		params.rect.p1.y = 18;

		mine.geom.menubar_rect.p0.x = 1;
		mine.geom.menubar_rect.p0.y = 1;
		mine.geom.menubar_rect.p1.x = params.rect.p1.x - 1;
		mine.geom.menubar_rect.p1.y = 2;
		mine.geom.entry_rect.p0.x = 4;
		mine.geom.entry_rect.p0.y = 3;
		mine.geom.entry_rect.p1.x = 34;
		mine.geom.entry_rect.p1.y = 4;
		mine.geom.btn_orig.x = 4;
		mine.geom.btn_orig.y = 5;
		mine.geom.btn_dim.x = 6;
		mine.geom.btn_dim.y = 2;
		mine.geom.btn_stride.x = 8;
		mine.geom.btn_stride.y = 3;
	} else {
		params.rect.p1.x = 370;
		params.rect.p1.y = 380;

		mine.geom.menubar_rect.p0.x = 4;
		mine.geom.menubar_rect.p0.y = 30;
		mine.geom.menubar_rect.p1.x = params.rect.p1.x - 4;
		mine.geom.menubar_rect.p1.y = 52;
		mine.geom.entry_rect.p0.x = 10;
		mine.geom.entry_rect.p0.y = 51;
		mine.geom.entry_rect.p1.x = 350;
		mine.geom.entry_rect.p1.y = 76;
		mine.geom.btn_orig.x = 10;
		mine.geom.btn_orig.y = 90;
		mine.geom.btn_dim.x = 40;
		mine.geom.btn_dim.y = 40;
		mine.geom.btn_stride.x = 60;
		mine.geom.btn_stride.y = 45;
	}

	rc = ui_window_create(ui, &params, &window);
	if (rc != EOK) {
		printf("Greska pri kreiranju prozora.\n"); // error create windows
		return rc;
	}

	ui_window_set_cb(window, &window_cb, (void *) &mine);
	mine.ui = ui;

	ui_res = ui_window_get_res(window);
	mine.ui_res = ui_res;

	rc = ui_fixed_create(&fixed);
	if (rc != EOK) {
		printf("Greska pri kreiranju fiksnog izgleda.\n");  //error fixing
		return rc;
	}

	rc = ui_menu_bar_create(ui, window, &mine.menubar);
	if (rc != EOK) {
		printf("Greska pri kreiranju menu bar-a.\n"); // error create menu
		return rc;
	}

	rc = ui_menu_dd_create(mine.menubar, "~G~ame", NULL, &mfile);
	if (rc != EOK) {
		printf("Greska pri kreiranju menija.\n"); // error create menu
		return rc;
	}

	rc = ui_menu_entry_create(mfile, "I~z~laz", "Alt-F4", &mexit);  // Menu option Quit or EXIT
	if (rc != EOK) {
		printf("Greska pri kreiranju menija.\n"); // error create menu
		return rc;
	}

	ui_menu_entry_set_cb(mexit, mine_file_exit, (void *) &mine);

	ui_menu_bar_set_rect(mine.menubar, &mine.geom.menubar_rect);

	rc = ui_fixed_add(fixed, ui_menu_bar_ctl(mine.menubar));
	if (rc != EOK) {
		printf("Neuspelo dodavanje kontrola izgledu.\n"); // error create menu
		return rc;
	}

	rc = ui_entry_create(window, NULL_DISPLAY, &display);
	     ui_entry_set_text(display, "God Luck!");
	if (rc != EOK) {
		printf("Greska pri kreiranju teksta sadrzaja");
		return rc;
	}

	ui_entry_set_rect(display, &mine.geom.entry_rect);
	ui_entry_set_halign(display, gfx_halign_right);
	ui_entry_set_read_only(display, true);

	rc = ui_fixed_add(fixed, ui_entry_ctl(display));
	if (rc != EOK) {
		printf("Neuspelo dodavanje kontrola izgledu.\n"); // error add controls
		return rc;
	}


// DUGMIcIIII 35 kom  ***********Buttons from 0 - 35 ***************************************
 if (final_minefield[0][0] != '*' ) {
	rc = mine_button_create(&mine, fixed, 0, 0, " ", &mine_pbutton_cb,(void *) "-", &mine.btn_0); }
else {rc = mine_button_create(&mine, fixed, 0, 0, " ", &mine_pbutton_cb,(void *) "*", &mine.btn_0);}
    if (rc == EOK) {
		ui_pbutton_set_light(mine.btn_0,true);
		mesta_dugmica_startX[0]=0;
        mesta_dugmica_startY[0]=0;
	               }
	else return rc;

 if (final_minefield[1][0] != '*' ) {
	rc = mine_button_create(&mine, fixed, 1, 0, " ", &mine_pbutton_cb, (void *) "-", &mine.btn_1);}
else {rc = mine_button_create(&mine, fixed, 1, 0, " ", &mine_pbutton_cb,(void *) "*", &mine.btn_1);}
    if (rc == EOK) {
		ui_pbutton_set_light(mine.btn_1,true);
		mesta_dugmica_startX[1]=1;
        mesta_dugmica_startY[1]=0;
	               }
	else return rc;

if  ( final_minefield[2][0] != '*') {
	rc = mine_button_create(&mine, fixed, 2, 0, " ", &mine_pbutton_cb,(void *) "-", &mine.btn_2);}
else {rc = mine_button_create(&mine, fixed, 2, 0, " ", &mine_pbutton_cb,(void *) "*", &mine.btn_2);}
    if (rc == EOK) {
		ui_pbutton_set_light(mine.btn_2,true);
		mesta_dugmica_startX[2]=2;
        mesta_dugmica_startY[2]=0;
	               }
	else return rc;

if  ( final_minefield[3][0] != '*') {
	rc = mine_button_create(&mine, fixed, 3, 0, " ", &mine_pbutton_cb,(void *) "-", &mine.btn_3);}
else {	rc = mine_button_create(&mine, fixed, 3, 0, " ", &mine_pbutton_cb,(void *) "*", &mine.btn_3);}
    if (rc == EOK) {
		ui_pbutton_set_light(mine.btn_3,true);
		mesta_dugmica_startX[3]=3;
        mesta_dugmica_startY[3]=0;
	               }
	else return rc;

if  ( final_minefield[4][0] != '*') {
	rc = mine_button_create(&mine, fixed, 4, 0, " ", &mine_pbutton_cb,(void *) "-", &mine.btn_4);}
else {rc = mine_button_create(&mine, fixed, 4, 0, " ", &mine_pbutton_cb,(void *) "*", &mine.btn_4);}
    if (rc == EOK) {
		ui_pbutton_set_light(mine.btn_4,true);
		mesta_dugmica_startX[4]=4;
        mesta_dugmica_startY[4]=0;
	               }
	else return rc;

if  ( final_minefield[5][0] != '*') {
	rc = mine_button_create(&mine, fixed, 5, 0, " ", &mine_pbutton_cb,(void *) "-", &mine.btn_5);}
else {rc = mine_button_create(&mine, fixed, 5, 0, " ", &mine_pbutton_cb,(void *) "*", &mine.btn_5);}
    if (rc == EOK) {
		ui_pbutton_set_light(mine.btn_5,true);
		mesta_dugmica_startX[5]=5;
        mesta_dugmica_startY[5]=0;
	               }
	else return rc;

if  ( final_minefield[0][1] != '*') {
	rc = mine_button_create(&mine, fixed, 0, 1, " ", &mine_pbutton_cb, (void *) "-", &mine.btn_6);}
else {rc = mine_button_create(&mine, fixed, 0, 1, " ", &mine_pbutton_cb, (void *) "*", &mine.btn_6);}
    if (rc == EOK) {
		ui_pbutton_set_light(mine.btn_6,true);
		mesta_dugmica_startX[6]=0;
        mesta_dugmica_startY[6]=1;
	               }
	else return rc;

if  ( final_minefield[1][1] != '*') {
	rc = mine_button_create(&mine, fixed, 1, 1, " ", &mine_pbutton_cb,(void *) "-", &mine.btn_7);}
else {rc = mine_button_create(&mine, fixed, 1, 1, " ", &mine_pbutton_cb,(void *) "*", &mine.btn_7);}
    if (rc == EOK) {
		ui_pbutton_set_light(mine.btn_7,true);
		mesta_dugmica_startX[7]=1;
        mesta_dugmica_startY[7]=1;
	               }
	else return rc;

if  ( final_minefield[2][1] != '*') {
	rc = mine_button_create(&mine, fixed, 2, 1, " ", &mine_pbutton_cb,(void *) "-", &mine.btn_8);}
else {rc = mine_button_create(&mine, fixed, 2, 1, " ", &mine_pbutton_cb,(void *) "*", &mine.btn_8);}
    if (rc == EOK) {
		ui_pbutton_set_light(mine.btn_8,true);
		mesta_dugmica_startX[8]=2;
        mesta_dugmica_startY[8]=1;
	               }
	else return rc;

if  ( final_minefield[3][1] != '*') {
	rc = mine_button_create(&mine, fixed, 3, 1, " ", &mine_pbutton_cb,(void *) "-", &mine.btn_9);}
else {rc = mine_button_create(&mine, fixed, 3, 1, " ", &mine_pbutton_cb,(void *) "*", &mine.btn_9);}
    if (rc == EOK) {
		ui_pbutton_set_light(mine.btn_9,true);
		mesta_dugmica_startX[9]=3;
        mesta_dugmica_startY[9]=1;
	               }
	else return rc;

if  ( final_minefield[4][1] != '*') {
	rc = mine_button_create(&mine, fixed, 4, 1, " ", &mine_pbutton_cb,(void *) "-", &mine.btn_10);}
else {rc = mine_button_create(&mine, fixed, 4, 1, " ", &mine_pbutton_cb,(void *) "*", &mine.btn_10);}
    if (rc == EOK) {
		ui_pbutton_set_light(mine.btn_10,true);
		mesta_dugmica_startX[10]=4;
        mesta_dugmica_startY[10]=1;
	               }
	else return rc;

if  ( final_minefield[5][1] != '*') {
	rc = mine_button_create(&mine, fixed, 5, 1, " ", &mine_pbutton_cb,(void *) "-", &mine.btn_11);}
else {rc = mine_button_create(&mine, fixed, 5, 1, " ", &mine_pbutton_cb,(void *) "*", &mine.btn_11);}
    if (rc == EOK) {
		ui_pbutton_set_light(mine.btn_11,true);
		mesta_dugmica_startX[11]=5;
        mesta_dugmica_startY[11]=1;
	               }
	else return rc;

if  ( final_minefield[0][2] != '*') {
	rc = mine_button_create(&mine, fixed, 0, 2, " ", &mine_pbutton_cb,(void *) "-", &mine.btn_12);}
else {rc = mine_button_create(&mine, fixed, 0, 2, " ", &mine_pbutton_cb,(void *) "*", &mine.btn_12);}
    if (rc == EOK) {
		ui_pbutton_set_light(mine.btn_12,true);
		mesta_dugmica_startX[12]=0;
        mesta_dugmica_startY[12]=2;
	               }
	else return rc;

if  ( final_minefield[1][2] != '*') {
	rc = mine_button_create(&mine, fixed, 1, 2, " ", &mine_pbutton_cb,(void *) "-", &mine.btn_13);}
else {rc = mine_button_create(&mine, fixed, 1, 2, " ", &mine_pbutton_cb,(void *) "*", &mine.btn_13);}
    if (rc == EOK) {
		ui_pbutton_set_light(mine.btn_13,true);
		mesta_dugmica_startX[13]=1;
        mesta_dugmica_startY[13]=2;
	               }
	else return rc;

if  ( final_minefield[2][2] != '*') {
	rc = mine_button_create(&mine, fixed, 2, 2, " ", &mine_pbutton_cb,(void *) "-", &mine.btn_14);}
else {rc = mine_button_create(&mine, fixed, 2, 2, " ", &mine_pbutton_cb,(void *) "*", &mine.btn_14);}
    if (rc == EOK) {
		ui_pbutton_set_light(mine.btn_14,true);
		mesta_dugmica_startX[14]=2;
        mesta_dugmica_startY[14]=2;
	               }
	else return rc;

if  ( final_minefield[3][2] != '*') {
	rc = mine_button_create(&mine, fixed, 3, 2, " ", &mine_pbutton_cb, (void *) "-", &mine.btn_15);}
else {rc = mine_button_create(&mine, fixed, 3, 2, " ", &mine_pbutton_cb, (void *) "*", &mine.btn_15);}
    if (rc == EOK) {
		ui_pbutton_set_light(mine.btn_15,true);
		mesta_dugmica_startX[15]=3;
        mesta_dugmica_startY[15]=2;
	               }
	else return rc;

if  ( final_minefield[4][2] != '*') {
	rc = mine_button_create(&mine, fixed, 4, 2, " ", &mine_pbutton_cb,(void *) "-", &mine.btn_16);}
else {rc = mine_button_create(&mine, fixed, 4, 2, " ", &mine_pbutton_cb,(void *) "*", &mine.btn_16);}
    if (rc == EOK) {
		ui_pbutton_set_light(mine.btn_16,true);
		mesta_dugmica_startX[16]=4;
        mesta_dugmica_startY[16]=2;
	               }
	else return rc;

if  ( final_minefield[5][2] != '*') {
	rc = mine_button_create(&mine, fixed, 5, 2, " ", &mine_pbutton_cb,(void *) "-", &mine.btn_17);}
else {rc = mine_button_create(&mine, fixed, 5, 2, " ", &mine_pbutton_cb,(void *) "*", &mine.btn_17);}
    if (rc == EOK) {
		ui_pbutton_set_light(mine.btn_17,true);
		mesta_dugmica_startX[17]=5;
        mesta_dugmica_startY[17]=2;
	               }
	else return rc;

if  ( final_minefield[0][3] != '*') {
	rc = mine_button_create(&mine, fixed, 0, 3, " ", &mine_pbutton_cb,(void *) "-", &mine.btn_18);}
else {rc = mine_button_create(&mine, fixed, 0, 3, " ", &mine_pbutton_cb,(void *) "*", &mine.btn_18);}
    if (rc == EOK) {
		ui_pbutton_set_light(mine.btn_18,true);
		mesta_dugmica_startX[18]=0;
        mesta_dugmica_startY[18]=3;
	               }
	else return rc;

if  ( final_minefield[1][3] != '*') {
	rc = mine_button_create(&mine, fixed, 1, 3, " ", &mine_pbutton_cb,(void *) "-", &mine.btn_19);}
else {rc = mine_button_create(&mine, fixed, 1, 3, " ", &mine_pbutton_cb,(void *) "*", &mine.btn_19);}
    if (rc == EOK) {
		ui_pbutton_set_light(mine.btn_19,true);
		mesta_dugmica_startX[19]=1;
        mesta_dugmica_startY[19]=3;
	               }
	else return rc;

if  ( final_minefield[2][3] != '*') {
	rc = mine_button_create(&mine, fixed, 2, 3, " ", &mine_pbutton_cb,(void *) "-", &mine.btn_20);}
else {rc = mine_button_create(&mine, fixed, 2, 3, " ", &mine_pbutton_cb,(void *) "*", &mine.btn_20);}
    if (rc == EOK) {
		ui_pbutton_set_light(mine.btn_20,true);
		mesta_dugmica_startX[20]=2;
        mesta_dugmica_startY[20]=3;
	               }
	else return rc;

if  ( final_minefield[3][3] != '*') {
	rc = mine_button_create(&mine, fixed, 3, 3, " ", &mine_pbutton_cb,(void *) "-", &mine.btn_21);}
else {rc = mine_button_create(&mine, fixed, 3, 3, " ", &mine_pbutton_cb,(void *) "*", &mine.btn_21);}
    if (rc == EOK) {
		ui_pbutton_set_light(mine.btn_21,true);
		mesta_dugmica_startX[21]=3;
        mesta_dugmica_startY[21]=3;
	               }
	else return rc;

if  ( final_minefield[4][3] != '*') {
	rc = mine_button_create(&mine, fixed, 4, 3, " ", &mine_pbutton_cb,(void *) "-", &mine.btn_22);}
else {rc = mine_button_create(&mine, fixed, 4, 3, " ", &mine_pbutton_cb,(void *) "*", &mine.btn_22);}
    if (rc == EOK) {
		ui_pbutton_set_light(mine.btn_22,true);
		mesta_dugmica_startX[22]=4;
        mesta_dugmica_startY[22]=3;
	               }
	else return rc;

if  ( final_minefield[5][3] != '*') {
	rc = mine_button_create(&mine, fixed, 5, 3, " ", &mine_pbutton_cb,(void *) "-", &mine.btn_23);}
else {rc = mine_button_create(&mine, fixed, 5, 3, " ", &mine_pbutton_cb,(void *) "*", &mine.btn_23);}
    if (rc == EOK) {
		ui_pbutton_set_light(mine.btn_23,true);
		mesta_dugmica_startX[23]=5;
        mesta_dugmica_startY[23]=3;
	               }
	else return rc;

if  ( final_minefield[0][4] != '*') {
	rc = mine_button_create(&mine, fixed, 0, 4, " ", &mine_pbutton_cb,(void *) "-", &mine.btn_24);}
else {rc = mine_button_create(&mine, fixed, 0, 4, " ", &mine_pbutton_cb,(void *) "*", &mine.btn_24);}
    if (rc == EOK) {
		ui_pbutton_set_light(mine.btn_24,true);
		mesta_dugmica_startX[24]=0;
        mesta_dugmica_startY[24]=4;
	               }
	else return rc;

if  ( final_minefield[1][4] != '*') {
	rc = mine_button_create(&mine, fixed, 1, 4, " ", &mine_pbutton_cb,(void *) "-", &mine.btn_25);}
else {rc = mine_button_create(&mine, fixed, 1, 4, " ", &mine_pbutton_cb,(void *) "*", &mine.btn_25);}
    if (rc == EOK) {
		ui_pbutton_set_light(mine.btn_25,true);
		mesta_dugmica_startX[25]=1;
        mesta_dugmica_startY[25]=4;
	               }
	else return rc;

if  ( final_minefield[2][4] != '*') {
	rc = mine_button_create(&mine, fixed, 2, 4, " ", &mine_pbutton_cb,(void *) "-", &mine.btn_26);}
else {rc = mine_button_create(&mine, fixed, 2, 4, " ", &mine_pbutton_cb,(void *) "*", &mine.btn_26);}
    if (rc == EOK) {
		ui_pbutton_set_light(mine.btn_26,true);
		mesta_dugmica_startX[26]=2;
        mesta_dugmica_startY[26]=4;
	               }
	else return rc;

if  ( final_minefield[3][4] != '*') {
	rc = mine_button_create(&mine, fixed, 3, 4, " ", &mine_pbutton_cb,(void *) "-", &mine.btn_27);}
else {rc = mine_button_create(&mine, fixed, 3, 4, " ", &mine_pbutton_cb,(void *) "*", &mine.btn_27);}
    if (rc == EOK) {
		ui_pbutton_set_light(mine.btn_27,true);
		mesta_dugmica_startX[27]=3;
        mesta_dugmica_startY[27]=4;
	               }
	else return rc;

if  ( final_minefield[4][4] != '*') {
	rc = mine_button_create(&mine, fixed, 4, 4, " ", &mine_pbutton_cb,(void *) "-", &mine.btn_28);}
else {rc = mine_button_create(&mine, fixed, 4, 4, " ", &mine_pbutton_cb,(void *) "*", &mine.btn_28);}
    if (rc == EOK) {
		ui_pbutton_set_light(mine.btn_28,true);
		mesta_dugmica_startX[28]=4;
        mesta_dugmica_startY[28]=4;
	               }
	else return rc;

if  ( final_minefield[5][4] != '*') {
	rc = mine_button_create(&mine, fixed, 5, 4, " ", &mine_pbutton_cb,(void *) "-", &mine.btn_29);}
else {rc = mine_button_create(&mine, fixed, 5, 4, " ", &mine_pbutton_cb,(void *) "*", &mine.btn_29);}
    if (rc == EOK) {
		ui_pbutton_set_light(mine.btn_29,true);
		mesta_dugmica_startX[29]=5;
        mesta_dugmica_startY[29]=4;
	               }
	else return rc;

if  ( final_minefield[0][5] != '*') {
	rc = mine_button_create(&mine, fixed, 0, 5, " ", &mine_pbutton_cb,(void *) "-", &mine.btn_30);}
else {rc = mine_button_create(&mine, fixed, 0, 5, " ", &mine_pbutton_cb,(void *) "*", &mine.btn_30);}
    if (rc == EOK) {
		ui_pbutton_set_light(mine.btn_30,true);
		mesta_dugmica_startX[30]=0;
        mesta_dugmica_startY[30]=5;
	               }
	else return rc;

if  ( final_minefield[1][5] != '*') {
	rc = mine_button_create(&mine, fixed, 1, 5, " ", &mine_pbutton_cb,(void *) "-", &mine.btn_31);}
else {rc = mine_button_create(&mine, fixed, 1, 5, " ", &mine_pbutton_cb,(void *) "*", &mine.btn_31);}
    if (rc == EOK) {
		ui_pbutton_set_light(mine.btn_31,true);
		mesta_dugmica_startX[31]=1;
        mesta_dugmica_startY[31]=5;
	               }
	else return rc;

if  ( final_minefield[2][5] != '*') {
	rc = mine_button_create(&mine, fixed, 2, 5, " ", &mine_pbutton_cb,(void *) "-", &mine.btn_32);}
else {rc = mine_button_create(&mine, fixed, 2, 5, " ", &mine_pbutton_cb,(void *) "*", &mine.btn_32);}
    if (rc == EOK) {
		ui_pbutton_set_light(mine.btn_32,true);
		mesta_dugmica_startX[32]=2;
        mesta_dugmica_startY[32]=5;
	               }
	else return rc;

if  ( final_minefield[3][5] != '*') {
	rc = mine_button_create(&mine, fixed, 3, 5, " ", &mine_pbutton_cb,(void *) "-", &mine.btn_33);}
else {rc = mine_button_create(&mine, fixed, 3, 5, " ", &mine_pbutton_cb,(void *) "*", &mine.btn_33);}
    if (rc == EOK) {
		ui_pbutton_set_light(mine.btn_33,true);
		mesta_dugmica_startX[33]=3;
        mesta_dugmica_startY[33]=5;
	               }
	else return rc;

if  ( final_minefield[4][5] != '*') {
	rc = mine_button_create(&mine, fixed, 4, 5, " ", &mine_pbutton_cb,(void *) "-", &mine.btn_34);}
else {rc = mine_button_create(&mine, fixed, 4, 5, " ", &mine_pbutton_cb,(void *) "*", &mine.btn_34);}
    if (rc == EOK) {
		ui_pbutton_set_light(mine.btn_34,true);
		mesta_dugmica_startX[34]=4;
        mesta_dugmica_startY[34]=5;
	               }
	else return rc;

if  ( final_minefield[5][5] != '*') {
	rc = mine_button_create(&mine, fixed, 5, 5, " ", &mine_pbutton_cb,(void *) "-", &mine.btn_35); }
else {rc = mine_button_create(&mine, fixed, 5, 5, " ", &mine_pbutton_cb,(void *) "*", &mine.btn_35);}
    if (rc == EOK) {
		ui_pbutton_set_light(mine.btn_35,true);
		mesta_dugmica_startX[35]=5;
        mesta_dugmica_startY[35]=5;
	               }
	else return rc;
// At last. Buttons for RESTART AND QUIT game.. You can Quit also thry Menu option, or if you click on "X"
    rc = mine_button_lose(&mine, fixed, -5, 5, "RESTART GAME", &mine_pbutton_cb,(char *) "/", &mine.lose);
    if (rc == EOK) {
		ui_pbutton_set_light(mine.lose,true);
		WinMx[0]=-3; WinMy[0]=3;
                	}
	else return rc;

    rc = mine_button_end(&mine, fixed, -5, 5, "QUIT GAME", &mine_pbutton_cb,(char *) "+", &mine.end);
    if (rc == EOK) {
		ui_pbutton_set_light(mine.end,true);
		EndMx[0]=-3; EndMy[0]=3;
                	}
	else return rc;


	ui_window_add(window, ui_fixed_ctl(fixed));

	rc = ui_window_paint(window);
	if (rc != EOK) {
		printf("Neuspelo bojanje prozora.\n");
		return rc;
	}
	ui_run(ui);
	ui_window_destroy(window);
	ui_destroy(ui);
	return 0;
}
