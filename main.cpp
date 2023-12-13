#include <csignal>
#include <iostream>
#include <thread>
#include <pthread.h>

#include "Timer.hpp"

static bool isRunning = true;

// handle interrupt signal
void sigintHandler(int sig_num)
{
    std::cout << "\nCtrl-C received. Exiting...\n";
    isRunning = false;
}

void* timerLoop(void *arg)
{
    wl::Timer *t{static_cast<wl::Timer*>(arg)};
    
    // register signal SIGINT and signal handler
    std::signal (SIGINT, sigintHandler);

    while (isRunning) {
        ++(*t);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    pthread_exit(NULL);
}

int main()
{
    wl::Timer *t{new wl::Timer()};
    std::cout << "hh mm ss\n";

    pthread_t timerThread;
    pthread_create(&timerThread, NULL, timerLoop, t);

    while (isRunning) {
        t->display();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    pthread_join(timerThread, NULL);

    std::cout << "Timer stopped.\n";
    std::cout << std::endl;

    delete t;

    return 0;
}
