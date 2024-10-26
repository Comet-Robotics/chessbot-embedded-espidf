#ifndef CHESSBOT_MUTEX_H
#define CHESSBOT_MUTEX_H

#include <freertos/FreeRTOS.h> // Mandatory first include
#include <semphr.h>

namespace chessbot {

class Mutex {
    SemaphoreHandle_t mutex = nullptr;

    public:
    class MutexLock {
        Mutex* held;

        public:
        MutexLock(Mutex* held_) : held(held_) {}

        MutexLock() = delete;
        MutexLock& operator= (MutexLock&&) = delete;
        MutexLock& operator= (const MutexLock&&) = delete;
        MutexLock(MutexLock&&) = delete;
        MutexLock(const MutexLock&&) = delete;

        ~MutexLock() {
            //printf("Give mutex %p %p\n", held, held->mutex);
            CHECK(xSemaphoreGive(held->mutex) == pdTRUE);
        }
    };

    Mutex() {
        mutex = xSemaphoreCreateMutex();
        CHECK(mutex != nullptr);
        //printf("Make mutex with ptr %p\n", mutex);
    }

    MutexLock take(TickType_t maxDelay = portMAX_DELAY) {
        //printf("Take mutex %p\n", mutex);
        CHECK(xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE);
        return MutexLock(this);
    }
};

}; // namespace chessbot

#endif // ifndef CHESSBOT_MUTEX_H