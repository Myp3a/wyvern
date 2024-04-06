#include <iostream>
#include <Windows.h>

#include "control.hpp"
#include "structs.hpp"
#include "ascii/asciiplotter.h"
#include "CLI11.hpp"

using namespace std;

void print_system_info(Control ctrl) {
    cout << "Wyvern v. master\n";
    cout << "System firmware: " << ctrl.firmware_name() << '\n';
    cout << "EC dump:\n";
    ctrl.dump_ec();
}

void print_fan_info(Control ctrl, bool pretty = false) {
    if (pretty) {
        cout << "Current mode: ";
    }
    if (ctrl.coolerboost_get()) {
        cout << "coolerboost\n";
    }
    else if (ctrl.silent_get()) {
        cout << "silent\n";
    }
    else if (!ctrl.custom_curve_get()) {
        cout << "auto\n";
    }
    else {
        cout << "custom\n";
        if (pretty) {
            FanCurve cur_cpu = ctrl.cpu_fan_curve_get();
            FanCurve cur_gpu = ctrl.gpu_fan_curve_get();
            vector<double> cpu_temp;
            vector<double> cpu_speed;
            vector<double> gpu_temp;
            vector<double> gpu_speed;
            for (int i = 0; i < 6; i++) {
                cpu_temp.push_back(cur_cpu.point[i].temp);
                cpu_speed.push_back(cur_cpu.point[i].speed);
                gpu_temp.push_back(cur_gpu.point[i].temp);
                gpu_speed.push_back(cur_gpu.point[i].speed);
            }
            AsciiPlotter plotter("Fan speeds", 40, 15);
            plotter.addPlot(vector<double>{100}, vector<double>{150}, " ", ' ');
            plotter.addPlot(cpu_temp, cpu_speed, "CPU", 'c');
            plotter.addPlot(gpu_temp, gpu_speed, "GPU", 'g');
            plotter.legend();
            plotter.xlabel("Deg (C)");
            plotter.show();
        }
    }
}

