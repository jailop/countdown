#include <cassert>
#include <chrono>
#include <thread>
#include "countdown.h"

void test_countdown() {
    Countdown countdown(10, false);
    assert(countdown.remainingTime() == 10);
    assert(!countdown.isRunning());
    countdown.run();
    assert(countdown.isRunning());
    std::this_thread::sleep_for(std::chrono::seconds(5));
    assert((countdown.remainingTime() < 10) && (countdown.remainingTime() > 0));
    std::this_thread::sleep_for(std::chrono::seconds(5));
    assert(countdown.remainingTime() == 0);
    countdown.stop();
    assert(!countdown.isRunning());
}

int main() {
    test_countdown();
    return 0;
}
