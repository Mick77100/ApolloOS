/* Definitions for AMD x86_64 running AROS systems with ELF64 format.
   Copyright (C) 1994, 1995, 1996, 1997, 1998, 1999, 2001, 2002
   Free Software Foundation, Inc.
   Contributed by Stanislaw Szymczyk <sszymczy@gmail.com>. Based on aros.h.

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

/* The svr4 ABI for the i386 says that records and unions are returned
   in memory.  In the 64bit compilation we will turn this flag off in
   override_options, as we never do pcc_struct_return scheme on this target.  */
#undef DEFAULT_PCC_STRUCT_RETURN
#define DEFAULT_PCC_STRUCT_RETURN 1

/* Provide a LINK_SPEC appropriate for AROS.  */

#undef	LINK_SPEC
#define LINK_SPEC "%{!m32:-m elf_x86_64} %{m32:-m elf_i386} -L %R/lib" 

/* FIXME: AROS doesn't support dw2 unwinding yet.  */
