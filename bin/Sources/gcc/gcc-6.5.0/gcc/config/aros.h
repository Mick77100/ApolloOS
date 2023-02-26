/* Definitions for AROS
   Copyright (C) 1995, 1996, 1997, 1998, 1999, 2000 Free Software Foundation, Inc.
   Contributed by Fabio Alemagna
   Based upon linux.h, contributed by Eric Youngdale.

This file is part of GNU CC.

GNU CC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GNU CC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GNU CC; see the file COPYING.  If not, write to
the Free Software Foundation, 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.  */

/* Don't assume anything about the header files.  */
#define NO_IMPLICIT_EXTERN_C

#undef SIZE_TYPE
#define SIZE_TYPE "long unsigned int"

#undef WCHAR_TYPE
#define WCHAR_TYPE "char"

#undef WCHAR_TYPE_SIZE
#define WCHAR_TYPE_SIZE 8

#undef PTRDIFF_TYPE
#define PTRDIFF_TYPE "long int"

#undef TARGET_OS_CPP_BUILTINS
#define TARGET_OS_CPP_BUILTINS()		\
  do						\
    {						\
	builtin_define_std ("AROS");            \
       builtin_define ("AMIGA");               \
       builtin_define ("_AMIGA");              \
	builtin_define ("__ELF__");		\
	builtin_assert ("system=posix");        \
    }						\
  while (0)

#undef ASM_APP_ON
#define ASM_APP_ON "#APP\n"

#undef ASM_APP_OFF
#define ASM_APP_OFF "#NO_APP\n"

#undef MD_EXEC_PREFIX
#undef MD_STARTFILE_PREFIX

/* Provide a STARTFILE_SPEC appropriate for AROS. AROS has its own
   set of startup files which also take care of handling C++
   constructors/destructors.  */

#undef	STARTFILE_SPEC
#define STARTFILE_SPEC \
  "startup%O%s %{detach:detach%O%s} %{nix:nixmain%O%s} %{static-stdc++:static-cxx-ops%O%s static-cxx-personality%O%s static-cxx-cxa-pure-virtual%O%s}"

/* Provide a ENDFILE_SPEC appropriate for AROS. */

#undef	ENDFILE_SPEC
#define ENDFILE_SPEC \
  "-lautoinit"

/* The compiler is configured with ONLY the gcc/g++ standard headers.
   N.B-: NATIVE_SYSTEM_HEADER_DIR must come after GCC_INCLUDE_DIR with gcc6!! */
#undef INCLUDE_DEFAULTS
#define INCLUDE_DEFAULTS \
{ \
    { GPLUSPLUS_INCLUDE_DIR, "G++", 1, 1, GPLUSPLUS_INCLUDE_DIR_ADD_SYSROOT, 0 }, \
    { GPLUSPLUS_TOOL_INCLUDE_DIR, "G++", 1, 1, GPLUSPLUS_INCLUDE_DIR_ADD_SYSROOT, 1 }, \
    { GPLUSPLUS_BACKWARD_INCLUDE_DIR, "G++", 1, 1, GPLUSPLUS_INCLUDE_DIR_ADD_SYSROOT, 0 }, \
    { GCC_INCLUDE_DIR, "GCC", 0, 0, 0, 0 }, \
    { NATIVE_SYSTEM_HEADER_DIR, NATIVE_SYSTEM_HEADER_COMPONENT, 0, 0, 1, 0 }, \
    { 0, 0, 0, 0, 0, 0 } \
}

#undef CPP_SPEC
#define CPP_SPEC "%(cpp_cpu) %{fPIC:-D__PIC__ -D__pic__} %{fpic:-D__PIC__ -D__pic__} %{posix:-D_POSIX_SOURCE} %{pthread:-D_REENTRANT} %{nix:-D__NIX__}  %{!nostdinc:%{!nostdc:%{!noposixc|noposixinc:-idirafter %R/include/aros/posixc} -idirafter %R/include/aros/stdc}}"

/* This is for -profile to use -lc_p instead of -lc.  */
#ifndef CC1_SPEC
#define CC1_SPEC "%{profile:-p}"
#endif

/* The GNU C++ standard library requires that these macros be defined.  */
#undef CPLUSPLUS_CPP_SPEC
#define CPLUSPLUS_CPP_SPEC "-D_GNU_SOURCE %(cpp)"

#undef	LIB_SPEC
#define LIB_SPEC \
  "--start-group -lmui -larossupport -lamiga %{!nostartfiles:%{!static:%{!nostdc:%{!noposixc:%{!deferposixc:-lposixc}} -lstdcio %{!noposixc:%{deferposixc:-lposixc}} -lstdc}}%{static:%{!nostdc:-lstdc.static}}} -lcodesets -lkeymap -lexpansion -lcommodities -ldiskfont -lasl -lmuimaster -ldatatypes -lcybergraphics -lworkbench -licon -lintuition -lgadtools -llayers -laros -lpartition -liffparse -lgraphics -llocale -ldos -lutility -loop %{!nosysbase:-lexec} -lautoinit -llibinit %{nostartfiles:%{static:-lstdc.static}} --end-group"

#undef	LIBSTDCXX_STATIC
#define LIBSTDCXX_STATIC "pthread"

#if !defined(USE_GNULIBC_1) && defined(HAVE_LD_EH_FRAME_HDR)
#define LINK_EH_SPEC "%{!static:--eh-frame-hdr} "
#endif

/* AROS uses its own collect-like program for the moment.  */
#undef LINKER_NAME
#define LINKER_NAME "collect-aros"

#define TARGET_HAS_F_SETLKW

#define TARGET_POSIX_IO
