dnl #######################################################################
dnl 
dnl  This file is part of Nannihilator and was derived from the extended
dnl  FORTRAN compiler checks in the WHIZARD package. Additional credit
dnl  goes to the WHIZARD authors.
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

dnl fortran.m4 -- Fortran compiler checks beyond Autoconf built-ins
dnl

dnl The standard Fortran compiler test is AC_PROG_FC.
dnl At the end FC, FCFLAGS and FCFLAGS_f90 are set, if successful.

### Check for the C bindings extensions of Fortran 2003
AC_DEFUN([WO_FC_CHECK_C_BINDING],
[AC_CACHE_CHECK([whether $FC supports ISO C binding and standard numeric types],
[wo_cv_fc_c_binding],
[dnl
AC_REQUIRE([AC_PROG_FC])
AC_LANG([Fortran])
AC_COMPILE_IFELSE([dnl
  program conftest
    use iso_c_binding
    type, bind(c) :: t
       integer(c_int) :: i
       real(c_float) :: x1
       real(c_double) :: x2
       complex(c_float_complex) :: z1
       complex(c_double_complex) :: z2
    end type t
  end program conftest
  ],
  [wo_cv_fc_c_binding="yes"],
  [wo_cv_fc_c_binding="no"])
])
FC_SUPPORTS_C_BINDING="$wo_cv_fc_c_binding"
AC_SUBST([FC_SUPPORTS_C_BINDING])
])
### end WO_FC_CHECK_C_BINDING

dnl
dnl --------------------------------------------------------------------
dnl
dnl COMPILE_FC(VARIABLE, COMPILER, EXTENSION, MODULE,
dnl                       VALUE_SUCCESS, VALUE_FAILURE, KEEP)
dnl
AC_DEFUN([COMPILE_FC],
[cat >conftest.$3 <<__END__
$4
program conftest
  print *, 42
end program conftest
__END__
$2 -o conftest conftest.$3 >/dev/null 2>&1
./conftest >conftest.out 2>/dev/null
if test 42 = "`$SED 's/ //g' conftest.out`"; then
  $1="$5"
else
  $1="$6"
fi
if test -z "$7"; then
  rm -rf conftest* CONFTEST*
fi])

dnl
dnl --------------------------------------------------------------------
dnl
dnl WO_FC_MODULE_FILE(NAME, EXTENSION, FORTRAN_COMPILER, SOURCE_EXTENSION)
dnl
AC_DEFUN([WO_FC_MODULE_FILE],
[AC_SUBST([$1])
AC_SUBST([$2])
AC_MSG_CHECKING([for Fortran90 module file naming convention])
COMPILE_FC([wo_result], [$3], [$4],
  [module module_NAME
     implicit none
     integer, parameter, public :: forty_two = 42
   end module module_NAME], [ok], [], [KEEP])
$1=""
$2=""
if test -n "[$]wo_result"; then
  for name in module_name module_NAME conftest; do
    for ext in m mod M MOD d D; do
      if test -f "[$]name.[$]ext"; then
        $1="$name"
        $2="$ext"
        break 2
      fi
    done
  done
  if test -n "[$]$2"; then
     AC_MSG_RESULT([[$]$1.[$]$2])
  else
     AC_MSG_RESULT([failed])
     AC_MSG_WARN([unusual unsupported module file name convention])
  fi
else
  AC_MSG_RESULT([failed])
  AC_MSG_WARN([FORTRAN compiler cannot create module files])
fi
rm -f conftest* CONFTEST* module_name* module_NAME* MODULE_NAME*])
