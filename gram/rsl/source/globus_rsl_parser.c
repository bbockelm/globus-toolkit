
/*  A Bison parser, made from globus_rsl_parser.y
 by  GNU Bison version 1.25
  */

#define YYBISON 1  /* Identify Bison output.  */

#define yyparse globus_rslparse
#define yylex globus_rsllex
#define yyerror globus_rslerror
#define yylval globus_rsllval
#define yychar globus_rslchar
#define yydebug globus_rsldebug
#define yynerrs globus_rslnerrs
#define	RSL_STRING	258
#define	RSL_OP_AND	259
#define	RSL_OP_OR	260
#define	RSL_OP_MULTIREQ	261
#define	RSL_OP_EQ	262
#define	RSL_OP_NEQ	263
#define	RSL_OP_LT	264
#define	RSL_OP_LTEQ	265
#define	RSL_OP_GT	266
#define	RSL_OP_GTEQ	267
#define	RSL_OP_CONCATENATE	268
#define	RSL_LPAREN	269
#define	RSL_RPAREN	270
#define	RSL_VARIABLE_START	271

#line 1 "globus_rsl_parser.y"

/*
 * Copyright 1999-2006 University of Chicago
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 * http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "globus_common.h"
#include <stdio.h>
#include <memory.h>
#include <string.h>
/* stdlib is required on aix */
#include <stdlib.h>
#include <assert.h>

#include "globus_rsl.h"

extern globus_mutex_t globus_i_rsl_mutex;

typedef struct globus_parse_state_s
{
    char                      * myinput;
    char                      * myinputptr;
    char                      * myinputlim;
    globus_rsl_t              * rsl_spec;
    globus_rsl_parse_error_t  * error_structure;
} globus_parse_state_t;

globus_parse_state_t parse_state;

int globus_parse_error_flag = 0;
    
#define GLOBUS_RSL_MIN(x,y) ((x) < (y) ? (x) : (y))

void
yyerror(char *str);

/* Provide our own function for reading input.  The default YY_INPUT
 * is overridden in globus_rsl_parser.l
 * from page 157 of Nutshell lex & yacc
 */
static int my_yyinput();


#line 60 "globus_rsl_parser.y"
typedef union
{
  int	               Int;
  char               * String;

  globus_rsl_t       * RSL;
  globus_rsl_value_t * RSLval;
  globus_list_t      * List;
} YYSTYPE;
#include <stdio.h>

#ifndef __cplusplus
#ifndef __STDC__
#define const
#endif
#endif



#define	YYFINAL		39
#define	YYFLAG		-32768
#define	YYNTBASE	17

#define YYTRANSLATE(x) ((unsigned)(x) <= 271 ? yytranslate[x] : 29)

static const char yytranslate[] = {     0,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     1,     2,     3,     4,     5,
     6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
    16
};

#if YYDEBUG != 0
static const short yyprhs[] = {     0,
     0,     2,     5,     7,     9,    11,    13,    18,    22,    26,
    28,    30,    32,    34,    36,    38,    40,    43,    45,    49,
    51,    55,    57,    59,    64
};

static const short yyrhs[] = {    18,
     0,    19,    20,     0,    21,     0,     6,     0,     4,     0,
     5,     0,    14,    18,    15,    20,     0,    14,    18,    15,
     0,    28,    22,    23,     0,     7,     0,     8,     0,    11,
     0,    12,     0,     9,     0,    10,     0,    24,     0,    25,
    24,     0,    25,     0,    14,    23,    15,     0,    26,     0,
    26,    13,    26,     0,    28,     0,    27,     0,    16,    14,
    23,    15,     0,     3,     0
};

#endif

#if YYDEBUG != 0
static const short yyrline[] = { 0,
    91,    95,   101,   108,   110,   111,   115,   121,   129,   138,
   140,   141,   142,   143,   144,   147,   155,   161,   169,   174,
   178,   184,   189,   193,   201
};
#endif


#if YYDEBUG != 0 || defined (YYERROR_VERBOSE)

