#include <freertos/FreeRTOS.h> // Mandatory first include

#include <hal/adc_types.h>

#include <chessbot/config.h>

namespace chessbot {
uint32_t configStore[(size_t)ConfigKey::CONFIG_COUNT] = {
    bitcast<uint32_t>((int)33),
    bitcast<uint32_t>((int)38),
    bitcast<uint32_t>((int)39),
    bitcast<uint32_t>((int)40),
    bitcast<uint32_t>((int)32),
    bitcast<uint32_t>((int)31),
    bitcast<uint32_t>((int)18),
    bitcast<uint32_t>((int)34),
    bitcast<uint32_t>((int)15),
    bitcast<uint32_t>((int)0),
    bitcast<uint32_t>((int)1),
    bitcast<uint32_t>((int)3),
    bitcast<uint32_t>((int)5),
    bitcast<uint32_t>((float)4.375),
    bitcast<uint32_t>((float)-1.0),
    bitcast<uint32_t>((float)1.0),
    bitcast<uint32_t>((float)-12000.0)
};
}; // namespace chessbot