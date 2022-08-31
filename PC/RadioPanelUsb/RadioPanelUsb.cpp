// This is the main DLL file.

#include "stdafx.h"
#include "RadioPanelUsb.h"
#include "FrontPanel.h"
#include "FrontPanelNative.h"
#include <msclr\marshal_cppstd.h>

namespace RadioPanelUsb {

    typedef std::function<void(FT_HANDLE, const std::string &)> iterFcn_t; // function OWNS FT_HANDLE

    void iterateDeviceInfoList(const iterFcn_t &fcn)
    {
        DWORD nc;
        FT_STATUS stat = (*Ftd2XXDynamic::FT_CreateDeviceInfoList)(&nc);
        if ((FT_OK == stat) && nc > 0)
        {
            std::vector<FT_DEVICE_LIST_INFO_NODE> nodes(nc);
            stat = (*Ftd2XXDynamic::FT_GetDeviceInfoList)(&nodes[0], &nc);
            for (int idx = 0; idx < static_cast<int>(nc); idx++)
            {
                if (nodes[idx].Flags & FT_FLAGS_OPENED)
                    continue;
                FT_HANDLE handle;
                stat = (*Ftd2XXDynamic::FT_OpenEx)(&nodes[idx].SerialNumber[0], FT_OPEN_BY_SERIAL_NUMBER, &handle);
                if (stat == FT_OK)
                    fcn(handle, nodes[idx].SerialNumber);
            }
        }
    }

    void AddFrontPanel(FT_HANDLE handle, const std::string &sn, gcroot<System::Collections::Generic::List<FrontPanel^> ^> lst)
    {
#ifdef _DEBUG
                    DWORD start = GetTickCount();
#endif                   
                    std::unique_ptr<CFrontPanel> f(new CFrontPanel(handle));
                    if (f->isFrontPanel() == CFrontPanel::DEV_OK)
                    {
                        FrontPanel ^np = gcnew FrontPanel(f.release(), gcnew System::String(sn.c_str()));
                        lst->Add(np);
                    }
#ifdef _DEBUG
                    start = GetTickCount() - start;
                    start += 0;
#endif
    }

    
    System::Collections::Generic::List<FrontPanel^> ^Finder::listDevices()
    {
        System::Collections::Generic::List<FrontPanel^> ^ret = gcnew System::Collections::Generic::List<FrontPanel^>();
        auto list = gcroot<System::Collections::Generic::List<FrontPanel^> ^>(ret);
        iterateDeviceInfoList(
            std::bind(&AddFrontPanel, std::placeholders::_1, std::placeholders::_2, list));
        return ret;
    }

    void AddFT232(FT_HANDLE handle, const std::string &sn, gcroot<System::Collections::Generic::List<FT232H^> ^> lst)
    {
#ifdef _DEBUG
                    DWORD start = GetTickCount();
#endif                   
                    std::unique_ptr<CFrontPanel> f(new CFrontPanel(handle));
                    if (f->isFrontPanel() == CFrontPanel::DEV_FAILED_ID_YOURSELF)
                    {
                        FT232H ^np = gcnew FT232H(f.release(), gcnew System::String(sn.c_str()));
                        lst->Add(np);
                    }
#ifdef _DEBUG
                    start = GetTickCount() - start;
                    start += 0;
#endif
    }

    System::Collections::Generic::List<FT232H^> ^Finder::listFT232HDevices()
    {
        System::Collections::Generic::List<FT232H^> ^ret = gcnew System::Collections::Generic::List<FT232H^>();
        auto list = gcroot<System::Collections::Generic::List<FT232H^> ^>(ret);
        iterateDeviceInfoList(
            std::bind(&AddFT232, std::placeholders::_1, std::placeholders::_2, list));
        return ret;
    }

    FrontPanel ^Finder::openByUsbSerialNumber(System::String ^UsbSerialNumber)
    {
        FT_HANDLE handle;
        FT_STATUS stat = (*Ftd2XXDynamic::FT_OpenEx)((PVOID)msclr::interop::marshal_as<std::string>(UsbSerialNumber).c_str(),
            FT_OPEN_BY_SERIAL_NUMBER, &handle);
        if (stat == FT_OK)
        {
            std::unique_ptr<CFrontPanel> f(new CFrontPanel(handle));
            if (f->isFrontPanel() == CFrontPanel::DEV_OK)
                return gcnew FrontPanel(f.release(), UsbSerialNumber);
        }
        return nullptr;
    }

    FrontPanel ^Finder::resetAndOpenDevice(System::String ^UsbSerialNumber)
    {
        static const DWORD RESET_WAIT_MSEC = 10000;
        static const unsigned MAX_AROUND = 2;
        for (unsigned i =0; i < MAX_AROUND; i++)
        {
            FT_HANDLE handle = 0;
            FT_STATUS stat = (*Ftd2XXDynamic::FT_OpenEx)((PVOID)msclr::interop::marshal_as<std::string>(UsbSerialNumber).c_str(),
                    FT_OPEN_BY_SERIAL_NUMBER, &handle);
            if (stat == FT_OK)
            {
                std::unique_ptr<CFrontPanel> f(new CFrontPanel(handle)); // will close it
                CFrontPanel::FailureType_t res = f->isFrontPanel();
                switch (res)
                {
                    case CFrontPanel::DEV_FAILED_SETUP:
                    case CFrontPanel::DEV_FAILED_FT232H_SYNC:
                    case CFrontPanel::DEV_FAILED_I2C_SETUP:
                        (*Ftd2XXDynamic::FT_CyclePort)(handle);
                        f.reset();   // FT_Close will be done in CFrontPanel destructor
                        ::Sleep(RESET_WAIT_MSEC);
                        break;

                    case CFrontPanel::DEV_FAILED_ID_YOURSELF:
                        f->ResetFrontPanel();
                        ::Sleep(RESET_WAIT_MSEC);
                        if (f->isFrontPanel() != CFrontPanel::DEV_OK)
                            break;
                        // else fall through
                    case CFrontPanel::DEV_OK:
                        return gcnew FrontPanel(f.release(), UsbSerialNumber);
                }
            }
            else
                break;
        }
        return nullptr;
    }
}