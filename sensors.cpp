#include "mbed.h" //https://os.mbed.com/docs/mbed-os/v6.16/debug-test/visual-studio-code.html
#include "QEI.h"  //Library to read motor encoders
#include "math.h"


Serial pc(USBTX, NC); // Creates an instance of a Serial Connection with default parameters
// NC is used as this example uses simplex link where the PC only receives data

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
            wait(0.02);
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

#define SAMPFREQ 4
SamplingSensor Sens0(PA_0, PA_5, SAMPFREQ, 0);
SamplingSensor Sens1(PA_1, PA_6, SAMPFREQ, 1);
SamplingSensor Sens2(PA_4, PA_7, SAMPFREQ, 2);
SamplingSensor Sens3(PB_0, PB_6, SAMPFREQ, 3);
SamplingSensor Sens4(PC_1, PC_7, SAMPFREQ, 4);
SamplingSensor Sens5(PC_0, PA_9, SAMPFREQ, 5);



int main() {
    pc.printf("Microcontroller program running...\r\n");
    
    while(1){        
       
    pc.printf("values: ");
    
    //calculate which pin(1-4) has highest value
    for(int i = 1; i<5; i++){
        pc.printf("%.2f, ", Output[i]);
        i
    }

    pc.printf("\r\n");


    }
}
