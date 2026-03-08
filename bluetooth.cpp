#include "mbed.h"
#include "C12832.h"

Serial hm10(PA_11, PA_12);
Serial pc(USBTX, USBRX);
C12832 lcd(PA_7, PA_5, PA_6, PA_8, PB_6);
DigitalOut LED(D8);

char box[20];
int index = 0;

int main() {
    hm10.baud(9600);
    while (!hm10.writeable()) { }  // wait until HM10 is ready
    
    while (1) {
        if (hm10.readable()) {
            char s = hm10.getc();

            if (s == '1') {
                LED = 1;
                continue;
            }
            if (s == '0') {
                LED = 0;
                continue;
            }

            if (s == '2') {
                hm10.printf("Recieved number 2!\n");
                continue;
            }

            if (s == '\n' || s == '\r') {
                if (index > 0) {
                    box[index] = '\0';
                    lcd.cls();
                    lcd.locate(1,1);
                    lcd.printf("%s", box);

                    hm10.printf("You sent: %s\n", box);
                    index = 0;
                }
                continue;
            }
            if (index < sizeof(box) - 1) {
                box[index++] = s;
            }
        }
    }
}
