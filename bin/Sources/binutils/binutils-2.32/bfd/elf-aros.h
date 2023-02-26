/* AROS support for ELF
   Copyright (C) 2019 Free Software Foundation, Inc.

   This file is part of BFD, the Binary File Descriptor library.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

#include "elf/common.h"
#include "elf/internal.h"

bfd_boolean elf_aros_emit_relocs
  (bfd *output_bfd, asection *input_section, Elf_Internal_Shdr *input_rel_hdr,
   Elf_Internal_Rela *internal_relocs, struct elf_link_hash_entry **rel_hash);

bfd_boolean elf_aros_final_link
  (bfd *abfd, struct bfd_link_info *link_info);
