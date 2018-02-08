//  edm - extensible display manager

//  Copyright (C) 1999 John W. Sinclair

//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.

//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.

//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

#include "cursor.h"

// set non-zero to disable special cursor shapes
static int noSpecialCursors = 0;

cursorClass::cursorClass ( void )
{

  display = NULL;
  curCursor = (Cursor) NULL;
  crossHair = (Cursor) NULL;
  tinyCrossHair = (Cursor) NULL;
  wait = (Cursor) NULL;
  no = (Cursor) NULL;
  willSet = (Cursor) NULL;
  willOpen = (Cursor) NULL;
  willOpenWithHelp = (Cursor) NULL;
  run = (Cursor) NULL;
  runWithHelp = (Cursor) NULL;
  upDown = (Cursor) NULL;
  pntrWithHelp = (Cursor) NULL;

}

cursorClass::~cursorClass ( void )
{

  if ( crossHair ) XFreeCursor( display, crossHair );
  if ( tinyCrossHair ) XFreeCursor( display, tinyCrossHair );
  if ( wait ) XFreeCursor( display, wait );
  if ( no ) XFreeCursor( display, no );
  if ( willSet ) XFreeCursor( display, willSet );
  if ( willOpen ) XFreeCursor( display, willOpen );
  if ( willOpenWithHelp ) XFreeCursor( display, willOpenWithHelp );
  if ( run ) XFreeCursor( display, run );
  if ( runWithHelp ) XFreeCursor( display, runWithHelp );
  if ( upDown ) XFreeCursor( display, upDown );
  if ( pntrWithHelp ) XFreeCursor( display, pntrWithHelp );

}

