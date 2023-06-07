#include "structs.hpp"

int FanCurve::get_interpolated(int temp) {
	Point start = point[0];
	Point end = point[5];
	for (int i = 0; i < 5; i++) {
		if (temp > point[i].temp) {
			start = point[i];
		}
		if (temp < point[i + 1].temp) {
			end = point[i + 1];
		}
	}
	double diff = temp - start.temp;
	double whole = end.temp - start.temp;
	double mult = diff / whole;
	int res = (int)((end.speed - start.speed) * mult + start.speed);
	return res;
}
