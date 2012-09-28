/***********************************************************************
*
* This file is part of Nannihilator.
*
* Copyright (C) 2012- by 
*     Christian Speckner <cnspeckn@googlemail.com>
*
* Nannihilator is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by 
* the Free Software Foundation; either version 3, or (at your option)
* any later version.
*
* Nannihilator is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*
************************************************************************/

#ifndef FPE_HANDLER_H
#define FPE_HANDLER_H

// Update the stored FPE mask.
void fpe_handler_update_mask ();

// (Re)arm the handler, making traps signaling.
void fpe_handler_arm ();

// Install the handler.
int fpe_handler_install ();

// Uninstall the handler.
int fpe_handler_uninstall ();

#endif
