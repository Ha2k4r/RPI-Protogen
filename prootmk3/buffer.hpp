#ifndef BUFFER_H
#define BUFFER_H
#include <map>
#include <set>
#include <cstdint>
#include "serialport.hpp"

struct fullDescriptor{
    int16_t name;
    int16_t descriptor;
    bool operator==(const fullDescriptor& other) const {
	return (*reinterpret_cast<const uint32_t*>(&other)) == (*reinterpret_cast<const uint32_t*>(this)); // magic
    }
    bool operator>(const fullDescriptor& other) const {
	return (*reinterpret_cast<const uint32_t*>(&other)) > (*reinterpret_cast<const uint32_t*>(this));
    }
    bool operator<(const fullDescriptor& other) const {
	return (*reinterpret_cast<const uint32_t*>(&other)) < (*reinterpret_cast<const uint32_t*>(this));
    }
};


class Buffer : public SerialPort{
private:
    std::map<fullDescriptor, int16_t> buffer;
    std::set<fullDescriptor> changed;
    void Update();
public:
    std::vector<std::pair<fullDescriptor, int16_t>> readBuf();
    Buffer(const std::string& port, unsigned int baud_rate);
    ~Buffer();
};
#endif
