#include "MCP4251.h"
#include <SPI.h>

// Constructor for the MCP4251 class
MCP4251::MCP4251(uint8_t slaveSelectPin, float pot0ResistanceRmax, float pot0ResistanceRmin, float pot1ResistanceRmax, float pot1ResistanceRmin)
{
    this->_slaveSelectPin = slaveSelectPin;

    this->_pot0ResistanceRmax = pot0ResistanceRmax;
    this->_pot0ResistanceRmin = pot0ResistanceRmin;
    this->_pot0ResistanceRAB = pot0ResistanceRmax - pot0ResistanceRmin;
    this->_pot0ResistanceRW = pot0ResistanceRmin;

    this->_pot1ResistanceRmax = pot1ResistanceRmax;
    this->_pot1ResistanceRmin = pot1ResistanceRmin;
    this->_pot1ResistanceRAB = pot1ResistanceRmax - pot1ResistanceRmin;
    this->_pot1ResistanceRW = pot1ResistanceRmin;
}

// Initialize the MCP4251
void MCP4251::begin()
{
    // Set the slave select pin as an output and disable the chip by default
    pinMode(_slaveSelectPin, OUTPUT);
    digitalWrite(_slaveSelectPin, HIGH);

    // Initialize the SPI communication
    SPI.begin();

    // Initialize the terminal control register (TCON)
    DigitalPotInitTcon();

    // Set the wiper position to the minimum for both channels
    DigitalPotSetWiperMin(0);
    DigitalPotSetWiperMin(1);
}

// Increment the wiper position of the specified potentiometer channel
void MCP4251::DigitalPotWiperIncrement(bool potNum)
{
    byte cmdByte = B00000000;
    digitalWrite(_slaveSelectPin, LOW);
    if (potNum)
    {
        cmdByte = ADDRESS_WIPER_1 | COMMAND_INCREMENT;
        SPI.transfer(cmdByte);
    }
    else
    {
        cmdByte = ADDRESS_WIPER_0 | COMMAND_INCREMENT;
        SPI.transfer(cmdByte);
    }
    digitalWrite(_slaveSelectPin, HIGH);
}

// Decrement the wiper position of the specified potentiometer channel
void MCP4251::DigitalPotWiperDecrement(bool potNum)
{
    byte cmdByte = B00000000;
    digitalWrite(_slaveSelectPin, LOW);
    if (potNum)
    {
        cmdByte = ADDRESS_WIPER_1 | COMMAND_DECREMENT;
        SPI.transfer(cmdByte);
    }
    else
    {
        cmdByte = ADDRESS_WIPER_0 | COMMAND_DECREMENT;
        SPI.transfer(cmdByte);
    }
    digitalWrite(_slaveSelectPin, HIGH);
}

// Set the wiper position of the specified potentiometer channel to the given value
void MCP4251::DigitalPotSetWiperPosition(bool potNum, unsigned int value)
{
    byte cmdByte = B00000000;
    byte dataByte = B00000000;
    if (value > 255)
        cmdByte |= B00000001;
    else
        dataByte = (byte)(value & 0X00FF);
    digitalWrite(_slaveSelectPin, LOW);
    if (potNum)
    {
        cmdByte = cmdByte | ADDRESS_WIPER_1 | COMMAND_WRITE;
        SPI.transfer(cmdByte);
        SPI.transfer(dataByte);
    }
    else
    {
        cmdByte = cmdByte | ADDRESS_WIPER_0 | COMMAND_WRITE;
        SPI.transfer(cmdByte);
        SPI.transfer(dataByte);
    }
    digitalWrite(_slaveSelectPin, HIGH);
}

// Set the wiper position of the specified potentiometer channel to the minimum value
void MCP4251::DigitalPotSetWiperMin(bool potNum)
{
    if (potNum)
        DigitalPotSetWiperPosition(1, 0);
    else
        DigitalPotSetWiperPosition(0, 0);
}

// Set the wiper position of the specified potentiometer channel to the maximum value
void MCP4251::DigitalPotSetWiperMax(bool potNum)
{
    if (potNum)
        DigitalPotSetWiperPosition(1, 256);
    else
        DigitalPotSetWiperPosition(0, 256);
}

// Set the wiper position of the specified potentiometer channel to the middle value
void MCP4251::DigitalPotSetWiperMid(bool potNum)
{
    if (potNum)
        DigitalPotSetWiperPosition(1, 128);
    else
        DigitalPotSetWiperPosition(0, 128);
}

// Read the wiper position of the specified potentiometer channel and return it
uint16_t MCP4251::DigitalPotReadWiperPosition(bool potNum)
{
    byte cmdByte = B00000000;
    byte hByte = B00000000;
    byte lByte = B00000000;
    digitalWrite(_slaveSelectPin, LOW);
    if (potNum)
    {
        cmdByte = ADDRESS_WIPER_1 | COMMAND_READ;
        hByte = SPI.transfer(cmdByte);
        lByte = SPI.transfer(DUMMY_DATA);
    }
    else
    {
        cmdByte = ADDRESS_WIPER_0 | COMMAND_READ;
        hByte = SPI.transfer(cmdByte);
        lByte = SPI.transfer(DUMMY_DATA);
    }
    digitalWrite(_slaveSelectPin, HIGH);
    return ((uint16_t)hByte << 8 | (uint16_t)lByte) & BITMASK_READ_DATA_REGISTER;
}

