/*
 * nlp.c
 * 2017-05-04
 */
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "nlp_state.h"
#include "nlp_variable.h"
#include "nlp_value.h"
#include "nlp_log.h"
#include "nlp.h"

extern int nlp_set_context(FILE *f, char *filename);

static FILE *inp;
static FILE *log;

static struct nlp_variable_list_t variable_list;


/**
 * @brief Trim white space
 * @param[in] char *s ... String to trim (Return value)
 * @param[in] int l ... Buffer size
 */
static void trim(char *s, int l)
{
  int i;
  for (i = l - 1; i > 0; i--)
  {
    if (s[i] == ' ')
    {
      s[i] = '\0';
    } else {
      break;
    }
  }
}



/*
 * Fortran interface
 */

/**
 * @brief Initialize NLP
 * Arguments:
 *   character(*), intent(in)  :: name    ! Dummy, some string needed
 *   integer, intent(in)       :: caseopt ! Variable name letter case option
 *                                          1/2, Follow case/Ignore case
 * Return value:
 *   integer ... 0/1, Success/Error
 * Example:
 *   name = TEST, Ignore case of variable name
 *   integer :: result
 *   result = nlp_init('TEST', 2)
 */
int nlp_init(char *name, int *caseopt, int len_name)
{
  char *szname;
  int result;

  inp = NULL;
  log = NULL;
  nlp_init_namelist_name();
  if (len_name < 0) return 1;
  if (*caseopt != NLP_FOLLOW_CASE && *caseopt != NLP_IGNORE_CASE) return 1;
  szname = malloc(len_name + 1);
  if (szname == NULL) return 1;
  memcpy(szname, name, len_name);
  szname[len_name] = '\0';
  trim(szname, len_name);
  result = nlp_variable_init_list(&variable_list, szname, *caseopt);
  free(szname);
  nlp_filename_list_init();
  return result;
}

/* Purpose: Add variable
 * Arguments:
 *   character(*), intent(in)  :: varname ! Name of variable in input data
 *   character(*), intent(in)  :: vartype ! Type of variable
 *
 *   integer, intent(in)       :: dim(:)  ! Dimension of variable
 *   integer, intent(in)       :: min(:)  ! The minimum of the sequence subscript each dimension
 *   integer, intent(in)       :: max(:)  ! The maximum of the sequence subscript each dimension
 *   ANY, intent(in)           :: - First element of variable -
 * Return value:
 *   integer ... 0/1, Success/Error
 * Example:
 *  
 */
int nlp_add_variable(char *varname, char *vartype, int *dim, int *min, int *max, void *p, int len_varname, int len_vartype)
{
  struct nlp_variable_t variable;
  char *szvarname;
  char *szvartype;
  int ivartype;
  int length;
  int varopt;
  char *c;
  int result;

  result = 1;
  szvarname = NULL;
  szvartype = NULL;
  if (len_varname <= 0) goto error_handler;
  if (len_vartype <= 0) goto error_handler;;
  szvarname = malloc(len_varname + 1);
  szvartype = malloc(len_vartype + 1);
  if (szvarname == NULL) goto error_handler;
  if (szvartype == NULL) goto error_handler;
  memcpy(szvarname, varname, len_varname);
  memcpy(szvartype, vartype, len_vartype);
  szvarname[len_varname] = '\0';
  szvartype[len_vartype] = '\0';
  trim(szvarname, len_varname);
  trim(szvartype, len_vartype);
  for (c = szvartype; *c != '\0'; c++) {
    *c = toupper(*c);
  }
  varopt = 0;
  switch (szvartype[0]) {
  case 'R':
    if (szvartype[1] == '\0')
    {
      length = 4;
    }
    else
    {
      length = atoi(&szvartype[1]);
    }
    if (length == 4)
    {
      ivartype = NLP_VARTYPE_REAL;
    }
    else if (length == 8)
    {
      ivartype = NLP_VARTYPE_REAL8;
    } else {
      goto error_handler;
    }
    break;
  case 'I':
    if (szvartype[1] == '\0')
    {
      length = 4;
    }
    else
    {
      length = atoi(&szvartype[1]);
    }
    if (length == 4)
    {
      ivartype = NLP_VARTYPE_INTEGER;
    }
    else if (length == 8)
    {
      ivartype = NLP_VARTYPE_INTEGER8;
    } else {
      goto error_handler;
    }
    break;
  case 'C':
    c = &szvartype[1];
    if (*c == 'U')
    {
      varopt = NLP_STRING_TOUPPER;
      c++;
    }
    else if (*c == 'L')
    {
      varopt = NLP_STRING_TOLOWER;
      c++;
    }
    
    length = atoi(c);
    if (length < 1 || length > NLP_MAX_FIXCHAR_LENGTH)
    {
      goto error_handler;
    }
    ivartype = NLP_VARTYPE_FIXCHAR;
    break;
  default:
    goto error_handler;
  }
  memset(&variable, 0, sizeof(struct nlp_variable_t));
  nlp_variable_set(&variable, szvarname, ivartype, varopt, length, *dim, min, max, p);
  nlp_variable_append(&variable_list, &variable);

error_handler:
  if (szvarname != NULL) free(szvarname);
  if (szvartype != NULL) free(szvartype);
  return result;
}



