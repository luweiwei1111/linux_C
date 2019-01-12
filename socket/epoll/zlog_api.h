#include "zlog.h"

#define ESC_START       "\033["
#define ESC_END         "\033[0m"
#define COLOR_FATAL     "31;40;5m"
#define COLOR_ALERT     "31;40;1m"
#define COLOR_CRIT      "31;40;1m"
#define COLOR_ERROR     "35;40;1m"
#define COLOR_WARN      "33;40;1m"
#define COLOR_NOTICE    "34;40;1m"
#define COLOR_INFO      "32;40;1m"
#define COLOR_DEBUG     "36;40;1m"
#define COLOR_TRACE     "37;40;1m"

extern zlog_category_t * log_category;
extern int log_init();
extern void log_fini();

#define LOG_FATAL(fmt,args...)         \
    zlog(log_category, __FILE__, sizeof(__FILE__)-1, \
    __func__, sizeof(__func__)-1, __LINE__, \
    ZLOG_LEVEL_FATAL, ESC_START COLOR_FATAL fmt ESC_END, ##args)

    
#define LOG_ERROR(fmt , args...)    \
    zlog(log_category, __FILE__, sizeof(__FILE__)-1, \
    __func__, sizeof(__func__)-1, __LINE__, \
    ZLOG_LEVEL_ERROR, ESC_START COLOR_ERROR fmt ESC_END, ##args)
    
#define LOG_WARN(fmt, args...)        \
    zlog(log_category, __FILE__, sizeof(__FILE__)-1, \
    __func__, sizeof(__func__)-1, __LINE__, \
    ZLOG_LEVEL_WARN, ESC_START COLOR_WARN fmt ESC_END, ##args)
    
#define LOG_NOTICE(fmt , args...)    \
    zlog(log_category, __FILE__, sizeof(__FILE__)-1, \
    __func__, sizeof(__func__)-1, __LINE__, \
    ZLOG_LEVEL_NOTICE, ESC_START COLOR_NOTICE fmt ESC_END, ##args)
    
#define LOG_INFO(fmt,args...)         \
    zlog(log_category, __FILE__, sizeof(__FILE__)-1, \
    __func__, sizeof(__func__)-1, __LINE__, \
    ZLOG_LEVEL_INFO, ESC_START COLOR_INFO fmt ESC_END, ##args)
    
#define LOG_DEBUG(fmt , args...)    \
    zlog(log_category, __FILE__, sizeof(__FILE__)-1, \
    __func__, sizeof(__func__)-1, __LINE__, \
    ZLOG_LEVEL_DEBUG, ESC_START COLOR_DEBUG fmt ESC_END, ##args)
