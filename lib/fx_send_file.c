/**********************************************************************
 * File Exchange client library
 *
 * $Id: fx_send_file.c,v 1.3 1999-01-22 23:18:03 ghudson Exp $
 *
 * Copyright 1989, 1990 by the Massachusetts Institute of Technology.
 *
 * For copying and distribution information, please see the file
 * <mit-copyright.h>.
 **********************************************************************/

#include <mit-copyright.h>

#ifndef lint
static char rcsid_fx_send_file_c[] = "$Id: fx_send_file.c,v 1.3 1999-01-22 23:18:03 ghudson Exp $";
#endif /* lint */

#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include "fxcl.h"

/*
 * fx_send_file -- send a file to the exchange
 */

long
fx_send_file(fxp, p, filename)
     FX *fxp;
     Paper *p;
     char *filename;
{
  FILE *fp;
  struct stat buf;
  Paper to_send;
  long code;

  paper_copy(p, &to_send);

  /* check file status */
  if (stat(filename, &buf)) return((long) errno);
  if (buf.st_mode & S_IFDIR) return((long) EISDIR);
  if (buf.st_mode & S_IEXEC) to_send.flags |= PAPER_EXECUTABLE;

  /* send file with correct name */
  if (!to_send.filename) {
    to_send.filename = strrchr(filename, '/') + 1;
    if (to_send.filename == (char *) 1) to_send.filename = filename;
  }
  if ((fp = fopen(filename, "r")) == NULL) return((long) errno);

  code = fx_send(fxp, &to_send, fp);
  (void) fclose(fp);
  return(code);
}
