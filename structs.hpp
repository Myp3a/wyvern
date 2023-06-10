#pragma once

#include <map>
#include <string>

struct Point {
	int temp;
	int speed;
};

class FanCurve {
public:
	Point point[6];
};

enum FAN_STATE {
	COOLERBOOST,
	SILENT,
	AUTO,
	CUSTOM
};

enum STATE {
	ENABLED,
	DISABLED
};

enum FN_SIDE {
	LEFT,
	RIGHT
};

// are turbo and sport different? no sport on GE77 HX
enum SHIFT_MODE {
	TURBO = 4,
	SPORT = 0,
	BALANCED = 1,
	ECO = 2
};

static const std::map <std::string, FAN_STATE> fan_state = {
	{"coolerboost", FAN_STATE::COOLERBOOST},
	{"silent",		FAN_STATE::SILENT},
	{"auto",		FAN_STATE::AUTO},
	{"custom",		FAN_STATE::CUSTOM}
};

static const std::map <std::string, STATE> state = {
	{"enabled", STATE::ENABLED},
	{"disabled", STATE::DISABLED}
};

static const std::map <std::string, FN_SIDE> fn_side = {
	{"left", FN_SIDE::LEFT},
	{"right", FN_SIDE::RIGHT}
};

static const std::map <std::string, SHIFT_MODE> shift_mode = {
	{"turbo", SHIFT_MODE::TURBO},
	{"sport", SHIFT_MODE::SPORT},
	{"balanced", SHIFT_MODE::BALANCED},
	{"eco", SHIFT_MODE::ECO}
};