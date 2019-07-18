//
// Created by Ivan Kishchenko (Lazada Group) on 2019-07-15.
//

#include <cstddef>
#include "SysInfo.h"

#include <limits>

#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/sysctl.h>

template <typename T, size_t N>
char (&ArraySizeHelper(const T (&array)[N]))[N];

#define arraysize(array) (sizeof(ArraySizeHelper(array)))

// static
int SysInfo::NumberOfProcessors() {
#if V8_OS_OPENBSD
    int mib[2] = {CTL_HW, HW_NCPU};
  int ncpu = 0;
  size_t len = sizeof(ncpu);
  if (sysctl(mib, arraysize(mib), &ncpu, &len, NULL, 0) != 0) {
    return 1;
  }
  return ncpu;
#elif V8_OS_POSIX
    long result = sysconf(_SC_NPROCESSORS_ONLN);  // NOLINT(runtime/int)
  if (result == -1) {
    return 1;
  }
  return static_cast<int>(result);
#elif V8_OS_WIN
    SYSTEM_INFO system_info = {};
  ::GetNativeSystemInfo(&system_info);
  return static_cast<int>(system_info.dwNumberOfProcessors);
#endif
}


// static
int64_t SysInfo::AmountOfPhysicalMemory() {
#ifdef V8_OS_MACOSX
    int mib[2] = {CTL_HW, HW_MEMSIZE};
    int64_t memsize = 0;
    size_t len = sizeof(memsize);
    if (sysctl(mib, arraysize(mib), &memsize, &len, nullptr, 0) != 0) {
        return 0;
    }
    return memsize;
#elif V8_OS_FREEBSD
    int pages, page_size;
  size_t size = sizeof(pages);
  sysctlbyname("vm.stats.vm.v_page_count", &pages, &size, NULL, 0);
  sysctlbyname("vm.stats.vm.v_page_size", &page_size, &size, NULL, 0);
  if (pages == -1 || page_size == -1) {
    return 0;
  }
  return static_cast<int64_t>(pages) * page_size;
#elif V8_OS_CYGWIN || V8_OS_WIN
    MEMORYSTATUSEX memory_info;
  memory_info.dwLength = sizeof(memory_info);
  if (!GlobalMemoryStatusEx(&memory_info)) {
    return 0;
  }
  int64_t result = static_cast<int64_t>(memory_info.ullTotalPhys);
  if (result < 0) result = std::numeric_limits<int64_t>::max();
  return result;
#elif V8_OS_QNX
    struct stat stat_buf;
  if (stat("/proc", &stat_buf) != 0) {
    return 0;
  }
  return static_cast<int64_t>(stat_buf.st_size);
#elif V8_OS_AIX
    int64_t result = sysconf(_SC_AIX_REALMEM);
  return static_cast<int64_t>(result) * 1024L;
#elif V8_OS_POSIX
    long pages = sysconf(_SC_PHYS_PAGES);    // NOLINT(runtime/int)
  long page_size = sysconf(_SC_PAGESIZE);  // NOLINT(runtime/int)
  if (pages == -1 || page_size == -1) {
    return 0;
  }
  return static_cast<int64_t>(pages) * page_size;
#endif
}


// static
int64_t SysInfo::AmountOfVirtualMemory() {
#if V8_OS_WIN
    return 0;
#elif V8_OS_POSIX
    struct rlimit rlim;
  int result = getrlimit(RLIMIT_DATA, &rlim);
  if (result != 0) {
    return 0;
  }
  return (rlim.rlim_cur == RLIM_INFINITY) ? 0 : rlim.rlim_cur;
#endif
}
