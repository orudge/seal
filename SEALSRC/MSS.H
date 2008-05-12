/*  MSS -- Memory Supervision System version 1.2
 *  Copyright (C) 1998  Juan Jesus Alcolea Picazo and Peter Palotas
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *  You can contact the authors of this library at the following e-mail
 *  addreses. For more information, look in the documentation that came
 *  with this library.
 *
 *  Juan Jesus Alcolea Picazo, a920101@zipi.fi.upm.es
 *  Peter Palotas, blizzar@hem1.passagen.se
 *
 */

#ifndef __mss_h__
#define __mss_h__

/*************************************************************************/
/*                      COMPILER SPECIFIC OPTIONS                        */
/*************************************************************************/

/* MSS_PTR_SIZE_T should be defined to an integer type that is large enough
   to hold the value of a pointer. */
#define MSS_PTR_SIZE_T size_t

/* MSS_PRINTF_SIZE_T should be defined to a printf() type-specifier string
   that will correctly print an argument of the type #defined by
   MSS_PTR_SIZE_T. (Ex. if MSS_PTR_SIZE_T is #defined to be 'long'
   MSS_PRINTF_SIZE_T should be "%l"). 
   (If MSS_PTR_SIZE_T is set to 'size_t' this should be "%lu" under DJGPP and
    "%u" under Linux GCC). */
#define MSS_PRINTF_SIZE_T "%lu"

/* MSS_FUNCTION_MACRO should represent a predefined macro or something like
   that, which will contain the current function name. (Set it to "unknown"
   or something like that if there is no such thing on your compiler.
   (GNU C Uses __PRETTY_FUNCTION__ or __FUNCTION__). */
#define MSS_FUNCTION_MACRO __PRETTY_FUNCTION__
/* #define MSS_FUNCTION_MACRO "(unknown function)" */


/* Some compiler needs a redefinition of new[] and delete[], some don't.
   If your compiler does, define this, otherwise don't. (GCC needs this). */
#define MSS_DEFINE_NEW_DELETE_ARRAY


/* MSS_USE_EXCEPTIONS should be defined if your compiler supports exceptions.
   If you get a warning/error about operator delete throwing diffrent
   exceptions try enabling/disabling this, that should help, if not, don't
   ask me, I don't know anything about exceptions yet. (GCC 2.8.0 supports
   exceptions, earlier versions does not, well, it does support them a little,
   but this should be undefined if you use any version earlier than GCC 2.8.0.
   (With PGCC-2.90.23 based on EGCS 1.0.1 this should be defined).
   Anyway, if you get the errors above, try undefining this if defined, and
   defining it if undefined. */
#define MSS_USE_EXCEPTIONS


/* MSS_DTOR_AFTER_ATEXIT should be defined in case your compiler will run
   the destructors of global objects AFTER any registered atexit functions.
   It is not essential that this setting is correct, but it will make the
   logfile look nicer :)
   If your compiler is advanced enough to follow the C++ standard that says
   that any "exit functions", including both destructors of global objects
   and functions registered with atexit should be called in the reverse order
   that they were registered, your compiler will probably mix atexit
   functions and destructors of global objects. In that case you will just
   have to test by toggeling this option and see which give the nicest
   looking result. Remember, whatever setting you choose for this option,
   a crash or severe error will NEVER occur. */
/* #define MSS_DTOR_AFTER_ATEXIT */

/* These macros are for multithreading environments.  They should disable
   respectively enable threading.  (If you work in a non-multithreading
   environment or you are certain that two threads never will use MSS at
   the same time, you may safely define these to nothing. */
#define	MSS_DISABLE_THREADING_FUNC
#define MSS_ENABLE_THREADING_FUNC

/**************************************************************************/
/*              DO NOT CHANGE ANYTHING BELOW THIS LINE                    */
/**************************************************************************/

#include <stdlib.h>
#include <string.h>

#define MSS_VERSION "1.2"

