/* nlp.h
 * 2017-05-04
 */

#ifndef  _NLP_DEFINE_H_
#define  _NLP_DEFINE_H_

#define NLP_ERR_NOERR              (0)
#define NLP_ERR_ERROR              (1)

#define NLP_STATE_INITIAL          (1)
#define NLP_STATE_CONTAINS         (2)
#define NLP_STATE_TERM             (3)
#define NLP_STATE_IDENTIFIER       (4)
#define NLP_STATE_ADDR             (5)
#define NLP_STATE_VALUES           (6)
#define NLP_STATE_END              (7)
#define NLP_STATE_STOP             (8)

#define NLP_MAX_DIMENSION          (8)
#define NLP_MAX_STRING_LENGTH      (1024)
#define NLP_MAX_IDENTIFIER_LENGTH  (63)


/*
 * identifier case option
 */
#define NLP_CASE_MASK              (1)
#define NLP_FOLLOW_CASE            (0)
#define NLP_IGNORE_CASE            (1)

/*
 * value type check option
 */
#define NLP_ERROR_MASK             (2)
#define NLP_ERROR_LOOSE            (0)
#define NLP_ERROR_STRICT           (2)

/*
 * error handling option
 */
#define NLP_WRN_ERROR_MASK         (4)
#define NLP_WRN_PASS               (0)
#define NLP_WRN_ERROR              (4)

/*
 * error report option
 */
#define NLP_WRN_MASK               (8)
#define NLP_WRN_OMIT               (0)
#define NLP_WRN_REPORT             (8)

/*
 * string option
 */
#define NLP_STRING_TOUPPER         (16)
#define NLP_STRING_TOLOWER         (32)
#define NLP_STRING_PAD_SPACE       (64)
#define NLP_STRING_PAD_NULL        (128)
#define NLP_STRING_PAD_MASK        (192)



#ifdef _MSC_VER
#define strncasecmp _strnicmp
#define strcasecmp _stricmp
#endif

#include <stdint.h>
#define _int32 int32_t
#define _int64 int64_t

#define _real32 float
#define _real64 double

#endif /*_NLP_DEFINE_H_*/
