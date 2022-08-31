#pragma once
#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif
#include <ftd2xx.h>
#include <vector>
#include <string>

// these are the functions we actually use
namespace FtdiInterface {
    typedef
        FT_STATUS(WINAPI * FT_Close)(
            FT_HANDLE ftHandle
            );

    typedef
        FT_STATUS(WINAPI*  FT_CyclePort)(
            FT_HANDLE ftHandle
            );

    typedef
        FT_STATUS(WINAPI *FT_OpenEx)(
            PVOID pArg1,
            DWORD Flags,
            FT_HANDLE *pHandle
            );
    typedef
        FT_STATUS(WINAPI *FT_CreateDeviceInfoList)(
            LPDWORD lpdwNumDevs
            );
    typedef
        FT_STATUS(WINAPI * FT_GetDeviceInfoList)(
            FT_DEVICE_LIST_INFO_NODE *pDest,
            LPDWORD lpdwNumDevs
            );

    typedef
        FT_STATUS(WINAPI *FT_Write)(
            FT_HANDLE ftHandle,
            LPVOID lpBuffer,
            DWORD dwBytesToWrite,
            LPDWORD lpBytesWritten
            );

    typedef
        FT_STATUS(WINAPI *FT_ResetDevice)(
            FT_HANDLE ftHandle
            );

    typedef
        FT_STATUS(WINAPI *FT_SetTimeouts)(
            FT_HANDLE ftHandle,
            ULONG ReadTimeout,
            ULONG WriteTimeout
            );

    typedef
        FT_STATUS(WINAPI *FT_GetQueueStatus)(
            FT_HANDLE ftHandle,
            DWORD *dwRxBytes
            );

    typedef

        FT_STATUS(WINAPI *FT_SetLatencyTimer)(
            FT_HANDLE ftHandle,
            UCHAR ucLatency
            );

    typedef
        FT_STATUS(WINAPI *FT_SetBitMode)(
            FT_HANDLE ftHandle,
            UCHAR ucMask,
            UCHAR ucEnable
            );

    typedef
        FT_STATUS(WINAPI *FT_SetUSBParameters)(
            FT_HANDLE ftHandle,
            ULONG ulInTransferSize,
            ULONG ulOutTransferSize
            );

    typedef

        FT_STATUS(WINAPI* FT_SetChars)(
            FT_HANDLE ftHandle,
            UCHAR EventChar,
            UCHAR EventCharEnabled,
            UCHAR ErrorChar,
            UCHAR ErrorCharEnabled
            );

    typedef
        FT_STATUS(WINAPI* FT_SetFlowControl)(
            FT_HANDLE ftHandle, USHORT usFlowControl, UCHAR uXon,UCHAR uXoff
            );

    typedef
        FT_STATUS(WINAPI* FT_Read)(
            FT_HANDLE ftHandle,
            LPVOID lpBuffer,
            DWORD dwBytesToRead,
            LPDWORD lpBytesReturned
            );

};

struct Ftd2XXDynamic
{
    Ftd2XXDynamic();
    // jump table into ftd2xx.dll
    static FtdiInterface::FT_Close FT_Close;
    static FtdiInterface::FT_CyclePort FT_CyclePort;
    static FtdiInterface::FT_OpenEx FT_OpenEx;
    static FtdiInterface::FT_CreateDeviceInfoList FT_CreateDeviceInfoList;
    static FtdiInterface::FT_GetDeviceInfoList FT_GetDeviceInfoList;
    static FtdiInterface::FT_Write FT_Write;
    static FtdiInterface::FT_ResetDevice FT_ResetDevice;
    static FtdiInterface::FT_SetTimeouts FT_SetTimeouts;
    static FtdiInterface::FT_GetQueueStatus FT_GetQueueStatus;
    static FtdiInterface::FT_SetLatencyTimer FT_SetLatencyTimer;
    static FtdiInterface::FT_SetBitMode FT_SetBitMode;
    static FtdiInterface::FT_SetUSBParameters FT_SetUSBParameters;
    static FtdiInterface::FT_SetChars FT_SetChars;
    static FtdiInterface::FT_SetFlowControl FT_SetFlowControl;
    static FtdiInterface::FT_Read FT_Read;
};

namespace RadioPanelUsb {
    typedef unsigned char BYTE;
    class CFrontPanel
    {
    public:
        enum FailureType_t { DEV_FAILED_SETUP, DEV_FAILED_FT232H_SYNC, DEV_FAILED_ID_YOURSELF,
            DEV_FAILED_I2C_SETUP, DEV_OK};
        CFrontPanel(FT_HANDLE);
        FailureType_t isFrontPanel(); // has side-effect of i2c initialization
        ~CFrontPanel();
        enum { NUM_DISPLAY_OBJECTS_IN_I2C_MESSAGE = 5, NUMBER_OF_ENCODERS = 7};

        struct SetDisplayObject {
            SetDisplayObject() : objType(0xffff), objIdx(0xffff), value(0) {}
            unsigned short objType;
            unsigned short objIdx;
            unsigned short value;
        };

        bool TestFrontPanel();
        bool ResetFrontPanel();
        std::string GetIdString();
        void SetIdString(const std::string &);
        bool SetStringObject(unsigned char, const std::string &);
        bool GetInputState(short encoders[NUMBER_OF_ENCODERS], unsigned short &switches, unsigned char &encoderSwitches);
        bool SetEncoderCenter(unsigned which, unsigned int encValue);
        bool SetTrellisGroup(unsigned char which, unsigned short mask, unsigned short displayObjType, unsigned short objIndex);
        bool SetEncoderMap(unsigned char which, unsigned short displayObjLowType, unsigned short displayObjLowIndex,
            unsigned short displayObjHighType, unsigned short displayObjHighIndex, short mult,
            int lowLimit, int highLimit);
        bool PressTrellisButton(unsigned char which, unsigned char set);
        bool SetDisplayObjects(const SetDisplayObject toDisplay[NUM_DISPLAY_OBJECTS_IN_I2C_MESSAGE]);
        bool SetTrellisBrightness(unsigned char b);
        bool ResetDisplayDefaults();
        bool SetEncoderSwitchState(unsigned char b);
        bool SetLcdImageFileName(const std::string &fn, std::string &result);
        bool GetLcdImageVersion(int &);
    protected:
        CFrontPanel(const CFrontPanel &) = delete;
        std::vector<BYTE> getCmdResults(const std::vector<BYTE> &cmd, int expectedReturnLength, int MaxTries = 2,
            bool ignoreCheckSum = false);
        bool SyncBadCommand(BYTE);
        FT_STATUS ClearReadQueue();

        static void HighSpeedSetI2CStart(std::vector<BYTE> &, unsigned count);
        static void HighSpeedSetI2CStop(std::vector<BYTE> &OutputBuffer);
        static int SendAddrAndReadAck(BYTE addr, bool Read, std::vector<BYTE> &);
        static int SendByteAndReadAck(BYTE dwDataSend, std::vector<BYTE> &);
        static int ReadByteAndSendAck(bool Acq, std::vector<BYTE> &);
        static void SetI2CLinesIdle(std::vector<BYTE> &);

        bool m_initialized;
        FT_HANDLE m_handle;
#ifdef _DEBUG
        std::vector<BYTE> lastRead;
        std::vector<BYTE> prevRead;
        std::vector<unsigned> m_WriteStarts;
        std::vector<unsigned> m_WriteFinished;
        std::vector<unsigned> m_ReadFinished;
        int m_numTries;
        int m_totalTries;
#endif
    };
}

