/**********************************************************************
 * File Exchange client library
 *
 * $Id: fx_retrieve.c,v 1.4 1999-01-22 23:18:01 ghudson Exp $
 *
 * Copyright 1989, 1990 by the Massachusetts Institute of Technology.
 *
 * For copying and distribution information, please see the file
 * <mit-copyright.h>.
 **********************************************************************/

#include <mit-copyright.h>

#ifndef lint
static char rcsid_fx_retrieve_c[] = "$Id: fx_retrieve.c,v 1.4 1999-01-22 23:18:01 ghudson Exp $";
#endif /* lint */

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "fxcl.h"
#include "memory.h"
#define brst(res) (res->retrieve_res_u.burst)

/*
 * fx_retrieve -- retrieve a stream from the exchange
 */

long
fx_retrieve(fxp, p, fp)
     FX *fxp;
     Paper *p;
     FILE *fp;
{
  FX *f;
  long *res1, code = 0L;
  init_res *ires;
  retrieve_res *res = NULL;
  int dummy;
  Paper to_retrieve;
  char new_owner[FX_UNAMSZ], new_author[FX_UNAMSZ];

  f = fxp;
  if (strcasecmp(fxp->host, p->location.host)) {
    if ((f = New(FX)) == NULL) return((long) errno);
    (void) strcpy(f->host, p->location.host);
    (void) strcpy(f->name, fxp->name);
    code = fx_init(f, &ires);
    if (code && code != ERR_NOT_SYNC) {
      free((char *)f);
      return(code);
    }
  }

  paper_copy(p, &to_retrieve);

#ifdef KERBEROS
  /* lengthen usernames to kerberos principals */
  to_retrieve.owner = _fx_lengthen(fxp, p->owner, new_owner);
  to_retrieve.author = _fx_lengthen(fxp, p->author, new_author);
#endif

  res1 = retrieve_file_1(&to_retrieve, f->cl);
  if (!res1) {
    code = _fx_rpc_errno(f->cl);
    goto FX_RETRIEVE_CLEANUP;
  }
  code = *res1;
  xdr_free(xdr_long, (char *) res1);
  if (code) goto FX_RETRIEVE_CLEANUP;

  do {
    if (res) xdr_free(xdr_retrieve_res, (char *) res);
    res = retrieve_burst_1(&dummy, f->cl);
    if (!res) {
      code = _fx_rpc_errno(f->cl);
      goto FX_RETRIEVE_CLEANUP;
    }
    if (code = res->local_errno)
      goto FX_RETRIEVE_CLEANUP;

    if (fwrite(brst(res).data, 1, brst(res).size, fp) < brst(res).size) {
      code = (long) errno;
      goto FX_RETRIEVE_CLEANUP;
    }
  } while (brst(res).size == MAX_BURST_SIZE);

 FX_RETRIEVE_CLEANUP:
  if (res) xdr_free(xdr_retrieve_res, (char *) res);
  if (f != fxp) fx_close(f);
  return(code);
}
