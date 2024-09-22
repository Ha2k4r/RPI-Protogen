#ifndef SERIALPORT_H
#define SERIALPORT_H
#include <string>
#include <boost/asio.hpp>
//SERIAL IS TERRIFYING
class SerialPort {
    bool stop = false;
public:
    SerialPort(const std::string& port, unsigned int baud_rate);

    ~SerialPort();

    std::vector<int16_t> GetData();

    void StartAsyncThread();

    void StopAsyncThread();

    virtual void Update();
    
protected:
    boost::asio::io_service io;
    boost::asio::serial_port serial;
    std::mutex serialDataMutex;
    std::vector<int16_t> serialData;
    std::thread asyncRead;
    virtual void ReadSerial();

    virtual void WorkerFunction();
};
#endif
