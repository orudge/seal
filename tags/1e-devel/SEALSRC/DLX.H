/*
   DLX Dynamic Loading and eXecution V3.0
   Copyright (c) 1997-1998, Nanosoft, Inc.
*/


#ifndef __DLX_INCLUDED_3__
#define __DLX_INCLUDED_3__

struct dlxheader {

        long magic;
        long numimports;
        long numrelocs;
        long libmainpos;
        long extablepos;
        long libloadpos;
        long prgsize;

};


typedef long long hdlx_t;

#define l_hdlx hdlx_t

struct dlxiddesc {  /* applies only to V2.0 DLX */

        hdlx_t MFID; /* manufacturer ID */
        hdlx_t PRID; /* product ID */
        hdlx_t VTBL; /* version description ID */
        hdlx_t UNID; /* unique DLX ID */

};


extern void(*dlx_first_ctor[])() __asm__("dlx_first_ctor");
extern void(*dlx_last_ctor[])() __asm__("dlx_last_ctor");
extern void(*dlx_first_dtor[])() __asm__("dlx_first_dtor");
extern void(*dlx_last_dtor[])() __asm__("dlx_last_dtor");


#define DLX_MAGIC          0x584c44
#define DLX2_MAGIC         0x32584c44
#define DLX3_MAGIC         0x33584c44
#define DLX_BUILD_DOS      0x534f444d4249LL

#ifdef __cplusplus

        #define DLXUSE_BEGIN extern "C" {
        #define DLXUSE_END }
        #define DLX_FN extern "C" {
        #define DLX_EF }
        #define DLX_IMPORT extern "C" {
        #define DLX_ENDIMPORT }

#else
        #define DLXUSE_BEGIN
        #define DLXUSE_END
        #define DLX_FN 
        #define DLX_EF 
        #define DLX_IMPORT 
        #define DLX_ENDIMPORT
        
#endif

#define LIBINFO_BEGIN
#define LIBINFO_END
#define LIBLOADS_BEGIN char* _LIBTOLOAD[]={
#define LIBLOADS_END "\0\0"};
#define LIBEXPORT_BEGIN   char* _LIBEXPORTTABLE[]= {
#define LIBEXPORT_END 0, 0 };
#define LIBEXPORTCLASS(x) "_"#x, (char *)((FUNCTYPE)(x)),
#define LIBEXPORT(x) "_"#x, (char *)(&x),
#define LIBALIAS(x, y) "_"#x, (char *)((FUNCTYPE)y),
#define LIBENTRY(x) (char*)0L, (char *)1L, #x, (char *)&x,
#define LIBWEAK(x) (char*)0L, (char *)2L, "_"#x, (char *)&x,
#define LIBLOAD(x) #x,
#define LIBCONSTRUCT  do {                                            \
                        long i=0;                                     \
                        for (i=0; i<dlx_last_ctor-dlx_first_ctor; i++)\
                        dlx_first_ctor[i]();                          \
                      } while (0);
#define LIBDESTRUCT   do {                                            \
                        long i=0;                                     \
                        for (i=0; i<dlx_last_dtor-dlx_first_dtor; i++)\
                        dlx_first_dtor[i]();                          \
                      } while(0);
#define LIBVERSION_BEGIN long long _DLXVERSIONTABL[]={
#define LIBVERSION_END 0,0,0,0};
#define LIBVERSION(x) x##LL,
#define LIBMYHANDLE _DLXVERSIONTABL[3]
#define LIBMCLASS(x) long _DLXMEMORYCLASS=x;
#define LIBDPLID(x) long long _DLXDPLCLASS=x;


extern void* (*DLXOpenFile)(char*);
extern void  (*DLXCloseFile)(void*);
extern long  (*DLXReadFile)(void*,long,void*);

extern void   (*DLXError)(long,char*);
extern hdlx_t (*DLXGetID)(char*);

extern void* (*DLXCodeAlloc)(unsigned long, long mclas);
extern void* (*DLXMalloc)(unsigned long);
extern void  (*DLXFree)(void*);
extern void* (*DLXRealloc)(void*,unsigned long);

extern long  (*DLXGetLMaxInstance)(hdlx_t n); /* my */
extern long  (*DLXGetLInstance)(hdlx_t n); /* my */
extern int   (*DLXGetLOverload)(hdlx_t n); /* my */
extern void  (*DLXSetLMaxInstance)(hdlx_t n, long i); /* my */
extern void  (*DLXSetLOverload)(hdlx_t n, int i); /* my */
extern void* (*DLXGetFileData)(hdlx_t n); /* my */

extern hdlx_t (*DLXLoad)(char* name, char *args);
extern int    (*DLXUnload)(hdlx_t handle);
extern int    (*DLXUnloadEx)(char *name);
extern int    (*DLXImport)(char** symbols);
extern void*  (*DLXGetEntry)(hdlx_t target, char* name);
extern void*  (*DLXGetMemoryBlock)(hdlx_t target);
extern long   (*DLXGetMemoryBlockLength)(hdlx_t target);
extern void*  (*DLXGetResource)(hdlx_t target);
extern long   (*DLXGetResourceLength)(hdlx_t target);

#ifndef __MAIN_INCLUDED__
static hdlx_t ap_id = 0; /* id for dlx aplication */
#endif

#define AP_ALLOC   0
#define AP_INIT    2
#define AP_DONE    3
#define AP_FREE    1

#define AP_CALL             DLXGetLInstance(ap_id)
#define AP_SETNUMOFCALLS(x) DLXSetLMaxInstance(ap_id, x)
#define AP_SETOVERLOAD(x)   DLXSetLOverload(ap_id, x)
#define AP_ISOVERLOAD       DLXGetLOverload(ap_id)
#define AP_DATAFILE         DLXGetFileData(ap_id)
#define AP_EXPORTLIB()      DLXImport(_LIBEXPORTTABLE)


#define AP_RET
#define AP_END } DLX_EF

#define app_end                             }
#define app_main ap_begin
#define app_begin(_x)                       \
                                             \
lib_exportable                                \
                                              \
DLX_FN void LibMain(int ap_process, hdlx_t id, char* ap_args) {     \
    if ( ap_process == AP_ALLOC ) {                  \
      LIBCONSTRUCT                            \
      ap_id = id;                             \
    }                                         \
    if ( ap_process == AP_FREE ) {                  \
      LIBDESTRUCT                             \
    }



#define lib_exportable                       \
DLXUSE_BEGIN                                 \
  LIBLOADS_BEGIN                             \
  LIBLOADS_END                               \
  LIBEXPORT_BEGIN                            \
  LIBEXPORT_END                              \
DLXUSE_END                                   \


#define lib_end                              }
#define lib_main lib_begin

#define lib_begin(_x)                        \
                                             \
                                             \
DLX_FN void LibMain(int ap_process, hdlx_t id, char* ap_args) {     \
    if ( ap_process == AP_ALLOC ) {                  \
      LIBCONSTRUCT                            \
      ap_id = id;                             \
    }                                         \
    if ( ap_process == AP_FREE ) {                  \
      LIBDESTRUCT                             \
    }


#endif

