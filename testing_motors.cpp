#include "mbed.h" //https://os.mbed.com/docs/mbed-os/v6.16/debug-test/visual-studio-code.html
#include "C12832.h" //Library to control shield board
#include "QEI.h"  //Library to read motor encoders


// This code is for testing the motor control on the board. It will turn the motor on at full speed and keep it running indefinitely.
DigitalOut motor_enable(PB_2);   // Enable 
PwmOut motor_pwm_a(PA_11);      // PWM1 
PwmOut motor_pwm_b(PB_15);      // PWM2  
DigitalOut motor_dir_a(PB_14);      // DIR   
DigitalOut motor_dir_b(PB_13);      // DIR   
DigitalOut motor_bipolar_a(PC_8);     // Bipolar
DigitalOut motor_bipolar_b(PC_6);     // Bipolar 

C12832 lcd(PA_7, PA_5, PA_6, PA_8, PB_6);

class Potentiometer { //Begin updated potentiometer class definition
    private: //Private data member declaration
    AnalogIn inputSignal;
        //Declaration of AnalogIn object
        float VDD, currentSampleNorm, currentSampleVolts; //Float variable to speficy the value of VDD (3.3 V for the Nucleo-64)
    
    public: // Public declarations
        Potentiometer(PinName pin, float v) : inputSignal(pin), VDD(v) {}
        //Constructor - user provided pin name assigned to AnalogIn...
        //VDD is also provided to determine maximum measurable voltage
        float amplitudeVolts(void) //Public member function to measure the amplitude in volts
        {
            return (inputSignal.read()*VDD); //Scales the 0.0-1.0 value by VDD to read the input in volts
        }
        float amplitudeNorm(void) //Public member function to measure the normalised amplitude
        {
            return inputSignal.read(); //Returns the ADC value normalised to range 0.0 - 1.0
        }
        void sample(void) //Public member function to read a sample and store the value as data members
        {
            currentSampleNorm = inputSignal.read(); //Read a sample from the ADC and store normalised representation [0..1]
            currentSampleVolts = currentSampleNorm*VDD; //Convert this to a voltage and store that as a data member too.
        }
        const float getCurrentSampleNorm(void) //Public member function to return the most recent normalised sample [0..1]
        {
            return currentSampleNorm; //Return the most recent normalised sample
        }
        const float getCurrentSampleVolts(void) //Public member function to return the most recent sampled voltage [0.. 3.3 V]
        {
            return currentSampleVolts; //Return the most recent sampled voltage
        }
};

class SamplingPotentiometer : public Potentiometer {
    private:
        float samplingFrequency, samplingPeriod;
        Ticker sampler;

    public:
        SamplingPotentiometer(PinName p, float v, float fs) :
        Potentiometer(p, v), samplingFrequency(fs) {
            samplingPeriod = 1.0f / samplingFrequency;
            sampler.attach(callback(this, &Potentiometer::sample), samplingPeriod);
        }
};

SamplingPotentiometer potL1(PA_0, 3.3, 20);
SamplingPotentiometer potR1(PA_1, 3.3, 20);

class Joystick { //Class copied from lecture notes and then built on top of to make it work
    private: 
        DigitalIn up, down, left, right, fire; 
    public: 
        Joystick(PinName u, PinName d, PinName l, PinName r, PinName f) : //Joystick class constructor that will collect user entered pin values to use
            up(u), down(d), left(l), right(r), fire(f) {}; 
        bool upPressed(void){ //Creates a boolean member function that will return true when the up pin is toggled, and false otherwise
            if(up==1) {
                return true;
            } else
                return false; }
        bool downPressed(void){
            if(down==1) {
                return true;
            } else
                return false; }
        bool leftPressed(void){
            if(left==1) {
                return true;
            } else
                return false; }
        bool rightPressed(void){
            if(right==1) {
                return true;
            } else
                return false; }
        bool firePressed(void){
            if(fire==1) {
                return true;
            } else
                return false; }
};
Joystick stick(PA_4, PB_0, PC_1, PC_0, PB_5);

// Using QEI class
QEI encoder_A(PC_10, PC_12, NC, 512);
QEI encoder_B(PA_13, PA_14, NC, 512);

int main() {
    // Start with everything off
    motor_enable = 0;

    // Turn motor fully on
    
    motor_pwm_a.period_us(100); //Each pulse lasts 0.1ms
    motor_pwm_b.period_us(100); //Each pulse lasts 0.1ms

    //Set the motor to run in unipolar mode
    motor_bipolar_a = 0;
    motor_bipolar_b = 0;
    
    //Set the directions to go forward
    motor_dir_a = 1; //goes back
    motor_dir_b = 1;

    while (true) {
        //Make it run for ever

        if (stick.firePressed()==1) {
            motor_enable = !motor_enable;
            wait(0.5);
        };

        //Print the potentiometer values 
        lcd.locate(1,1);
        lcd.printf("%.2f and %.2f", potL1.amplitudeNorm(), potR1.amplitudeNorm());   
        
        //Print the encoder values
        lcd.locate(1, 10);
        lcd.printf("%d and %d", encoder_A.getPulses()/512, encoder_B.getPulses()/512);

        motor_pwm_a = potL1.amplitudeNorm();
        motor_pwm_b = potR1.amplitudeNorm();
    
        // wait(0.1);
    };
}