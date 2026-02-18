#include "mbed.h" //https://os.mbed.com/docs/mbed-os/v6.16/debug-test/visual-studio-code.html
#include "C12832.h"

// This code is for testing the motor control on the board. It will turn the motor on at full speed and keep it running indefinitely.
DigitalOut motor_enable(PB_2);   // Enable 
PwmOut motor_pwm_a(PB_1);      // PWM1 
PwmOut motor_pwm_b(PB_15);      // PWM2  
DigitalOut motor_dir_a(PB_14);      // DIR   
DigitalOut motor_dir_b(PB_13);      // DIR   
DigitalOut motor_bipolar_a(PC_8);     // Bipolar
DigitalOut motor_bipolar_b(PC_6);     // Bipolar 

C12832 lcd(D11, D13, D12, D7, D10);

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

SamplingPotentiometer potL1(A0, 3.3, 20);
SamplingPotentiometer potR1(A1, 3.3, 20);

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
    motor_dir_b = 0;
    
    while (true) {
        //Make it run for ever

        int val_x = potL1.amplitudeNorm()*100;
        int val_y = potR1.amplitudeNorm()*100;

        motor_pwm_a.pulsewidth_us(val_x); //75us running, 25us not running  
        motor_pwm_b.pulsewidth_us(val_y); //75us running, 25us not running  

        lcd.locate(1,10);
        lcd.printf("%d and %d", val_x, val_y);     

        wait(0.1);
    };
}