static const char * const yytname[] = {   "$","error","$undefined.","RSL_STRING",
"RSL_OP_AND","RSL_OP_OR","RSL_OP_MULTIREQ","RSL_OP_EQ","RSL_OP_NEQ","RSL_OP_LT",
"RSL_OP_LTEQ","RSL_OP_GT","RSL_OP_GTEQ","RSL_OP_CONCATENATE","RSL_LPAREN","RSL_RPAREN",
"RSL_VARIABLE_START","start","specification","prefix_operator","spec_list","request",
"infix_operator","sequence","value_list","value","simple_value","variable","literal_string", NULL
};
#endif

static const short yyr1[] = {     0,
    17,    18,    18,    19,    19,    19,    20,    20,    21,    22,
    22,    22,    22,    22,    22,    23,    24,    24,    25,    25,
    26,    26,    26,    27,    28
};

static const short yyr2[] = {     0,
     1,     2,     1,     1,     1,     1,     4,     3,     3,     1,
     1,     1,     1,     1,     1,     1,     2,     1,     3,     1,
     3,     1,     1,     4,     1
};

static const short yydefact[] = {     0,
    25,     5,     6,     4,     1,     0,     3,     0,     0,     2,
    10,    11,    14,    15,    12,    13,     0,     0,     0,     0,
     9,    16,    18,    20,    23,    22,     8,     0,     0,    17,
     0,     7,    19,     0,    21,    24,     0,     0,     0
};

static const short yydefgoto[] = {    37,
     5,     6,    10,     7,    17,    21,    22,    23,    24,    25,
    26
};

static const short yypact[] = {     4,
-32768,-32768,-32768,-32768,-32768,   -11,-32768,     9,     4,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,    -3,   -10,    -3,     8,
-32768,-32768,    -3,    -9,-32768,-32768,   -11,    10,    -3,-32768,
    -2,-32768,-32768,    11,    -9,-32768,    23,    24,-32768
};

static const short yypgoto[] = {-32768,
    18,-32768,     1,-32768,-32768,   -17,     7,-32768,     0,-32768,
     6
};


#define	YYLAST		31


static const short yytable[] = {     1,
     1,    28,     9,    31,    27,     8,     1,     2,     3,     4,
    19,    34,    20,    20,     8,    11,    12,    13,    14,    15,
    16,    29,    38,    39,    33,    36,    18,    32,     0,    30,
    35
};

static const short yycheck[] = {     3,
     3,    19,    14,    13,    15,     0,     3,     4,     5,     6,
    14,    29,    16,    16,     9,     7,     8,     9,    10,    11,
    12,    14,     0,     0,    15,    15,     9,    27,    -1,    23,
    31
};
#define YYPURE 1

/* -*-C-*-  Note some compilers choke on comments on `#line' lines.  */
#line 3 "/Users/bester/tmp/bison-1.25/share/bison.simple"

/* Skeleton output parser for bison,
   Copyright (C) 1984, 1989, 1990 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

#ifndef alloca
#ifdef __GNUC__
#define alloca __builtin_alloca
#else /* not GNU C.  */
#if (!defined (__STDC__) && defined (sparc)) || defined (__sparc__) || defined (__sparc) || defined (__sgi)
#include <alloca.h>
#else /* not sparc */
#if defined (MSDOS) && !defined (__TURBOC__)
#include <malloc.h>
#else /* not MSDOS, or __TURBOC__ */
#if defined(_AIX)
#include <malloc.h>
 #pragma alloca
#else /* not MSDOS, __TURBOC__, or _AIX */
#ifdef __hpux
#ifdef __cplusplus
extern "C" {
void *alloca (unsigned int);
};
#else /* not __cplusplus */
void *alloca ();
#endif /* not __cplusplus */
#endif /* __hpux */
#endif /* not _AIX */
#endif /* not MSDOS, or __TURBOC__ */
#endif /* not sparc.  */
#endif /* not GNU C.  */
#endif /* alloca not defined.  */

/* This is the parser code that is written into each bison parser
  when the %semantic_parser declaration is not specified in the grammar.
  It was written by Richard Stallman by simplifying the hairy parser
  used when %semantic_parser is specified.  */

/* Note: there must be only one dollar sign in this file.
   It is replaced by the list of actions, each action
   as one case of the switch.  */

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		-2
#define YYEOF		0
#define YYACCEPT	return(0)
#define YYABORT 	return(1)
#define YYERROR		goto yyerrlab1
/* Like YYERROR except do call yyerror.
   This remains here temporarily to ease the
   transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */
