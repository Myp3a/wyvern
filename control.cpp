#include "control.hpp"
#include "offsets.hpp"
#include "structs.hpp"

Control::Control() {
	this->ec = EmbeddedController();
}

Control::~Control() {
	this->ec.close();
}

bool Control::coolerboost_get() {
	return bool(ec.readByte(COOLER_BOOST) >> 7);
}

bool Control::coolerboost_set(bool enabled) {
	BYTE b = ec.readByte(COOLER_BOOST);
	if (enabled) {
		b |= 0b10000000;
	}
	else {
		b &= 0b01111111;
	}
	return ec.writeByte(COOLER_BOOST, b);
}

FanCurve Control::gpu_fan_curve_get() {
	FanCurve cur = FanCurve();
	for (int i = 0; i < 6; i++) {
		Point p = Point();
		p.temp = ec.readByte(GPU_TEMP_ARR + i);
		p.speed = ec.readByte(GPU_FAN_ARR + i);
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
		status &= ec.writeByte(GPU_TEMP_ARR + i, p.temp);
		status &= ec.writeByte(GPU_FAN_ARR + i, p.speed);
	}
	return status;
}

FanCurve Control::cpu_fan_curve_get() {
	FanCurve cur = FanCurve();
	for (int i = 0; i < 6; i++) {
		Point p = Point();
		p.temp = ec.readByte(CPU_TEMP_ARR + i);
		p.speed = ec.readByte(CPU_FAN_ARR + i);
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
		status &= ec.writeByte(CPU_TEMP_ARR + i, p.temp);
		status &= ec.writeByte(CPU_FAN_ARR + i, p.speed);
	}
	return status;
}

int Control::cpu_fan_percent() {
	return ec.readByte(CPU_FAN_PERCENT);
}

int Control::gpu_fan_percent() {
	return ec.readByte(GPU_FAN_PERCENT);
}

int Control::cpu_fan_rpm() {
	int rpm = ec.readByte(CPU_FAN_RPM);
	return (int)(478000 / rpm);
}

int Control::gpu_fan_rpm() {
	int rpm = ec.readByte(GPU_FAN_RPM);
	return (int)(478000 / rpm);
}