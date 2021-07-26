#pragma once

#include "Arduino.h"
typedef struct
{
    bool t10ms, t50ms, t100ms, t200ms, t500ms, t1s, t2s, t5s, t10s;
} flag_t;

typedef struct
{
    uint32_t t10ms, t50ms, t100ms, t200ms, t500ms, t1s, t2s, t5s, t10s;
} time_prev_t;

class TIME_KEEPER
{
public:
    TIME_KEEPER();
    void time_keeper_begin();
    void time_keeper_end();
    flag_t time_flag;
    time_prev_t time_prev;

private:
    void check_timer();
};

extern TIME_KEEPER tk;