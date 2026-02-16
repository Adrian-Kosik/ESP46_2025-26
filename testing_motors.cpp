#include "mbed.h" //https://os.mbed.com/docs/mbed-os/v6.16/debug-test/visual-studio-code.html

// This code is for testing the motor control on the board. Motor A is the left one, B is right
DigitalOut motor_enable(PB_2);          // Enable motors to run
PwmOut motor_pwm_a(PB_1);               // PWM for motors
PwmOut motor_pwm_b(PB_15);              
DigitalOut motor_dir_a(PB_14);          // Direction of movement
DigitalOut motor_dir_b(PB_13);          
DigitalOut motor_bipolar_a(PC_8);       // Bipolar or Unipolar mode
DigitalOut motor_bipolar_b(PC_6);       

int main() {
    // Start with everything off
    motor_enable = 0;

    wait(1.0);   // 1 second delay

    // Turn motor fully on
    motor_enable = 1;
    motor_pwm_a.period_us(100); //Each pulse lasts 0.1ms
    motor_pwm_b.period_us(100); //Each pulse lasts 0.1ms

    //Set the motor to run in unipolar mode
    motor_bipolar_a = 0;
    motor_bipolar_b = 0;
    
    //Set the directions to go forward
    motor_dir_a = 1; //goes back
    motor_dir_b = 0; //goes forward
    
    while (true) {
        //Make it run for ever
        motor_pwm_a.pulsewidth_us(75); //75us running, 25us not running  
        motor_pwm_b.pulsewidth_us(75); //75us running, 25us not running       

        wait(2);

        motor_pwm_a.pulsewidth_us(25);      
        motor_pwm_b.pulsewidth_us(25); 

        wait(2);
    };
}