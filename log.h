/**
 * Copyright (c) 2021-2022, Haier
 *
 * log util for unit test.
 *
 * Change Logs:
 * Date             Author              Notes
 * 2022-01-28       YangZhikang         first version
 */

#ifndef LOG_H
#define LOG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/time.h>
#include <stddef.h>
#include <stdint.h>

/*--- Configurations -------------------------------------------------------------------------------*/

/* 全局日志输出级别 */
#ifndef LOG_GLOBAL_OUTPUT_LVL
#define LOG_GLOBAL_OUTPUT_LVL           LOG_LVL_VERBOSE
#endif

/* 日志输出函数实现 */
#include <stdio.h>
#define log_printf(log_level, fmt, ...) printf(fmt, ##__VA_ARGS__)
#define log_output(log_level, fmt, ...) if (log_level <= LOG_GLOBAL_OUTPUT_LVL) log_printf(log_level, fmt, ##__VA_ARGS__)

/* 日志颜色输出使能 */
#ifndef LOG_COLOR_ENABLE
#define LOG_COLOR_ENABLE
#endif


/*--- Common Definitions ---------------------------------------------------------------------------*/

/* output log's level */
#define LOG_LVL_ASSERT                  0
#define LOG_LVL_ERROR                   1
#define LOG_LVL_WARN                    2
#define LOG_LVL_INFO                    3
#define LOG_LVL_DEBUG                   4
#define LOG_LVL_VERBOSE                 5

#ifdef LOG_COLOR_ENABLE
/**
 * CSI(Control Sequence Introducer/Initiator) sign
 * more information on https://en.wikipedia.org/wiki/ANSI_escape_code
 */
#define CSI_START                       "\033["
#define CSI_END                         "\033[0m"
/* output log front color */
#define F_BLACK                         "30;"
#define F_RED                           "31;"
#define F_GREEN                         "32;"
#define F_YELLOW                        "33;"
#define F_BLUE                          "34;"
#define F_MAGENTA                       "35;"
#define F_CYAN                          "36;"
#define F_WHITE                         "37;"
/* output log background color */
#define B_NULL
#define B_BLACK                         "40;"
#define B_RED                           "41;"
#define B_GREEN                         "42;"
#define B_YELLOW                        "43;"
#define B_BLUE                          "44;"
#define B_MAGENTA                       "45;"
#define B_CYAN                          "46;"
#define B_WHITE                         "47;"
/* output log fonts style */
#define S_BOLD                          "1m"
#define S_UNDERLINE                     "4m"
#define S_BLINK                         "5m"
#define S_NORMAL                        "22m"
/* output log default color definition: [front color] + [background color] + [show style] */
#ifndef LOG_COLOR_ASSERT
#define LOG_COLOR_ASSERT                F_MAGENTA B_NULL S_NORMAL
#endif
#ifndef LOG_COLOR_ERROR
#define LOG_COLOR_ERROR                 F_RED B_NULL S_NORMAL
#endif
#ifndef LOG_COLOR_WARN
#define LOG_COLOR_WARN                  F_YELLOW B_NULL S_NORMAL
#endif
#ifndef LOG_COLOR_INFO
#define LOG_COLOR_INFO                  F_GREEN B_NULL S_NORMAL
#endif
#ifndef LOG_COLOR_DEBUG
#define LOG_COLOR_DEBUG                 S_NORMAL
#endif
#ifndef LOG_COLOR_VERBOSE
#define LOG_COLOR_VERBOSE               S_NORMAL
#endif

/* for test_assert */
#ifndef LOG_COLOR_TEST_ASSERT_FAILED
#define LOG_COLOR_TEST_ASSERT_FAILED    F_MAGENTA B_NULL S_BOLD
#endif
#ifndef LOG_COLOR_TEST_ASSERT_PASSED
#define LOG_COLOR_TEST_ASSERT_PASSED    F_GREEN B_NULL S_NORMAL
#endif

#define COLOR_START(log_level, color)   log_output(log_level, CSI_START color)
#define COLOR_END(log_level)            log_output(log_level, CSI_END)

#else
#define COLOR_START(log_level, color)   ((void)0)
#define COLOR_END(log_level)            ((void)0)

#endif /* LOG_COLOR_ENABLE */

static inline int64_t log_get_timestamp(void)
{
    struct timeval tv;
    int ret = gettimeofday(&tv, NULL);
    if (ret < 0)
        return ret;
    return ((int64_t)tv.tv_sec * 1000 + tv.tv_usec / 1000);
}


/*--- Log Functions --------------------------------------------------------------------------------*/

