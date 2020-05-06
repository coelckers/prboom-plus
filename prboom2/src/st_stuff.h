/* Emacs style mode select   -*- C++ -*-
 *-----------------------------------------------------------------------------
 *
 *
 *  PrBoom: a Doom port merged with LxDoom and LSDLDoom
 *  based on BOOM, a modified and improved DOOM engine
 *  Copyright (C) 1999 by
 *  id Software, Chi Hoang, Lee Killough, Jim Flynn, Rand Phares, Ty Halderman
 *  Copyright (C) 1999-2000 by
 *  Jess Haas, Nicolas Kalkhof, Colin Phipps, Florian Schulze
 *  Copyright 2005, 2006 by
 *  Florian Schulze, Colin Phipps, Neil Stevens, Andrey Budko
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 2
 *  of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
 *  02111-1307, USA.
 *
 * DESCRIPTION:
 *      Status bar code.
 *      Does the face/direction indicator animatin.
 *      Does palette indicators as well (red pain/berserk, bright pickup)
 *
 *-----------------------------------------------------------------------------*/

#ifndef __STSTUFF_H__
#define __STSTUFF_H__

#include "doomtype.h"
#include "d_event.h"
#include "r_defs.h"

// Size of statusbar.
// Now sensitive for scaling.

// proff 08/18/98: Changed for high-res
#define ST_HEIGHT 32
#define ST_WIDTH  320
#define ST_Y      (200 - ST_HEIGHT)

// e6y: wide-res
extern int ST_SCALED_HEIGHT;
extern int ST_SCALED_WIDTH;
extern int ST_SCALED_Y;

//
// STATUS BAR
//

// Called by main loop.
dboolean ST_Responder(event_t* ev);

// Called by main loop.
void ST_Ticker(void);

// Called by main loop.
void ST_Drawer(dboolean st_statusbaron, dboolean refresh, dboolean fullmenu);

// Called when the console player is spawned on each level.
void ST_Start(void);

// Called by startup code.
void ST_Init(void);

// After changing videomode;
void ST_SetResolution(void);

// States for status bar code.
typedef enum
{
  AutomapState,
  FirstPersonState
} st_stateenum_t;

// States for the chat code.
typedef enum
{
  StartChatState,
  WaitDestState,
  GetChatState
} st_chatstateenum_t;

// killough 5/2/98: moved from m_misc.c:

extern int health_red;    // health amount less than which status is red
extern int health_yellow; // health amount less than which status is yellow
extern int health_green;  // health amount above is blue, below is green
extern int armor_red;     // armor amount less than which status is red
extern int armor_yellow;  // armor amount less than which status is yellow
extern int armor_green;   // armor amount above is blue, below is green
extern int ammo_red;      // ammo percent less than which status is red
extern int ammo_yellow;   // ammo percent less is yellow more green
extern int sts_always_red;// status numbers do not change colors
extern int sts_pct_always_gray;// status percents do not change colors
extern int sts_traditional_keys;  // display keys the traditional way
extern int sts_armorcolor_type;  // armor color depends on type

extern int st_palette;    // cph 2006/04/06 - make palette visible

typedef enum {
  ammo_colour_behaviour_no,
  ammo_colour_behaviour_full_only,
  ammo_colour_behaviour_yes,
  ammo_colour_behaviour_max
} ammo_colour_behaviour_t;
extern ammo_colour_behaviour_t ammo_colour_behaviour;
extern const char *ammo_colour_behaviour_list[];

// e6y: makes sense for wide resolutions
extern patchnum_t grnrock;
extern patchnum_t brdr_t, brdr_b, brdr_l, brdr_r;
extern patchnum_t brdr_tl, brdr_tr, brdr_bl, brdr_br;

#endif
