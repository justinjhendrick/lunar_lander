#include <cmath>
#include "PID.hpp"
#include "Utils.hpp"

// downloaded in part from 
// https://gist.github.com/bradley219/5373998
PID::PID(double _dt,
         double _max,
         double _min,
         double _Kp,
         double _Kd,
         double _Ki) {
    dt = _dt;
    max = _max;
    min = _min;
    Kp = _Kp;
    Kd = _Kd;
    Ki = _Ki;
    pre_error = 0.;
    integral = 0.;
}

double PID::calculate( double setpoint, double pv ) {
    // Calculate error
    double error;
    Utils::angle_diff(setpoint, pv, &error);

    // Proportional term
    double Pout = Kp * error;

    // Integral term
    integral += error * dt;
    double Iout = Ki * integral;

    // Derivative term
    double derivative = (error - pre_error) / dt;
    double Dout = Kd * derivative;

    // Calculate total output
    double output = Pout + Iout + Dout;

    // Restrict to max/min
    if (output > max) {
        output = max;
    } else if(output < min) {
        output = min;
    }

    // Save error to previous error
    pre_error = error;

    return output;
}
