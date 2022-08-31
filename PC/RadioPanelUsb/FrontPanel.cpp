#include "Stdafx.h"

#include "FrontPanel.h"
#include "FrontPanelNative.h"
#include <msclr\marshal_cppstd.h>

namespace RadioPanelUsb {

    // managed
    FT232H::FT232H(CFrontPanel *f, System::String ^s)
        : m_frontPanel(f)
        , m_Serial(s)
    {
    }
    FT232H::~FT232H()
    {
        this->!FT232H();
    }

    FT232H::!FT232H()
    {
        if (m_frontPanel)
            delete m_frontPanel;
        m_frontPanel = 0;
    }

    bool FT232H::Reset()
    {
        if (m_frontPanel)
            return m_frontPanel->ResetFrontPanel();
        return false;
    }

    bool FT232H::Test()
    {
        if (m_frontPanel)
            return m_frontPanel->TestFrontPanel();
        return false;
    }

    System::String ^FT232H::GetUsbSerialNumber()
    {
        return m_Serial;
    }

    FrontPanel::FrontPanel(CFrontPanel *f, System::String ^s) : FT232H(f,s)
    { }

    System::String ^FrontPanel::GetIdString()
    {
        if (m_frontPanel)
        {
            if ((m_IdStringCached == nullptr) || (m_IdStringCached->Length == 0))
                m_IdStringCached = gcnew System::String(m_frontPanel->GetIdString().c_str());
            return m_IdStringCached;
        }
        return gcnew System::String("");
    }

    void FrontPanel::SetIdString(System::String ^v)
    {
        if (m_frontPanel)
        {
            m_frontPanel->SetIdString(msclr::interop::marshal_as<std::string>(v));
            m_IdStringCached = nullptr;
        }
    }

    bool FrontPanel::SetLcdImageFileName(System::String ^fn, System::String ^%res)
    {
        if (m_frontPanel)
        {
            std::string rr;
            bool ret = m_frontPanel->SetLcdImageFileName(msclr::interop::marshal_as<std::string>(fn), rr);
            res = gcnew System::String(rr.c_str());
            return ret;
        }
        return false;
    }

    bool FrontPanel::GetLcdImageVersion(int %v)
    {
        v = -1;
        if (m_frontPanel)
        {
            int vs;
            bool ret = m_frontPanel->GetLcdImageVersion(vs);
            v = vs;
            return ret;
        }
        return false;
    }


    bool FrontPanel::SetStringObject(unsigned char w, System::String ^v)
    {
        if (m_frontPanel)
            return m_frontPanel->SetStringObject(w, msclr::interop::marshal_as<std::string>(v));
        return false;
    }


    bool FrontPanel::GetInputState(array<short> ^encoders7, unsigned short %numEncoders, unsigned short %switches, unsigned char %encoderSwitches)
    {
        if (m_frontPanel)
        {
            short enc[CFrontPanel::NUMBER_OF_ENCODERS]; unsigned short eswitches;
            numEncoders = CFrontPanel::NUMBER_OF_ENCODERS;
            unsigned char encoderSwitchMask;
            bool ret = m_frontPanel->GetInputState(enc, eswitches, encoderSwitchMask);
            switches = eswitches; 
            encoderSwitches = encoderSwitchMask;
            for (int i = 0; (i < CFrontPanel::NUMBER_OF_ENCODERS) && (i < encoders7->Length); i++)
                encoders7->SetValue(enc[i], i);
            return ret;
        }
        return false;
    }

    bool FrontPanel::SetEncoderCenter(unsigned which, unsigned encValue)
    {
        if (m_frontPanel)
            return m_frontPanel->SetEncoderCenter(which, encValue);
        return false;
    }

    bool FrontPanel::SetTrellisGroup(unsigned char which, unsigned short mask, unsigned short displayObjType, unsigned short objIndex)
    {
        if (m_frontPanel)
            return m_frontPanel->SetTrellisGroup(which, mask, displayObjType, objIndex);
        return false;
    }

    bool FrontPanel::SetEncoderMap(unsigned char which, unsigned short displayObjLowType, unsigned short displayObjLowIndex,
        unsigned short displayObjHighType, unsigned short displayObjHighIndex, short mult,
        int lowLimit, int highLimit) 
    {
        if (m_frontPanel)
            return m_frontPanel->SetEncoderMap(which, displayObjLowType, displayObjLowIndex, displayObjHighType, displayObjHighIndex, mult, lowLimit, highLimit);
        return false;
    }

    bool FrontPanel::PressTrellisButton(unsigned char which, unsigned char set)
    {
        if (m_frontPanel)
            return m_frontPanel->PressTrellisButton(which, set);
        return false;
    }

    bool FrontPanel::SetDisplayObjects(array<DisplayObject^> ^toDisplay)
    {
        if (m_frontPanel && toDisplay->Length > 0 && toDisplay->Length <= CFrontPanel::NUM_DISPLAY_OBJECTS_IN_I2C_MESSAGE)
        {
            CFrontPanel::SetDisplayObject toSend[CFrontPanel::NUM_DISPLAY_OBJECTS_IN_I2C_MESSAGE];
            for (int i = 0; i < toDisplay->Length; i++)
            {
                if (i >= CFrontPanel::NUM_DISPLAY_OBJECTS_IN_I2C_MESSAGE)
                    break;
                toSend[i].objType = toDisplay[i]->objType;
                toSend[i].objIdx = toDisplay[i]->objIndex;
                toSend[i].value = toDisplay[i]->value;
            }
            return m_frontPanel->SetDisplayObjects(toSend);
        }
        return false;
    }

    bool FrontPanel::SetTrellisBrightness(unsigned char b)
    {
        if (m_frontPanel)
            return m_frontPanel->SetTrellisBrightness(b);
        return false;
    }

    bool FrontPanel::ResetDisplayDefaults()
    {
        if (m_frontPanel)
            return m_frontPanel->ResetDisplayDefaults();
        return false;
    }

    bool FrontPanel::SetEncoderSwitchState(unsigned char b)
    {
        if (m_frontPanel)
            return m_frontPanel->SetEncoderSwitchState(b);
        return false;
    }


 }