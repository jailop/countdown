#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <chrono>
#include <thread>
#include "countdown.h"

using namespace std;

void displayTime(time_t t) {
    cout << '\r'<< setfill('0') << setw(2) << t / 60 << ":" 
        << setw(2) << t % 60 << flush;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cout << "Usage: countdown <minutes>" << endl;
        return 1;
    } 
    Countdown countdown(atoi(argv[1]) * 60, false);
    countdown.run();
    while (true) {
        int remaining = countdown.remainingTime();
        displayTime(remaining);
        std::this_thread::sleep_for(std::chrono::seconds(1)); 
        if (remaining == 0) {
            countdown.stop();
            break;
        }
    }
    cout << endl;
    return 0;
}