#define YYFAIL		goto yyerrlab
#define YYRECOVERING()  (!!yyerrstatus)
#define YYBACKUP(token, value) \
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    { yychar = (token), yylval = (value);			\
      yychar1 = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { yyerror ("syntax error: cannot back up"); YYERROR; }	\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

#ifndef YYPURE
#define YYLEX		yylex()
#endif

#ifdef YYPURE
#ifdef YYLSP_NEEDED
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, &yylloc, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval, &yylloc)
#endif
#else /* not YYLSP_NEEDED */
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval)
#endif
#endif /* not YYLSP_NEEDED */
#endif

/* If nonreentrant, generate the variables here */

#ifndef YYPURE

int	yychar;			/*  the lookahead symbol		*/
YYSTYPE	yylval;			/*  the semantic value of the		*/
				/*  lookahead symbol			*/

#ifdef YYLSP_NEEDED
YYLTYPE yylloc;			/*  location data for the lookahead	*/
				/*  symbol				*/
#endif

int yynerrs;			/*  number of parse errors so far       */
#endif  /* not YYPURE */

#if YYDEBUG != 0
int yydebug;			/*  nonzero means print parse trace	*/
/* Since this is uninitialized, it does not stop multiple parsers
   from coexisting.  */
#endif

/*  YYINITDEPTH indicates the initial size of the parser's stacks	*/

#ifndef	YYINITDEPTH
#define YYINITDEPTH 200
#endif

/*  YYMAXDEPTH is the maximum size the stacks can grow to
    (effective only if the built-in stack extension method is used).  */

#if YYMAXDEPTH == 0
#undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
#define YYMAXDEPTH 10000
#endif

/* Prevent warning if -Wstrict-prototypes.  */
#ifdef __GNUC__
int yyparse (void);
#endif

#if __GNUC__ > 1		/* GNU C and GNU C++ define this.  */
#define __yy_memcpy(TO,FROM,COUNT)	__builtin_memcpy(TO,FROM,COUNT)
#else				/* not GNU C or C++ */
#ifndef __cplusplus

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (to, from, count)
     char *to;
     char *from;
     int count;
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#else /* __cplusplus */

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (char *to, char *from, int count)
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#endif
#endif

#line 196 "/Users/bester/tmp/bison-1.25/share/bison.simple"

/* The user can define YYPARSE_PARAM as the name of an argument to be passed
   into yyparse.  The argument should have type void *.
   It should actually point to an object.
   Grammar actions can access the variable by casting it
   to the proper pointer type.  */

#ifdef YYPARSE_PARAM
#ifdef __cplusplus
#define YYPARSE_PARAM_ARG void *YYPARSE_PARAM
#define YYPARSE_PARAM_DECL
#else /* not __cplusplus */
#define YYPARSE_PARAM_ARG YYPARSE_PARAM
#define YYPARSE_PARAM_DECL void *YYPARSE_PARAM;
#endif /* not __cplusplus */
#else /* not YYPARSE_PARAM */
#define YYPARSE_PARAM_ARG
#define YYPARSE_PARAM_DECL
#endif /* not YYPARSE_PARAM */

