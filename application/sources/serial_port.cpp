//==============================================================================//
//                                                                              //
//    RDB Diplomaterv Monitor                                                   //
//    A monitor program for the RDB Diplomaterv project                         //
//    Copyright (C) 2018  András Gergő Kocsis                                   //
//                                                                              //
//    This program is free software: you can redistribute it and/or modify      //
//    it under the terms of the GNU General Public License as published by      //
//    the Free Software Foundation, either version 3 of the License, or         //
//    (at your option) any later version.                                       //
//                                                                              //
//    This program is distributed in the hope that it will be useful,           //
//    but WITHOUT ANY WARRANTY; without even the implied warranty of            //
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             //
//    GNU General Public License for more details.                              //
//                                                                              //
//    You should have received a copy of the GNU General Public License         //
//    along with this program.  If not, see <https://www.gnu.org/licenses/>.    //
//                                                                              //
//==============================================================================//



#include "serial_port.hpp"



SerialPort::SerialPort() : QObject()
{

}

SerialPort::~SerialPort()
{
    if(port)
    {
        port->close();
        port.reset();
    }
}

bool SerialPort::Open(const std::string& port_name = SERIAL_PORT_DEFAULT_PORT_NAME)
{
    bool result = false;

    if(!port)
    {
        try
        {
            port = std::make_unique<QSerialPort>();
            port->setPortName(QString::fromStdString(port_name));
            port->setBaudRate(SERIAL_PORT_DEFAULT_BAUDRATE);
            port->setDataBits(QSerialPort::Data8);
            port->setStopBits(QSerialPort::OneStop);
            port->setParity(QSerialPort::NoParity);
            port->setFlowControl(QSerialPort::NoFlowControl);

            if(port->open(QIODevice::ReadOnly))
            {
                result = true;
            }
            else
            {
                port.reset();
            }
        }
        catch(...)
        {
            port.reset();
            throw("Could not open port (" + port_name + "), probably a bad allocation in std::make_unique().");
        }
    }
    else
    {
        if(port_name == port->portName().toStdString())
        {
            result = true;
        }
        else
        {
            throw("Another serial port was already openend with this object: " + port_name);
        }
    }

    return result;
}

void SerialPort::Close()
{
    QObject::disconnect(port.get(), &QSerialPort::readyRead, this, &SerialPort::ReadLineFromPort);

    if(port)
    {
        port->close();
        port.reset();
    }
}

bool SerialPort::IsOpen()
{
    return (nullptr != port);
}

bool SerialPort::StartListening(void)
{
    bool result = false;

    if(IsOpen())
    {
        QObject::connect(port.get(), &QSerialPort::readyRead,       this, &SerialPort::ReadLineFromPort);
        QObject::connect(port.get(), &QSerialPort::errorOccurred,   this, &SerialPort::HandleErrors);
        result = true;
    }

    return result;
}

void SerialPort::ReadLineFromPort(void)
{
    bool at_least_one_line_was_received = false;
    std::string received_lines;
    while(port->canReadLine())
    {
        at_least_one_line_was_received = true;
        received_lines += port->readLine().toStdString();
    }
    if(at_least_one_line_was_received)
    {
        std::stringstream received_data_stream(received_lines);
        emit DataReceived(received_data_stream);
    }
}

void SerialPort::HandleErrors(QSerialPort::SerialPortError error)
{
    if(QSerialPort::ReadError == error)
    {
        emit ErrorReport((QObject::tr("An I/O error occurred while reading the data from port %1, error: %2").arg(port->portName()).arg(port->errorString())).toStdString());
    }
}