// Read the status register of the MCP4251 and return its value
uint16_t MCP4251::DigitalPotReadStatusRegister()
{
    byte cmdByte = B00000000;
    byte hByte = B00000000;
    byte lByte = B00000000;
    digitalWrite(_slaveSelectPin, LOW);
    cmdByte = ADDRESS_STATUS | COMMAND_READ;
    hByte = SPI.transfer(cmdByte);
    lByte = SPI.transfer(DUMMY_DATA);
    digitalWrite(_slaveSelectPin, HIGH);
    return ((uint16_t)hByte << 8 | (uint16_t)lByte) & BITMASK_READ_DATA_REGISTER;
}

// Read the terminal control register (TCON) of the MCP4251 and return its value
uint16_t MCP4251::DigitalPotReadTconRegister()
{
    byte cmdByte = B00000000;
    byte hByte = B00000000;
    byte lByte = B00000000;
    digitalWrite(_slaveSelectPin, LOW);
    cmdByte = ADDRESS_TCON | COMMAND_READ;
    hByte = SPI.transfer(cmdByte);
    lByte = SPI.transfer(DUMMY_DATA);
    digitalWrite(_slaveSelectPin, HIGH);
    return ((uint16_t)hByte << 8 | (uint16_t)lByte) & BITMASK_READ_DATA_REGISTER;
}

// Write the given value to the terminal control register (TCON) of the MCP4251

void MCP4251::DigitalPotWriteTconRegister(uint16_t value)
{
    byte cmdByte = B00000000;
    byte dataByte = B00000000;
    if (value > 255)
        cmdByte |= B00000001;
    else
        dataByte = (byte)(value & 0X00FF);
    digitalWrite(_slaveSelectPin, LOW);
    cmdByte = cmdByte | ADDRESS_TCON | COMMAND_WRITE;
    SPI.transfer(cmdByte);
    SPI.transfer(dataByte);
    digitalWrite(_slaveSelectPin, HIGH);
}

// Configure the startup state of the specified potentiometer channel
void MCP4251::DigitalPotStartup(bool potNum)
{
    uint16_t tconData = this->DigitalPotReadTconRegister();
    byte hByte = (uint8_t)(tconData >> 8);
    byte lByte = (uint8_t)(tconData & 0xff);

    if (potNum)
        lByte = lByte | BITMASK_POT1_STARTUP;
    else
        lByte = lByte | BITMASK_POT0_STARTUP;

    tconData = (uint16_t)(hByte << 8) | (uint16_t)lByte;
    this->DigitalPotWriteTconRegister(tconData);
}

// Configure the shutdown state of the specified potentiometer channel
void MCP4251::DigitalPotShutdown(bool potNum)
{
    uint16_t tconData = this->DigitalPotReadTconRegister();
    byte hByte = (uint8_t)(tconData >> 8);
    byte lByte = (uint8_t)(tconData & 0xff);

    if (potNum)
        lByte = lByte & ~BITMASK_POT1_STARTUP;
    else
        lByte = lByte & ~BITMASK_POT0_STARTUP;

    tconData = (uint16_t)(hByte << 8) | (uint16_t)lByte;
    this->DigitalPotWriteTconRegister(tconData);
}

// Connect terminal B of the specified potentiometer channel
void MCP4251::DigitalPotTerminalBConnect(bool potNum)
{
    uint16_t tconData = this->DigitalPotReadTconRegister();
    byte hByte = (uint8_t)(tconData >> 8);
    byte lByte = (uint8_t)(tconData & 0xff);

    if (potNum)
        lByte = lByte | BITMASK_POT1_B_TERMINAL_CONNECT;
    else
        lByte = lByte | BITMASK_POT0_B_TERMINAL_CONNECT;

    tconData = (uint16_t)(hByte << 8) | (uint16_t)lByte;
    this->DigitalPotWriteTconRegister(tconData);
}

// Disconnect terminal B of the specified potentiometer channel
void MCP4251::DigitalPotTerminalBDisconnect(bool potNum)
{
    uint16_t tconData = this->DigitalPotReadTconRegister();
    byte hByte = (uint8_t)(tconData >> 8);
    byte lByte = (uint8_t)(tconData & 0xff);

    if (potNum)
        lByte = lByte & ~BITMASK_POT1_B_TERMINAL_CONNECT;
    else
        lByte = lByte & ~BITMASK_POT0_B_TERMINAL_CONNECT;

    tconData = (uint16_t)(hByte << 8) | (uint16_t)lByte;
    this->DigitalPotWriteTconRegister(tconData);
}

