//Based on 
//https://forum.arduino.cc/t/arduino-due-adc-dma-channel-ordering-in-buffer/620520

volatile bool flag;

int led_pin = 2;
int cnt;
int a0;
void setup() {  
  pinMode(led_pin, OUTPUT);
  pinMode(A0, INPUT);
  SerialUSB.begin(1000000);

  while (!SerialUSB);
  SerialUSB.write("SerialUSB initialized.");

  ADC->ADC_MR |= 0x80; // these lines set free running mode on adc 7 (pin A0)
  ADC->ADC_CR=2;
  ADC->ADC_CHER=0x80;

  //PWM counter and flag
  cnt = 1;
  flag = true;
}

void loop() {
  if (flag == true){
    PIOB->PIO_SODR |= 0x1<<25;
  }else{
    PIOB->PIO_CODR |= 0x1<<25; 
  }
  
  cnt++;
  if (cnt == 15){
    flag = false;
  }
  if (cnt == 20){
    flag = true;
    cnt = 1;
  }
  
  while((ADC->ADC_ISR & 0x80)==0); // wait for conversion
  a0=ADC->ADC_CDR[7];              // read data
  SerialUSB.println(a0);
}
