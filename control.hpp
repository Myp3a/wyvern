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
	bool _offsets_available;

public:
	Control();
	~Control();

	std::string firmware_name();
	void dump_ec();
	bool offsets_available();

	bool coolerboost_get();
	bool coolerboost_set(bool enabled);
	bool silent_get();
	bool silent_set(bool enabled);
	bool custom_curve_get();
	bool custom_curve_set(bool enabled);

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

	bool webcam_first_get();
	bool webcam_second_get();
	bool webcam_first_set(bool state);
	bool webcam_second_set(bool state);

	bool fn_on_the_left_get();
	bool fn_on_the_left_set(bool state);

	SHIFT_MODE shift_mode_get();
	bool shift_mode_set(SHIFT_MODE mode);
};