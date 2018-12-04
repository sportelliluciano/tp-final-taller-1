#include "comun/lock.h"

#include <mutex>

Lock::Lock(std::mutex& m) : mutex(m) {
    mutex.lock();
}

Lock::~Lock() {
    mutex.unlock();
}
