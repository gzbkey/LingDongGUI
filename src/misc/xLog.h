#ifndef _X_LOG_H_
#define _X_LOG_H_

#ifdef __cplusplus
extern "C" {
#endif

#define LOG_LEVEL_NONE           (-1)
#define LOG_LEVEL_ERROR          (0)
#define LOG_LEVEL_WARNING        (1)
#define LOG_LEVEL_INFO           (2)
#define LOG_LEVEL_DEBUG          (3)

#define USE_LOG_COLOR            0

#define SET_LOG_LEVEL            LOG_LEVEL_DEBUG

#define LOG_PRINT                printf

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

#if (SET_LOG_LEVEL>=LOG_LEVEL_ERROR)
#define LOG_ERROR(fmt, ...)      LOG_PRINT(ANSI_RED"[E] " fmt ANSI_RESET,##__VA_ARGS__)
#else
#define LOG_ERROR(...)           {}
#endif
#if (SET_LOG_LEVEL>=LOG_LEVEL_WARNING)
#define LOG_WARNING(fmt, ...)    LOG_PRINT(ANSI_YELLOW"[W] " fmt ANSI_RESET,##__VA_ARGS__)
#else
#define LOG_WARNING(...)         {}
#endif
#if (SET_LOG_LEVEL>=LOG_LEVEL_INFO)
#define LOG_INFO(fmt, ...)       LOG_PRINT(ANSI_GREEN"[I] " fmt ANSI_RESET,##__VA_ARGS__)
#else
#define LOG_INFO(...)            {}
#endif
#if (SET_LOG_LEVEL>=LOG_LEVEL_DEBUG)
#define LOG_DEBUG(fmt, ...)      LOG_PRINT(ANSI_RESET"[D] " fmt ANSI_RESET,##__VA_ARGS__)
#define LOG_REGION(str,region)   LOG_PRINT("[REGION] %s %d,%d,%d,%d\n",str,(region).tLocation.iX,(region).tLocation.iY,(region).tSize.iWidth,(region).tSize.iHeight);
#define LOG_POINT(str,point)     LOG_PRINT("[POINT] %s x=%d,y=%d\n",str,(point).x,(point).y);
#define LOG_XY(str,x,y)          LOG_PRINT("[XY] %s x=%d,y=%d\n",str,(x),(y));
#else
#define LOG_DEBUG(...)           {}
#define LOG_REGION(...)          {}
#define LOG_POINT(...)           {}
#define LOG_XY(...)              {}
#endif
















#ifdef __cplusplus
}
#endif

#endif //_X_LOG_H_
