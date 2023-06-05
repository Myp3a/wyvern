#include <iostream>
#include <Windows.h>

#include "control.hpp"
#include "CLI11.hpp"

using namespace std;

int main(int argc, char** argv)
{
    CLI::App app{ "Wyvern" };

    app.require_subcommand(0, 1);

    CLI::App* fan_app = app.add_subcommand("fan", "Fan curve control");
    CLI::App* coolerboost_app = fan_app->add_subcommand("coolerboost", "CoolerBoost management");
    int coolerboost_action{ 0 };
    coolerboost_app->add_flag("--enable{1},--disable{2}", coolerboost_action, "CoolerBoost state");
    fan_app->require_subcommand(0, 1);
    string curve_text = "";
    bool gpu{ false };
    bool cpu{ false };
    bool fan_set{ false };
    vector<pair<int,int>> points;
    fan_app->add_flag("--gpu", gpu, "Only GPU fan");
    fan_app->add_flag("--cpu", cpu, "Only CPU fan");
    CLI::Option* fan_do_set = fan_app->add_flag("--set", fan_set, "Set new curve");
    fan_app->add_option("-p,--point", points, "Point on the curve in format: [temp,speed]. Max 6 points.")->expected(0, 7)->needs(fan_do_set);
    
    CLI::App* battery_app = app.add_subcommand("battery", "Battery threshold control");
    int batt_threshold{ -1 };
    battery_app->add_option("--percent", batt_threshold, "Battery charge limit");

    CLI11_PARSE(app, argc, argv);
    Control ctrl = Control();

    if (*fan_app) {
        if (!(gpu) && !(cpu)) {
            gpu = true;
            cpu = true;
        }
        if (*coolerboost_app) {
            string s;
            switch (coolerboost_action) {
            case 0:
                s = "CoolerBoost state: ";
                if (ctrl.coolerboost_get()) {
                    s += "enabled\n";
                }
                else {
                    s += "disabled\n";
                }
                cout << s;
                break;
            case 1:
                ctrl.coolerboost_set(true);
                break;
            case 2:
                ctrl.coolerboost_set(false);
                break;
            }
            return 0;
        }
        if (fan_set) {
            FanCurve cur = ctrl.cpu_fan_curve_get();
            int i = 0;
            for (; i < points.size(); i++) {
                cur.point[i].temp = points[i].first;
                cur.point[i].speed = points[i].second;
            }
            for (; i < 6; i++) {
                cur.point[i].temp = points.back().first;
                cur.point[i].speed = points.back().second;
            }
            if (gpu) {
                ctrl.gpu_fan_curve_set(cur);
            }
            if (cpu) {
                ctrl.cpu_fan_curve_set(cur);
            }
            return 0;
        }
        FanCurve cur_cpu = ctrl.cpu_fan_curve_get();
        FanCurve cur_gpu = ctrl.gpu_fan_curve_get();
        if (cpu) {
            cout << "CPU Fan speed: " << ctrl.cpu_fan_rpm() << " RPM (" << ctrl.cpu_fan_percent() << "%)\n";
            cout << "CPU fan curve:\n";
            for (int i = 0; i < 6; i++) {
                cout << cur_cpu.point[i].temp << " C: " << cur_cpu.point[i].speed << " %\n";
            }
            cout << "\n";
        }
        if (gpu) {
            cout << "GPU Fan speed: " << ctrl.gpu_fan_rpm() << " RPM (" << ctrl.gpu_fan_percent() << "%)\n";
            cout << "GPU fan curve:\n";
            for (int i = 0; i < 6; i++) {
                cout << cur_gpu.point[i].temp << " C: " << cur_gpu.point[i].speed << " %\n";
            }
            cout << "\n";
        }
        return 0;
    }

    if (*battery_app) {
        if (batt_threshold > -1) {
            if (batt_threshold < 10 || batt_threshold > 100) {
                cout << "Battery threshold should be between 10 and 100\n";
                return 1;
            }
            ctrl.battery_threshold_set(batt_threshold);
            return 0;
        }
        cout << "Battery charge threshold: " << ctrl.battery_threshold_get() << "%\n";
        return 0;
    }
    return 0;
}
