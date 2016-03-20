#ifndef _PID_H_
#define _PID_H_

// https://en.wikipedia.org/wiki/PID_controller
//
// downloaded in part from
// https://gist.github.com/bradley219/5373998
class PID {
    private:
        double dt;
        double max;
        double min;
        double Kp;
        double Kd;
        double Ki;
        double pre_error;
        double integral;
    public:
        // Kp -  proportional gain
        // Ki -  Integral gain
        // Kd -  derivative gain
        // dt -  loop interval time
        // max - maximum value of manipulated variable
        // min - minimum value of manipulated variable
        PID(double _dt,
            double _max,
            double _min,
            double _Kp,
            double _Kd,
            double _Ki);

        // Returns the manipulated variable given a setpoint and current
        // process value
        double calculate(double setpoint, double pv);
};

#endif
