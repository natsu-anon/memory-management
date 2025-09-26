#ifndef MAP_WRAP_H
#define MAP_WRAP_H

// wrapped functions for platform specific memory-mapping

#include <stddef.h>
#include <stdio.h>
#include <stdint.h>

#if _WIN32
typedef void* mw_handle_t; // jfc microsoft, a HANDLE
#else
typedef int mw_handle_t; // a filedesc for *NIX systems
#endif

void* mw_mmap(void* addr, size_t len);

int mw_munmap(void* addr, size_t len);

void* mw_mmap_file(const char* filename, size_t len, mw_handle_t* out_handle);

void* mw_mmap_existing_file(const char* filename, int32_t* out_len, mw_handle_t* out_handle);

int mw_munmap_file(void* addr, size_t len, mw_handle_t handle);


#endif /* End MAP_WRAP_H */
