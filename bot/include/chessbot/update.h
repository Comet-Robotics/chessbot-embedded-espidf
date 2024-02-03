#ifndef CHESSBOT_UPDATE_H
#define CHESSBOT_UPDATE_H

#include <freertos/FreeRTOS.h> // Mandatory first include

#include <cstdint>

namespace chessbot {
extern uint64_t currentFirmwareVersion;

// Create the low-priority task to occasionally check for OTA updates
void launchUpdater();

// Skip the update delay
void checkUpdateNow();
}; // namespace chessbot

#endif // ifndef CHESSBOT_UPDATE_H