int
yyparse(YYPARSE_PARAM_ARG)
     YYPARSE_PARAM_DECL
{
  register int yystate;
  register int yyn;
  register short *yyssp;
  register YYSTYPE *yyvsp;
  int yyerrstatus;	/*  number of tokens to shift before error messages enabled */
  int yychar1 = 0;		/*  lookahead token as an internal (translated) token number */

  short	yyssa[YYINITDEPTH];	/*  the state stack			*/
  YYSTYPE yyvsa[YYINITDEPTH];	/*  the semantic value stack		*/

  short *yyss = yyssa;		/*  refer to the stacks thru separate pointers */
  YYSTYPE *yyvs = yyvsa;	/*  to allow yyoverflow to reallocate them elsewhere */

#ifdef YYLSP_NEEDED
  YYLTYPE yylsa[YYINITDEPTH];	/*  the location stack			*/
  YYLTYPE *yyls = yylsa;
  YYLTYPE *yylsp;

#define YYPOPSTACK   (yyvsp--, yyssp--, yylsp--)
#else
#define YYPOPSTACK   (yyvsp--, yyssp--)
#endif

  int yystacksize = YYINITDEPTH;

#ifdef YYPURE
  int yychar;
  YYSTYPE yylval;
  int yynerrs;
#ifdef YYLSP_NEEDED
  YYLTYPE yylloc;
#endif
#endif

  YYSTYPE yyval;		/*  the variable used to return		*/
				/*  semantic values from the action	*/
				/*  routines				*/

  int yylen;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Starting parse\n");
#endif

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss - 1;
  yyvsp = yyvs;
#ifdef YYLSP_NEEDED
  yylsp = yyls;
#endif

/* Push a new state, which is found in  yystate  .  */
/* In all cases, when you get here, the value and location stacks
   have just been pushed. so pushing a state here evens the stacks.  */
yynewstate:

  *++yyssp = yystate;

  if (yyssp >= yyss + yystacksize - 1)
    {
      /* Give user a chance to reallocate the stack */
      /* Use copies of these so that the &'s don't force the real ones into memory. */
      YYSTYPE *yyvs1 = yyvs;
      short *yyss1 = yyss;
#ifdef YYLSP_NEEDED
      YYLTYPE *yyls1 = yyls;
#endif

      /* Get the current used size of the three stacks, in elements.  */
      int size = yyssp - yyss + 1;

#ifdef yyoverflow
      /* Each stack pointer address is followed by the size of
	 the data in use in that stack, in bytes.  */
#ifdef YYLSP_NEEDED
      /* This used to be a conditional around just the two extra args,
	 but that might be undefined if yyoverflow is a macro.  */
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yyls1, size * sizeof (*yylsp),
		 &yystacksize);
#else
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yystacksize);
#endif

      yyss = yyss1; yyvs = yyvs1;
#ifdef YYLSP_NEEDED
      yyls = yyls1;
#endif
#else /* no yyoverflow */
      /* Extend the stack our own way.  */
      if (yystacksize >= YYMAXDEPTH)
	{
	  yyerror("parser stack overflow");
	  return 2;
	}
      yystacksize *= 2;
      if (yystacksize > YYMAXDEPTH)
	yystacksize = YYMAXDEPTH;
      yyss = (short *) alloca (yystacksize * sizeof (*yyssp));
      __yy_memcpy ((char *)yyss, (char *)yyss1, size * sizeof (*yyssp));
      yyvs = (YYSTYPE *) alloca (yystacksize * sizeof (*yyvsp));
      __yy_memcpy ((char *)yyvs, (char *)yyvs1, size * sizeof (*yyvsp));
#ifdef YYLSP_NEEDED
      yyls = (YYLTYPE *) alloca (yystacksize * sizeof (*yylsp));
      __yy_memcpy ((char *)yyls, (char *)yyls1, size * sizeof (*yylsp));
#endif
#endif /* no yyoverflow */

      yyssp = yyss + size - 1;
      yyvsp = yyvs + size - 1;
#ifdef YYLSP_NEEDED
      yylsp = yyls + size - 1;
#endif

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Stack size increased to %d\n", yystacksize);
#endif

      if (yyssp >= yyss + yystacksize - 1)
	YYABORT;
    }

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Entering state %d\n", yystate);
#endif

  goto yybackup;
 yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* yychar is either YYEMPTY or YYEOF
     or a valid token in external form.  */

  if (yychar == YYEMPTY)
    {
#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Reading a token: ");
#endif
      yychar = YYLEX;
    }

  /* Convert token to internal form (in yychar1) for indexing tables with */

  if (yychar <= 0)		/* This means end of input. */
    {
      yychar1 = 0;
      yychar = YYEOF;		/* Don't call YYLEX any more */

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Now at end of input.\n");
#endif
    }
  else
    {
      yychar1 = YYTRANSLATE(yychar);

#if YYDEBUG != 0
      if (yydebug)
	{
	  fprintf (stderr, "Next token is %d (%s", yychar, yytname[yychar1]);
	  /* Give the individual parser a way to print the precise meaning
	     of a token, for further debugging info.  */
#ifdef YYPRINT
	  YYPRINT (stderr, yychar, yylval);
#endif
	  fprintf (stderr, ")\n");
	}
#endif
    }

  yyn += yychar1;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != yychar1)
    goto yydefault;

  yyn = yytable[yyn];

  /* yyn is what to do for this token type in this state.
     Negative => reduce, -yyn is rule number.
     Positive => shift, yyn is new state.
       New state is final state => don't bother to shift,
       just return success.
     0, or most negative number => error.  */

  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrlab;

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting token %d (%s), ", yychar, yytname[yychar1]);
#endif

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  /* count tokens shifted since error; after three, turn off error status.  */
  if (yyerrstatus) yyerrstatus--;

  yystate = yyn;
  goto yynewstate;

