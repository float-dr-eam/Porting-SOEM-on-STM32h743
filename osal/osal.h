/*
 * Licensed under the GNU General Public License version 2 with exceptions. See
 * LICENSE file in the project root for full license information
 */

#ifndef _osal_
#define _osal_

#ifdef __cplusplus
extern "C"
{
#endif

#include "osal_defs.h"
#include <stdint.h>
#include <stdbool.h>
/* General types */
#ifndef TRUE
#define TRUE                1
#endif
#ifndef FALSE
#define FALSE               0
#endif
typedef uint8_t             boolean;
typedef int8_t              int8;
typedef int16_t             int16;
typedef int32_t             int32;
typedef uint8_t             uint8;
typedef uint16_t            uint16;
typedef uint32_t            uint32;
typedef int64_t             int64;
typedef uint64_t            uint64;
typedef float               float32;
typedef double              float64;

typedef struct
{
    uint32 sec;     /*< Seconds elapsed since the Epoch (Jan 1, 1970) */
    uint32 usec;    /*< Microseconds elapsed since last second boundary */
} ec_timet;

typedef struct
{
    uint32 tv_sec;
    uint32 tv_usec;
}timeval;

typedef struct
{
	int tz_minuteswest; /* 格林尼治时间往西方的时差，以分钟为单位 */
	int tz_dsttime; /* 日光节约时间的修正方式 */
}timezone;

typedef struct osal_timer
{
    ec_timet stop_time;
} osal_timert;

void osal_timer_start(osal_timert * self, uint32 timeout_us);
boolean osal_timer_is_expired(osal_timert * self);
int osal_usleep(uint32 usec);
ec_timet osal_current_time(void);
void osal_time_diff(ec_timet *start, ec_timet *end, ec_timet *diff);
int osal_thread_create(void *thandle, int stacksize, void *func, void *param);
int osal_thread_create_rt(void *thandle, int stacksize, void *func, void *param);


int gettimeofday(timeval *tv);
#ifdef __cplusplus
}
#endif

#endif
