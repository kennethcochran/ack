/*
 * (c) copyright 1987 by the Vrije Universiteit, Amsterdam, The Netherlands.
 * See the copyright notice in the ACK home directory, in the file "Copyright".
 *
 * Author: Ceriel J.H. Jacobs
 */

/* D E S I G N A T O R   E V A L U A T I O N */

/* $Header$ */

/*	Code generation for designators.
   	This file contains some routines that generate code common to address
	as well as value computations, and leave a description in a "desig"
	structure.  It also contains routines to load an address, load a value
	or perform a store.
*/

#include	"debug.h"

#include	<em_arith.h>
#include	<em_label.h>
#include	<em_code.h>
#include	<assert.h>
#include	<alloc.h>

#include	"squeeze.h"
#include	"type.h"
#include	"LLlex.h"
#include	"def.h"
#include	"scope.h"
#include	"desig.h"
#include	"node.h"
#include	"warning.h"
#include	"walk.h"

extern int	proclevel;

int
WordOrDouble(ds, size)
	register t_desig *ds;
	arith size;
{
	if ((int) (ds->dsg_offset) % (int) word_size == 0) {
		if (size == word_size) return 1;
		if (size == dword_size) return 2;
	}
	return 0;
}

int
DoLoad(ds, size)
	register t_desig *ds;
	arith size;
{
	switch (WordOrDouble(ds, size)) {
	default:
		return 0;
	case 1:
		if (ds->dsg_name) {
			C_loe_dnam(ds->dsg_name, ds->dsg_offset);
		}
		else	C_lol(ds->dsg_offset);
		break;
	case 2:
		if (ds->dsg_name) {
			C_lde_dnam(ds->dsg_name, ds->dsg_offset);
		}
		else	C_ldl(ds->dsg_offset);
		break;
	}
	return 1;
}

int
DoStore(ds, size)
	register t_desig *ds;
	arith size;
{
	switch (WordOrDouble(ds, size)) {
	default:
		return 0;
	case 1:
		if (ds->dsg_name) {
			C_ste_dnam(ds->dsg_name, ds->dsg_offset);
		}
		else	C_stl(ds->dsg_offset);
		break;
	case 2:
		if (ds->dsg_name) {
			C_sde_dnam(ds->dsg_name, ds->dsg_offset);
		}
		else	C_sdl(ds->dsg_offset);
		break;
	}
	return 1;
}

STATIC int
properly(ds, tp)
	register t_desig *ds;
	register t_type *tp;
{
	/*	Check if it is allowed to load or store the value indicated
		by "ds" with LOI/STI.
		- if the size is not either a multiple or a dividor of the
		  wordsize, then not.
		- if the alignment is at least "word" then OK.
		- if size is dividor of word_size and alignment >= size then OK.
		- otherwise check alignment of address. This can only be done
		  with DSG_FIXED.
	*/

	int szmodword = (int) (tp->tp_size) % (int) word_size;
						/* 0 if multiple of wordsize */
	int wordmodsz = word_size % tp->tp_size;/* 0 if dividor of wordsize */

	if (szmodword && wordmodsz) return 0;
	if (tp->tp_align >= word_align) return 1;
	if (szmodword && tp->tp_align >= szmodword) return 1;

	return ds->dsg_kind == DSG_FIXED &&
	       ((! szmodword && (int) (ds->dsg_offset) % word_align == 0) ||
		(! wordmodsz && ds->dsg_offset % tp->tp_size == 0));
}

CodeValue(ds, tp)
	register t_desig *ds;
	register t_type *tp;
{
	/*	Generate code to load the value of the designator described
		in "ds"
	*/
	arith sz;

	switch(ds->dsg_kind) {
	case DSG_LOADED:
		break;

	case DSG_FIXED:
		if (DoLoad(ds, tp->tp_size)) break;
		/* Fall through */
	case DSG_PLOADED:
	case DSG_PFIXED:
		sz = WA(tp->tp_size);
		if (properly(ds, tp)) {
			CodeAddress(ds);
			C_loi(tp->tp_size);
			break;
		}
		if (ds->dsg_kind == DSG_PLOADED) {
			sz -= pointer_size;

			C_asp(-sz);
			C_lor((arith) 1);
			C_adp(sz);
			C_loi(pointer_size);
		}
		else  {
			C_asp(-sz);
		}
		CodeAddress(ds);
		C_loc(tp->tp_size);
		C_cal("_load");
		C_asp(2 * word_size);
		break;

	case DSG_INDEXED:
		C_lar(word_size);
		break;

	default:
		crash("(CodeValue)");
	}

	ds->dsg_kind = DSG_LOADED;
}

