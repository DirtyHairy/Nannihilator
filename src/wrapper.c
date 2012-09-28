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

#ifdef SUPPORT_TRAPS
#define _GNU_SOURCE
#endif

#include <fenv.h>
#include "include/nannihilator.h"

int fpe_supports_traps () {
#ifdef FPE_WITH_TRAP_CONTROL
   return 1;
#else
   return 0;
#endif
}

int fpe_enable_traps (const int excepts) {
#ifdef FPE_WITH_TRAP_CONTROL
   return feenableexcept (excepts);
   fpe_handler_update_mask ();
#else
   return -1;
#endif
}

int fpe_disable_traps (const int excepts) {
#ifdef FPE_WITH_TRAP_CONTROL
   return fedisableexcept (excepts);
   fpe_handler_update_mask ();
#else
   return -1;
#endif
}

int fpe_get_traps () {
#ifdef FPE_WITH_TRAP_CONTROL
   return fegetexcept ();
#else
   return -1;
#endif
}

int fpe_supports_handler () {
#ifdef FPE_WITH_TRAP_HANDLER
   return 1;
#else
   return 0;
#endif
}

#ifndef FPE_WITH_TRAP_HANDLER
   void fpe_handler_arm () {}

   int fpe_handler_install () {return -1;}

   int fpe_handler_uninstall () {return -1;}

   void fpe_handler_update_mask () {}
#endif
