#include "mbed.h" //https://os.mbed.com/docs/mbed-os/v6.16/debug-test/visual-studio-code.html
#include "C12832.h" //Library to control shield board
#include "QEI.h"  //Library to read motor encoders
#include "math.h"


Serial pc(USBTX, NC); // Creates an instance of a Serial Connection with default parameters
// NC is used as this example uses simplex link where the PC only receives data
InterruptIn button(D4); // Create an InterruptIn associated with the centre pin of the joystick
volatile bool trigger_flag = false; // A volatile boolean flag to indicate when our ISR has triggered.
void buttonISR() // The ISR for the InterruptIn. This function has minimal code for efficiency
{
 trigger_flag=true; // A single flag is set to indicate that the button has been pressed.
}
float Output[6];
class Sensor {
    private:
    AnalogIn inputSignal;
    DigitalOut control;
    int i;
    float value;
    
    public:
        Sensor(PinName sensor, PinName light, int i) : inputSignal(sensor), control(light), i(i) {}

        void GetValue(void)
        {
            control = 1;
            // wait(0.05);
            Output[i] = inputSignal.read();
            control = 0;
            return;
            
        }
};

class SamplingSensor : public Sensor{
    private:
        float samplingFrequency, samplingPeriod;
        Ticker sampler;

    public:
        SamplingSensor (PinName sens, PinName light, float fs, int i) : 
        Sensor (sens, light, i), samplingFrequency(fs) {
            samplingPeriod = 1.0f / samplingFrequency;
            sampler.attach(callback(this, &Sensor::GetValue), samplingPeriod);
        }
};

#define SAMPFREQ 2
//SamplingSensor Sens0(A0, PC_10, SAMPFREQ, 0);
SamplingSensor Sens1(A1, PC_10, SAMPFREQ, 1);
SamplingSensor Sens2(A2, PC_12, SAMPFREQ, 2);
SamplingSensor Sens3(A3, PC_10, SAMPFREQ, 3);
SamplingSensor Sens4(A4, PC_12, SAMPFREQ, 4);
//SamplingSensor Sens5(A5, PC_12, SAMPFREQ, 5);





int main() {
    pc.printf("Microcontroller program running...\r\n");
    
    //float Output[6];
    float Darkest[2]; //0:Pin number, 1:pin value
    while(1){        
       
    pc.printf("values: ");
    Darkest[0] = 0; 
    Darkest[1] = 0;
    //calculate which pin(1-4) has highest value
    for(int i = 1; i<5; i++){
        pc.printf("%.2f, ", Output[i]);
        if(Output[i]>Darkest[1]){
            Darkest[1] = Output[i];
            Darkest[0] = i;
        }
    }

    pc.printf("  Darkest value is pin %.0f \r\n", Darkest[0]);


    }
}