/* Do the default action for the current state.  */
yydefault:

  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;

/* Do a reduction.  yyn is the number of a rule to reduce with.  */
yyreduce:
  yylen = yyr2[yyn];
  if (yylen > 0)
    yyval = yyvsp[1-yylen]; /* implement default value of the action */

#if YYDEBUG != 0
  if (yydebug)
    {
      int i;

      fprintf (stderr, "Reducing via rule %d (line %d), ",
	       yyn, yyrline[yyn]);

      /* Print the symbols being reduced, and their result.  */
      for (i = yyprhs[yyn]; yyrhs[i] > 0; i++)
	fprintf (stderr, "%s ", yytname[yyrhs[i]]);
      fprintf (stderr, " -> %s\n", yytname[yyr1[yyn]]);
    }
#endif


  switch (yyn) {

case 1:
#line 92 "globus_rsl_parser.y"
{ parse_state.rsl_spec = yyvsp[0].RSL; ;
    break;}
case 2:
#line 97 "globus_rsl_parser.y"
{
      yyval.RSL = globus_rsl_make_boolean (yyvsp[-1].Int, yyvsp[0].List);
      assert ((yyval.RSL)!=NULL);
    ;
    break;}
case 3:
#line 102 "globus_rsl_parser.y"
{ 
      yyval.RSL = yyvsp[0].RSL;
    ;
    break;}
case 4:
#line 109 "globus_rsl_parser.y"
{ yyval.Int = GLOBUS_RSL_MULTIREQ; ;
    break;}
case 5:
#line 110 "globus_rsl_parser.y"
{ yyval.Int = GLOBUS_RSL_AND; ;
    break;}
case 6:
#line 111 "globus_rsl_parser.y"
{ yyval.Int = GLOBUS_RSL_OR; ;
    break;}
case 7:
#line 117 "globus_rsl_parser.y"
{
      yyval.List = globus_list_cons ((void *) yyvsp[-2].RSL, yyvsp[0].List);
      assert ((yyval.List)!=NULL);
    ;
    break;}
case 8:
#line 122 "globus_rsl_parser.y"
{
      yyval.List = globus_list_cons ((void *) yyvsp[-1].RSL, NULL);
      assert ((yyval.List)!=NULL);
    ;
    break;}
case 9:
#line 131 "globus_rsl_parser.y"
{
      yyval.RSL = globus_rsl_make_relation (yyvsp[-1].Int, yyvsp[-2].String, yyvsp[0].RSLval);
      assert ((yyval.RSL)!=NULL);
    ;
    break;}
case 10:
#line 139 "globus_rsl_parser.y"
{ yyval.Int = GLOBUS_RSL_EQ; ;
    break;}
case 11:
#line 140 "globus_rsl_parser.y"
{ yyval.Int = GLOBUS_RSL_NEQ; ;
    break;}
case 12:
#line 141 "globus_rsl_parser.y"
{ yyval.Int = GLOBUS_RSL_GT; ;
    break;}
case 13:
#line 142 "globus_rsl_parser.y"
{ yyval.Int = GLOBUS_RSL_GTEQ; ;
    break;}
case 14:
#line 143 "globus_rsl_parser.y"
{ yyval.Int = GLOBUS_RSL_LT; ;
    break;}
case 15:
#line 144 "globus_rsl_parser.y"
{ yyval.Int = GLOBUS_RSL_LTEQ; ;
    break;}
case 16:
#line 149 "globus_rsl_parser.y"
{
      yyval.RSLval = globus_rsl_value_make_sequence (yyvsp[0].List);
      assert ((yyval.RSLval)!=NULL);
    ;
    break;}
case 17:
#line 157 "globus_rsl_parser.y"
{
      yyval.List = globus_list_cons ((void *) yyvsp[-1].RSL, yyvsp[0].List);
      assert ((yyval.List)!=NULL);
    ;
    break;}
case 18:
#line 162 "globus_rsl_parser.y"
{
      yyval.List = globus_list_cons ((void *) yyvsp[0].RSL, NULL);
      assert ((yyval.List)!=NULL);
    ;
    break;}
case 19:
#line 171 "globus_rsl_parser.y"
{ 
      yyval.RSLval = yyvsp[-1].RSLval;
    ;
    break;}
case 20:
#line 174 "globus_rsl_parser.y"
{ yyval.RSLval = yyvsp[0].RSLval; ;
    break;}
case 21:
#line 180 "globus_rsl_parser.y"
{
      yyval.RSLval = globus_rsl_value_make_concatenation (yyvsp[-2].RSLval, yyvsp[0].RSLval);
      assert ((yyval.RSLval)!=NULL);
    ;
    break;}
case 22:
#line 185 "globus_rsl_parser.y"
{ 
      yyval.RSLval = globus_rsl_value_make_literal (yyvsp[0].String);
      assert ((yyval.RSLval)!=NULL);
    ;
    break;}
case 23:
#line 189 "globus_rsl_parser.y"
{ yyval.RSLval = yyvsp[0].RSLval; ;
    break;}
case 24:
#line 195 "globus_rsl_parser.y"
{
      yyval.RSLval = globus_rsl_value_make_variable (yyvsp[-1].RSLval);
      assert ((yyval.RSLval)!=NULL);
    ;
    break;}
case 25:
#line 202 "globus_rsl_parser.y"
{ yyval.String = yyvsp[0].String;
    ;
    break;}
}
   /* the action file gets copied in in place of this dollarsign */
