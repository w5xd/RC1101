// RadioPanelUsb.h

#pragma once

using namespace System;

namespace RadioPanelUsb {
    ref class FrontPanel;
	public ref class Finder
	{
    public:
        System::Collections::Generic::List<FrontPanel^> ^listDevices();
        FrontPanel ^openByUsbSerialNumber(System::String ^);
        FrontPanel ^resetAndOpenDevice(System::String ^);
	};
}
