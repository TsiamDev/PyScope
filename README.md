# PyScope
This is a rudimentary oscilloscope built with python for Arduino (DUE)

Sample Output


![Παλμογράφος](https://user-images.githubusercontent.com/56920806/165308068-2d1d19a3-3057-49af-8ddb-01bc1efa9bbf.png)

PyScope - Introduction
So, the other day I was playing around with a pet project involving electronics. And I came upon a situation where I needed to generate a very fast PWM pulse (with a frequency of 1MHz).
Thus, I came upon two problems.
1.	Generate the pulse, and
2.	check to see if it the pulse is generated correctly.

# Arduino Code

To tackle the first problem, I utilized an Arduino Due that I had lying around. The problem was that digitalWrite(), digitalRead() and analogRead() were too slow for my purposes. And so, I set up, and then, directly wrote to the PIO registers of the Atmel Chip.
First, initialize digital pin 2 as output:

 ![3](https://user-images.githubusercontent.com/56920806/167269602-18969c61-f80d-4f48-8f1f-2d95f6594383.png)

Then, write the corresponding value (HIGH, or LOW) to the pin, based on the duty cycle defined by the <cnt > counter:
 
 ![4](https://user-images.githubusercontent.com/56920806/167269606-dd1a2ade-0f16-4c9e-b4a5-8a1d003faa9e.png)

The bottleneck happened when reading the pin that generated the pulse. I modified somewhat (rather, I stripped down) the code that I found here:
https://forum.arduino.cc/t/arduino-due-adc-dma-channel-ordering-in-buffer/620520   
First, set up the ADC on pin A0:
 
 ![1](https://user-images.githubusercontent.com/56920806/167269596-9d86f326-c991-438b-8828-97d59e159b78.png)
 

Then, simply wait until the ADC conversion happens. Finally, read the data from pin A0:
 
![2](https://user-images.githubusercontent.com/56920806/167269600-a2a1d68d-89e2-40e1-956f-8a2b8b82cc65.png)
 
Problem (1) solved!
 
# Python Osciloscope
 
And now, off to the second problem. How do I visualize correctly the data that ADC read? Well, with the trusty contemporary multitool of course! Which is none other than Python.
The code I wrote is rather simple and most of it is for visualization purposes rather than calculations.
Firstly, we open a connection to the Serial Port that our Arduino is connected to, we set the buffer size and clear the initial (garbage) input data:
 
 ![5](https://user-images.githubusercontent.com/56920806/167269613-6f462956-65c1-4a44-a88d-beb96250acff.png)

Afterwards, we read the input data whenever a threshold (in bytes) is reached 
 
  ![6](https://user-images.githubusercontent.com/56920806/167269616-3ee3e40b-28ba-412f-9219-cd41b31a6bcf.png)
 
Then, we convert the read data to a python string and parse it (while surrounding the code with a <try> block to catch the times when input is malformed, or some other type of exception is thrown)

 ![7](https://user-images.githubusercontent.com/56920806/167269624-3783dedc-ae9d-47ed-be2b-dd0acc38ce16.png)
 
If the resulting list is not empty, we convert it to a list of integers, and finally we read the voltage amplitude with the calculation at line 81 (3.3 volts is the digital HIGH on Due and 4095 is the resolution of the ADC). Int_data now has the measured voltages. 
 ![8](https://user-images.githubusercontent.com/56920806/167269629-72d76aec-ad65-4564-9835-18e15d03a10c.png)

The final part of the calculations, concerns triggering. To do this, we search the converted array to find the first time where the voltage goes from LOW to HIGH, and toss the previous samples.
Int_data is now triggered. Pun intended 😊

![9](https://user-images.githubusercontent.com/56920806/167269787-e0e87991-037a-45de-8922-5ed27169f389.png)

The rest of the python code utilizes the matplotlib library to render the information to the screen and I consider it to be self-explanatory. Nevertheless, If you feel it should be further explained drop me  a line and I will remedy that 😊.
Oh! Also don’t forget! At the end, we close the port we opened.
Here is a sample output!
 
 ![scope_demo](https://user-images.githubusercontent.com/56920806/167269790-a3521bcc-75bc-4802-9ed5-b66e9f8ad285.png)

# Till next time, have a good one!

