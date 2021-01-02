#pragma once
/*
used for macro system_error_info
*/
#include <stdio.h>
#include <sys/errno.h>
#include <string.h>
#define systemcall_error_info() fprintf(stderr, "*** ERROR %s,%d, failed:%d(%s)\n", __func__,__LINE__,errno, strerror(errno));