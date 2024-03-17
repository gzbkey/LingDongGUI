#ifndef _X_LOG_H_
#define _X_LOG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "ldCommon.h"

#define LOG_LEVEL_NONE           (0)
#define LOG_LEVEL_ERROR          (1)
#define LOG_LEVEL_WARNING        (2)
#define LOG_LEVEL_INFO           (3)
#define LOG_LEVEL_DEBUG          (4)

#ifndef USE_LOG_COLOR
#define USE_LOG_COLOR            0
#endif

#ifndef USE_LOG_LEVEL
#define USE_LOG_LEVEL            LOG_LEVEL_DEBUG
#endif

#ifndef LOG_PRINT
#define LOG_PRINT                printf
#endif

#if     USE_LOG_COLOR == 1
#define ANSI_RED                 "\x1b[31m"
#define ANSI_GREEN               "\x1b[32m"
#define ANSI_YELLOW              "\x1b[33m"
#define ANSI_BLUE                "\x1b[34m"
#define ANSI_MAGENTA             "\x1b[35m"
#define ANSI_CYAN                "\x1b[36m"
#define ANSI_WHITE               "\x1b[37m"
#define ANSI_RESET               "\x1b[0m"
#else
#define ANSI_RED
#define ANSI_GREEN
#define ANSI_YELLOW
#define ANSI_BLUE
#define ANSI_MAGENTA
#define ANSI_CYAN
#define ANSI_WHITE
#define ANSI_RESET
#endif

#if (USE_LOG_LEVEL>=LOG_LEVEL_ERROR)
#define LOG_ERROR(fmt, ...)             LOG_PRINT(ANSI_RED"[E] " fmt ANSI_RESET,##__VA_ARGS__)
#else
#define LOG_ERROR(...)                  {}
#endif
#if (USE_LOG_LEVEL>=LOG_LEVEL_WARNING)
#define LOG_WARNING(fmt, ...)           LOG_PRINT(ANSI_YELLOW"[W] " fmt ANSI_RESET,##__VA_ARGS__)
#else
#define LOG_WARNING(...)                {}
#endif
#if (USE_LOG_LEVEL>=LOG_LEVEL_INFO)
#define LOG_INFO(fmt, ...)              LOG_PRINT(ANSI_GREEN"[I] " fmt ANSI_RESET,##__VA_ARGS__)
#define LOG_DIRTY_REGION(fmt, ...)      LOG_PRINT(ANSI_BLUE"[DIRTY REGION] " fmt ANSI_RESET,##__VA_ARGS__)
#else
#define LOG_INFO(...)                   {}
#define LOG_DIRTY_REGION(...)           {}
#endif
#if (USE_LOG_LEVEL>=LOG_LEVEL_DEBUG)
#define LOG_DEBUG(fmt, ...)             LOG_PRINT(ANSI_RESET"[D] " fmt ANSI_RESET,##__VA_ARGS__)
#define LOG_REGION(str,region)          LOG_PRINT("[REGION] %s %d,%d,%d,%d\n",str,(region).tLocation.iX,(region).tLocation.iY,(region).tSize.iWidth,(region).tSize.iHeight);
#define LOG_LOCATION(str,location)      LOG_PRINT("[LOCATION] %s x=%d,y=%d\n",str,(location).iX,(location).iY);
#define LOG_POINT(str,point)            LOG_PRINT("[POINT] %s x=%d,y=%d\n",str,(point).x,(point).y);
#define LOG_XY(str,x,y)                 LOG_PRINT("[XY] %s x=%d,y=%d\n",str,(x),(y));
#else
#define LOG_DEBUG(...)                  {}
#define LOG_REGION(...)                 {}
#define LOG_LOCATION(...)               {}
#define LOG_POINT(...)                  {}
#define LOG_XY(...)                     {}
#endif
















#ifdef __cplusplus
}
#endif

#endif //_X_LOG_H_
