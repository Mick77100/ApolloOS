/* AROS support for ELF
   Copyright (C) 2005-2019 Free Software Foundation, Inc.

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

/* This file provides routines used by all AROS targets.  */

#include "sysdep.h"
#include "bfd.h"
#include "libbfd.h"
#include "elf-bfd.h"
#include "elf-aros.h"

/* Copy relocations into the output file. Changes all relocations
   from symbol-relative to section-relative. In next step strip can
   then remove the symbols initially involved with relocations */

bfd_boolean
elf_aros_emit_relocs (bfd *output_bfd,
		      asection *input_section,
		      Elf_Internal_Shdr *input_rel_hdr,
		      Elf_Internal_Rela *internal_relocs,
		      struct elf_link_hash_entry **rel_hash)
{
  const struct elf_backend_data *bed;
  int j;

  bed = get_elf_backend_data (output_bfd);

  if (output_bfd->flags & (DYNAMIC|EXEC_P))
    {
      Elf_Internal_Rela *irela;
      Elf_Internal_Rela *irelaend;
      struct elf_link_hash_entry **hash_ptr;

      for (irela = internal_relocs,
	     irelaend = irela + (NUM_SHDR_ENTRIES (input_rel_hdr)
				 * bed->s->int_rels_per_ext_rel),
	     hash_ptr = rel_hash;
	   irela < irelaend;
	   irela += bed->s->int_rels_per_ext_rel,
	     hash_ptr++)
	{
	  if (*hash_ptr
	      && ((*hash_ptr)->root.type == bfd_link_hash_defined
		  || (*hash_ptr)->root.type == bfd_link_hash_defweak
		  || (*hash_ptr)->root.type == bfd_link_hash_common)
	      && (*hash_ptr)->root.u.def.section->output_section != NULL)
	    {
	      /* Remove relocation dependency on symbol. Use sections only */
	      for (j = 0; j < bed->s->int_rels_per_ext_rel; j++)
		{
		  asection *sec = (*hash_ptr)->root.u.def.section;
		  int this_idx = sec->output_section->target_index;

		  irela[j].r_info
		    = ELF32_R_INFO (this_idx, ELF32_R_TYPE (irela[j].r_info));
		  irela[j].r_addend += (*hash_ptr)->root.u.def.value;
		  irela[j].r_addend += sec->output_offset;
		}
	      /* Stop the generic routine adjusting this entry.  */
	      *hash_ptr = NULL;
	    }
	}
    }
  return _bfd_elf_link_output_relocs (output_bfd, input_section,
				      input_rel_hdr, internal_relocs,
				      rel_hash);
}

bfd_boolean
elf_aros_final_link(bfd *abfd, struct bfd_link_info *link_info)
{
  link_info->emitrelocations = 1;
  link_info->output_bfd->flags &= ~D_PAGED;
  return bfd_elf_final_link(abfd, link_info);
}