ChkForFOR(nd)
	t_node *nd;
{
	if (nd->nd_class == Def) {
		register t_def *df = nd->nd_def;

		if (df->df_flags & D_FORLOOP) {
			node_warning(nd,
				     W_ORDINARY,
				     "assignment to FOR-loop control variable");
			df->df_flags &= ~D_FORLOOP;
		}
	}
}

CodeStore(ds, tp)
	register t_desig *ds;
	register t_type *tp;
{
	/*	Generate code to store the value on the stack in the designator
		described in "ds"
	*/
	t_desig save;

	save = *ds;

	switch(ds->dsg_kind) {
	case DSG_FIXED:
		if (DoStore(ds, tp->tp_size)) break;
		/* Fall through */
	case DSG_PLOADED:
	case DSG_PFIXED:
		CodeAddress(&save);
		if (properly(ds, tp)) {
			C_sti(tp->tp_size);
			break;
		}
		C_loc(tp->tp_size);
		C_cal("_store");
		C_asp(2 * word_size + WA(tp->tp_size));
		break;

	case DSG_INDEXED:
		C_sar(word_size);
		break;

	default:
		crash("(CodeStore)");
	}

	ds->dsg_kind = DSG_INIT;
}

CodeCopy(lhs, rhs, sz, psize)
	register t_desig *lhs, *rhs;
	arith sz, *psize;
{
	t_desig l, r;

	l = *lhs; r = *rhs;
	*psize -= sz;
	lhs->dsg_offset += sz;
	rhs->dsg_offset += sz;
	CodeAddress(&r);
	C_loi(sz);
	CodeAddress(&l);
	C_sti(sz);
}

CodeMove(rhs, left, rtp)
	register t_desig *rhs;
	register t_node *left;
	t_type *rtp;
{
	register t_desig *lhs = new_desig();
	register t_type *tp = left->nd_type;
	int	loadedflag = 0;

	/*	Generate code for an assignment. Testing of type
		compatibility and the like is already done.
		Go through some (considerable) trouble to see if a BLM can be
		generated.
	*/

	ChkForFOR(left);
	switch(rhs->dsg_kind) {
	case DSG_LOADED:
		CodeDesig(left, lhs);
		if (rtp->tp_fund == T_STRING) {
			CodeAddress(lhs);
			C_loc(rtp->tp_size);
			C_loc(tp->tp_size);
			C_cal("_StringAssign");
			C_asp(word_size << 2);
			break;
		}
		CodeStore(lhs, tp);
		break;
	case DSG_PLOADED:
	case DSG_PFIXED:
		CodeAddress(rhs);
		if ((int) (tp->tp_size) % (int) word_size == 0 &&
		    tp->tp_align >= (int) word_size) {
			CodeDesig(left, lhs);
			CodeAddress(lhs);
			C_blm(tp->tp_size);
			break;
		}
		CodeValue(rhs, tp);
		CodeDStore(left);
		break;
	case DSG_FIXED:
		CodeDesig(left, lhs);
		if (lhs->dsg_kind == DSG_FIXED &&
		    (int) (lhs->dsg_offset) % (int) word_size ==
		    (int) (rhs->dsg_offset) % (int) word_size) {
			register int sz;
			arith size = tp->tp_size;

			while (size &&
			       (sz = ((int)(lhs->dsg_offset) % (int)word_size))) {
				/*	First copy up to word-aligned
					boundaries
				*/
				if (sz < 0) sz = -sz;	/* bloody '%' */
				while ((int) word_size % sz) sz--;
				CodeCopy(lhs, rhs, (arith) sz, &size);
			}
			if (size > 3*dword_size) {
				/*	Do a block move
				*/
				t_desig l, r;
				arith sz;

				sz = (size / word_size) * word_size;
				l = *lhs; r = *rhs;
				CodeAddress(&r);
				CodeAddress(&l);
				C_blm((arith) sz);
				rhs->dsg_offset += sz;
				lhs->dsg_offset += sz;
				size -= sz;
			}
			else for (sz = (int) dword_size; sz; sz -= (int) word_size) {
				while (size >= sz) {
					/*	Then copy dwords, words.
						Depend on peephole optimizer
					*/
					CodeCopy(lhs, rhs, (arith) sz, &size);
				}
			}
			sz = word_size;
			while (size && --sz) {
				/*	And then copy remaining parts
				*/
				while ((int) word_size % sz) sz--;
				while (size >= sz) {
					CodeCopy(lhs, rhs, (arith) sz, &size);
				}
			}
			break;
		}
		if (lhs->dsg_kind == DSG_PLOADED ||
		    lhs->dsg_kind == DSG_INDEXED) {
			CodeAddress(lhs);
			loadedflag = 1;
		}
		if ((int)(tp->tp_size) % (int) word_size == 0 &&
		    tp->tp_align >= word_size) {
			CodeAddress(rhs);
			if (loadedflag) C_exg(pointer_size);
			else CodeAddress(lhs);
			C_blm(tp->tp_size);
			break;
		}
		{
			arith tmp;
			extern arith NewPtr();

			if (loadedflag) {	
				tmp = NewPtr();
				lhs->dsg_offset = tmp;
				lhs->dsg_name = 0;
				lhs->dsg_kind = DSG_PFIXED;
				lhs->dsg_def = 0;
				C_stl(tmp);		/* address of lhs */
			}
			CodeValue(rhs, tp);
			CodeStore(lhs, tp);
			if (loadedflag) FreePtr(tmp);
			break;
		}
	default:
		crash("CodeMove");
	}
	free_desig(lhs);
}

