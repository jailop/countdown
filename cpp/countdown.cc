#include "countdown.h"

Countdown::Countdown(time_t seconds, bool keepCounting)
    : limit(seconds), keep_counting(keepCounting) {
    this->value = seconds;
}

void Countdown::updateStatus() {
    if (this->is_running) {
        time_t curr = time(nullptr);
        this->value -= curr - this->mark;
        this->mark = curr;
        while (this->value < 0)
            if (this->keep_counting)
                this->value += this->limit;
            else {
                this->value = 0;
            }
    }
}

time_t Countdown::remainingTime() {
    this->updateStatus();
    return this->value;
}

bool Countdown::isRunning() {
    return this->is_running;
}

void Countdown::run() {
    if (this->is_running)
        return;
    this->mark = time(nullptr);
    this->is_running = true;
}

void Countdown::stop() {
    this->updateStatus();
    this->is_running = false;
}
