#include "serialport.hpp"
#include <iostream>
SerialPort::SerialPort(const std::string& port, unsigned int baud_rate)
	: io(), serial(io, port) { // ?
	serial.set_option(boost::asio::serial_port_base::baud_rate(baud_rate)); 
	// initialize array
	serialData = { -1, -1, -1 };
}

SerialPort::~SerialPort() {
    // Destructor will close the serial port automatically
    if (serial.is_open()) {
        serial.close();
    }
}

std::vector<int16_t> SerialPort::GetData(){
    std::unique_lock lk(serialDataMutex);
    return serialData;
}

void SerialPort::StartAsyncThread(){
    asyncRead = std::thread(&SerialPort::WorkerFunction, this);
    stop = false;
}

void SerialPort::StopAsyncThread(){
    stop = true;
    if (asyncRead.joinable())
        asyncRead.join();
}

void SerialPort::ReadSerial() { 
    std::vector<int16_t> values(3);

    try {
        uint8_t buffer[6];

        // Read exactly 6 bytes from the serial port
        boost::asio::read(serial, boost::asio::buffer(buffer, sizeof(buffer)));

        // Interpret the bytes as 2 byte values
        std::unique_lock lk(serialDataMutex);
        for (int i = 0; i < 3; ++i) {
            serialData[i] = (buffer[i * 2 + 1] << 8) | buffer[i * 2];
        }

    } catch (boost::system::system_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    Update();
}

void SerialPort::WorkerFunction(){
    while(true){
        if(stop)
            return;
        ReadSerial();
    }
}

void SerialPort::Update(){

}
