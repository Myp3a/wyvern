#pragma once

#include <string>

//int FIRMWARE_NAME = 0xA0;

struct Offsets
{
	int coolerboost;
	int fan_mode;

	int cpu_temp_arr;
	int cpu_fan_arr;
	int gpu_temp_arr;
	int gpu_fan_arr;
	int cpu_fan_percent;
	int cpu_fan_rpm;
	int gpu_fan_percent;
	int gpu_fan_rpm;

	int battery_threshold;

	int webcam_enabled_first;
	int webcam_enabled_second;
};

Offsets get_offsets(std::string firmware_name);