/*
 *
 */
int nlp_open(char *inp_filename, char *log_filename, int len_inp_filename, int len_log_filename)
{
  char *szinp_filename;
  char *szlog_filename;

  szinp_filename = malloc(len_inp_filename + 1);
  szlog_filename = malloc(len_log_filename + 1);
  if (szinp_filename == NULL || szlog_filename == NULL)
  {
    if (szinp_filename != NULL) free(szinp_filename);
    if (szlog_filename != NULL) free(szlog_filename);
    return 1;
  }
  memcpy(szinp_filename, inp_filename, len_inp_filename);
  szinp_filename[len_inp_filename] = '\0';
  trim(szinp_filename, len_inp_filename);
  memcpy(szlog_filename, log_filename, len_log_filename);
  szlog_filename[len_log_filename] = '\0';
  trim(szlog_filename, len_log_filename);

  inp = fopen(szinp_filename, "r");
  log = fopen(szlog_filename, "w");

  if (inp == NULL || log == NULL)
  {
    if (inp != NULL) fclose(inp);
    if (log != NULL) fclose(log);
    inp = NULL;
    log = NULL;
    free(szinp_filename);
    free(szlog_filename);
    return 1;
  }

  nlp_log_set(log);
  nlp_variable_set_list(&variable_list);

  nlp_set_error(NLP_ERR_NOERR);
  nlp_set_state(NLP_STATE_INITIAL);
  nlp_set_current_value_type(NLP_VALTYPE_NULL);
  nlp_value_init();
  nlp_decode_init();

  if (nlp_set_context(inp, szinp_filename) == 1)
  {
    fclose(inp);
    fclose(log);
    inp = NULL;
    log = NULL;
    free(szinp_filename);
    free(szlog_filename);
    return 1;
  }

  free(szinp_filename);
  free(szlog_filename);

  return 0;
}


int nlp_read()
{
  int state;

  nlp_set_state(NLP_STATE_INITIAL);
  if (yyparse() == 1)
  {
    /*
     * YYABORT
     */
    return -1;
  }
  state = nlp_get_state();
  if (state == NLP_STATE_END)
  {
    return 1;
  }
  if (state == NLP_STATE_STOP)
  {
    return 2;
  }
  return 0;
}


int nlp_close()
{
  if (inp != NULL) fclose(inp);
  if (log != NULL) fclose(log);
  inp = NULL;
  log = NULL;
  return 0;
}



int nlp_cleanup()
{
  return nlp_variable_free_list(&variable_list);
}
