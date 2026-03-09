#include "mbed.h"
#include "C12832.h"

Serial hm10(PA_11, PA_12);
Serial pc(USBTX, USBRX);
C12832 lcd(PA_7, PA_5, PA_6, PA_8, PB_6);
DigitalOut LED(D8), LED_2(D9);

char box[20];
int index = 0;
bool authenticated = false;

int main() {
    LED = 1; LED_2 = 1;
    hm10.baud(9600);
    while (!hm10.writeable()) { }  // wait until HM10 is ready
    hm10.printf("Code: \n");

    while (1) {
        if (hm10.readable()) {

            while(authenticated == false) {
                char c = hm10.getc();
                if (c == '='){
                    authenticated = true;
                    hm10.printf("Guess you figured it out\n");
                    continue;
                }
                else {
                    hm10.printf("Wrong password. Screw you.\n");
                    continue;
                }
            }
            
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
                    
                    //Command prompts
                    if (strcmp(box, "bob") == 0){
                        LED_2 = 1;
                    } else if (strcmp(box, "bab") == 0){
                        LED_2 = 0;
                    } else if (strncmp(box, "lm", 2) == 0) {
                        int value = atoi(box+2);
                        hm10.printf("LM command set to = %d\n", value);
                    }
                    
                    index = 0;
                }
                continue;
            }
            if (index < sizeof(box) - 1) {
                box[index++] = s;
                continue;
            }
        }
    }
}
