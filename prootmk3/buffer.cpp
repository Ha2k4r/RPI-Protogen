#include "buffer.hpp"

std::vector<std::pair<fullDescriptor, int16_t >> Buffer::readBuf(){
    std::vector<std::pair<fullDescriptor, int16_t>> res;
    for(fullDescriptor f : changed){
        res.push_back(std::make_pair(f, buffer[f]));
    }
    changed.clear();
    return res;
}

void Buffer::Update(){
    changed.insert({serialData[0], serialData[1]}); // mark this descriptor as changed
    buffer[{serialData[0], serialData[1]}] = serialData[2]; // add the value to the map
}

Buffer::Buffer(const std::string& port, unsigned int baud_rate) : SerialPort(port, baud_rate){
    StartAsyncThread();
}

Buffer::~Buffer(){
    // Destructor will close the serial port automatically
    if (serial.is_open()) {
        serial.close();
    }
}
