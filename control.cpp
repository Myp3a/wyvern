#include "control.hpp"
#include "offsets.hpp"
#include "structs.hpp"

Control::Control() {
	this->ec = EmbeddedController();
	for (int i = 0; i < 12; i++) {
		this->firmware += (char)ec.readByte(0xA0 + i);
	}
	this->offsets = get_offsets(firmware);
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
	return (int)(478000 / rpm);
}

int Control::gpu_fan_rpm() {
	int rpm = ec.readByte(offsets.gpu_fan_rpm);
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