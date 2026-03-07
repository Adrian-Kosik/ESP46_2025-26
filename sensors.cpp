#include "mbed.h" //https://os.mbed.com/docs/mbed-os/v6.16/debug-test/visual-studio-code.html
#include "C12832.h" //Library to control shield board
#include "QEI.h"  //Library to read motor encoders
#include "math.h"

AnalogIn Pin0(A0), Pin1(A1), Pin2(A2), Pin3(A3), Pin4(A4), Pin5(A5);
DigitalOut control1(PC_10), control2(PC_12);


Serial pc(USBTX, NC); // Creates an instance of a Serial Connection with default parameters
// NC is used as this example uses simplex link where the PC only receives data
InterruptIn button(D4); // Create an InterruptIn associated with the centre pin of the joystick
volatile bool trigger_flag = false; // A volatile boolean flag to indicate when our ISR has triggered.
void buttonISR() // The ISR for the InterruptIn. This function has minimal code for efficiency
{
 trigger_flag=true; // A single flag is set to indicate that the button has been pressed.
}

// class Sensor {
//     private:
//     AnalogIn inputSignal;
//     DigitalOut control;
    
//     public:
//         Sensor(PinName sensor, PinName light) : inputSignal(sensor), control(light) {}

//         float Brightness(void)
//         {
//             control = 1;
//             wait(0.05);
//             return inputSignal.read();
//             control = 0;
//         }
//         void sample(void)
//         {
            
//         }
// };

// class SamplingSensor : public Sensor{
//     private:
//         float samplingFrequency, samplingPeriod;
//         Ticker sampler;

//     public:
//         SamplingSensor (PinName i, PinName o, float fs) : 
//         Sensor (i, o), samplingFrequency(fs) {
//             samplingPeriod = 1.0f / samplingFrequency;
//             sampler.attach(callback(this, &Sensor::sample), samplingPeriod);
//         }
// };

// SamplingSensor Sens0(A0, PC_10, 20);
// SamplingSensor Sens1(A1, PC_12, 20);
// SamplingSensor Sens2(A2, PC_10, 20);
// SamplingSensor Sens3(A3, PC_12, 20);
// SamplingSensor Sens4(A4, PC_10, 20);
// SamplingSensor Sens5(A5, PC_12, 20);





int main() {
    pc.printf("Microcontroller program running...\r\n");
    
    float Output[6];
    float Darkest[2]; //0:Pin number, 1:pin value
    while(1){
    control1 = 1;
    control2 = 0;
    wait(0.1);
    Output[1] = Pin1.read();
    Output[3] = Pin3.read();
    Output[5] = Pin5.read();
    wait(0.5);

    control1 = 0;
    control2 = 1;
    wait(0.1);
    Output[0] = Pin0.read();
    Output[2] = Pin2.read();
    Output[4] = Pin4.read();

    
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
    wait(0.5);

    }
}
