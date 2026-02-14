#include "mbed.h" //https://os.mbed.com/docs/mbed-os/v6.16/debug-test/visual-studio-code.html

// Outer CN10 pins you selected
DigitalOut motor_enable(PB_2);   // Enable -> JP1a
DigitalOut motor_pwm(PB_1);      // PWM1  -> JP1a

int main() {
    // Start with everything off
    motor_enable = 0;
    motor_pwm = 0;

    wait(1.0);   // 1 second delay

    // Turn motor fully on
    motor_enable = 1;
    motor_pwm = 1;

    // Stay on forever
    while (true) {
        wait(1.0);
    }
}