#ifdef __cplusplus
extern "C" {
#endif /* !__cplusplus */

/****************/
/*    USER.C    */
/****************/
void mss_enter_scope(const char *filename, const char *function, unsigned long line);
void mss_leave_scope(const char *filename, const char *function, unsigned long line);

void mss_unregister_constant_block(void *ptr, const char *filename, const char *function, unsigned long line);
void mss_register_constant_block(void *ptr, const char *filename, const char *function, unsigned long line);

unsigned int mss_currently_used_mem(void);
unsigned int mss_maximum_used_nem(void);
unsigned int mss_number_of_allocated_blocks(void);

void mss_set_block_label(void *ptr, const char *message, const char *filename, const char *function, unsigned long line);
void mss_dump_block(void *ptr, const char *file, const char *filename, const char *function, unsigned long line);

/****************/
/*   ALLOC.C    */
/****************/
#define MSS_BY_NEW		1
#define MSS_BY_NEW_ARRAY	2
#define MSS_BY_DELETE		3
#define MSS_BY_DELETE_ARRAY	4
#define MSS_BY_MALLOC		5
#define MSS_BY_REALLOC		6
#define MSS_BY_CALLOC		7
#define MSS_BY_FREE		8
#define MSS_BY_XMALLOC		9
#define MSS_BY_XREALLOC		10
#define MSS_BY_XFREE		11
#define MSS_BY_CFREE		12
#define MSS_BY_STRDUP		13

void *mss_malloc(size_t s, int called_by, const char *filename, const char *function, unsigned long line);
char *mss_strdup(const char *source, const char *filename, const char *function, unsigned long line);
void *mss_realloc(void *ptr, size_t size, int called_by, const char *filename, const char *function, unsigned long line);
void mss_free(void *ptr, int called_by, const char *filename, const char *function, unsigned long line);

/****************/
/*   CHECK.C    */
/****************/
void mss_check_pointer_validity(void *ptr, const char *filename, const char *function, unsigned long line);
void mss_check_all_blocks(const char *filename, const char *function, unsigned long line);
void mss_check_block(void *ptr, const char *filename, const char *function, unsigned long line);


/****************/
/*  CPPSPEC.CC  */
/****************/
#ifdef __cplusplus
} /* End of extern "C" */

extern unsigned int MSS_DEL_LINE;
extern char *MSS_DEL_FILENAME;
extern char *MSS_DEL_FUNCTION;

/* Overloaded and redefined operators */
void *operator new(size_t s);
void *operator new(size_t s, const char *filename, const char *function, unsigned long line);
//void operator delete(void *p)
#ifdef MSS_USE_EXCEPTIONS
//throw()
#endif
//;  /* Yes, this semicolon has to be here! */

#ifdef MSS_DEFINE_NEW_DELETE_ARRAY
void *operator new[](size_t s);
void *operator new[](size_t s, const char *filename, const char *function, unsigned long line);
void operator delete[](void *p)
#ifdef MSS_USE_EXCEPTIONS
//throw()
#endif /* !MSS_USE_EXCEPTIONS */
; /* Yes, this semicolon has to be here! */
#endif /* !MSS_DEFINE_NEW_DELETE_ARRAY */

extern "C" {
#endif /* !__cplusplus */


/****************/
/*    LOG.C     */
/****************/
typedef int (*MssCallBackFunc)  (char *		outbuf,
				 void *		blkptr,
		              	 unsigned int 	blklen,
	 			 const char * 	blklab,
				 const char * 	blksrc,
				 const char * 	blkfnc,
				 unsigned int blklin);

void mss_log_block_list_filtered(const char * filename, const char * function, unsigned int line, MssCallBackFunc callback_func);
void mss_log_msg(const char *msg, const char *filename, const char *function, unsigned long line);
void mss_log_disable(const char *filename, const char *function, unsigned long line);
void mss_log_enable(const char *filename, const char *function, unsigned long line);
void mss_log_info(const char *filename, const char *function, unsigned long line);
void mss_log_internal_info(const char *filename, const char *function, unsigned long line);
void mss_log_list_blocks(const char *filename, const char *function, unsigned long line);


/****************/
/*    INIT.C    */
/****************/
void mss_startup(void);

#ifdef __cplusplus
}
#endif /* !__cplusplus */

/****************/
/* User  Macros */
/****************/
/* Don't define these if in MSS source */
#ifndef __mss_internal__

/* If MSS is not defined, we do not use MSS either ;) */
#ifdef MSS
#ifdef __cplusplus
#define new new(__FILE__, MSS_FUNCTION_MACRO, __LINE__)
#define delete MSS_DEL_FILENAME = __FILE__, MSS_DEL_LINE = __LINE__, MSS_DEL_FUNCTION = MSS_FUNCTION_MACRO, delete
#endif /* !__cplusplus */

#define malloc(s) mss_malloc(s, MSS_BY_MALLOC, __FILE__, MSS_FUNCTION_MACRO, __LINE__)
#define xmalloc(s) mss_malloc(s, MSS_BY_XMALLOC, __FILE__, MSS_FUNCTION_MACRO, __LINE__)
#define free(p) mss_free(p, MSS_BY_FREE, __FILE__, MSS_FUNCTION_MACRO, __LINE__)
#define realloc(p, s) mss_realloc(p, s, MSS_BY_REALLOC, __FILE__, MSS_FUNCTION_MACRO, __LINE__)
#define xrealloc(p, s) mss_realloc(p, s, MSS_BY_XREALLOC, __FILE__, MSS_FUNCTION_MACRO, __LINE__)
#define calloc(num, size) mss_malloc((num) * (size), MSS_BY_CALLOC, __FILE__, MSS_FUNCTION_MACRO, __LINE__)
#define xfree(p) mss_free(p, MSS_BY_XFREE, __FILE__, MSS_FUNCTION_MACRO, __LINE__)
#define cfree(p) mss_free(p, MSS_BY_CFREE, __FILE__, MSS_FUNCTION_MACRO, __LINE__)
#define strdup(s) mss_strdup(s, __FILE__, MSS_FUNCTION_MACRO, __LINE__)

