#ifdef __cplusplus
   extern "C" {
#endif

#if SEAL_PLATFORM_DOS == 1
   #if SEAL_KERNEL == 1
      #define ___SXL_HOST___
   #endif

   #ifndef __SXL_H_INCLUDED__
      #include <platform/dos/sxl.h>
   #endif

   typedef h_sxl               p_execfile_hndl;
   #define seal_lib_handle     sxl_handle

   #define SEAL_EXPORT_GLOBAL(x)             SXL_REGISTER(x)
   #define SEAL_EXPORT_GLOBAL_AS(x,y)        SXL_REGISTER_AS(x,y)

   #define SEAL_KERNEL_EXPORT(x)             SXL_HOST_REGISTER(x)
   #define SEAL_KERNEL_EXPORT_AS(x,y)        SXL_HOST_REGISTER(x)

   #define SEAL_EXPORT_PRIVATE(x)            SXL_EXPORT(x)
   #define SEAL_EXPORT_PRIVATE_AS(x,y)       SXL_EXPORT_AS(x,y)
#else
   #error This file is only applicable to the DOS version of Seal 3.0.
#endif

#ifdef __cplusplus
   }
#endif