// Connect terminal A of the specified potentiometer channel
void MCP4251::DigitalPotTerminalAConnect(bool potNum)
{
    uint16_t tconData = this->DigitalPotReadTconRegister();
    byte hByte = (uint8_t)(tconData >> 8);
    byte lByte = (uint8_t)(tconData & 0xff);

    if (potNum)
        lByte = lByte | BITMASK_POT1_A_TERMINAL_CONNECT;
    else
        lByte = lByte | BITMASK_POT0_A_TERMINAL_CONNECT;

    tconData = (uint16_t)(hByte << 8) | (uint16_t)lByte;
    this->DigitalPotWriteTconRegister(tconData);
}

// Disconnect terminal A of the specified potentiometer channel
void MCP4251::DigitalPotTerminalADisconnect(bool potNum)
{
    uint16_t tconData = this->DigitalPotReadTconRegister();
    byte hByte = (uint8_t)(tconData >> 8);
    byte lByte = (uint8_t)(tconData & 0xff);

    if (potNum)
        lByte = lByte & ~BITMASK_POT1_A_TERMINAL_CONNECT;
    else
        lByte = lByte & ~BITMASK_POT0_A_TERMINAL_CONNECT;

    tconData = (uint16_t)(hByte << 8) | (uint16_t)lByte;
    this->DigitalPotWriteTconRegister(tconData);
}

// Connect the wiper terminal of the specified potentiometer channel
void MCP4251::DigitalPotWiperConnect(bool potNum)
{
    uint16_t tconData = this->DigitalPotReadTconRegister();
    byte hByte = (uint8_t)(tconData >> 8);
    byte lByte = (uint8_t)(tconData & 0xff);

    if (potNum)
        lByte = lByte | BITMASK_POT1_WIPER_TERMINAL_CONNECT;
    else
        lByte = lByte | BITMASK_POT0_WIPER_TERMINAL_CONNECT;

    tconData = (uint16_t)(hByte << 8) | (uint16_t)lByte;
    this->DigitalPotWriteTconRegister(tconData);
}

// Disconnect the wiper terminal of the specified potentiometer channel
void MCP4251::DigitalPotWiperDisconnect(bool potNum)
{
    uint16_t tconData = this->DigitalPotReadTconRegister();
    byte hByte = (uint8_t)(tconData >> 8);
    byte lByte = (uint8_t)(tconData & 0xff);

    if (potNum)
        lByte = lByte & ~BITMASK_POT1_WIPER_TERMINAL_CONNECT;
    else
        lByte = lByte & ~BITMASK_POT0_WIPER_TERMINAL_CONNECT;

    tconData = (uint16_t)(hByte << 8) | (uint16_t)lByte;
    this->DigitalPotWriteTconRegister(tconData);
}

// Initialize the terminal control register (TCON) of the MCP4251
void MCP4251::DigitalPotInitTcon()
{
    uint16_t tconData = this->DigitalPotReadTconRegister();
    byte hByte = (uint8_t)(tconData >> 8);
    byte lByte = (uint8_t)(tconData & 0xff);
      // If there's a specific initialization you'd like to perform for TCON, you can add it here.
    // For now, the function remains empty since the original code didn't have a specific implementation.

    tconData = (uint16_t)(hByte << 8) | (uint16_t)lByte;
    this->DigitalPotWriteTconRegister(tconData);
}

// Convert the given resistance value to the equivalent wiper position for the specified potentiometer channel
uint16_t MCP4251::DigitalPotResistanceToPosition(bool potNum, float resistance)
{
    if (potNum)
    {
        if (resistance <= this->_pot1ResistanceRmin)
            return 0;
        else if (resistance >= this->_pot1ResistanceRmax)
            return 256;

        return static_cast<uint16_t>((((resistance - this->_pot1ResistanceRW) / (this->_pot1ResistanceRAB)) * 256) + 0.5);
    }
    else
    {
        if (resistance <= this->_pot0ResistanceRmin)
            return 0;
        else if (resistance >= this->_pot0ResistanceRmax)
            return 256;

        return static_cast<uint16_t>((((resistance - this->_pot0ResistanceRW) / (this->_pot0ResistanceRAB)) * 256) + 0.5);
    }
}

// Convert the given wiper position to the equivalent resistance value for the specified potentiometer channel
float MCP4251::DigitalPotPositionToResistance(bool potNum, uint16_t position)
{
    if (potNum)
    {
        if (position < 0)
            return this->_pot1ResistanceRmin;
        else if (position > 256)
            return this->_pot1ResistanceRmax;

        return (this->_pot1ResistanceRAB / 256.0) * static_cast<float>(position) + this->_pot1ResistanceRW;
    }
    else
    {
        if (position < 0)
            return this->_pot0ResistanceRmin;
        else if (position > 256)
            return this->_pot0ResistanceRmax;

        return (this->_pot0ResistanceRAB / 256.0) * static_cast<float>(position) + this->_pot0ResistanceRW;
    }
}

