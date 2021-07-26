#include "time_keeper.h"
TIME_KEEPER tk;
TIME_KEEPER::TIME_KEEPER()
{
}
void TIME_KEEPER::check_timer()
{
    if ((millis() - time_prev.t10ms) >= 10)
    {
        time_flag.t10ms = 1;
        time_prev.t10ms = millis();
    }
    if ((millis() - time_prev.t50ms) >= 50)
    {
        time_flag.t50ms = 1;
        time_prev.t50ms = millis();
    }
    if ((millis() - time_prev.t100ms) >= 100)
    {
        time_flag.t100ms = 1;
        time_prev.t100ms = millis();
    }
    if ((millis() - time_prev.t200ms) >= 200 - 1)
    {
        time_flag.t200ms = 1;
        time_prev.t200ms = millis();
    }
    if ((millis() - time_prev.t500ms) >= 500 - 1)
    {
        time_flag.t500ms = 1;
        time_prev.t500ms = millis();
    }
    if ((millis() - time_prev.t1s) >= 1000 - 1)
    {
        time_flag.t1s = 1;
        time_prev.t1s = millis();
    }
    if ((millis() - time_prev.t2s) >= 2000 - 1)
    {
        time_flag.t2s = 1;
        time_prev.t2s = millis();
    }
    if ((millis() - time_prev.t5s) >= 5000 - 1)
    {
        time_flag.t5s = 1;
        time_prev.t5s = millis();
    }
    if ((millis() - time_prev.t10s) >= 10000 - 1)
    {
        time_flag.t10s = 1;
        time_prev.t10s = millis();
    }
}
void TIME_KEEPER::time_keeper_begin()
{
    check_timer();
}
void TIME_KEEPER::time_keeper_end()
{
    time_flag = {0, 0, 0, 0, 0, 0, 0, 0, 0};
}