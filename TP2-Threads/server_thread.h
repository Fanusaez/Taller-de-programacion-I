//  "Copyright 2023 elDipa martinp.dipaola@gmail.com

#ifndef SERVER_THREAD_H_
#define SERVER_THREAD_H_

#include <thread>
#include <iostream>

class Thread {
 private:
    std::thread thread;

 public:
    Thread() {}

    void start() {
        thread = std::thread(&Thread::main, this);
    }

    void join() {
        thread.join();
    }

    void main() {
        try {
            this->run();
        } catch(const std::exception &err) {
            std::cerr << "Unexpected exception: " << err.what() << "\n";
        } catch(...) {
            std::cerr << "Unexpected exception: <unknown>\n";
        }
    }
    virtual bool sigue_vivo() = 0;
    virtual void kill() = 0;
    virtual void run() = 0;
    virtual ~Thread() {}

    Thread(const Thread&) = delete;
    Thread& operator=(const Thread&) = delete;

    Thread(Thread&& other) = delete;
    Thread& operator=(Thread&& other) = delete;
};
#endif  //  SERVER_THREAD_H_
