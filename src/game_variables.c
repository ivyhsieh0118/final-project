#include "game_variables.h"

void init_log() {
    char command[1024];
    sprintf(command, "touch %s", LOG_FILENAME);
    printf("%s", command);
    system(command);
}

void log_record(const char *format, ...) {
    FILE *logfile = fopen(LOG_FILENAME, "a");
    if (!logfile) return;

    va_list args;
    va_start(args, format);

    // 获取当前时间
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    char time_buf[20];
    strftime(time_buf, 20, "%Y-%m-%d %H:%M:%S", tm_info);

    // 写入时间和错误消息
    fprintf(logfile, "[%s] ", time_buf);
    vfprintf(logfile, format, args);
    fprintf(logfile, "\n");

    va_end(args);
    fclose(logfile);
}