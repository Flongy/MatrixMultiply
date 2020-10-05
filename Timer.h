#ifndef PARALLELPROGRAM1_TIMER_H
#define PARALLELPROGRAM1_TIMER_H

#include <chrono>

class Timer {
public:
    Timer() {
        text = "Time =";
        start();
    }

    Timer(const std::string& message) {
        text = message;
        start();
    }

    ~Timer() {
        stop();
        print();
    }

    void start() {
        stopped = false;
        t_start = std::chrono::high_resolution_clock::now();
    }

    void stop() {
        if (!stopped) {
            t_end = std::chrono::high_resolution_clock::now();
        }
        stopped = true;
    }

    void print() {
        printf("%s %fms\n", text.c_str(), std::chrono::duration<double, std::milli>(t_end - t_start).count());
    }

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> t_start;
    std::chrono::time_point<std::chrono::high_resolution_clock> t_end;
    bool stopped;
    std::string text;
};


#endif //PARALLELPROGRAM1_TIMER_H