#define MSS_ENTER_SCOPE mss_enter_scope(__FILE__, MSS_FUNCTION_MACRO, __LINE__)
#define MSS_LEAVE_SCOPE mss_leave_scope(__FILE__, MSS_FUNCTION_MACRO, __LINE__)

#define MSS_LOG_INFO mss_log_info(__FILE__, MSS_FUNCTION_MACRO, __LINE__)
#define MSS_LOG_INTERNAL_INFO mss_log_internal_info(__FILE__, MSS_FUNCTION_MACRO, __LINE__)
#define MSS_LOG_BLOCK_LIST mss_log_list_blocks(__FILE__, MSS_FUNCTION_MACRO, __LINE__)
#define MSS_LOG_BLOCK_LIST_FILTERED(callback_func) mss_log_block_list_filtered(__FILE__, MSS_FUNCTION_MACRO, __LINE__, callback_func)
#define MSS_LOG_MSG(m) mss_log_msg(m, __FILE__, MSS_FUNCTION_MACRO, __LINE__)

#define MSS_CHECK_ALL_BLOCKS mss_check_all_blocks(__FILE__, MSS_FUNCTION_MACRO, __LINE__)
#define MSS_CHECK_POINTER_VALIDITY(ptr) mss_check_pointer_validity(ptr, __FILE__, MSS_FUNCTION_MACRO, __LINE__)
#define MSS_CHECK_BLOCK_AT(ptr) mss_check_block(ptr, __FILE__, MSS_FUNCTION_MACRO, __LINE__)

#define MSS_DISABLE_LOG_OUTPUT mss_log_disable(__FILE__, MSS_FUNCTION_MACRO, __LINE__)
#define MSS_ENABLE_LOG_OUTPUT mss_log_enable(__FILE__, MSS_FUNCTION_MACRO, __LINE__)

#define MSS_REGISTER_CONSTANT_BLOCK(ptr) mss_register_constant_block(ptr, __FILE__, MSS_FUNCTION_MACRO, __LINE__)
#define MSS_UNREGISTER_CONSTANT_BLOCK(ptr) mss_unregister_constant_block(ptr, __FILE__, MSS_FUNCTION_MACRO, __LINE__)

#define MSS_CURRENTLY_USED_MEM mss_currently_used_mem()
#define MSS_MAXIMUM_USED_MEM mss_maximum_used_nem()
#define MSS_NUMBER_OF_ALLOCATED_BLOCKS mss_number_of_allocated_blocks()

#define MSS_STARTUP mss_startup()

#define MSS_SET_BLOCK_LABEL(ptr, str) mss_set_block_label(ptr, str, __FILE__, MSS_FUNCTION_MACRO, __LINE__)
#define MSS_DUMP_BLOCK(ptr, filename) mss_dump_block(ptr, filename, __FILE__, MSS_FUNCTION_MACRO, __LINE__)

#else /* #ifndef MSS */
/* We define these macros to nothing if MSS is undefined */
#define MSS_ENTER_SCOPE	;
#define MSS_LEAVE_SCOPE	;

#define MSS_LOG_INFO ;
#define MSS_LOG_INTERNAL_INFO ;
#define MSS_LOG_BLOCK_LIST ;
#define MSS_LOG_BLOCK_LIST_FILTERED(callback_func) ;
#define MSS_LOG_MSG(m) ;

#define MSS_CHECK_ALL_BLOCKS ;
#define MSS_CHECK_POINTER_VALIDITY(ptr) ;
#define MSS_CHECK_BLOCK_AT(ptr) ;

#define MSS_DISABLE_LOG_OUTPUT ;
#define MSS_ENABLE_LOG_OUTPUT ;

#define MSS_REGISTER_CONSTANT_BLOCK(ptr) ;
#define MSS_UNREGISTER_CONSTANT_BLOCK(ptr) ;

#define MSS_CURRENTLY_USED_MEM 0
#define MSS_MAXIMUM_USED_MEM 0
#define MSS_NUMBER_OF_ALLOCATED_BLOCKS ;

#define MSS_STARTUP ;

#define MSS_SET_BLOCK_LABEL(ptr, str) ;
#define MSS_DUMP_BLOCK(ptr, filename) ;


#endif /* !MSS */
#endif /* !__mss_internal__ */

#endif /* !__mss_h__ */

