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
      TARGET_BPABI_CPP_BUILTINS();		\
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

/* We default to a softfp ABI so that we are compatible
   with the Linux EABI (as used by the linker).  */
#undef  TARGET_DEFAULT_FLOAT_ABI
#define TARGET_DEFAULT_FLOAT_ABI ARM_FLOAT_ABI_SOFTFP

/* We default to the "aapcs-linux" ABI so that enums are int-sized by
   default.  */
#undef  ARM_DEFAULT_ABI
#define ARM_DEFAULT_ABI ARM_ABI_AAPCS_LINUX

/* Default to armv5t so that thumb shared libraries work.
   The ARM10TDMI core is the default for armv5t, so set
   SUBTARGET_CPU_DEFAULT to achieve this.  */
#undef  SUBTARGET_CPU_DEFAULT
#define SUBTARGET_CPU_DEFAULT TARGET_CPU_arm10tdmi

/* TARGET_BIG_ENDIAN_DEFAULT is set in
   config.gcc for big endian configurations.  */
#undef  TARGET_LINKER_EMULATION
#if TARGET_BIG_ENDIAN_DEFAULT
#define TARGET_LINKER_EMULATION "armelfb_aros"
#else
#define TARGET_LINKER_EMULATION "armelf_aros"
#endif

#undef SUBTARGET_CPP_SPEC
#define SUBTARGET_CPP_SPEC "-D__ELF__ %{!nostdinc:%{!nostdc:%{!noposixc|noposixinc: -idirafter %R/include/aros/posixc} -idirafter %R/include/aros/stdc}}"

#undef  SUBTARGET_EXTRA_LINK_SPEC
#define SUBTARGET_EXTRA_LINK_SPEC " -m " TARGET_LINKER_EMULATION

/* Provide a LINK_SPEC appropriate for AROS.  */
#undef  LINK_SPEC
#define LINK_SPEC SUBTARGET_EXTRA_LINK_SPEC " -L %R/lib"

/* Use own supplement to libgcc.  */
#undef LIBGCC_SPEC
#define LIBGCC_SPEC "-laeabi -lgcc"

#undef	STARTFILE_SPEC
#define STARTFILE_SPEC \
  "startup%O%s %{detach:detach%O%s} %{nix:nixmain%O%s} %{static-stdc++:static-cxx-ops%O%s static-cxx-personality%O%s static-cxx-cxa-pure-virtual%O%s} arm-exidx-unwind%O%s "

/* FIXME: AROS doesn't support dw2 unwinding yet.  */
#undef MD_FALLBACK_FRAME_STATE_FOR
