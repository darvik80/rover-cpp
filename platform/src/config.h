//
// Created by Ivan Kishchenko (Lazada Group) on 2019-07-18.
//

#ifndef ROVER_CONFIG_H
#define ROVER_CONFIG_H

#if defined(__APPLE__)
# define CFG_OS_BSD 1
# define CFG_OS_MACOSX 1
# define CFG_OS_POSIX 1
#elif defined(__linux__)
# define CFG_OS_LINUX 1
# define CFG_OS_POSIX 1
#endif

#endif //ROVER_CONFIG_H
