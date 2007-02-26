

#define INCL_RXSHV      /* Shared variable support */
#define INCL_RXFUNC     /* External functions support */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "rexxsaa.h"

#ifdef _MSC_VER
/* This picky compiler claims about unused formal parameters.
 * This is correct but hides (for human eyes) other errors since they
 * are many and we can't reduce them all.
 * Error 4100 is "unused formal parameter".
 */
# pragma warning(disable:4100)
#endif

#define DLLNAME "test1"


#define FUNCTION1 Test1Function1
#define FUNCTION2 Test1Function2
#define LOADFUNCS Test1LoadFuncs
#define DROPFUNCS Test1DropFuncs

#define NAME_FUNCTION1 "Test1Function1"
#define NAME_FUNCTION2 "Test1Function2"
#define NAME_LOADFUNCS "Test1LoadFuncs"
#define NAME_DROPFUNCS "Test1DropFuncs"

RexxFunctionHandler Test1Function1;
RexxFunctionHandler Test1Function2;
RexxFunctionHandler Test1LoadFuncs;
RexxFunctionHandler Test1DropFuncs;

/*-----------------------------------------------------------------------------
 * Table entry for a REXX/SQL function.
 *----------------------------------------------------------------------------*/
typedef struct {
   PSZ   function_name;
   PFN   EntryPoint;
} RexxFunction;

/*-----------------------------------------------------------------------------
 * Table of REXX/SQL Functions. Used to install/de-install functions.
 *----------------------------------------------------------------------------*/
static const RexxFunction RexxSqlFunctions[] = {
   {(PSZ)NAME_FUNCTION1,   (PFN)Test1Function1  },
   {(PSZ)NAME_FUNCTION2,   (PFN)Test1Function2  },
   {(PSZ)NAME_DROPFUNCS,   (PFN)Test1DropFuncs  },
   {(PSZ)NAME_LOADFUNCS,   (PFN)Test1LoadFuncs  },
   {NULL,NULL}
};

static void static_show_parameter(ULONG argc, RXSTRING argv[], PSZ func_name)
{
   char buf[100];
   if (argc == 0)
   {
      printf("%s(static): *** No parameters passed ***\n",DLLNAME);
      return;
   }
   memcpy(buf,argv[0].strptr,argv[0].strlength);
   buf[argv[0].strlength] = '\0';
   if (strcmp(func_name,buf) != 0)
      printf("%s(static): *** Mismatch of parameters: %s is NOT expected: %s ***\n",
             DLLNAME,buf,func_name);
 return;
}

#if defined(DYNAMIC_STATIC)
static void global_show_parameter(ULONG argc, RXSTRING argv[], PSZ func_name)
#else
void global_show_parameter(ULONG argc, RXSTRING argv[], PSZ func_name)
#endif
{
   char buf[100];
   if (argc == 0)
   {
      printf("%s(global): *** No parameters passed ***\n",DLLNAME);
      return;
   }
   memcpy(buf,argv[0].strptr,argv[0].strlength);
   buf[argv[0].strlength] = '\0';
   if (strcmp(func_name,buf) != 0)
      printf("%s(global): *** Mismatch of parameters: %s is NOT expected: %s ***\n",
             DLLNAME,buf,func_name);
   return;
}

APIRET APIENTRY FUNCTION1(PCSZ name,ULONG argc,PRXSTRING argv,PCSZ stck,PRXSTRING retstr)
{
   int i=0;
   for (i=0;i<(int) argc;i++)
      printf("%s(Test1Function1): Arg: %d <%s>\n",DLLNAME,i,argv[i].strptr);
   static_show_parameter(argc,argv,NAME_FUNCTION1);
   global_show_parameter(argc,argv,NAME_FUNCTION1);
   strcpy(retstr->strptr,"0");
   retstr->strlength = 1;
   return 0L;
}

APIRET APIENTRY FUNCTION2(PCSZ name,ULONG argc,PRXSTRING argv,PCSZ stck,PRXSTRING retstr)
{
   int i=0;
   for (i=0;i<(int) argc;i++)
      printf("%s(Test1Function2): Arg: %d <%s>\n",DLLNAME,i,argv[i].strptr);
   static_show_parameter(argc,argv,NAME_FUNCTION2);
   global_show_parameter(argc,argv,NAME_FUNCTION2);
   strcpy(retstr->strptr,"0");
   retstr->strlength = 1;
   return 0L;
}


APIRET APIENTRY DROPFUNCS(PCSZ name,ULONG argc,PRXSTRING argv,PCSZ stck,PRXSTRING retstr)
{
   int rc=0;
   const RexxFunction  *func=NULL;

   /* DeRegister all REXX/SQL functions */
   for (func = RexxSqlFunctions; func->function_name; func++)
      rc = RexxDeregisterFunction(func->function_name);
   sprintf(retstr->strptr,"%d",rc);
   retstr->strlength = strlen(retstr->strptr);
   return 0L;
}


/*-----------------------------------------------------------------------------
 * This function is called to initiate REXX/SQL interface.
 *----------------------------------------------------------------------------*/
static int InitRexxSQL(PSZ progname)
{
   const RexxFunction  *func=NULL;
   ULONG rc=0L;

   /* Register all REXX/SQL functions */
   for (func = RexxSqlFunctions; func->function_name; func++)
      rc = RexxRegisterFunctionDll(func->function_name,DLLNAME,func->function_name);

   return 0;
}

APIRET APIENTRY LOADFUNCS(PCSZ name,ULONG argc,PRXSTRING argv,PCSZ stck,PRXSTRING retstr)
{
   int rc=0;

   rc = InitRexxSQL(DLLNAME);
   sprintf(retstr->strptr,"%d",rc);
   retstr->strlength = strlen(retstr->strptr);
   return 0L;
}

#if defined( DYNAMIC_STATIC )
void *getTest1FunctionAddress( char *name )
{
   const RexxFunction  *func=NULL;
   for (func = RexxSqlFunctions; func->function_name; func++)
   {
      if ( strcmp( func->function_name, name) == 0 )
         return func->EntryPoint;
   }
   return NULL;
}
#endif