#line 498 "/Users/bester/tmp/bison-1.25/share/bison.simple"

  yyvsp -= yylen;
  yyssp -= yylen;
#ifdef YYLSP_NEEDED
  yylsp -= yylen;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

  *++yyvsp = yyval;

#ifdef YYLSP_NEEDED
  yylsp++;
  if (yylen == 0)
    {
      yylsp->first_line = yylloc.first_line;
      yylsp->first_column = yylloc.first_column;
      yylsp->last_line = (yylsp-1)->last_line;
      yylsp->last_column = (yylsp-1)->last_column;
      yylsp->text = 0;
    }
  else
    {
      yylsp->last_line = (yylsp+yylen-1)->last_line;
      yylsp->last_column = (yylsp+yylen-1)->last_column;
    }
#endif

  /* Now "shift" the result of the reduction.
     Determine what state that goes to,
     based on the state we popped back to
     and the rule number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTBASE] + *yyssp;
  if (yystate >= 0 && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTBASE];

  goto yynewstate;

yyerrlab:   /* here on detecting error */

  if (! yyerrstatus)
    /* If not already recovering from an error, report this error.  */
    {
      ++yynerrs;

#ifdef YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (yyn > YYFLAG && yyn < YYLAST)
	{
	  int size = 0;
	  char *msg;
	  int x, count;

	  count = 0;
	  /* Start X at -yyn if nec to avoid negative indexes in yycheck.  */
	  for (x = (yyn < 0 ? -yyn : 0);
	       x < (sizeof(yytname) / sizeof(char *)); x++)
	    if (yycheck[x + yyn] == x)
	      size += strlen(yytname[x]) + 15, count++;
	  msg = (char *) malloc(size + 15);
	  if (msg != 0)
	    {
	      strcpy(msg, "parse error");

	      if (count < 5)
		{
		  count = 0;
		  for (x = (yyn < 0 ? -yyn : 0);
		       x < (sizeof(yytname) / sizeof(char *)); x++)
		    if (yycheck[x + yyn] == x)
		      {
			strcat(msg, count == 0 ? ", expecting `" : " or `");
			strcat(msg, yytname[x]);
			strcat(msg, "'");
			count++;
		      }
		}
	      yyerror(msg);
	      free(msg);
	    }
	  else
	    yyerror ("parse error; also virtual memory exceeded");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror("parse error");
    }

  goto yyerrlab1;
yyerrlab1:   /* here on error raised explicitly by an action */

  if (yyerrstatus == 3)
    {
      /* if just tried and failed to reuse lookahead token after an error, discard it.  */

      /* return failure if at end of input */
      if (yychar == YYEOF)
	YYABORT;

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Discarding token %d (%s).\n", yychar, yytname[yychar1]);
#endif

      yychar = YYEMPTY;
    }

  /* Else will try to reuse lookahead token
     after shifting the error token.  */

  yyerrstatus = 3;		/* Each real token shifted decrements this */

  goto yyerrhandle;

yyerrdefault:  /* current state does not do anything special for the error token. */

#if 0
  /* This is wrong; only states that explicitly want error tokens
     should shift them.  */
  yyn = yydefact[yystate];  /* If its default is to accept any token, ok.  Otherwise pop it.*/
  if (yyn) goto yydefault;
#endif

yyerrpop:   /* pop the current state because it cannot handle the error token */

  if (yyssp == yyss) YYABORT;
  yyvsp--;
  yystate = *--yyssp;
#ifdef YYLSP_NEEDED
  yylsp--;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "Error: state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

yyerrhandle:

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yyerrdefault;

  yyn += YYTERROR;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != YYTERROR)
    goto yyerrdefault;

  yyn = yytable[yyn];
  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrpop;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrpop;

  if (yyn == YYFINAL)
    YYACCEPT;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting error token, ");
