#pragma once

#include "ec.hpp"
#include "structs.hpp"

class Control {
private:
	EmbeddedController ec;
public:
	Control();
	~Control();

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