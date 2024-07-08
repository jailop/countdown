#ifndef _COUNTDOWN_H_
#define _COUNTDOWN_H_

#include <ctime>

class Countdown {
    public:
        Countdown(time_t seconds, bool keepCounting = false);
        time_t remainingTime();
        void run();
        void stop();
        bool isRunning();
    private:
        void updateStatus();
        time_t mark = 0;  // real time on last update
        bool is_running = false;
        time_t value;     // remaining time
        time_t limit;     // time limit
        bool keep_counting; // no stop when limit reached
};

#endif // _COUNTDOWN_H_
