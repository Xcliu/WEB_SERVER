#pragma once

#define code_position_info() \
    printf("\n******\nfunction: %s() in file: %s (line: %d)\n******\n", \
             __func__ , __FILE__, __LINE__ );

#define systemcall_error_info() \
    fprintf(stderr, "*** ERROR %s, failed:%d(%s)\n", \
            __func__,errno, strerror(errno));

#define error_exit() exit(EXIT_FAILURE)