CodeAddress(ds)
	register t_desig *ds;
{
	/*	Generate code to load the address of the designator described
	   	in "ds"
	*/

	switch(ds->dsg_kind) {
	case DSG_PLOADED:
		if (ds->dsg_offset) {
			C_adp(ds->dsg_offset);
		}
		break;

	case DSG_FIXED:
		if (ds->dsg_name) {
			C_lae_dnam(ds->dsg_name, ds->dsg_offset);
			break;
		}
		C_lal(ds->dsg_offset);
		if (ds->dsg_def) ds->dsg_def->df_flags |= D_NOREG;
		break;
		
	case DSG_PFIXED:
		DoLoad(ds, word_size);
		break;

	case DSG_INDEXED:
		C_aar(word_size);
		break;

	default:
		crash("(CodeAddress)");
	}

	ds->dsg_offset = 0;
	ds->dsg_kind = DSG_PLOADED;
}

CodeFieldDesig(df, ds)
	register t_def *df;
	register t_desig *ds;
{
	/* Generate code for a field designator. Only the code common for
	   address as well as value computation is generated, and the
	   resulting information on where to find the designator is placed
	   in "ds". "df" indicates the definition of the field.
	*/

	if (ds->dsg_kind == DSG_INIT) {
		/* In a WITH statement. We must find the designator in the
		   WITH statement, and act as if the field is a selection
		   of this designator.
		   So, first find the right WITH statement, which is the
		   first one of the proper record type, which is
		   recognized by its scope indication.
		*/
		register struct withdesig *wds = WithDesigs;

		assert(wds != 0);

		while (wds->w_scope != df->df_scope) {
			wds = wds->w_next;
			assert(wds != 0);
		}

		/* Found it. Now, act like it was a selection.
		*/
		*ds = wds->w_desig;
		assert(ds->dsg_kind == DSG_PFIXED);
	}

	switch(ds->dsg_kind) {
	case DSG_PLOADED:
	case DSG_FIXED:
		ds->dsg_offset += df->fld_off;
		break;

	case DSG_PFIXED:
	case DSG_INDEXED:
		CodeAddress(ds);
		ds->dsg_kind = DSG_PLOADED;
		ds->dsg_offset = df->fld_off;
		break;

	default:
		crash("(CodeFieldDesig)");
	}
}