void cursorClass::create (
  Display *dsp,
  Window rootWin,
  Colormap cmap )
{

#define pntrWithHelp_width 32
#define pntrWithHelp_height 32
#define pntrWithHelp_x_hot 5
#define pntrWithHelp_y_hot 3

static unsigned char pntrWithHelp_bits[] = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x42,
   0x20, 0x00, 0x00, 0x40, 0x60, 0x00, 0x00, 0x40, 0xe0, 0x00, 0x00, 0x60,
   0xe0, 0x01, 0x00, 0x30, 0xe0, 0x03, 0x00, 0x08, 0xe0, 0x07, 0x00, 0x08,
   0xe0, 0x0f, 0x00, 0x08, 0xe0, 0x1f, 0x00, 0x00, 0xe0, 0x3f, 0x00, 0x08,
   0xe0, 0x03, 0x00, 0x00, 0xe0, 0x06, 0x00, 0x00, 0x60, 0x06, 0x00, 0x00,
   0x20, 0x0c, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00,
   0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

static unsigned char pntrWithHelp_mask_bits[] = {
   0x00, 0x00, 0x00, 0x7e, 0x00, 0x00, 0x00, 0xff, 0x30, 0x00, 0x00, 0xff,
   0x70, 0x00, 0x00, 0xe7, 0xf0, 0x00, 0x00, 0xf0, 0xf0, 0x01, 0x00, 0xf0,
   0xf0, 0x03, 0x00, 0xfc, 0xf0, 0x07, 0x00, 0x7c, 0xf0, 0x0f, 0x00, 0x1c,
   0xf0, 0x1f, 0x00, 0x1c, 0xf0, 0x3f, 0x00, 0x1c, 0xf0, 0x7f, 0x00, 0x1c,
   0xf0, 0x7f, 0x00, 0x1c, 0xf0, 0x0f, 0x00, 0x00, 0xf0, 0x0f, 0x00, 0x00,
   0x70, 0x1e, 0x00, 0x00, 0x30, 0x1e, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00,
   0x00, 0x3c, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

#define cross_width 16
#define cross_height 16
#define cross_x_hot 7
#define cross_y_hot 8

static unsigned char cross_bits[] = {
   0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x9e, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00,
   0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x00, 0x00};

static unsigned char cross_mask_bits[] = {
   0x00, 0x00, 0xc0, 0x01, 0xc0, 0x01, 0xc0, 0x01, 0xc0, 0x01, 0xc0, 0x01,
   0xc0, 0x01, 0xff, 0x7f, 0xff, 0x7f, 0xff, 0x7f, 0xc0, 0x01, 0xc0, 0x01,
   0xc0, 0x01, 0xc0, 0x01, 0xc0, 0x01, 0xc0, 0x01};

#define tinyCross_width 16
#define tinyCross_height 16
#define tinyCross_x_hot 7
#define tinyCross_y_hot 8

static unsigned char tinyCross_bits[] = {
   0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x86, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x80, 0x00, 0x80, 0x00, 0x00, 0x00};

static unsigned char tinyCross_mask_bits[] = {
   0x00, 0x00, 0xc0, 0x01, 0xc0, 0x01, 0xc0, 0x01, 0xc0, 0x01, 0x00, 0x00,
   0x00, 0x00, 0x0f, 0x78, 0x8f, 0x78, 0x0f, 0x78, 0x00, 0x00, 0x00, 0x00,
   0xc0, 0x01, 0xc0, 0x01, 0xc0, 0x01, 0xc0, 0x01};

#define wait_width 16
#define wait_height 16
#define wait_x_hot 7
#define wait_y_hot 6

static unsigned char wait_bits[] = {
   0xc0, 0x01, 0x30, 0x06, 0x0c, 0x18, 0x04, 0x10, 0x02, 0x24, 0x02, 0x23,
   0x81, 0x41, 0xc1, 0x40, 0x81, 0x41, 0x02, 0x23, 0x02, 0x24, 0x04, 0x10,
   0x0c, 0x18, 0x30, 0x06, 0xc0, 0x01, 0x00, 0x00};

//     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x01, 0x20, 0x02, 0x10, 0x04,
//     0x10, 0x04, 0x00, 0x02, 0x80, 0x01, 0x80, 0x00, 0x80, 0x00, 0x00, 0x00,
//     0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

static unsigned char wait_mask_bits[] = {
   0xf0, 0x07, 0xf8, 0x0f, 0xfc, 0x1f, 0xfe, 0x3f, 0xff, 0x7f, 0xff, 0x7f,
   0xff, 0x7f, 0xff, 0x7f, 0xff, 0x7f, 0xff, 0x7f, 0xff, 0x7f, 0xfe, 0x3f,
   0xfc, 0x1f, 0xf8, 0x0f, 0xf0, 0x07, 0x00, 0x00};

//     0x00, 0x00, 0x00, 0x00, 0xe0, 0x03, 0xf0, 0x07, 0xf8, 0x0f, 0xf8, 0x0f,
//     0xf8, 0x0f, 0xf8, 0x0f, 0xc0, 0x07, 0xc0, 0x03, 0xc0, 0x01, 0xc0, 0x01,
//     0xc0, 0x01, 0xc0, 0x01, 0x00, 0x00, 0x00, 0x00};

#define willSet_width 16
#define willSet_height 16
#define willSet_x_hot 8
#define willSet_y_hot 1
static unsigned char willSet_bits[] = {
   0x00, 0x00, 0xc0, 0x03, 0xc0, 0x03, 0xc0, 0x03, 0xc0, 0x03, 0xc0, 0x03,
   0xc0, 0x03, 0xc0, 0x03, 0xc0, 0x03, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01,
   0x00, 0x00, 0x80, 0x01, 0x80, 0x01, 0x00, 0x00};

static unsigned char willSet_mask_bits[] = {
   0xe0, 0x07, 0xe0, 0x07, 0xe0, 0x07, 0xe0, 0x07, 0xe0, 0x07, 0xe0, 0x07,
   0xe0, 0x07, 0xe0, 0x07, 0xe0, 0x07, 0xe0, 0x07, 0xc0, 0x03, 0xc0, 0x03,
   0xc0, 0x03, 0xc0, 0x03, 0xc0, 0x03, 0xc0, 0x03};

#define willOpen_width 16
#define willOpen_height 16
#define willOpen_x_hot 10
#define willOpen_y_hot 1
static unsigned char willOpen_bits[] = {
   0x00, 0x00, 0xe0, 0x7f, 0x20, 0x40, 0xfe, 0x47, 0x02, 0x44, 0x02, 0x44,
   0x02, 0x44, 0x02, 0x44, 0x02, 0x44, 0x02, 0x44, 0x02, 0x44, 0x02, 0x44,
   0x02, 0x7c, 0x02, 0x04, 0xfe, 0x07, 0x00, 0x00};

static unsigned char willOpen_mask_bits[] = {
   0xf0, 0xff, 0xf0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
   0xff, 0xff, 0xff, 0xff, 0xff, 0x0f, 0xff, 0x0f};

#define willOpenWithHelp_width 32
#define willOpenWithHelp_height 32
#define willOpenWithHelp_x_hot 10
#define willOpenWithHelp_y_hot 17
static unsigned char willOpenWithHelp_bits[] = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x01, 0x00, 0x00, 0x10, 0x02,
   0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x03,
   0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x40, 0x00,
   0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x7f, 0x00, 0x00,
   0x20, 0x40, 0x00, 0x00, 0xfe, 0x47, 0x00, 0x00, 0x02, 0x44, 0x00, 0x00,
   0x02, 0x44, 0x00, 0x00, 0x02, 0x44, 0x00, 0x00, 0x02, 0x44, 0x00, 0x00,
   0x02, 0x44, 0x00, 0x00, 0x02, 0x44, 0x00, 0x00, 0x02, 0x44, 0x00, 0x00,
   0x02, 0x44, 0x00, 0x00, 0x02, 0x7c, 0x00, 0x00, 0x02, 0x04, 0x00, 0x00,
   0xfe, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

static unsigned char willOpenWithHelp_mask_bits[] = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0xf0, 0x03, 0x00, 0x00, 0xf8, 0x07, 0x00, 0x00, 0xf8, 0x07,
   0x00, 0x00, 0x38, 0x07, 0x00, 0x00, 0x80, 0x07, 0x00, 0x00, 0xc0, 0x07,
   0x00, 0x00, 0xe0, 0x07, 0x00, 0x00, 0xe0, 0x03, 0x00, 0x00, 0xe0, 0x00,
   0x00, 0x00, 0xe0, 0x00, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x00, 0xe0, 0x00,
   0x00, 0x00, 0xe0, 0x00, 0xf0, 0xff, 0x00, 0x00, 0xf0, 0xff, 0x00, 0x00,
   0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00,
   0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00,
   0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00,
   0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00,
   0xff, 0x0f, 0x00, 0x00, 0xff, 0x0f, 0x00, 0x00};

#define run_width 32
#define run_height 32
#define run_x_hot 16
#define run_y_hot 9
static unsigned char run_bits[] = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0xf8, 0x01, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00,
   0x08, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0xf8, 0x00, 0x00, 0x00,
   0x08, 0x88, 0x38, 0x1c, 0x08, 0x50, 0x44, 0x02, 0x08, 0x20, 0x7c, 0x02,
   0x08, 0x20, 0x04, 0x02, 0x08, 0x50, 0x04, 0x02, 0xf8, 0x89, 0x78, 0x1c,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

static unsigned char run_mask_bits[] = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x03, 0x00, 0x00,
   0xfc, 0x03, 0x00, 0x00, 0xfc, 0x03, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00,
   0x1c, 0x00, 0x00, 0x00, 0xfc, 0x01, 0x00, 0x00, 0xfc, 0x89, 0x38, 0x3e,
   0xfc, 0xdd, 0x7d, 0x3e, 0x1c, 0xf8, 0x7e, 0x3f, 0x1c, 0x70, 0x7e, 0x07,
   0x1c, 0x70, 0x7e, 0x07, 0xfc, 0xfb, 0x7e, 0x3f, 0xfc, 0xdf, 0x7d, 0x3e,
   0xfc, 0x8b, 0x78, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

#define runWithHelp_width 32
#define runWithHelp_height 32
#define runWithHelp_x_hot 16
#define runWithHelp_y_hot 9
static unsigned char runWithHelp_bits[] = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x42,
   0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x60,
   0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x08,
   0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x7e, 0x00, 0x00, 0x08,
   0x02, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00,
   0x02, 0x00, 0x00, 0x00, 0x3e, 0x00, 0x00, 0x00, 0x02, 0x22, 0x0e, 0x07,
   0x02, 0x14, 0x91, 0x00, 0x02, 0x08, 0x9f, 0x00, 0x02, 0x08, 0x81, 0x00,
   0x02, 0x14, 0x81, 0x00, 0x7e, 0x22, 0x1e, 0x07, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

static unsigned char runWithHelp_mask_bits[] = {
   0x00, 0x00, 0x00, 0x7e, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff,
   0x00, 0x00, 0x00, 0xe7, 0x00, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x00, 0xf0,
   0x00, 0x00, 0x00, 0xfc, 0x00, 0x00, 0x00, 0x7c, 0x00, 0x00, 0x00, 0x1c,
   0x00, 0x00, 0x00, 0x1c, 0xff, 0x00, 0x00, 0x1c, 0xff, 0x00, 0x00, 0x1c,
   0xff, 0x00, 0x00, 0x1c, 0x07, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00,
   0x7f, 0x00, 0x00, 0x00, 0x7f, 0x22, 0x8e, 0x0f, 0x7f, 0x77, 0x9f, 0x0f,
   0x07, 0xbe, 0xdf, 0x0f, 0x07, 0x9c, 0xdf, 0x01, 0x07, 0x9c, 0xdf, 0x01,
   0xff, 0xbe, 0xdf, 0x0f, 0xff, 0x77, 0x9f, 0x0f, 0xff, 0x22, 0x1e, 0x0f,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

#define upDown_width 16
#define upDown_height 16
#define upDown_x_hot 8
#define upDown_y_hot 1
static unsigned char upDown_bits[] = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x04, 0x30, 0x0c, 0x38, 0x1c,
   0x3c, 0x3c, 0x3e, 0x7c, 0x3c, 0x3c, 0x38, 0x1c, 0x30, 0x0c, 0x20, 0x04,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

static unsigned char upDown_mask_bits[] = {
   0x00, 0x00, 0x00, 0x00, 0x60, 0x06, 0x70, 0x0e, 0x78, 0x1e, 0x7c, 0x3e,
   0x7e, 0x7e, 0x7f, 0xfe, 0x7e, 0x7e, 0x7c, 0x3e, 0x78, 0x1e, 0x70, 0x0e,
   0x60, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

#define no_width 64
#define no_height 64
#define no_x_hot 31
#define no_y_hot 31
static unsigned char no_bits[] = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0,
   0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xff, 0xff, 0x00, 0x00, 0x00,
   0x00, 0x00, 0xf0, 0xff, 0xff, 0x07, 0x00, 0x00, 0x00, 0x00, 0xfc, 0xff,
   0xff, 0x1f, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0x7f, 0x00, 0x00,
   0x00, 0xc0, 0xff, 0xff, 0xff, 0xff, 0x01, 0x00, 0x00, 0xe0, 0xff, 0xff,
   0xff, 0xff, 0x03, 0x00, 0x00, 0xf0, 0xff, 0xff, 0xff, 0xff, 0x07, 0x00,
   0x00, 0xf8, 0xff, 0xff, 0xff, 0xff, 0x0f, 0x00, 0x00, 0xfc, 0xff, 0x01,
   0xc0, 0xff, 0x1f, 0x00, 0x00, 0xfe, 0x3f, 0x00, 0x00, 0xfe, 0x3f, 0x00,
   0x00, 0xff, 0x0f, 0x00, 0x00, 0xf8, 0x7f, 0x00, 0x80, 0xff, 0x07, 0x00,
   0x00, 0xf0, 0xff, 0x00, 0xc0, 0xff, 0x07, 0x00, 0x00, 0xc0, 0xff, 0x01,
   0xc0, 0xff, 0x0f, 0x00, 0x00, 0x80, 0xff, 0x01, 0xe0, 0xff, 0x1f, 0x00,
   0x00, 0x00, 0xff, 0x03, 0xe0, 0xff, 0x3f, 0x00, 0x00, 0x00, 0xfe, 0x03,
   0xf0, 0xff, 0x7f, 0x00, 0x00, 0x00, 0xfe, 0x07, 0xf0, 0xdf, 0xff, 0x00,
   0x00, 0x00, 0xfc, 0x07, 0xf8, 0x8f, 0xff, 0x01, 0x00, 0x00, 0xf8, 0x0f,
   0xf8, 0x0f, 0xff, 0x03, 0x00, 0x00, 0xf8, 0x0f, 0xf8, 0x07, 0xfe, 0x07,
   0x00, 0x00, 0xf0, 0x0f, 0xfc, 0x07, 0xfc, 0x0f, 0x00, 0x00, 0xf0, 0x1f,
   0xfc, 0x07, 0xf8, 0x1f, 0x00, 0x00, 0xf0, 0x1f, 0xfc, 0x03, 0xf0, 0x3f,
   0x00, 0x00, 0xe0, 0x1f, 0xfc, 0x03, 0xe0, 0x7f, 0x00, 0x00, 0xe0, 0x1f,
   0xfc, 0x03, 0xc0, 0xff, 0x00, 0x00, 0xe0, 0x1f, 0xfc, 0x03, 0x80, 0xff,
   0x01, 0x00, 0xe0, 0x1f, 0xfe, 0x03, 0x00, 0xff, 0x03, 0x00, 0xe0, 0x3f,
   0xfe, 0x03, 0x00, 0xfe, 0x07, 0x00, 0xe0, 0x3f, 0xfe, 0x03, 0x00, 0xfc,
   0x0f, 0x00, 0xe0, 0x3f, 0xfe, 0x03, 0x00, 0xf8, 0x1f, 0x00, 0xe0, 0x3f,
   0xfe, 0x03, 0x00, 0xf0, 0x3f, 0x00, 0xe0, 0x3f, 0xfc, 0x03, 0x00, 0xe0,
   0x7f, 0x00, 0xe0, 0x1f, 0xfc, 0x03, 0x00, 0xc0, 0xff, 0x00, 0xe0, 0x1f,
   0xfc, 0x03, 0x00, 0x80, 0xff, 0x01, 0xe0, 0x1f, 0xfc, 0x03, 0x00, 0x00,
   0xff, 0x03, 0xe0, 0x1f, 0xfc, 0x07, 0x00, 0x00, 0xfe, 0x07, 0xf0, 0x1f,
   0xfc, 0x07, 0x00, 0x00, 0xfc, 0x0f, 0xf0, 0x1f, 0xf8, 0x07, 0x00, 0x00,
   0xf8, 0x1f, 0xf0, 0x0f, 0xf8, 0x0f, 0x00, 0x00, 0xf0, 0x3f, 0xf8, 0x0f,
   0xf8, 0x0f, 0x00, 0x00, 0xe0, 0x7f, 0xf8, 0x0f, 0xf0, 0x1f, 0x00, 0x00,
   0xc0, 0xff, 0xfc, 0x07, 0xf0, 0x3f, 0x00, 0x00, 0x80, 0xff, 0xff, 0x07,
   0xe0, 0x3f, 0x00, 0x00, 0x00, 0xff, 0xff, 0x03, 0xe0, 0x7f, 0x00, 0x00,
   0x00, 0xfe, 0xff, 0x03, 0xc0, 0xff, 0x00, 0x00, 0x00, 0xfc, 0xff, 0x01,
   0xc0, 0xff, 0x01, 0x00, 0x00, 0xf8, 0xff, 0x01, 0x80, 0xff, 0x07, 0x00,
   0x00, 0xf0, 0xff, 0x00, 0x00, 0xff, 0x0f, 0x00, 0x00, 0xf8, 0x7f, 0x00,
   0x00, 0xfe, 0x3f, 0x00, 0x00, 0xfe, 0x3f, 0x00, 0x00, 0xfc, 0xff, 0x01,
   0xc0, 0xff, 0x1f, 0x00, 0x00, 0xf8, 0xff, 0xff, 0xff, 0xff, 0x0f, 0x00,
   0x00, 0xf0, 0xff, 0xff, 0xff, 0xff, 0x07, 0x00, 0x00, 0xe0, 0xff, 0xff,
   0xff, 0xff, 0x03, 0x00, 0x00, 0xc0, 0xff, 0xff, 0xff, 0xff, 0x01, 0x00,
   0x00, 0x00, 0xff, 0xff, 0xff, 0x7f, 0x00, 0x00, 0x00, 0x00, 0xfc, 0xff,
   0xff, 0x1f, 0x00, 0x00, 0x00, 0x00, 0xf0, 0xff, 0xff, 0x07, 0x00, 0x00,
   0x00, 0x00, 0x80, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0,
   0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

static unsigned char no_mask_bits[] = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0,
   0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xff, 0xff, 0x00, 0x00, 0x00,
   0x00, 0x00, 0xf0, 0xff, 0xff, 0x07, 0x00, 0x00, 0x00, 0x00, 0xfc, 0xff,
   0xff, 0x1f, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0x7f, 0x00, 0x00,
   0x00, 0xc0, 0xff, 0xff, 0xff, 0xff, 0x01, 0x00, 0x00, 0xe0, 0xff, 0xff,
   0xff, 0xff, 0x03, 0x00, 0x00, 0xf0, 0xff, 0xff, 0xff, 0xff, 0x07, 0x00,
   0x00, 0xf8, 0xff, 0xff, 0xff, 0xff, 0x0f, 0x00, 0x00, 0xfc, 0xff, 0x01,
   0xc0, 0xff, 0x1f, 0x00, 0x00, 0xfe, 0x3f, 0x00, 0x00, 0xfe, 0x3f, 0x00,
   0x00, 0xff, 0x0f, 0x00, 0x00, 0xf8, 0x7f, 0x00, 0x80, 0xff, 0x07, 0x00,
   0x00, 0xf0, 0xff, 0x00, 0xc0, 0xff, 0x07, 0x00, 0x00, 0xc0, 0xff, 0x01,
   0xc0, 0xff, 0x0f, 0x00, 0x00, 0x80, 0xff, 0x01, 0xe0, 0xff, 0x1f, 0x00,
   0x00, 0x00, 0xff, 0x03, 0xe0, 0xff, 0x3f, 0x00, 0x00, 0x00, 0xfe, 0x03,
   0xf0, 0xff, 0x7f, 0x00, 0x00, 0x00, 0xfe, 0x07, 0xf0, 0xdf, 0xff, 0x00,
   0x00, 0x00, 0xfc, 0x07, 0xf8, 0x8f, 0xff, 0x01, 0x00, 0x00, 0xf8, 0x0f,
   0xf8, 0x0f, 0xff, 0x03, 0x00, 0x00, 0xf8, 0x0f, 0xf8, 0x07, 0xfe, 0x07,
   0x00, 0x00, 0xf0, 0x0f, 0xfc, 0x07, 0xfc, 0x0f, 0x00, 0x00, 0xf0, 0x1f,
   0xfc, 0x07, 0xf8, 0x1f, 0x00, 0x00, 0xf0, 0x1f, 0xfc, 0x03, 0xf0, 0x3f,
   0x00, 0x00, 0xe0, 0x1f, 0xfc, 0x03, 0xe0, 0x7f, 0x00, 0x00, 0xe0, 0x1f,
   0xfc, 0x03, 0xc0, 0xff, 0x00, 0x00, 0xe0, 0x1f, 0xfc, 0x03, 0x80, 0xff,
   0x01, 0x00, 0xe0, 0x1f, 0xfe, 0x03, 0x00, 0xff, 0x03, 0x00, 0xe0, 0x3f,
   0xfe, 0x03, 0x00, 0xfe, 0x07, 0x00, 0xe0, 0x3f, 0xfe, 0x03, 0x00, 0xfc,
   0x0f, 0x00, 0xe0, 0x3f, 0xfe, 0x03, 0x00, 0xf8, 0x1f, 0x00, 0xe0, 0x3f,
   0xfe, 0x03, 0x00, 0xf0, 0x3f, 0x00, 0xe0, 0x3f, 0xfc, 0x03, 0x00, 0xe0,
   0x7f, 0x00, 0xe0, 0x1f, 0xfc, 0x03, 0x00, 0xc0, 0xff, 0x00, 0xe0, 0x1f,
   0xfc, 0x03, 0x00, 0x80, 0xff, 0x01, 0xe0, 0x1f, 0xfc, 0x03, 0x00, 0x00,
   0xff, 0x03, 0xe0, 0x1f, 0xfc, 0x07, 0x00, 0x00, 0xfe, 0x07, 0xf0, 0x1f,
   0xfc, 0x07, 0x00, 0x00, 0xfc, 0x0f, 0xf0, 0x1f, 0xf8, 0x07, 0x00, 0x00,
   0xf8, 0x1f, 0xf0, 0x0f, 0xf8, 0x0f, 0x00, 0x00, 0xf0, 0x3f, 0xf8, 0x0f,
   0xf8, 0x0f, 0x00, 0x00, 0xe0, 0x7f, 0xf8, 0x0f, 0xf0, 0x1f, 0x00, 0x00,
   0xc0, 0xff, 0xfc, 0x07, 0xf0, 0x3f, 0x00, 0x00, 0x80, 0xff, 0xff, 0x07,
   0xe0, 0x3f, 0x00, 0x00, 0x00, 0xff, 0xff, 0x03, 0xe0, 0x7f, 0x00, 0x00,
   0x00, 0xfe, 0xff, 0x03, 0xc0, 0xff, 0x00, 0x00, 0x00, 0xfc, 0xff, 0x01,
   0xc0, 0xff, 0x01, 0x00, 0x00, 0xf8, 0xff, 0x01, 0x80, 0xff, 0x07, 0x00,
   0x00, 0xf0, 0xff, 0x00, 0x00, 0xff, 0x0f, 0x00, 0x00, 0xf8, 0x7f, 0x00,
   0x00, 0xfe, 0x3f, 0x00, 0x00, 0xfe, 0x3f, 0x00, 0x00, 0xfc, 0xff, 0x01,
   0xc0, 0xff, 0x1f, 0x00, 0x00, 0xf8, 0xff, 0xff, 0xff, 0xff, 0x0f, 0x00,
   0x00, 0xf0, 0xff, 0xff, 0xff, 0xff, 0x07, 0x00, 0x00, 0xe0, 0xff, 0xff,
   0xff, 0xff, 0x03, 0x00, 0x00, 0xc0, 0xff, 0xff, 0xff, 0xff, 0x01, 0x00,
   0x00, 0x00, 0xff, 0xff, 0xff, 0x7f, 0x00, 0x00, 0x00, 0x00, 0xfc, 0xff,
   0xff, 0x1f, 0x00, 0x00, 0x00, 0x00, 0xf0, 0xff, 0xff, 0x07, 0x00, 0x00,
   0x00, 0x00, 0x80, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0,
   0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

#if 0
#define no_width 32
#define no_height 32
#define no_x_hot 15
#define no_y_hot 15
static unsigned char no_bits[] = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x03, 0x00, 0x00, 0xfc, 0x1f, 0x00,
   0x00, 0x0f, 0x78, 0x00, 0x80, 0x01, 0xc0, 0x00, 0xe0, 0x00, 0x80, 0x03,
   0x20, 0x00, 0x00, 0x03, 0x30, 0x00, 0x80, 0x07, 0x18, 0x00, 0xc0, 0x0c,
   0x08, 0x00, 0x60, 0x08, 0x0c, 0x00, 0x30, 0x18, 0x0c, 0x00, 0x18, 0x18,
   0x04, 0x00, 0x0c, 0x10, 0x06, 0x00, 0x06, 0x30, 0x06, 0x00, 0x03, 0x30,
   0x06, 0x80, 0x01, 0x30, 0x06, 0xc0, 0x00, 0x30, 0x06, 0x60, 0x00, 0x30,
   0x04, 0x30, 0x00, 0x10, 0x0c, 0x18, 0x00, 0x18, 0x0c, 0x0c, 0x00, 0x18,
   0x08, 0x06, 0x00, 0x08, 0x18, 0x03, 0x00, 0x0c, 0xb0, 0x01, 0x00, 0x06,
   0xe0, 0x00, 0x00, 0x02, 0xe0, 0x00, 0x80, 0x03, 0x80, 0x01, 0xc0, 0x00,
   0x00, 0x0f, 0x78, 0x00, 0x00, 0xfc, 0x1f, 0x00, 0x00, 0xe0, 0x03, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

static unsigned char no_mask_bits[] = {
   0x00, 0xe0, 0x03, 0x00, 0x00, 0xfc, 0x1f, 0x00, 0x00, 0xff, 0x7f, 0x00,
   0x80, 0xff, 0xff, 0x00, 0xe0, 0x0f, 0xf8, 0x03, 0xf0, 0x01, 0xc0, 0x07,
   0xf0, 0x00, 0x80, 0x07, 0x78, 0x00, 0xc0, 0x0f, 0x3c, 0x00, 0xe0, 0x1f,
   0x1c, 0x00, 0xf0, 0x1c, 0x1e, 0x00, 0x78, 0x3c, 0x1e, 0x00, 0x3c, 0x3c,
   0x0e, 0x00, 0x1e, 0x38, 0x0f, 0x00, 0x0f, 0x78, 0x0f, 0x80, 0x07, 0x78,
   0x0f, 0xc0, 0x03, 0x78, 0x0f, 0xe0, 0x01, 0x78, 0x0f, 0xf0, 0x00, 0x78,
   0x0e, 0x78, 0x00, 0x38, 0x1e, 0x3c, 0x00, 0x3c, 0x1e, 0x1e, 0x00, 0x3c,
   0x1c, 0x0f, 0x00, 0x1c, 0xbc, 0x07, 0x00, 0x1e, 0xf8, 0x03, 0x00, 0x0f,
   0xf0, 0x01, 0x80, 0x07, 0xf0, 0x01, 0xc0, 0x07, 0xe0, 0x0f, 0xf8, 0x03,
   0x80, 0xff, 0xff, 0x00, 0x00, 0xff, 0x7f, 0x00, 0x00, 0xfc, 0x1f, 0x00,
   0x00, 0xe0, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00};
#endif

  display = dsp;
  colormap = cmap;
  XParseColor( dsp, cmap, "Black", &shapeColor );
  XParseColor( dsp, cmap, "White", &maskColor );


  crossHairShape = XCreatePixmapFromBitmapData ( dsp, rootWin, (char *) cross_bits,
   cross_width, cross_height, 1, 0, 1 );

  crossHairMask = XCreatePixmapFromBitmapData ( dsp, rootWin, (char *) cross_mask_bits,
   cross_width, cross_height, 1, 0, 1 );

  crossHair = XCreatePixmapCursor( dsp, crossHairShape, crossHairMask,
   &shapeColor, &maskColor, cross_x_hot, cross_y_hot );


  tinyCrossHairShape = XCreatePixmapFromBitmapData ( dsp, rootWin,
   (char *) tinyCross_bits, tinyCross_width, tinyCross_height, 1, 0, 1 );

  tinyCrossHairMask = XCreatePixmapFromBitmapData ( dsp, rootWin,
   (char *) tinyCross_mask_bits, tinyCross_width, tinyCross_height, 1, 0, 1 );

  tinyCrossHair = XCreatePixmapCursor( dsp, tinyCrossHairShape,
   tinyCrossHairMask, &shapeColor, &maskColor, tinyCross_x_hot,
   tinyCross_y_hot );


  waitShape = XCreatePixmapFromBitmapData ( dsp, rootWin, (char *) wait_bits,
   wait_width, wait_height, 1, 0, 1 );

  waitMask = XCreatePixmapFromBitmapData ( dsp, rootWin, (char *) wait_mask_bits,
   wait_width, wait_height, 1, 0, 1 );

  wait = XCreatePixmapCursor( dsp, waitShape, waitMask,
   &shapeColor, &maskColor, wait_x_hot, wait_y_hot );

  noShape = XCreatePixmapFromBitmapData ( dsp, rootWin, (char *) no_bits,
   no_width, no_height, 1, 0, 1 );

  noMask = XCreatePixmapFromBitmapData ( dsp, rootWin, (char *) no_mask_bits,
   no_width, no_height, 1, 0, 1 );

  XParseColor( dsp, cmap, "Red", &noShapeColor );
  XParseColor( dsp, cmap, "White", &noMaskColor );

  no = XCreatePixmapCursor( dsp, noShape, noMask,
   &noShapeColor, &noMaskColor, no_x_hot, no_y_hot );

  willSetShape = XCreatePixmapFromBitmapData ( dsp, rootWin,
   (char *) willSet_bits, willSet_width, willSet_height, 1, 0, 1 );

  willSetMask = XCreatePixmapFromBitmapData ( dsp, rootWin,
   (char *) willSet_mask_bits, willSet_width, willSet_height, 1, 0, 1 );

  willSet = XCreatePixmapCursor( dsp, willSetShape,
   willSetMask, &shapeColor, &maskColor, willSet_x_hot,
   willSet_y_hot );

  willOpenShape = XCreatePixmapFromBitmapData ( dsp, rootWin,
   (char *) willOpen_bits, willOpen_width, willOpen_height, 1, 0, 1 );

  willOpenMask = XCreatePixmapFromBitmapData ( dsp, rootWin,
   (char *) willOpen_mask_bits, willOpen_width, willOpen_height, 1, 0, 1 );

  willOpen = XCreatePixmapCursor( dsp, willOpenShape,
   willOpenMask, &shapeColor, &maskColor, willOpen_x_hot,
   willOpen_y_hot );

  willOpenWithHelpShape = XCreatePixmapFromBitmapData ( dsp, rootWin,
   (char *) willOpenWithHelp_bits, willOpenWithHelp_width, willOpenWithHelp_height,
   1, 0, 1 );

  willOpenWithHelpMask = XCreatePixmapFromBitmapData ( dsp, rootWin,
   (char *) willOpenWithHelp_mask_bits, willOpenWithHelp_width, willOpenWithHelp_height,
   1, 0, 1 );

  willOpenWithHelp = XCreatePixmapCursor( dsp, willOpenWithHelpShape,
   willOpenWithHelpMask, &shapeColor, &maskColor, willOpenWithHelp_x_hot,
   willOpenWithHelp_y_hot );

  runShape = XCreatePixmapFromBitmapData ( dsp, rootWin,
   (char *) run_bits, run_width, run_height, 1, 0, 1 );

  runMask = XCreatePixmapFromBitmapData ( dsp, rootWin,
   (char *) run_mask_bits, run_width, run_height, 1, 0, 1 );

  run = XCreatePixmapCursor( dsp, runShape,
   runMask, &shapeColor, &maskColor, run_x_hot,
   run_y_hot );

  runWithHelpShape = XCreatePixmapFromBitmapData ( dsp, rootWin,
   (char *) runWithHelp_bits, runWithHelp_width, runWithHelp_height, 1, 0, 1 );

  runWithHelpMask = XCreatePixmapFromBitmapData ( dsp, rootWin,
   (char *) runWithHelp_mask_bits, runWithHelp_width, runWithHelp_height, 1, 0, 1 );

  runWithHelp = XCreatePixmapCursor( dsp, runWithHelpShape,
   runWithHelpMask, &shapeColor, &maskColor, runWithHelp_x_hot,
   runWithHelp_y_hot );

  upDownShape = XCreatePixmapFromBitmapData ( dsp, rootWin,
   (char *) upDown_bits, upDown_width, upDown_height, 1, 0, 1 );

  upDownMask = XCreatePixmapFromBitmapData ( dsp, rootWin,
   (char *) upDown_mask_bits, upDown_width, upDown_height, 1, 0, 1 );

  upDown = XCreatePixmapCursor( dsp, upDownShape,
   upDownMask, &shapeColor, &maskColor, upDown_x_hot,
   upDown_y_hot );

  pntrWithHelpShape = XCreatePixmapFromBitmapData ( dsp, rootWin,
   (char *) pntrWithHelp_bits, pntrWithHelp_width, pntrWithHelp_height,
   1, 0, 1 );

  pntrWithHelpMask = XCreatePixmapFromBitmapData ( dsp, rootWin,
   (char *) pntrWithHelp_mask_bits, pntrWithHelp_width, pntrWithHelp_height,
   1, 0, 1 );

  pntrWithHelp = XCreatePixmapCursor( dsp, pntrWithHelpShape,
   pntrWithHelpMask, &shapeColor, &maskColor, pntrWithHelp_x_hot,
   pntrWithHelp_y_hot );

}

int cursorClass::set (
  Window win,
  int cursorId )
{

  if ( !display ) return 0;

  switch ( cursorId ) {

  case CURSOR_K_DEFAULT:
    XUndefineCursor( display, win );
    curCursor = (Cursor) NULL;
    break;

  case CURSOR_K_CROSSHAIR:
    XDefineCursor( display, win, crossHair );
    curCursor = crossHair;
    break;

  case CURSOR_K_TINYCROSSHAIR:
    XDefineCursor( display, win, tinyCrossHair );
    curCursor = tinyCrossHair;
    break;

  case CURSOR_K_WAIT:
    XDefineCursor( display, win, wait );
    curCursor = wait;
    break;

  case CURSOR_K_NO:
    XDefineCursor( display, win, no );
    curCursor = no;
    break;

  case CURSOR_K_WILL_SET:
    if ( noSpecialCursors ) {
      XUndefineCursor( display, win );
      curCursor = (Cursor) NULL;
      break;
    }
    XDefineCursor( display, win, willSet );
    curCursor = willSet;
    break;

  case CURSOR_K_WILL_OPEN:
    XDefineCursor( display, win, willOpen );
    curCursor = willOpen;
    break;

  case CURSOR_K_WILL_OPEN_WITH_HELP:
    XDefineCursor( display, win, willOpenWithHelp );
    curCursor = willOpenWithHelp;
    break;

  case CURSOR_K_RUN:
    if ( noSpecialCursors ) {
      XUndefineCursor( display, win );
      curCursor = (Cursor) NULL;
      break;
    }
    XDefineCursor( display, win, run );
    curCursor = run;
    break;

  case CURSOR_K_RUN_WITH_HELP:
    if ( noSpecialCursors ) {
      XUndefineCursor( display, win );
      curCursor = (Cursor) NULL;
      break;
    }
    XDefineCursor( display, win, runWithHelp );
    curCursor = runWithHelp;
    break;

  case CURSOR_K_UPDOWN:
    XDefineCursor( display, win, upDown );
    curCursor = upDown;
    break;

  case CURSOR_K_PNTR_WITH_HELP:
    if ( noSpecialCursors ) {
      XUndefineCursor( display, win );
      curCursor = (Cursor) NULL;
      break;
    }
    XDefineCursor( display, win, pntrWithHelp );
    curCursor = pntrWithHelp;
    break;

  default:
    return 0;

  }

  return 1;

}

int cursorClass::setColor (
  unsigned int fg,
  unsigned int bg )
{

int stat;
XColor fgc, bgc;

  if ( !display ) return 0;
  if ( curCursor == ( Cursor ) NULL ) return 1;

  fgc.pixel = fg;
  stat = XQueryColor( display, colormap, &fgc );
  if ( !stat ) return 0;

  bgc.pixel = bg;
  stat = XQueryColor( display, colormap, &bgc );
  if ( !stat ) return 0;

  XRecolorCursor( display, curCursor, &fgc, &bgc );

  return 1;

}