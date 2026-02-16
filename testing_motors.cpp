#include "mbed.h" //https://os.mbed.com/docs/mbed-os/v6.16/debug-test/visual-studio-code.html

// This code is for testing the motor control on the board. It will turn the motor on at full speed and keep it running indefinitely.
DigitalOut motor_enable(PB_2);   // Enable -> JP1a
PWMOut motor_pwm_a(PB_1);      // PWM1  -> JP1a
DigitalOut motor_dir_a(PB_14);      // DIR   -> JP1a
DigitalOut motor_pwm_b(PB_15);      // PWM2  -> JP1a
DigitalOut motor_dir_b(PB_13);      // DIR   -> JP1a
DigitalOut motor_bipolar_a(PC_8);     // Bipolar -> JP1a
DigitalOut motor_bipolar_b(PC_6);     // Bipolar -> JP1a

int main() {
    // Start with everything off
    motor_enable = 0;

    wait(1.0);   // 1 second delay

    // Turn motor fully on
    motor_enable = 1;
    motor_pwm_a.period_us(100); //Each pulse lasts 0.1ms
    motor_pwm_a.pulsewidth_us(75); //75us running, 25us not running       

    while (true) {
        //Make it run for ever
    }
}