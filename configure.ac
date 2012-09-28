dnl #######################################################################
dnl 
dnl  This file is part of Nannihilator.
dnl 
dnl  Copyright (C) 2012- by 
dnl      Christian Speckner <cnspeckn@googlemail.com>
dnl 
dnl  Nannihilator is free software; you can redistribute it and/or modify it
dnl  under the terms of the GNU General Public License as published by 
dnl  the Free Software Foundation; either version 3, or (at your option)
dnl  any later version.
dnl 
dnl  Nannihilator is distributed in the hope that it will be useful, but
dnl  WITHOUT ANY WARRANTY; without even the implied warranty of
dnl  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
dnl  GNU General Public License for more details.
dnl 
dnl  You should have received a copy of the GNU General Public License
dnl  along with this program; if not, write to the Free Software
dnl  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
dnl 
dnl #######################################################################

AC_INIT([Nannihilator], [0.1])
AC_CONFIG_MACRO_DIR([./m4])
AM_INIT_AUTOMAKE()

dnl ***************************************************************************
dnl options
dnl ***************************************************************************

dnl Support trap control?
with_trap_control=yes
AC_ARG_ENABLE([trap-control],
   [AS_HELP_STRING([--disable-trap-control],[don't support trap control])], dnl'
   [test "$enableval" = no && with_trap_control=no]
)

dnl Build the trap handler?
with_trap_handler=yes
AC_ARG_ENABLE([trap-handler],[AS_HELP_STRING(
   [--disable-trap-handler],[don't build the tracing trap handler])], dnl'
   [test "$enableval" = no && with_trap_handler=no]
)

dnl Support fancy traces?
with_fancy_traces=yes
AC_ARG_ENABLE([fancy-traces],[AS_HELP_STRING(
   [--disable-fancy-traces],[use vanilla glibc traces])],
   [test "$enableval" = no && with_fancy_traces=no]
)

dnl Build FORTRAN interface?
with_fortran_interface=yes
AC_ARG_ENABLE([fortran-interface],[AS_HELP_STRING(
   [--disable-fortran-interface],[don't build the FORTRAN interface])], dnl'
   [test "$enableval" = no && with_fortran_interface=no]
)

dnl ***************************************************************************
dnl libtool
dnl ***************************************************************************

CS_SECTION([Libtool])
LT_INIT()

dnl ***************************************************************************
dnl C stuff
dnl ***************************************************************************

CS_SECTION([C stuff])

dnl C compiler
AC_PROG_CC()
AC_LANG(C)
dnl The nonstandard glibc fenv functions need libm
AC_CHECK_LIB([m],[sqrt])

dnl Check for C99 style fenv
AC_CHECK_HEADER([fenv.h], [], [
   AC_MSG_ERROR([This package requires fenv.h support (C99)])
])
dnl grab the FPE flag defines for FORTRAN
AC_MSG_NOTICE([Determining values of the FPE flags])
AC_RUN_IFELSE([AC_LANG_SOURCE([
   #include <stdio.h>
   #include <fenv.h>

   FILE* file;

   void declare (const char* var, const int val) {
      if (!fprintf (file, "%s=\"%i\"\n", var, val)) exit (1);
   }

   int main () {
      if (!(file = fopen ("./conftest.out", "w"))) return 1;
      declare ("FPE_DIVISION_BY_ZERO", FE_DIVBYZERO);
      declare ("FPE_INEXACT", FE_INEXACT);
      declare ("FPE_INVALID", FE_INVALID);
      declare ("FPE_OVERFLOW", FE_OVERFLOW);
      declare ("FPE_UNDERFLOW", FE_UNDERFLOW);
      declare ("FPE_ALL", FE_ALL_EXCEPT);
      fclose (file);
      return 0;
   }
])], [
   source ./conftest.out
], [
   AC_MSG_FAILURE([Test program failed!])
])
AC_SUBST([FPE_DIVISION_BY_ZERO])
AC_SUBST([FPE_INEXACT])
AC_SUBST([FPE_INVALID])
AC_SUBST([FPE_OVERFLOW])
AC_SUBST([FPE_UNDERFLOW])
AC_SUBST([FPE_ALL])

dnl Check for the glibc extensions for trap control
AC_MSG_CHECKING([for FPE trap control])
if test "$with_trap_control" = yes; then
   AC_LINK_IFELSE([AC_LANG_SOURCE([
      #define _GNU_SOURCE
      #include <fenv.h>

      int main () {
         fegetexcept ();
         feenableexcept (0);
         fedisableexcept (0);
      }
   ])], [
      AC_MSG_RESULT([yes])
   ], [
      with_trap_control=no
      AC_MSG_RESULT([no])
      AC_MSG_WARN(
   [No support for trap control --- trap handler will not be built.])
   ])
else
   AC_MSG_RESULT([skipped])
fi
test "$with_trap_control" = yes || with_trap_handler=no

dnl Check whether we can access the coprocessor / SSE state
AC_MSG_CHECKING([for access to the floating point state])
if test "$with_trap_handler" = yes; then
   AC_COMPILE_IFELSE([AC_LANG_SOURCE([
      #include <ucontext.h>

      int main () {
         ucontext_t uc;
         struct _fpstate* fps;
         getcontext (&uc);
         fps = (struct _fpstate*) uc.uc_mcontext.fpregs;
         int mxcsr = fps->mxcsr, swd = fps->swd, cwd = fps->cwd;
      }
   ])], [
      AC_MSG_RESULT([yes])
   ], [
      with_trap_handler="no"
      AC_MSG_RESULT([failed])
      AC_MSG_WARN(
   [the trap handler is not supported on your system, sorry.])
   ])
else
   AC_MSG_RESULT([skipped])
fi
test "$with_trap_handler" = yes || with_fancy_traces=no

dnl Check for libbfd (for fancy traces)
if test "$with_fancy_traces" = yes; then
   AC_CHECK_HEADER([bfd.h], [], [with_fancy_traces=no])
   AC_CHECK_LIB([bfd], [bfd_init], [], [with_fancy_traces=no])
   if test "$with_fancy_traces" = no; then
      AC_MSG_WARN(
[Fancy traces rely on libbfd --- falling back to vanilla glibc traces])
   fi
fi

dnl ***************************************************************************
dnl FORTRAN
dnl ***************************************************************************

dnl Interface requested?
if test "$with_fortran_interface" = yes; then
   CS_SECTION([FORTRAN stuff])
   fc_valid=1
fi

dnl Check for executability if we were supplied a compiler via FC
if test -n "$FC" && -n "$fc_valid"; then
   fc_valid=""
   test -x "$FC" && fc_valid="1"
   if test -z "$fc_valid"; then
      AC_CHECK_PROG([fc_valid],[$FC],[found],[])
   fi
fi

dnl If not, try to determine the compiler
if test -n "$fc_valid"; then
   AC_PROG_FC()
   fc_valid="$FC"
fi

dnl Check for iso_c_binding
if test -n "$fc_valid"; then
   AC_LANG([Fortran])
   AC_FC_SRCEXT([f90])
   WO_FC_CHECK_C_BINDING()
   test "$FC_SUPPORTS_C_BINDING" = yes || fc_valid=""
fi

dnl Check for module file extension
if test -n "$fc_valid"; then
   WO_FC_MODULE_FILE([MODFILE], [MODEXT], [$FC], [f90])
   fc_valid="$MODEXT"
fi

if test -z "$fc_valid" && test "$with_fortran_interface" = yes; then
   AC_MSG_WARN(
[No suitable FORTRAN compiler found, FORTRAN interface disabled])
fi

dnl ***************************************************************************
dnl Output
dnl ***************************************************************************

dnl Create the FORTRAN versions of the exception flags
if test "$with_fortran_interface" = yes; then
   AC_CONFIG_FILES([src/nannihilator.f90])
fi

dnl Propagate the configuration to the header. 
FPE_WITH_TRAP_CONTROL=undef
FPE_WITH_TRAP_HANDLER=undef
FPE_WITH_FANCY_TRACES=undef
test "$with_trap_control" = yes && FPE_WITH_TRAP_CONTROL=define
test "$with_trap_handler" = yes && FPE_WITH_TRAP_HANDLER=define
test "$with_fancy_traces" = yes && FPE_WITH_FANCY_TRACES=define
AC_SUBST([FPE_WITH_TRAP_CONTROL])
AC_SUBST([FPE_WITH_TRAP_HANDLER])
AC_SUBST([FPE_WITH_FANCY_TRACES])
AC_CONFIG_FILES([src/include/nannihilator.h])

dnl Automake conditionals
AM_CONDITIONAL([WITH_TRAP_CONTROL], [test "$with_trap_control" = yes])
AM_CONDITIONAL([WITH_TRAP_HANDLER], [test "$with_trap_handler" = yes])
AM_CONDITIONAL([WITH_FANCY_TRACES], [test "$with_fancy_traces" = yes])
AM_CONDITIONAL([WITH_FORTRAN_INTERFACE], [test "$with_fortran_interface" = yes])

AC_CONFIG_FILES([Makefile src/Makefile src/include/Makefile])

CS_SECTION([Creating output files])

AC_OUTPUT()

dnl ***************************************************************************
dnl Summary
dnl ***************************************************************************

CS_SECTION([Summary])

cat <<EOI
   Support trap control  : $with_trap_control
   Build tracing handler : $with_trap_handler
   Fancy traces          : $with_fancy_traces
   FORTRAN interface     : $with_fortran_interface
EOI
