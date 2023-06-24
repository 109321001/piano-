
#include <8051.h>

unsigned char Pressed();
void sound(unsigned char n);
void display(unsigned char n);

__xdata unsigned char song;
const unsigned short node[8] = {63628,63836,64021,64104, 64260,64399,64523,64580};
const unsigned short light[8] = {0b11000000, 0b11111001, 0b10100100, 0b10110000, 0b10011001, 0b10010010, 0b10000010, 0b11011000};

unsigned char node_n, de = 0, size = 0, i;

int main(void)
{
    TMOD = 0x11;         
	TR0 = 1;             //open timer0
    TR1 = 1;
    IE = 0x8A;

    unsigned char tmp = 17, mode = 0;

    while (1) {

        if(de > 8){
            de = 0;
            tmp = Pressed();
        }

        if(tmp > 10){
            ET0 = 0; 
        }
        else if(tmp == 8){
            mode = 1;
        }
        else if(tmp == 9){
            mode = 0;
        }
        else if(tmp == 10){
            mode = 2;
        }
        else{
            display(tmp);
            sound(tmp);
        }


        if(mode == 1){
            // P3 = 1;
            // P2 = 0b11111001;

            P1_1 = 1;

            if(tmp < 8){
                display(tmp);
                song = tmp;
                size++;
            }

        }
        else if(mode == 2){

            sound(song);

            size = 0;
        }
        else{
            P1_1 = 0;          
        }

	}

}

void sound(unsigned char n){ 
    ET0 = 1;  
    node_n = n;
}

void display(unsigned char n){
    P3 = 1;
    P2 = light[n];
    for (unsigned i = 0; i < 1024; i++){

    }
}


void timer0_isr(void)__interrupt(1)__using(1) {
	TH0 = node[node_n] >> 8;
    TL0 = node[node_n] & 0xff;
    P1_0 = !P1_0;
}

void timer1_isr(void)__interrupt(3)__using(2) { //change
	TH1 = 15536 >> 8;
	TL1 = 15536 & 0xff;
	de++;
}

unsigned char Pressed() {
	for (unsigned char c1=0; c1 < 4; c1++) { //column
		P0 = ~(0x10 << c1);
		if ((P0 & 0x0f) != 0x0f) {
			for (unsigned char c2 = 0; c2 < 4; c2++) { //row
				P0 = ~(0x01 << c2);
				if ((P0 & 0xf0) != 0xf0)
					return  c1 * 4 + c2;
			}
		}
	}
	return 17;
}