int main(int argc, char** argv)
{
    CLI::App app{ "Wyvern" };

    app.require_subcommand(0, 1);

    CLI::App* version_app = app.add_subcommand("version", "Information about system and program");

    CLI::App* fan_app = app.add_subcommand("fan", "Fan management");
    CLI::Option* fan_set = fan_app->add_flag("--set", "Set fans mode");
    string desired_fan_mode;
    fan_app->add_option("fan_mode", desired_fan_mode, "Selected fan mode")->check(CLI::IsMember({ "coolerboost", "silent", "auto", "custom" }));
    vector<pair<int, int>> desired_fan_curve;
    CLI::Option* fan_curve_points = fan_app->add_option("-p,--point", desired_fan_curve, "Fan curve points in format [temp,speed %]")->expected(0, 7)->needs(fan_set);
    bool ignore_cpu{ false };
    bool ignore_gpu{ false };
    fan_app->add_flag("--no-cpu", ignore_cpu, "Disables CPU curve setting");
    fan_app->add_flag("--no-gpu", ignore_gpu, "Disables GPU curve setting");

    CLI::App* webcam_app = app.add_subcommand("webcam", "Webcam lock management");
    string desired_webcam_state;
    webcam_app->add_option("state", desired_webcam_state, "Webcam state")->check(CLI::IsMember({ "enabled", "disabled" }));
    int webcam_first_second_lock{ 0 };
    webcam_app->add_flag("-1{1},-2{2}", webcam_first_second_lock, "Lock selection");

    CLI::App* battery_app = app.add_subcommand("battery", "Battery charging limit");
    int battery_percent{ 0 };
    battery_app->add_option("percent", battery_percent, "Percent to end charge at")->check(CLI::Range(10,100));

    CLI::App* fn_app = app.add_subcommand("fn", "FN key position");
    string desired_fn_position;
    fn_app->add_option("position", desired_fn_position, "Keyboard side")->check(CLI::IsMember({ "left", "right" }));

    // superbattery handled as shift mode eco. SB switch appears to do nothing on it's own?
    CLI::App* shift_mode_app = app.add_subcommand("shift", "System performance mode selection");
    string desired_shift_mode;
    shift_mode_app->add_option("mode", desired_shift_mode, "Shift mode")->check(CLI::IsMember({ "turbo", "sport", "balanced", "eco" }));

    CLI11_PARSE(app, argc, argv);
    Control ctrl = Control();

    if (*version_app) {
        print_system_info(ctrl);
        return 0;
    }

    if (!ctrl.offsets_available()) {
        cout << "No offsets found for your firmware. Can't continue working. Please, report your firmware version and EC dump with \"version\".\n";
        return 1;
    }

    if (*fan_app) {
        if (!desired_fan_curve.empty()) {
            FanCurve cur = ctrl.cpu_fan_curve_get();
            int i = 0;
            // fill with user input
            for (; i < desired_fan_curve.size(); i++) {
                cur.point[i].temp = desired_fan_curve[i].first;
                cur.point[i].speed = desired_fan_curve[i].second;
            }
            // finish with repeating last one
            for (; i < 6; i++) {
                cur.point[i].temp = desired_fan_curve.back().first;
                cur.point[i].speed = desired_fan_curve.back().second;
            }
            if (!ignore_gpu) {
                ctrl.gpu_fan_curve_set(cur);
            }
            if (!ignore_cpu) {
                ctrl.cpu_fan_curve_set(cur);
            }
        }
        if (!desired_fan_mode.empty()) {
            if (*fan_set) {
                switch (fan_state.at(desired_fan_mode)) {
                case FAN_STATE::COOLERBOOST:
                    ctrl.coolerboost_set(true);
                    ctrl.silent_set(false);
                    ctrl.custom_curve_set(false);
                    break;
                case FAN_STATE::SILENT:
                    ctrl.coolerboost_set(false);
                    ctrl.silent_set(true);
                    ctrl.custom_curve_set(false);
                    break;
                case FAN_STATE::AUTO:
                    ctrl.coolerboost_set(false);
                    ctrl.silent_set(false);
                    ctrl.custom_curve_set(false);
                    break;
                case FAN_STATE::CUSTOM:
                    ctrl.coolerboost_set(false);
                    ctrl.silent_set(false);
                    ctrl.custom_curve_set(true);
                    break;
                }
                return 0;
            }
            else {
                switch (fan_state.at(desired_fan_mode)) {
                case FAN_STATE::COOLERBOOST:
                    cout << ctrl.coolerboost_get();
                    break;
                case FAN_STATE::SILENT:
                    cout << ctrl.silent_get();
                    break;
                case FAN_STATE::AUTO:
                    cout << !ctrl.custom_curve_get();
                    break;
                case FAN_STATE::CUSTOM:
                    cout << ctrl.custom_curve_get();
                    break;
                }
                return 0;
            }
        }
        print_fan_info(ctrl, true);
        return 0;
    }

    if (*webcam_app) {
        if (!desired_webcam_state.empty()) {
            switch (state.at(desired_webcam_state)) {
            case STATE::ENABLED:
                if (webcam_first_second_lock == 0 || webcam_first_second_lock == 1) {
                    ctrl.webcam_first_set(true);
                }
                if (webcam_first_second_lock == 0 || webcam_first_second_lock == 2) {
                    ctrl.webcam_second_set(true);
                }
                break;
            case STATE::DISABLED:
                if (webcam_first_second_lock == 0 || webcam_first_second_lock == 1) {
                    ctrl.webcam_first_set(false);
                }
                if (webcam_first_second_lock == 0 || webcam_first_second_lock == 2) {
                    ctrl.webcam_second_set(false);
                }
                break;
            }
            return 0;
        }
        switch (webcam_first_second_lock) {
        case 0:
            cout << "First webcam state: " << ctrl.webcam_first_get() << "\n";
            cout << "Second webcam state: " << ctrl.webcam_second_get() << "\n";
            break;
        case 1:
            cout << ctrl.webcam_first_get() << "\n";
            break;
        case 2:
            cout << ctrl.webcam_second_get() << "\n";
            break;
        }
        return 0;
    }

    if (*battery_app) {
        if (battery_percent > 0) {
            ctrl.battery_threshold_set(battery_percent);
        }
        else {
            cout << "Battery charge threshold: ";
            cout << ctrl.battery_threshold_get() << "\n";
        }
        return 0;
    }

    if (*fn_app) {
        if (!desired_fn_position.empty()) {
            switch (fn_side.at(desired_fn_position)) {
            case FN_SIDE::LEFT:
                ctrl.fn_on_the_left_set(true);
                break;
            case FN_SIDE::RIGHT:
                ctrl.fn_on_the_left_set(false);
                break;
            }
            return 0;
        }
        cout << "FN key position: ";
        if (ctrl.fn_on_the_left_get()) {
            cout << "left\n";
        }
        else {
            cout << "right\n";
        }
        return 0;
    }

    if (*shift_mode_app) {
        if (!desired_shift_mode.empty()) {
            ctrl.shift_mode_set(shift_mode.at(desired_shift_mode));
            return 0;
        }
        cout << "Current mode: ";
        SHIFT_MODE mode = ctrl.shift_mode_get();
        switch (mode) {
        case SHIFT_MODE::TURBO:
            cout << "turbo\n";
            break;
        case SHIFT_MODE::SPORT:
            cout << "sport\n";
            break;
        case SHIFT_MODE::BALANCED:
            cout << "balanced";
            break;
        case SHIFT_MODE::ECO:
            cout << "eco\n";
            break;
        }
        return 0;
    }
}
