#include <freertos/FreeRTOS.h> // Mandatory first include

#include <chessbot/util.h>
#include <chessbot/desc.h>
#include <chessbot/log.h>

namespace chessbot {

MotorDesc::MotorDesc(gpio_num_t motorChannelA_,
        gpio_num_t motorChannelB_,
        gpio_num_t encoderChannelA_,
        gpio_num_t encoderChannelB_,
        float& driveMultiplier_) :
            pin(motorChannelA_), driveMultiplier(driveMultiplier_)
            {
                // Allow lock on to neutral as soon as possible
                pin.setDuty(NEUTRAL);
            }

MotorDesc::~MotorDesc() {}

void MotorDesc::set(float power) {
    power *= driveMultiplier;

    // Find desired length of pulses in microseconds
    uint32_t us = 0;
    uint32_t duty = 0;
    if (power == 0.0) {
        duty = NEUTRAL;
    }
    else if (power >= 0.0) {
        duty = fmap(power, 0.0, 1.0, FORWARD_MIN, FORWARD_MAX);
    }
    else {
        duty = fmap(power, -1.0, 0.0, REVERSE_MIN, REVERSE_MAX);
    }

    ESP_LOGI("", "Setting power=%f and multiplying with driveMultiplier=%f us=%d duty=%d", power, driveMultiplier, (int)us, (int)duty);

    pin.setDuty(duty);
}

int32_t MotorDesc::pos() { 
    return 0; // todo: estimate?
}

void MotorDesc::tick(uint64_t us) {}

}; // namespace chessbot