/**
 * log API short definition
 * NOTE: The `LOG_TAG` and `LOG_LVL` must defined before including the <log.h> when you want to use log_x API.
 */
#if !defined(LOG_TAG)
    #define LOG_TAG                     ""
#endif
#if !defined(LOG_LVL)
    #define LOG_LVL                     LOG_LVL_VERBOSE
#endif

#undef log_a
#undef log_e
#undef log_w
#undef log_i
#undef log_d
#undef log_v
#undef log_hex
#undef log_raw
#undef assert

#define log_common(log_level, color, tag, fmt, ...)  { COLOR_START(log_level, color); log_output(log_level, tag "[%lld] [func:%s] " fmt, (long long)log_get_timestamp(), __FUNCTION__, ##__VA_ARGS__); COLOR_END(log_level); log_output(log_level, "\n"); }

#if LOG_LVL >= LOG_LVL_ASSERT
    #define log_a(fmt, ...)             log_common(LOG_LVL_ASSERT, LOG_COLOR_ASSERT, "[A] " "[" LOG_TAG "] ", fmt, ##__VA_ARGS__)
#else
    #define log_a(fmt, ...)             ((void)0)
#endif
#if LOG_LVL >= LOG_LVL_ERROR
    #define log_e(fmt, ...)             log_common(LOG_LVL_ERROR, LOG_COLOR_ERROR, "[E] " "[" LOG_TAG "] ", fmt, ##__VA_ARGS__)
#else
    #define log_e(fmt, ...)             ((void)0)
#endif
#if LOG_LVL >= LOG_LVL_WARN
    #define log_w(fmt, ...)             log_common(LOG_LVL_WARN, LOG_COLOR_WARN, "[W] " "[" LOG_TAG "] ", fmt, ##__VA_ARGS__)
#else
    #define log_w(fmt, ...)             ((void)0)
#endif
#if LOG_LVL >= LOG_LVL_INFO
    #define log_i(fmt, ...)             log_common(LOG_LVL_INFO, LOG_COLOR_INFO, "[I] " "[" LOG_TAG "] ", fmt, ##__VA_ARGS__)
#else
    #define log_i(fmt, ...)             ((void)0)
#endif
#if LOG_LVL >= LOG_LVL_DEBUG
    #define log_d(fmt, ...)             log_common(LOG_LVL_DEBUG, LOG_COLOR_DEBUG, "[D] " "[" LOG_TAG "] ", fmt, ##__VA_ARGS__)
#else
    #define log_d(fmt, ...)             ((void)0)
#endif
#if LOG_LVL >= LOG_LVL_VERBOSE
    #define log_v(fmt, ...)             log_common(LOG_LVL_VERBOSE, LOG_COLOR_VERBOSE, "[V] " "[" LOG_TAG "] ", fmt, ##__VA_ARGS__)
#else
    #define log_v(fmt, ...)             ((void)0)
#endif

// #if LOG_LVL >= LOG_LVL_DEBUG
//     #define log_hex(name, width, buf, size)     elog_hexdump(name, width, buf, size)
// #else
    #define log_hex(name, width, buf, size)     ((void)0)
// #endif

#define log_raw(fmt, ...)               log_output(LOG_LVL_DEBUG, fmt, ##__VA_ARGS__)

/* assert definition general purpose */
#define assert(expr)                                                        \
    if (!(expr))                                                            \
    {                                                                       \
        log_a("(%s) assert failed at %s:%ld.", #expr, __FUNCTION__, __LINE__); \
        while (1);                                                          \
    }

/* assert definition for unit test */
#define test_assert(expr)                                                   \
    if (!(expr))                                                            \
    {                                                                       \
        COLOR_START(LOG_LVL_ASSERT, LOG_COLOR_TEST_ASSERT_FAILED);          \
        log_output(LOG_LVL_ASSERT, "[Failed] (%s) assert failed at %s:%d.\n", #expr, __FUNCTION__, __LINE__); \
        COLOR_END(LOG_LVL_ASSERT);                                          \
    }                                                                       \
    else                                                                    \
    {                                                                       \
        COLOR_START(LOG_LVL_ASSERT, LOG_COLOR_TEST_ASSERT_PASSED);          \
        log_output(LOG_LVL_ASSERT, "[Passed] (%s) assert passed at %s:%d.\n", #expr, __FUNCTION__, __LINE__); \
        COLOR_END(LOG_LVL_ASSERT);                                          \
    }


#define log_func()                      log_d("function: %s()", __FUNCTION__)


#ifdef __cplusplus
}   /* extern "C" */
#endif

#endif  /* LOG_H */
