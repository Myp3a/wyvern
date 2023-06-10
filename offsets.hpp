#pragma once

#include <string>
#include <map>
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

	int fn_side;

	int shift_mode;
	int super_battery;
};

static const Offsets GE77HXOffsets{
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

static const std::map <std::string, Offsets> offsets_mappings = {
	{"17K5IMS1.106", GE77HXOffsets}
};

Offsets get_offsets(std::string firmware_name);