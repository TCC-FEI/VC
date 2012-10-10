#ifndef VC_H
#define VC_H

#include <utils.h>
#include <stdio.h>

#ifdef VC_DEBUG

#define vc_trace        printf
#define vc_trace_hash   _vc_trace_hash
#define vc_trace_strv   _vc_trace_strv

#else

#define vc_trace        0 && printf
#define vc_trace_hash   0 && printf
#define vc_trace_strv   0 && printf

#endif

#endif
