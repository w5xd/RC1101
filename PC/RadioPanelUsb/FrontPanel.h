#pragma once
namespace RadioPanelUsb {
    class CFrontPanel;

    public ref class DisplayObject {
    public:
        DisplayObject() : objType(0xffff), objIndex(0), value(0) {}
        unsigned short objType;
        unsigned short objIndex;
        unsigned short value;
    };

    public ref class FT232H
    {
    public:
        FT232H(CFrontPanel *, System::String ^);
        System::String ^GetUsbSerialNumber();
        bool Reset();
        bool Test();
        ~FT232H();
        !FT232H();
    protected:
        CFrontPanel *m_frontPanel;
        System::String ^m_Serial;
    };

    public ref class FrontPanel : public FT232H
    {
    internal:
        FrontPanel(CFrontPanel *, System::String ^);
    public:
        System::String ^GetIdString();
        void SetIdString(System::String ^idString);

        bool GetInputState(array<short> ^encoders7, unsigned short %numencoders, unsigned short %switches, unsigned char %encoderSwitches);
        bool SetEncoderCenter(unsigned which, unsigned int encValue);
        bool SetStringObject(unsigned char which, System::String ^DisplayString);
        bool SetTrellisGroup(unsigned char which, unsigned short mask, unsigned short displayObjType, unsigned short objIndex);
        bool SetEncoderMap(unsigned char which, unsigned short displayObjLowType, unsigned short displayObjLowIndex,
            unsigned short displayObjHighType, unsigned short displayObjHighIndex, short mult, int lowLimit, int highLimit);
        bool PressTrellisButton(unsigned char which, unsigned char set);
        bool SetDisplayObjects(array<DisplayObject^> ^toDisplay);
        bool SetTrellisBrightness(unsigned char b);
        bool ResetDisplayDefaults();
        bool SetEncoderSwitchState(unsigned char b);
        bool SetLcdImageFileName(System::String ^fn, System::String ^%res);
        bool GetLcdImageVersion(int %v);

    protected:
        System::String ^m_IdStringCached;
    };
}
