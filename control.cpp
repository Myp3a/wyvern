#include "control.hpp"
#include "offsets.hpp"
#include "structs.hpp"

Control::Control() {
	this->ec = EmbeddedController();
	for (int i = 0; i < 12; i++) {
		this->firmware += (char)ec.readByte(0xA0 + i);
	}
	try {
		this->offsets = get_offsets(firmware);
		this->_offsets_available = true;
	}
	catch (...) {
		this->_offsets_available = false;
	};
}

Control::~Control() {
	this->ec.close();
}

std::string Control::firmware_name() {
	return firmware;
}

void Control::dump_ec() {
	ec.printDump();
}

bool Control::offsets_available() {
	return this->_offsets_available;
}

bool Control::coolerboost_get() {
	return bool(ec.readByte(offsets.coolerboost) >> 7);
}

bool Control::coolerboost_set(bool enabled) {
	BYTE b = ec.readByte(offsets.coolerboost);
	if (enabled) {
		b |= 0b10000000;
	}
	else {
		b &= 0b01111111;
	}
	return ec.writeByte(offsets.coolerboost, b);
}

bool Control::silent_get() {
	return (bool)((ec.readByte(offsets.fan_mode) >> 4) % 2);
}

bool Control::silent_set(bool enabled) {
	BYTE b = ec.readByte(offsets.fan_mode);
	if (enabled) {
		b |= 0b00010000;
	}
	else {
		b &= 0b11101111;
	}
	return ec.writeByte(offsets.fan_mode, b);
}

bool Control::custom_curve_get() {
	return (bool)(ec.readByte(offsets.fan_mode) >> 7);
}

bool Control::custom_curve_set(bool enabled) {
	BYTE b = ec.readByte(offsets.fan_mode);
	if (enabled) {
		b |= 0b10000000;
	}
	else {
		b &= 0b01111111;
	}
	return ec.writeByte(offsets.fan_mode, b);
}

FanCurve Control::gpu_fan_curve_get() {
	FanCurve cur = FanCurve();
	for (int i = 0; i < 6; i++) {
		Point p = Point();
		p.temp = ec.readByte(offsets.gpu_temp_arr + i);
		p.speed = ec.readByte(offsets.gpu_fan_arr + i);
		cur.point[i] = p;
	}
	return cur;
}

bool Control::gpu_fan_curve_set(FanCurve cur) {
	for (int i = 0; i < 6; i++) {
		Point p = cur.point[i];
		if (p.speed < 0 || p.speed > 150) {
			return false;
		}
		if (p.temp < 0) {
			return false;
		}
	}
	bool status = true;
	for (int i = 0; i < 6; i++) {
		Point p = cur.point[i];
		status &= ec.writeByte(offsets.gpu_temp_arr + i, p.temp);
		status &= ec.writeByte(offsets.gpu_fan_arr + i, p.speed);
	}
	return status;
}

FanCurve Control::cpu_fan_curve_get() {
	FanCurve cur = FanCurve();
	for (int i = 0; i < 6; i++) {
		Point p = Point();
		p.temp = ec.readByte(offsets.cpu_temp_arr + i);
		p.speed = ec.readByte(offsets.cpu_fan_arr + i);
		cur.point[i] = p;
	}
	return cur;
}

bool Control::cpu_fan_curve_set(FanCurve cur) {
	for (int i = 0; i < 6; i++) {
		Point p = cur.point[i];
		if (p.speed < 0 || p.speed > 150) {
			return false;
		}
		if (p.temp < 0) {
			return false;
		}
	}
	bool status = true;
	for (int i = 0; i < 6; i++) {
		Point p = cur.point[i];
		status &= ec.writeByte(offsets.cpu_temp_arr + i, p.temp);
		status &= ec.writeByte(offsets.cpu_fan_arr + i, p.speed);
	}
	return status;
}

int Control::cpu_fan_percent() {
	return ec.readByte(offsets.cpu_fan_percent);
}

int Control::gpu_fan_percent() {
	return ec.readByte(offsets.gpu_fan_percent);
}

int Control::cpu_fan_rpm() {
	int rpm = ec.readByte(offsets.cpu_fan_rpm);
	if (rpm == 0) return 0;
	return (int)(478000 / rpm);
}

int Control::gpu_fan_rpm() {
	int rpm = ec.readByte(offsets.gpu_fan_rpm);
	if (rpm == 0) return 0;
	return (int)(478000 / rpm);
}

int Control::battery_threshold_get() {
	int threshold = ec.readByte(offsets.battery_threshold);
	threshold &= 0b01111111;
	return threshold;
}

bool Control::battery_threshold_set(int threshold) {
	threshold |= 0b10000000;
	return ec.writeByte(offsets.battery_threshold, threshold);
}

bool Control::webcam_first_get() {
	bool res = (bool)((0b00000010 & ec.readByte(offsets.webcam_enabled_first)) >> 1);
	return res;
}

bool Control::webcam_first_set(bool state) {
	if (state) {
		int val = ec.readByte(offsets.webcam_enabled_first);
		val |= 0b00000010;
		return ec.writeByte(offsets.webcam_enabled_first, val);
	}
	else {
		int val = ec.readByte(offsets.webcam_enabled_first);
		val &= 0b11111101;
		return ec.writeByte(offsets.webcam_enabled_first, val);
	}
}

bool Control::webcam_second_get() {
	bool res = (bool)((0b00000010 & ec.readByte(offsets.webcam_enabled_second)) >> 1);
	return res;
}

bool Control::webcam_second_set(bool state) {
	if (state) {
		int val = ec.readByte(offsets.webcam_enabled_second);
		val |= 0b00000010;
		return ec.writeByte(offsets.webcam_enabled_second, val);
	}
	else {
		int val = ec.readByte(offsets.webcam_enabled_second);
		val &= 0b11111101;
		return ec.writeByte(offsets.webcam_enabled_second, val);
	}
}

bool Control::fn_on_the_left_get() {
	return bool(ec.readByte(offsets.fn_side) >> 4);
}

bool Control::fn_on_the_left_set(bool state) {
	BYTE b = ec.readByte(offsets.fn_side);
	if (state) {
		b |= 0b00010000;
	}
	else {
		b &= 0b11101111;
	}
	return ec.writeByte(offsets.fn_side, b);
}

SHIFT_MODE Control::shift_mode_get() {
	return (SHIFT_MODE)(0b00000111 & (int)ec.readByte(offsets.shift_mode));
}

bool Control::shift_mode_set(SHIFT_MODE mode) {
	BYTE b = ec.readByte(offsets.shift_mode);
	b &= 0b11111000;
	switch (mode) {
	case SHIFT_MODE::TURBO:
		b |= 0b00000100;
		break;
	case SHIFT_MODE::SPORT:
		b |= 0b00000000;
		break;
	case SHIFT_MODE::BALANCED:
		b |= 0b00000001;
		break;
	case SHIFT_MODE::ECO:
		b |= 0b00000010;
		break;
	}
	return ec.writeByte(offsets.shift_mode, b);
}