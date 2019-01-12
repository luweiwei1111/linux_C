#include <stdio.h>
#include "zlog_api.h"

zlog_category_t * log_category = NULL;

int log_init() {
    //初始化.配置文件名是固定的log.conf
    if (zlog_init("zlog.conf"))  {
        printf("Error: zlog_init\n");
    zlog_fini();
        return -1;
    }
    //找到分类,在配置文件中的category
    log_category = zlog_get_category("my_cat");
    if (!log_category) {
    printf("Error: get cat fail\n");
    zlog_fini();
    return -2;
    }
    return 0 ;
}

void log_fini() {
    zlog_fini();
}