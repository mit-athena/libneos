/**********************************************************************
 * File Exchange client library
 *
 * $Id: _fx_shorten.c,v 1.3 1999-01-22 23:17:50 ghudson Exp $
 *
 * Copyright 1989, 1990 by the Massachusetts Institute of Technology.
 *
 * For copying and distribution information, please see the file
 * <mit-copyright.h>.
 **********************************************************************/

#include <mit-copyright.h>

#ifndef lint
static char rcsid__fx_shorten_c[] = "$Id: _fx_shorten.c,v 1.3 1999-01-22 23:17:50 ghudson Exp $";
#endif /* lint */

#include <string.h>
#include "fxcl.h"

/*
 * _fx_shorten -- strip off local kerberos realm
 */

void
_fx_shorten(fxp, name)
     FX *fxp;
     char *name;
{
  register char *s;
  s = strchr(name, '@');
  if (s)
    if (strcmp(s, fxp->extension) == 0)
      *s = '\0';
}
