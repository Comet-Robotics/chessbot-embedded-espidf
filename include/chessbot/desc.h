#ifndef CHESSBOT_DESC_H
#define CHESSBOT_DESC_H

#include <freertos/FreeRTOS.h> // Mandatory first include

#include <driver/ledc.h>
#include <driver/gpio.h>

#include "chessbot/dac.h"

namespace chessbot {

// Brushed Electronic Speed Controller
class Desc {
    public:
    constexpr static uint32_t FORWARD_MIN = 1900;
    constexpr static uint32_t FORWARD_MAX = 2200;
    constexpr static uint32_t REVERSE_MIN = 800;
    constexpr static uint32_t REVERSE_MAX = 1100;
    constexpr static uint32_t NEUTRAL = 1500;

    constexpr static uint32_t TIMER_WIDTH_TICKS = 1024;

    private:
    PwmPin pin;
    float& driveMultiplier;

    public:
    Desc(gpio_num_t motorChannelA_,
        gpio_num_t motorChannelB_,
        gpio_num_t encoderChannelA_,
        gpio_num_t encoderChannelB_,
        float& driveMultiplier_);
    ~Desc();

    // Set speed in [-0.0, 1.0]
    void set(float power);

    void stop();

    void start();

    int32_t pos();

    void tick(uint64_t us);
};

}; // namespace chessbot

#endif // ifndef CHESSBOT_DESC_H