CodeVarDesig(df, ds)
	register t_def *df;
	register t_desig *ds;
{
	/*	Generate code for a variable represented by a "def" structure.
		Of course, there are numerous cases: the variable is local,
		it is a value parameter, it is a var parameter, it is one of
		those of an enclosing procedure, or it is global.
	*/
	register t_scope *sc = df->df_scope;

	/* Selections from a module are handled earlier, when identifying
	   the variable, so ...
	*/
	assert(ds->dsg_kind == DSG_INIT);

	if (df->df_flags & D_ADDRGIVEN) {
		/* the programmer specified an address in the declaration of
		   the variable. Generate code to push the address.
		*/
		CodeConst(df->var_off, (int) pointer_size);
		ds->dsg_kind = DSG_PLOADED;
		ds->dsg_offset = 0;
		return;
	}

	if (df->var_name) {
		/* this variable has been given a name, so it is global.
		   It is directly accessible.
		*/
		ds->dsg_name = df->var_name;
		ds->dsg_offset = 0;
		ds->dsg_kind = DSG_FIXED;
		return;
	}

	if (sc->sc_level != proclevel) {
		/* the variable is local to a statically enclosing procedure.
		*/
		assert(proclevel > sc->sc_level);

		df->df_flags |= D_NOREG;
		if (df->df_flags & (D_VARPAR|D_VALPAR)) {
			/* value or var parameter
			*/
			C_lxa((arith) (proclevel - sc->sc_level));
			if ((df->df_flags & D_VARPAR) ||
			    IsConformantArray(df->df_type)) {
				/* var parameter or conformant array.
				   For conformant array's, the address is
				   passed.
				*/
				C_adp(df->var_off);
				C_loi(pointer_size);
				ds->dsg_offset = 0;
				ds->dsg_kind = DSG_PLOADED;
				return;
			}
		}
		else	C_lxl((arith) (proclevel - sc->sc_level));
		ds->dsg_kind = DSG_PLOADED;
		ds->dsg_offset = df->var_off;
		return;
	}

	/* Now, finally, we have a local variable or a local parameter
	*/
	if ((df->df_flags & D_VARPAR) || IsConformantArray(df->df_type)) {
		/* a var parameter; address directly accessible.
		*/
		ds->dsg_kind = DSG_PFIXED;
	}
	else	ds->dsg_kind = DSG_FIXED;
	ds->dsg_offset = df->var_off;
	ds->dsg_def = df;
}

CodeDesig(nd, ds)
	register t_node *nd;
	register t_desig *ds;
{
	/*	Generate code for a designator. Use divide and conquer
		principle
	*/
	register t_def *df;

	switch(nd->nd_class) {	/* Divide */
	case Def:
		df = nd->nd_def;
		if (nd->nd_left) CodeDesig(nd->nd_left, ds);

		switch(df->df_kind) {
		case D_FIELD:
			CodeFieldDesig(df, ds);
			break;

		case D_VARIABLE:
			CodeVarDesig(df, ds);
			break;

		default:
			crash("(CodeDesig) Def");
		}
		break;

	case Arrsel:
		assert(nd->nd_symb == '[');

		CodeDesig(nd->nd_left, ds);
		CodeAddress(ds);
		CodePExpr(nd->nd_right);

		/* Now load address of descriptor
		*/
		if (IsConformantArray(nd->nd_left->nd_type)) {
			assert(nd->nd_left->nd_class == Def);

			df = nd->nd_left->nd_def;
			if (proclevel > df->df_scope->sc_level) {
				C_lxa((arith) (proclevel - df->df_scope->sc_level));
				C_adp(df->var_off + pointer_size);
			}
			else	C_lal(df->var_off + pointer_size);
		}
		else	{
			c_lae_dlb(nd->nd_left->nd_type->arr_descr);
		}
		ds->dsg_kind = DSG_INDEXED;
		break;

	case Arrow:
		assert(nd->nd_symb == '^');

		CodeDesig(nd->nd_right, ds);
		switch(ds->dsg_kind) {
		case DSG_LOADED:
			ds->dsg_kind = DSG_PLOADED;
			break;

		case DSG_INDEXED:
		case DSG_PLOADED:
		case DSG_PFIXED:
			CodeValue(ds, nd->nd_right->nd_type);
			ds->dsg_kind = DSG_PLOADED;
			ds->dsg_offset = 0;
			break;

		case DSG_FIXED:
			ds->dsg_kind = DSG_PFIXED;
			break;

		default:
			crash("(CodeDesig) Uoper");
		}
		break;
		
	default:
		crash("(CodeDesig) class");
	}
}