#endif

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  yystate = yyn;
  goto yynewstate;
}
#line 206 "globus_rsl_parser.y"


#include "globus_rsl_parser.lex.c"

void
yyerror(char *str)
{
    parse_state.error_structure =
       (globus_rsl_parse_error_t *) malloc(sizeof(globus_rsl_parse_error_t));
    parse_state.rsl_spec = (globus_rsl_t *) NULL;
    parse_state.error_structure->code = 1;
    parse_state.error_structure->line = 42;
    parse_state.error_structure->position = 3;
    sprintf(parse_state.error_structure->message,"%s: %s",
            str,
            yytext);
}

int
yywrap()
{
    return(1);
}

/**
 * @defgroup globus_rsl_parse RSL Parsing
 */

/**
 * @brief Parse an RSL string
 * @ingroup globus_rsl_parse
 *
 * @details
 *     The globus_rsl_parse() function parses the string pointed to by
 *     the @a buf parameter into an RSL syntax tree. The caller is responsible
 *     for freeing that tree by calling globus_rsl_free_recursive().
 *
 * @param buf
 *     A NULL-terminated string that contains an RSL relation or boolean
 *     composition.
 *
 * @return
 *     Upon success, the globus_rsl_parse() function returns the parse
 *     tree generated by processing its input. If an error occurs, 
 *     globus_rsl_parse() returns NULL.
 */
globus_rsl_t *globus_rsl_parse(char *buf)
{
    globus_rsl_t *                      rsl = NULL;

    if (!buf)
    {
        goto null_buf;
    }

    globus_mutex_lock(&globus_i_rsl_mutex);
    if (parse_state.error_structure)
    {
        parse_state.error_structure->code = 0;
    }
    parse_state.myinput = buf;
    parse_state.myinputptr = buf;
    parse_state.myinputlim = buf + strlen(buf);

    /* George clever kludge to make yyparse() */
    /* work more than once ... otherwise yyparse always */
    /* rejects string (even valid strings) on calls after the first */
    if (yyin)
        yyrestart(yyin);

    yyparse();

    if (globus_parse_error_flag)
    {
        goto parse_error;
    }
    else
    {
        rsl = parse_state.rsl_spec;
    }
parse_error:
    globus_mutex_unlock(&globus_i_rsl_mutex);
null_buf:
    return rsl;
} /* globus_rsl_parse() */

/*
 * from page 157 Nutshell lex & yacc
 */
static int my_yyinput(char *buf, int max_size)
{

    int n = GLOBUS_RSL_MIN(max_size,
                          (parse_state.myinputlim - parse_state.myinputptr));

    if (n > 0)
    {
        memcpy(buf, parse_state.myinputptr, n);
        parse_state.myinputptr += n;
    } /* endif */

    return n;

} /* end my_yyinput() */
