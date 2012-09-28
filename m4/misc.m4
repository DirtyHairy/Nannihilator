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

dnl ###########################################################################

AC_DEFUN([CS_SECTION],[
   echo
   AC_MSG_NOTICE(
[============================================================================])
   AC_MSG_NOTICE($1)
   AC_MSG_NOTICE(
[============================================================================])
   echo
])

dnl ###########################################################################

AC_DEFUN([CS_SUBCONFIG],[
   AC_MSG_NOTICE([

***
$1
***
   ])
])
