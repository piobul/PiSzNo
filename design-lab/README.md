Project: [PISZNO] ??? Progress and Current Status:

During the first two weeks of the project, we focused on gathering and documenting the essential information regarding the hardware utilized. Following the instructor's guidelines, we located and uploaded the official technical documentation (Datasheets, User Guides, etc.) for the following components to the repository:
- ATMEGA328PB Xplained Mini: The main microcontroller platform.
- USB-UART converter module (PL2303): The chip enabling serial communication.
- 1602 LCD Display (based on the Hitachi HD44780 controller): An alphanumeric screen for data presentation.

This documentation serves as a necessary foundation for further software development and understanding the principles of operation for each element.
We also focused on practical hardware and software setup during Weeks 1 and 2, which included:
- Starting the ATmega328 boards.
- Connecting the 7-segment display and LED bar.
- Developing our first program, which features:
    An incrementing counter from 1-100, displaying the value on the screen.
    Logic that, after "turning" the counter, lights up the next LED on the bar and restarts the counting.
    Logic that turns off the LEDs in reverse order after the bar is completely lit.
- Beginning to draw the schematic diagram in KiCad, including the processor, display, and 8 LEDs.

Looking ahead to Weeks III-IV, our tasks include implementing data sending to a PC via the UART/USB port and expanding our KiCad schematic with the PL2303GT chip, which is now documented in our repository.

Future Plans: Interdisciplinary Integration
The current project serves as a starting point for a broader, integrated initiative. Our goal is to combine the work done so far into one cohesive system that will cover topics from three related subjects:
1. Design Lab: Within this subject, we will focus on the system architecture, integration of components (ATmega328PB, UART module, LCD), and the overall design of the device.
2. Komputerowe systemy pomiarowe (Computer Measurement Systems): The project will be used to acquire, process, and present measurement data using the assembled hardware.
3. Systemy i sieci telekomunikacyjne (Telecommunication Systems and Networks): We will focus on the data transmission aspect, likely utilizing the USB-UART module and appropriate communication protocols to transmit the gathered information to external systems.
   
This integration will allow us to create a comprehensive solution, combining circuit design, data acquisition, and network communication.

