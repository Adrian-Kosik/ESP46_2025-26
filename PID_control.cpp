//Yi will work on this code

#include "mbed.h"
#include "sensors.cpp"


#include "mbed.h"
#include "C12832.h"
#include <math.h>

//for simulation
PwmOut red_led(D5);
PwmOut blue_led(D8);
PwmOut green_led(D9);//for simulated，Left0 is BLUE,R0 is RED, Stop is Green, max v in straight is purple, not max v is flashing
//class SimulatedControl{
    //private:
    //    PwmOut red_led;
    //    PwmOut blue_led;
    //    PwmOut green_led;//for PWM Logic
    //public:
    //    SimulatedControl(PinName redPin, PinName bluePin, PinName greenPin)//, float L, float R, float T)
    //    : red_led(redPin), blue_led(bluePin), green_led(greenPin){        
    //    };
//};
//SimulatedControl(D5,D8,D9,0.9,0.9,1.0);

//条件一，传感器
//假设1：AtoD 假定大于4v为黑（4-5v），小于1v为白（0-1v），中间值说明踩线 translate below
//Assume, 4-5v is dark for sure, 0-1v is white for sure
//AnalogIn s1(A0), s2(A1), s3(A2), s4(A3), s5(A4), s6(A5);Sensors pin？
//float Sensor[6] = {0};
//void readSensors(){Sensor[0] = s1.read();Sensor[1] = s2.read();Sensor[2] = s3.read();Sensor[3] = s4.read();Sensor[4] = s5.read();Sensor[5] = s6.read();}
//需要调整为0-5v:Need to change the value we read into 0-5v
//use potentiometer
class Potentiometer                                                     //Begin updated potentiometer class definition
{
private:                                                                //Private data member declaration
    AnalogIn inputSignal;                                               //Declaration of AnalogIn object
    float VDD, currentSampleNorm, currentSampleVolts;                   //Float variable to speficy the value of VDD (3.3 V for the Nucleo-64)
public:                                                                 // Public declarations
    Potentiometer(PinName pin, float v) : inputSignal(pin), VDD(v) {}   //Constructor - user provided pin name assigned to AnalogIn...
                                                                        //VDD is also provided to determine maximum measurable voltage
    float amplitudeVolts(void)                                          //Public member function to measure the amplitude in volts
    {
        return (inputSignal.read()*VDD);                                //Scales the 0.0-1.0 value by VDD to read the input in volts
    }
    float amplitudeNorm(void)                                           //Public member function to measure the normalised amplitude
    {
        return inputSignal.read();                                      //Returns the ADC value normalised to range 0.0 - 1.0
    }
    void sample(void)                                                   //Public member function to read a sample and store the value as data members
    {
        currentSampleNorm = inputSignal.read();                         //Read a sample from the ADC and store normalised representation [0..1]
        currentSampleVolts = currentSampleNorm*VDD;                     //Convert this to a voltage and store that as a data member too.
    }
    const float getCurrentSampleNorm(void)                              //Public member function to return the most recent normalised sample [0..1]
    {
        return currentSampleNorm;                                       //Return the most recent normalised sample
    }
    const float getCurrentSampleVolts(void)                             //Public member function to return the most recent sampled voltage [0.. 3.3 V]
    {
        return currentSampleVolts;                                      //Return the most recent sampled voltage
    }
};
class SamplingPotentiometer: public Potentiometer{
    private:
        float samplingFrequency, samplingPeriod;
        Ticker sampler;
    public: 
        SamplingPotentiometer(PinName pin, float v, float fs): Potentiometer(pin, v), samplingFrequency(fs){
            samplingPeriod = 1.0f / samplingFrequency;
            sampler.attach(callback(this, &Potentiometer::sample), samplingPeriod);//this: pointer, adress--&; 
        };
};
SamplingPotentiometer pot1(A0,3.3f,100);
SamplingPotentiometer pot2(A1,3.3f,100);
//end potentiometer

float clamp(float a, float b, float c){
    if (a>c){
        return c;
    }else if (a<b){
        return b;
    }else{
        return a;
    }
}

int main()
{
    float T=1.0;//后续应该需要调整，1s太大了
    red_led.period(T);
    blue_led.period(T);//decide duty cycle
    green_led.period(T);

    //Given Sensors 0-5v （Sam会给定Sensors）Assume：
    //注释是走直线的理想情况Annotated the ideal scenario of moving in a straight line
    float sensor[6];//assume 4-5 is black 100%; 0-1 is white 100%
    sensor[0] = 4.5f;//4-5.0f
    sensor[1] = 4.5f;//4-5.0f
    sensor[2] = 4.5f;//0-1f
    sensor[3] = 4.5f;//0-1f
    sensor[4] = 2.5f;//4-5.0f
    sensor[5] = 0.5f;//4-5.0f


    while (1) {
        //Sensor：计算权重
        //error：1.计算白线强度；2.给定不同传感器的权值；3.结合白线强度加权平均
        //clamp：使得越界的强制收回
        float WC[6];//white centre, 0 is black, 1 is white
        int BW=0;//裁定是否全黑需要停下,只要有白线就+1，需要在每一个采样周期处理
        for (int i=0; i <= 5; i++){
            if (sensor[i]>=4.0f){//参考DR2实验记录，这里当作sensor高度为5mm，但是实际上如果是10mm需要调整
                WC[i]= 0.0f;
            }else if(sensor[i]<=0.3f){
                WC[i]= 1.0f;
                BW=BW+1;
            }else {
                WC[i]=1-sensor[i]/5;
                BW=BW+1;
            }
        }
        //Error: Assume the weight:-3; -2; -1; 1; 2; 3
        float er=0;
        er=(-3)*WC[0]+(-2)*WC[1]+(-1)*WC[2]+(1)*WC[3]+(2)*WC[4]+(3)*WC[5];
        
        float kp, ki, kd;//set， assume
        kp=1; ki=1; kd=1;
        float u;
        //asume dt is 0.001s 1000Hz
        缺少u的离散表达式，完成后PID控制应当完成，只需要调参；停止已完成；考虑H-bridge，是否引入反转倒车

        //如果全黑了（终点）是不是应该让L，R都为0？后续考虑是否会超过终点范围需要后退（暂时不考虑）（可以限定时间范围（余裕）防止）
        float L=0;
        float R=0;
        //如果偏左（应该右转），此时sensor6读取1，e>0, u>0，此时L应该发力，L-u，R应该降速，R+u，因为0速度最快
        L=clamp(L-u,0,0.9);
        R=clamp(R+u,0,0.9);

        //readSensors();for sensors
        L = 0.9f * pot1.getCurrentSampleNorm();
        R = 0.9f * pot2.getCurrentSampleNorm();
        if(BW!=0){L=0.9;R=0.9;};//强制停下，如果没有白线了
        //Set
        float DLon=(0.9-L)/0.9;//Duty Cycle L on
        float DRon=(0.9-R)/0.9;
        //PWM Logic
            if(L>=0.899 and R>=0.899){blue_led.write(1.0f);red_led.write(1.0f);green_led.write(0.0f);}
            else{blue_led.write(DLon);red_led.write(DRon);green_led.write(1.0f);}
            //L，R为0时停下
        //这样就完成了给定LR，就能控制速度，接下来的问题是如何读取Sensor并返回一个恰当的LR参数

        

    }
}
