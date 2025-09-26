#include "map_wrap.h"
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <stdint.h>
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <memoryapi.h>
#else
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#endif

// TODO: thorough windows testing

// TODO: ERRNO. lol. lmao even.


#ifdef _WIN32
void* mw_mmap(void* addr, size_t len)
{
	// see: https://learn.microsoft.com/en-us/windows/win32/memory/allocating-virtual-memory
	// eg: https://learn.microsoft.com/en-us/windows/win32/Memory/reserving-and-committing-memory
	void* ptr = VirtualAlloc(NULL, len, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	assert(ptr != NULL && "VirtualAlloc failed!");
	return NULL;
}
#else
void* mw_mmap(void* addr, size_t len)
{
	void* ptr = mmap(addr, len, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	assert(ptr != MAP_FAILED &&  "mmap() failed!\n");
	return ptr;
}
#endif

#ifdef _WIN32
int mw_munmap(void* addr, size_t len)
{
	return VirtualFree(addr, 0, MEM_RELEASE);
}
#else
int mw_munmap(void* addr, size_t len)
{
	return munmap(addr, len);
}
#endif

#ifdef _WIN32
void* mw_mmap_file(const char* filename, size_t len, mw_handle_t* out_handle)
{
	// see: https://learn.microsoft.com/en-us/windows/win32/memory/file-mapping
	HANDLE h_file = CreateFileA(filename,
								GENERIC_READ | GENERIC_WRITE,
								FILE_SHARE_READ | FILE_SHARE_WRITE,
								NULL,
								OPEN_ALWAYS,
								FILE_ATTRIBUTE_NORMAL,
								NULL);
	assert(h_file != INVALID_HANDLE_VALUE && "CreateFileA failed\n");
	HANDLE file_map = CreateFileMappingA(h_file, NULL, PAGE_READWRITE, 0, len, NULL);
	assert(file_map != NULL && "CreateFileMappingA failed");
	// see: https://learn.microsoft.com/en-us/windows/win32/memory/creating-a-file-view
	void* res = MapViewOfFile(file_map, FILE_MAP_ALL_ACCESS, 0, 0, len);
	assert(res != NULL && "MapViewOfFile of failed");
	*out_handle = file_map;
	return res;
}
#else
void* mw_mmap_file(const char* filename, size_t len, mw_handle_t* out_handle)
{
	int filedes = open(filename, O_RDWR | O_CREAT | O_APPEND, 0400 | 0200);
	assert(filedes != -1 && "failed to get fd in ms_map_file().  Check the perms!\n");
	assert(ftruncate(filedes, len) != -1 && "failed to truncate file");
	*out_handle = filedes;
	return mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, filedes, 0);
}
#endif

#ifdef _WIN32
void* mw_mmap_existing_file(const char* filename, int32_t* out_len, mw_handle_t* out_handle)
{
	assert(0 && "SORRY! I DIDNT BOTHER WITH THIS PART!  YOU SHOULD BE ABLE TO IF YOU LOOK AT THE PREVIOUS FUNC");
}
#else
void* mw_mmap_existing_file(const char* filename, int32_t* out_len, mw_handle_t* out_handle)
{
	int filedesc = open(filename, O_RDWR, 0400 | 0200);
	assert(filedesc != -1 && "failed to get filedesc in mw_map_entire_file().  Check the perms!\n");
	const int32_t len = lseek(filedesc, 0, SEEK_END);
	*out_len = len;
	*out_handle = filedesc;
	return mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, filedesc, 0);
}
#endif


#ifdef _WIN32
int mw_munmap_file(void* addr, size_t len, mw_handle_t handle)
{
	BOOL res = UnmapViewOfFile(addr);
	res &= CloseHandle(handle);
	return 1 - res;
}
#else
int mw_munmap_file(void* addr, size_t len, mw_handle_t handle)
{
	int res = munmap(addr, len);
	/*
       The  mmap() function shall add an extra reference to the file associated
       with the file descriptor fildes which is not  removed  by  a  subsequent
       close()  on  that  file descriptor. This reference shall be removed when
       there are no more mappings to the file.

	   So, should I close the handle or not?
	 */
	assert(close(handle) == 0 && "failed to close filehandle");
	return res;
#endif
}
