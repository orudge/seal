#ifdef __cplusplus
   extern "C" {
#endif

#if SEAL_PLATFORM_DOS == 1
   typedef  int                l_bool;
   typedef  long               l_long;
   typedef  int                l_int;
   typedef  char               l_char;
   typedef  long long          l_big;
   typedef  unsigned char      l_byte,  l_uchar;
   typedef  unsigned int       l_word,  l_uint;
   typedef  unsigned long      l_dword, l_ulong;
   typedef  unsigned long long l_ubig;
   typedef  char               l_tag[32];
   typedef  long               l_rect;
   typedef  int                l_color;
   typedef  char*              l_text;
   typedef  void*              l_ptr;
#else
   #error This file is only applicable to the DOS version of Seal 3.0.
#endif

#ifdef __cplusplus
   }
#endif
