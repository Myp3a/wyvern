#pragma once

#include "offsets.hpp"
#include "ec.hpp"
#include "structs.hpp"
#include <string>

class Control {
private:
	EmbeddedController ec;
	std::string firmware;
	Offsets offsets;

public:
	Control();
	~Control();

	std::string firmware_name();
	void dump_ec();

	bool coolerboost_get();
	bool coolerboost_set(bool enabled);
	FanCurve gpu_fan_curve_get();
	bool gpu_fan_curve_set(FanCurve cur);
	FanCurve cpu_fan_curve_get();
	bool cpu_fan_curve_set(FanCurve cur);
	int cpu_fan_percent();
	int cpu_fan_rpm();
	int gpu_fan_percent();
	int gpu_fan_rpm();

	int battery_threshold_get();
	bool battery_threshold_set(int threshold);
};