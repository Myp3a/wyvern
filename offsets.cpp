#include "offsets.hpp"
#include <string>

Offsets myOffsets{
	.coolerboost = 0x98,
	.fan_mode = 0xd4,
	.cpu_temp_arr = 0x6a,
	.cpu_fan_arr = 0x73,
	.gpu_temp_arr = 0x82,
	.gpu_fan_arr = 0x8a,
	.cpu_fan_percent = 0x71,
	.cpu_fan_rpm = 0xCB,
	.gpu_fan_percent = 0x89,
	.gpu_fan_rpm = 0xc9,
	.battery_threshold = 0xd7,
	.webcam_enabled_first = 0x2e,
	.webcam_enabled_second = 0x2f,
	.fn_side = 0xe8,
	.shift_mode = 0xd2,
	.super_battery = 0xeb,
};

Offsets get_offsets(std::string firmware_name) {
	return myOffsets;
}