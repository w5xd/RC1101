// RadioPanelUsb.h

#pragma once

using namespace System;

namespace RadioPanelUsb {
    ref class FrontPanel;
    ref class FT232H;
	public ref class Finder
	{
    public:
        System::Collections::Generic::List<FrontPanel^> ^listDevices();
        FrontPanel ^openByUsbSerialNumber(System::String ^);
        FrontPanel ^resetAndOpenDevice(System::String ^);
        System::Collections::Generic::List<FT232H^> ^listFT232HDevices();
	};
}
