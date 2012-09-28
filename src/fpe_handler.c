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

#define _GNU_SOURCE

#include <stdio.h>
#include <signal.h>
#include <ucontext.h>
#include <fenv.h>
#include <execinfo.h>
#include <stdlib.h>
#include "include/fpe_handler.h"

static int fpemask;
static struct sigaction saved_sa;
static int active = 0;

#define MXCSR_MASK     0x00001F90
#define CWD_MASK       0x0000003F
#define MXCSR_FLAGS    0x0000003F

#define FPE_INVALID    1
#define FPE_DENORMAL   2
#define FPE_ZERO       4
#define FPE_OVERFLOW   8
#define FPE_UNDERFLOW  16
#define FPE_PRECISION  32

#define FRAMES 50
#define DELIMIT "*******************************************\n"

static void print_backtrace (int off) {
   if (off <= 0) return;
   void* trace[FRAMES];
   int tracelen = backtrace (trace, FRAMES);
   if (tracelen < off) {
      fprintf (stderr, "No backtrace available %i\n", off);
      return;
   }
   char** trace_strings = backtrace_symbols (trace, tracelen);
   if (!trace_strings) {
      fprintf (stderr, "No backtrace available\n");
      return;
   }
   fprintf (stderr, "Backtrace:\n");
   int i;
   for (i = off; i < tracelen; i++) {
      fprintf (stderr, "   %s\n", trace_strings[i]);
   }
   free (trace_strings);
}

static void fpe_handler (int signal, siginfo_t *siginfo, void* uc) {
   struct _fpstate* fps =
      (struct _fpstate*) ((ucontext_t*) uc)->uc_mcontext.fpregs;
   if (signal == SIGFPE) {
      int fpe_exc = (fps->mxcsr & MXCSR_FLAGS) | (fps->swd & CWD_MASK);
      fps->mxcsr |= MXCSR_MASK;
      fps->cwd |= CWD_MASK;
      fprintf (stderr, DELIMIT);
      fprintf (stderr,"Floating point exception:");
      if (fpe_exc & FPE_INVALID) fprintf (stderr, " invalid");
      if (fpe_exc & FPE_DENORMAL) fprintf (stderr, " denormal");
      if (fpe_exc & FPE_ZERO) fprintf (stderr, " zero");
      if (fpe_exc & FPE_UNDERFLOW) fprintf (stderr, " undeflow");
      if (fpe_exc & FPE_OVERFLOW) fprintf (stderr, " overflow");
      if (fpe_exc & FPE_PRECISION) fprintf (stderr, " precision");
      fprintf (stderr, "\n");
      print_backtrace (2);
      fprintf (stderr, DELIMIT);
   }
}

void fpe_handler_update_mask () {
   fpemask = fegetexcept ();
}

void fpe_handler_arm () {
   if (!active) return;
   feclearexcept (FE_ALL_EXCEPT);
   feenableexcept (fpemask);
}

int fpe_handler_install () {
   if (active) return 1;
   struct sigaction sa;
   sa.sa_sigaction = &fpe_handler;
   sigemptyset (&sa.sa_mask);
   sa.sa_flags = SA_SIGINFO;
   if (sigaction (SIGFPE, &sa, &saved_sa)) return 0;
   active = 1;
   fpe_handler_update_mask ();
   fpe_handler_arm ();
   return 1;
}

int fpe_handler_uninstall () {
   if (!active) return 1;
   struct sigaction sa;
   feenableexcept (fpemask);
   if (sigaction (SIGFPE, &saved_sa, &sa)) return 0;
   active = 0;
   return 1;
}
