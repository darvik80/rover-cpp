//
// Created by Kishchenko, Ivan on 2019-08-27.
//

#include "SerialManager.h"
#include <vector>


void SerialManager::listPorts() {
    vector<serial::PortInfo> devices_found = serial::list_ports();

    vector<serial::PortInfo>::iterator iter = devices_found.begin();

    while( iter != devices_found.end() )
    {
        serial::PortInfo device = *iter++;

        printf( "(%s, %s, %s)\n", device.port.c_str(), device.description.c_str(),
                device.hardware_id.c_str() );
    }
}