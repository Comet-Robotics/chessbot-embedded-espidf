#include <freertos/FreeRTOS.h> // Mandatory first include

#include <chessbot/differentialKinematics.h>

#include <math.h>

#include <pid_ctrl.h>

#include <chessbot/config.h>
#include <chessbot/log.h>
#include <chessbot/motor.h>

namespace chessbot {
DifferentialKinematics::DifferentialKinematics(Motor& leftMotor_, Motor& rightMotor_)
{
    leftConfig.init_param.kp = 0.1; // Proportional
    leftConfig.init_param.ki = 0; // Integral
    leftConfig.init_param.kd = 0; // Derivative

    leftConfig.init_param.max_output = 1.0;
    leftConfig.init_param.min_output = -1.0;

    leftConfig.init_param.max_integral = 10.0;
    leftConfig.init_param.min_integral = -10.0;

    leftConfig.init_param.cal_type = PID_CAL_TYPE_POSITIONAL;

    rightConfig = leftConfig;

    CHECK(pid_new_control_block(&leftConfig, &leftPid));
    CHECK(pid_new_control_block(&rightConfig, &rightPid));
}

void DifferentialKinematics::tick(uint32_t delta)
{
    float leftIn = leftTarget - leftMotor->pos();
    float rightIn = rightTarget - rightMotor->pos();

    float leftOut = 0.0;
    CHECK(pid_compute(leftPid, leftIn, &leftOut));

    float rightOut = 0.0;
    CHECK(pid_compute(rightPid, rightIn, &rightOut));

    leftMotor->set(leftOut);
    rightMotor->set(rightOut);

    ESP_LOGI("", "PID in %f %f out %f %f", leftIn, rightIn, leftOut, rightOut);
}

// Map inches of driving to encoder ticks
int32_t DifferentialKinematics::distanceToTicks(float dist)
{
    return dist * getTicksPerInch();
}

float DifferentialKinematics::ticksToDistance(int32_t ticks)
{
    return (float)(ticks) / getTicksPerInch();
}

float DifferentialKinematics::getTicksPerInch()
{
    return FCONFIG(ENCODER_MULTIPLIER) / (FCONFIG(WHEEL_DIAMETER_INCHES) * M_PI);
}

void DifferentialKinematics::forward(float dist)
{
    leftTarget = leftMotor->pos() + distanceToTicks(dist);
    rightTarget = rightMotor->pos() + distanceToTicks(dist);
}

void DifferentialKinematics::refresh()
{
    pid_update_parameters(leftPid, &leftConfig.init_param);
    pid_update_parameters(rightPid, &rightConfig.init_param);
}
}; // namespace chessbot