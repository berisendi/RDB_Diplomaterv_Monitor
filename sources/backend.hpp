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



#include <iostream>
#include <functional>
#include <mutex>

#include <QApplication>

#include "global.hpp"
#include "backend_signal_interface.hpp"
#include "gui_signal_interface.h"
#include "diagram.hpp"
#include "serial_port.hpp"
#include "measurement_data_protocol.hpp"
#include "network_handler.hpp"



#ifndef GUI_FRAMEWORK_HPP
#define GUI_FRAMEWORK_HPP



class Backend : public QObject, public BackendSignalInterface
{
    Q_OBJECT
    Q_INTERFACES(BackendSignalInterface)

public:
    static constexpr std::size_t report_date_and_time_string_size = 10;

    Backend();
    Backend(const Backend& new_backend) = delete;
    Backend(Backend&& new_backend) = delete;

    ~Backend() = default;

    Backend& operator=(const Backend&  new_backend) = delete;
    Backend& operator=(Backend&& new_backend) = delete;

    void RegisterGuiSignalInterface(GuiSignalInterface* new_gui_signal_interface);

    void StoreNewDiagrams(std::vector<std::unique_ptr<DiagramSpecialized> >&& new_diagrams);

    void NotifyAboutDiagramContainerChange(void);

    void ReportStatus(const std::string& message);

signals:
    void NewStatusMessage(const std::string& message_text) override;
    void NetworkOperationFinished(const std::string& port_name, bool result) override;
    void ShowThisDiagram(const DiagramSpecialized& diagram) override;
    void DiagramListHasChanged(const std::vector<std::string>& available_diagrams) override;

private slots:
    void OpenNetwokConnection(const std::string&);
    void CloseNetworkConnection(const std::string&);
    void RequestForDiagram(const DataIndexType& diagram_index);

private:
    SerialPort serial_port;
    MeasurementDataProtocol measurement_data_protocol;
    NetworkHandler serial_network_handler;

    GuiSignalInterface *gui_signal_interface;

    std::mutex mutex_diagram_container;
    std::mutex mutex_report_status;

    std::vector<DiagramSpecialized> diagram_container;
};



#endif /* GUI_FRAMEWORK_HPP */