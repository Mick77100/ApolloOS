/* Configuration file for ARM AROS EABI targets.
   Copyright (C) 2004, 2005, 2006, 2007, 2010
   Free Software Foundation, Inc.
   Contributed by Pavel Fedin
   Based on linux-eabi.h and other AROS targets

   This file is part of GCC.

   GCC is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published
   by the Free Software Foundation; either version 3, or (at your
   option) any later version.

   GCC is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
   License for more details.

   You should have received a copy of the GNU General Public License
   along with GCC; see the file COPYING3.  If not see
   <http://www.gnu.org/licenses/>.  */

/* On EABI GNU/Linux, we want both the BPABI builtins and the
   GNU/Linux builtins.  */
#undef  TARGET_OS_CPP_BUILTINS
#define TARGET_OS_CPP_BUILTINS() 		\
  do 						\
    {						\
	builtin_define_std ("AROS");		\
       builtin_define ("AMIGA");               \
       builtin_define ("_AMIGA");              \
	builtin_assert ("system=posix");	\
	if (flag_pic)				\
	  {					\
	    builtin_define ("__PIC__");		\
	    builtin_define ("__pic__");		\
	  }					\
    }						\
  while (false)

/* TARGET_BIG_ENDIAN_DEFAULT is set in
   config.gcc for big endian configurations.  */
#undef  TARGET_LINKER_EMULATION
#if TARGET_BIG_ENDIAN_DEFAULT
#define TARGET_LINKER_EMULATION "aarch64elfb_aros"
#else
#define TARGET_LINKER_EMULATION "aarch64elf_aros"
#endif

#undef SUBTARGET_CPP_SPEC
#define SUBTARGET_CPP_SPEC "-D__ELF__ %{!nostdinc:%{!nostdc:%{!noposixc|noposixinc: -idirafter %R/include/aros/posixc} -idirafter %R/include/aros/stdc}}"

#undef  SUBTARGET_EXTRA_LINK_SPEC
#define SUBTARGET_EXTRA_LINK_SPEC " -m " TARGET_LINKER_EMULATION

/* Provide a LINK_SPEC appropriate for AROS.  */
#undef  LINK_SPEC
#if TARGET_BIG_ENDIAN_DEFAULT
#define LINK_SPEC "-m aarch64elfb_aros -L %R/lib"
#else
#define LINK_SPEC "-m aarch64elf_aros -L %R/lib"
#endif

/* Use own supplement to libgcc.  */
#undef LIBGCC_SPEC
#define LIBGCC_SPEC "-lgcc"

