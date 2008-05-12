#ifdef __cplusplus
   extern "C" {
#endif

#if SEAL_PLATFORM_DOS == 1
   #include <platform/dos/execfile.h>
#endif

l_int seal_sys_execfile_init();
l_int seal_sys_execfile_term();
p_execfile_hndl seal_sys_execfile_load(l_text filename, l_text arguments);
l_ptr seal_sys_execfile_find_symbol(p_execfile_hndl handle, l_text name);
void seal_sys_execfile_unload(p_execfile_hndl handle);

#ifdef __cplusplus
   }
#endif
