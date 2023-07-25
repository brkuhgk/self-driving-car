C++ implementation of a library for controlling the MCP4251 digital potentiometer using the SPI communication protocol. The MCP4251 is a dual-channel digital potentiometer, meaning it has two separate potentiometer channels.

The library includes functions to perform various operations on the MCP4251, such as setting the wiper position, reading wiper position, connecting/disconnecting terminals, and more. Let's go through the code and explain each function:

1. `MCP4251::MCP4251(uint8_t slaveSelectPin, float pot0ResistanceRmax, float pot0ResistanceRmin, float pot1ResistanceRmax, float pot1ResistanceRmin)`
   - Constructor for the MCP4251 class. It is used to create an instance of the class and set initial values for the MCP4251 parameters.

2. `void MCP4251::begin()`
   - Initializes the MCP4251 by configuring the SPI communication and setting some initial parameters.

3. `void MCP4251::DigitalPotWiperIncrement(bool potNum)`
   - Increments the wiper position of the specified potentiometer channel.

4. `void MCP4251::DigitalPotWiperDecrement(bool potNum)`
   - Decrements the wiper position of the specified potentiometer channel.

5. `void MCP4251::DigitalPotSetWiperPosition(bool potNum, unsigned int value)`
   - Sets the wiper position of the specified potentiometer channel to the given value.

6. `void MCP4251::DigitalPotSetWiperMin(bool potNum)`
   - Sets the wiper position of the specified potentiometer channel to the minimum value.

7. `void MCP4251::DigitalPotSetWiperMax(bool potNum)`
   - Sets the wiper position of the specified potentiometer channel to the maximum value.

8. `void MCP4251::DigitalPotSetWiperMid(bool potNum)`
   - Sets the wiper position of the specified potentiometer channel to the middle value.

9. `uint16_t MCP4251::DigitalPotReadWiperPosition(bool potNum)`
   - Reads the wiper position of the specified potentiometer channel and returns it.

10. `uint16_t MCP4251::DigitalPotReadStatusRegister()`
    - Reads the status register of the MCP4251 and returns its value.

11. `uint16_t MCP4251::DigitalPotReadTconRegister()`
    - Reads the terminal control register (TCON) of the MCP4251 and returns its value.

12. `void MCP4251::DigitalPotWriteTconRegister(uint16_t value)`
    - Writes the given value to the terminal control register (TCON) of the MCP4251.

13. `void MCP4251::DigitalPotStartup(bool potNum)`
    - Configures the startup state of the specified potentiometer channel.

14. `void MCP4251::DigitalPotShutdown(bool potNum)`
    - Configures the shutdown state of the specified potentiometer channel.

15. `void MCP4251::DigitalPotTerminalBConnect(bool potNum)`
    - Connects terminal B of the specified potentiometer channel.

16. `void MCP4251::DigitalPotTerminalBDisconnect(bool potNum)`
    - Disconnects terminal B of the specified potentiometer channel.

17. `void MCP4251::DigitalPotTerminalAConnect(bool potNum)`
    - Connects terminal A of the specified potentiometer channel.

18. `void MCP4251::DigitalPotTerminalADisconnect(bool potNum)`
    - Disconnects terminal A of the specified potentiometer channel.

19. `void MCP4251::DigitalPotWiperConnect(bool potNum)`
    - Connects the wiper terminal of the specified potentiometer channel.

20. `void MCP4251::DigitalPotWiperDisconnect(bool potNum)`
    - Disconnects the wiper terminal of the specified potentiometer channel.

21. `void MCP4251::DigitalPotInitTcon()`
    - Initializes the terminal control register (TCON) of the MCP4251.

22. `uint16_t MCP4251::DigitalPotResistanceToPosition(bool potNum, float resistance)`
    - Converts a given resistance value to the equivalent wiper position for the specified potentiometer channel.

23. `float MCP4251::DigitalPotPositionToResistance(bool potNum, uint16_t position)`
    - Converts a given wiper position value to the equivalent resistance for the specified potentiometer channel.

