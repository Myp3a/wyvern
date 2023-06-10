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

enum SIDE {
	LEFT,
	RIGHT
};

static const std::map <std::string, FAN_STATE> fan_mapping = {
	{"coolerboost", FAN_STATE::COOLERBOOST},
	{"silent",		FAN_STATE::SILENT},
	{"auto",		FAN_STATE::AUTO},
	{"custom",		FAN_STATE::CUSTOM}
};

static const std::map <std::string, STATE> state = {
	{"enabled", STATE::ENABLED},
	{"disabled", STATE::DISABLED}
};

static const std::map <std::string, SIDE> fn_side = {
	{"left", SIDE::LEFT},
	{"right", SIDE::RIGHT}
};