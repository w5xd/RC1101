#include "FrontPanelNative.h"
#include <algorithm>
#include <chrono>
#include <thread>

// From the FTDI website, use their driver dll:
// http://www.ftdichip.com/Drivers/D2XX.htm
// ...and much of the code below is taken directly from the ftdichip i2c example...

namespace Ftd2xxDummy {
    // so we don't crash if there is no FTD2XX.dll
    FT_STATUS WINAPI FT_CreateDeviceInfoList(
        LPDWORD lpdwNumDevs
    )
    {
        return FT_OTHER_ERROR;
    }

    FT_STATUS WINAPI FT_GetDeviceInfoList(
        FT_DEVICE_LIST_INFO_NODE *pDest,
        LPDWORD lpdwNumDevs
    )
    {
        return FT_OTHER_ERROR;
    }

    FT_STATUS WINAPI FT_OpenEx(
        PVOID pArg1,
        DWORD Flags,
        FT_HANDLE *pHandle
    )
    {
        return FT_OTHER_ERROR;
    }

    FT_STATUS WINAPI FT_CyclePort(
        FT_HANDLE ftHandle
    )
    {
        return FT_OTHER_ERROR;
    }

    FT_STATUS WINAPI FT_Close(
        FT_HANDLE ftHandle
    )
    {
        return FT_OTHER_ERROR;
    }

    FT_STATUS WINAPI FT_ResetDevice(
        FT_HANDLE ftHandle
    )
    {
        return FT_OTHER_ERROR;
    }

    FT_STATUS WINAPI FT_SetTimeouts(
        FT_HANDLE ftHandle,
        ULONG ReadTimeout,
        ULONG WriteTimeout
    )
    {
        return FT_OTHER_ERROR;
    }

    FT_STATUS WINAPI FT_SetUSBParameters(
        FT_HANDLE ftHandle,
        ULONG ulInTransferSize,
        ULONG ulOutTransferSize
    )
    {
        return FT_OTHER_ERROR;
    }
    FT_STATUS WINAPI FT_Write(
        FT_HANDLE ftHandle,
        LPVOID lpBuffer,
        DWORD dwBytesToWrite,
        LPDWORD lpBytesWritten
    )
    {
        return FT_OTHER_ERROR;
    }

    FT_STATUS WINAPI FT_Read(
        FT_HANDLE ftHandle,
        LPVOID lpBuffer,
        DWORD dwBytesToRead,
        LPDWORD lpBytesReturned
    )
    {
        return FT_OTHER_ERROR;
    }

    FT_STATUS WINAPI FT_GetQueueStatus(
        FT_HANDLE ftHandle,
        DWORD *dwRxBytes
    )
    {
        return FT_OTHER_ERROR;
    }

    FT_STATUS WINAPI FT_SetLatencyTimer(
        FT_HANDLE ftHandle,
        UCHAR ucLatency
    )
    {
        return FT_OTHER_ERROR;
    }

    FT_STATUS WINAPI FT_SetBitMode(
        FT_HANDLE ftHandle,
        UCHAR ucMask,
        UCHAR ucEnable
    )
    {
        return FT_OTHER_ERROR;
    }

    FT_STATUS WINAPI FT_SetChars(
        FT_HANDLE ftHandle,
        UCHAR EventChar,
        UCHAR EventCharEnabled,
        UCHAR ErrorChar,
        UCHAR ErrorCharEnabled
    )
    {
        return FT_OTHER_ERROR;
    }

    FT_STATUS WINAPI FT_SetFlowControl (FT_HANDLE ftHandle, USHORT usFlowControl, UCHAR uXon,UCHAR uXoff)
    {
        return FT_OTHER_ERROR;
    }
}

// C++ required redeclare of static members
FtdiInterface::FT_Close Ftd2XXDynamic::FT_Close;
FtdiInterface::FT_CyclePort Ftd2XXDynamic::FT_CyclePort;
FtdiInterface::FT_OpenEx Ftd2XXDynamic::FT_OpenEx;
FtdiInterface::FT_CreateDeviceInfoList Ftd2XXDynamic::FT_CreateDeviceInfoList;
FtdiInterface::FT_GetDeviceInfoList Ftd2XXDynamic::FT_GetDeviceInfoList;
FtdiInterface::FT_Write Ftd2XXDynamic::FT_Write;
FtdiInterface::FT_ResetDevice Ftd2XXDynamic::FT_ResetDevice;
FtdiInterface::FT_SetTimeouts Ftd2XXDynamic::FT_SetTimeouts;
FtdiInterface::FT_GetQueueStatus Ftd2XXDynamic::FT_GetQueueStatus;
FtdiInterface::FT_SetLatencyTimer Ftd2XXDynamic::FT_SetLatencyTimer;
FtdiInterface::FT_SetBitMode Ftd2XXDynamic::FT_SetBitMode;
FtdiInterface::FT_SetUSBParameters Ftd2XXDynamic::FT_SetUSBParameters;
FtdiInterface::FT_SetChars Ftd2XXDynamic::FT_SetChars;
FtdiInterface::FT_SetFlowControl Ftd2XXDynamic::FT_SetFlowControl;
FtdiInterface::FT_Read Ftd2XXDynamic::FT_Read;

/* This RadioPanelUsb dll needs to load even if the ftdi drivers
** are not installed...*/
Ftd2XXDynamic::Ftd2XXDynamic()
{
    FT_Close = &Ftd2xxDummy::FT_Close;
    FT_CyclePort = &Ftd2xxDummy::FT_CyclePort;
    FT_OpenEx = &Ftd2xxDummy::FT_OpenEx;
    FT_CreateDeviceInfoList = &Ftd2xxDummy::FT_CreateDeviceInfoList;
    FT_GetDeviceInfoList = &Ftd2xxDummy::FT_GetDeviceInfoList;
    FT_Write = &Ftd2xxDummy::FT_Write;
    FT_ResetDevice = &Ftd2xxDummy::FT_ResetDevice;
    FT_SetTimeouts = &Ftd2xxDummy::FT_SetTimeouts;
    FT_GetQueueStatus = &Ftd2xxDummy::FT_GetQueueStatus;
    FT_SetLatencyTimer = &Ftd2xxDummy::FT_SetLatencyTimer;
    FT_SetBitMode = &Ftd2xxDummy::FT_SetBitMode;
    FT_SetUSBParameters = &Ftd2xxDummy::FT_SetUSBParameters;
    FT_SetChars = &Ftd2xxDummy::FT_SetChars;
    FT_SetFlowControl = &Ftd2xxDummy::FT_SetFlowControl;
    FT_Read = &Ftd2xxDummy::FT_Read;

    HMODULE hm = LoadLibrary(L"FTD2XX.DLL");
    if (hm != 0)
    {
        FT_Close = reinterpret_cast<FtdiInterface::FT_Close>(GetProcAddress(hm, "FT_Close"));
        FT_CyclePort = reinterpret_cast<FtdiInterface::FT_CyclePort>(GetProcAddress(hm, "FT_CyclePort"));
        FT_OpenEx = reinterpret_cast<FtdiInterface::FT_OpenEx>(GetProcAddress(hm, "FT_OpenEx"));
        FT_CreateDeviceInfoList = reinterpret_cast<FtdiInterface::FT_CreateDeviceInfoList>(GetProcAddress(hm, "FT_CreateDeviceInfoList"));
        FT_GetDeviceInfoList = reinterpret_cast<FtdiInterface::FT_GetDeviceInfoList>(GetProcAddress(hm, "FT_GetDeviceInfoList"));
        FT_Write = reinterpret_cast<FtdiInterface::FT_Write>(GetProcAddress(hm, "FT_Write"));
        FT_ResetDevice = reinterpret_cast<FtdiInterface::FT_ResetDevice>(GetProcAddress(hm, "FT_ResetDevice"));
        FT_SetTimeouts = reinterpret_cast<FtdiInterface::FT_SetTimeouts>(GetProcAddress(hm, "FT_SetTimeouts"));
        FT_GetQueueStatus = reinterpret_cast<FtdiInterface::FT_GetQueueStatus>(GetProcAddress(hm, "FT_GetQueueStatus"));
        FT_SetLatencyTimer = reinterpret_cast<FtdiInterface::FT_SetLatencyTimer>(GetProcAddress(hm, "FT_SetLatencyTimer"));
        FT_SetBitMode = reinterpret_cast<FtdiInterface::FT_SetBitMode>(GetProcAddress(hm, "FT_SetBitMode"));
        FT_SetUSBParameters = reinterpret_cast<FtdiInterface::FT_SetUSBParameters>(GetProcAddress(hm, "FT_SetUSBParameters"));
        FT_SetChars = reinterpret_cast<FtdiInterface::FT_SetChars>(GetProcAddress(hm, "FT_SetChars"));
        FT_SetFlowControl = reinterpret_cast<FtdiInterface::FT_SetFlowControl>(GetProcAddress(hm, "FT_SetFlowControl"));
        FT_Read = reinterpret_cast<FtdiInterface::FT_Read>(GetProcAddress(hm, "FT_Read"));
    }
}

namespace {
    Ftd2XXDynamic init; // run the constructor above...
    const long CLOCKHz = 51000;
    const char REQUIRED_ID_YOURSELF_RESPONSE[] = "WriteLog 12.33";
    const ULONG READ_TIMEOUT_MSEC = 950;
    const ULONG WRITE_TIMEOUT_MSEC = 950;
    const unsigned char LATENCY_MSEC = 2;
}

namespace RadioPanelUsb {
    // unmanaged...
    const int WlRemoteId = 42; // i2c address of the arduino in the rc-1100
    const unsigned WL_DEVICE_BUF_MAX = 32;

    // WE specify FALLING_EDGE clock for OUT
    // RISING_EDGE clock for IN
    // ...because that is what the FTDI I2C example does.

    const BYTE MSB_FALLING_EDGE_CLOCK_BIT_OUT = 0x13;
    const BYTE MSB_FALLING_EDGE_CLOCK_BYTE_OUT = 0x11;
    const BYTE MSB_RISING_EDGE_CLOCK_BIT_OUT = 0X12;
    const BYTE MSB_RISING_EDGE_CLOCK_BIT_IN = 0x22;
    const BYTE MSB_FALLING_EDGE_CLOCK_BIT_IN = 0x26;

    const BYTE ENABLE_3PHASE_CLOCK = 0x8C;

    const BYTE SET_8PIN_DIRECTION_AND_STATE = 0x80;
    const BYTE SET_ACBUS_DIRECTION_AND_STATE = 0x82;
    const BYTE WAIT_FOR_IO_HIGH = 0x88;
    const BYTE WAIT_FOR_IO_LOW = 0x89;
    const BYTE MPSSE_SEND_IMMEDIATE = 0x87;

    const BYTE  DIRECTION_SCLIN_SDAIN		=		0x0;
    const BYTE  DIRECTION_SCLOUT_SDAIN		=		0x1;
    const BYTE  DIRECTION_SCLIN_SDAOUT		=		0x2;
    const BYTE  DIRECTION_SCLOUT_SDAOUT		=		0x3;
    const BYTE   DATA_SIZE_8BITS	=		 	    0x07;
    const BYTE   DATA_SIZE_1BIT			=			0x00;

    /*SCL & SDA values*/
    const BYTE  VALUE_SCLLOW_SDALOW			=		0x00;
    const BYTE  VALUE_SCLHIGH_SDALOW		=		0x01;
    const BYTE  VALUE_SCLLOW_SDAHIGH		=		0x02;
    const BYTE  VALUE_SCLHIGH_SDAHIGH		=		0x03;

    /* These are tuning parameters determined empirically*/
    const int ADD_DELAY_TO_I2C_READ = 200; // one should be enough, but...
    const int ADD_DELAY_TO_I2C_WRITE = 200; // one should be enough, but...
    const int START_STOP_REPEAT = 10;
    const int REPEATED_START_REPEAT = 400;

    enum Command_t {
        COMMAND_ID_YOURSELF = 1,
        COMMAND_LONG_TEST,
        COMMAND_SHORT_TEST,
        COMMAND_GET_ID_STRING,
        COMMAND_SET_ID_STRING,
        COMMAND_GET_INPUT_STATE,
        COMMAND_SET_ENCODER_CENTER,
        COMMAND_SET_TRELLIS_GROUP,
        COMMAND_SET_ENCODER_MAP,
        COMMAND_SET_DISPLAY_STRING,
        COMMAND_PRESS_TRELLIS_BUTTON,
        COMMAND_SET_DISPLAY_OBJECT,
        COMMAND_SET_TRELLIS_BRIGHTNESS,
        COMMAND_RESET_DISPLAY_DEFAULTS,
        COMMAND_SET_ENCODER_SWITCH_STATE,
        COMMAND_SET_LCD_IMAGE_NAME,
        COMMAND_GET_LCD_IMAGE_VERSION,
    };

    CFrontPanel::CFrontPanel(FT_HANDLE f)
        : m_handle(f), m_initialized(false)
#ifdef _DEBUG
        , m_numTries(0), m_totalTries(0)
#endif
    { }

    CFrontPanel::~CFrontPanel()
    {
        if (m_handle != 0)
        {
            // This sequence seems to be critical.
            // without it, the FT232H that is NOT an RC-1101
            // but IS used as a COM port cannot be used as a COM port anymore.
            FT_STATUS ft = (*Ftd2XXDynamic::FT_ResetDevice)(m_handle);
            ft = (*Ftd2XXDynamic::FT_SetBitMode)(m_handle, 0x0, 0x00);
            ft = (*Ftd2XXDynamic::FT_Close)(m_handle);
            m_handle = 0;
        }
    }

    /* The lengths of both the write and the read buffers in the i2c protocol 
    ** are specified by the master...and CFrontPanel is the master. There
    ** are errors possible in the mpsse communications between CFrontPanel and the FT232H
    ** that could lead to a different sized return buffer, but a successful command
    ** response cycle will return a vector with size of expectedReturnLength */
    std::vector<unsigned char> CFrontPanel::getCmdResults(
        const std::vector<unsigned char> &cmd, int expectedReturnLength, int MaxTries,
        bool ignoreChecksum)
    {
#ifdef _DEBUG
        DWORD startTime = GetTickCount();
        DWORD WriteStart, WriteFinish, ReadFinish;
#endif
        std::vector<unsigned char> callerRet;
        for (int tr = 0; tr < MaxTries; tr++)
        {
            DWORD dwNumBytesSent;
            FT_STATUS ftStatus;
            if (tr > 0) // subsequent tries get a short delay...
                Sleep(tr);
            // First check that we appear to still be in sync with the FT232H. There
            // should be no characters to read back. If there are, then clear them out
            ClearReadQueue();
            static const bool READ = true;
            static const bool WRITE = false;
            std::vector<BYTE> toDevice;

            SetI2CLinesIdle(toDevice);
            // sync with Arduino...
            toDevice.push_back(WAIT_FOR_IO_HIGH); // it lets us master its i2c bus only sometimes

            // I2C START
            HighSpeedSetI2CStart(toDevice, START_STOP_REPEAT);

            // I2C address
            int toRead = SendAddrAndReadAck(WlRemoteId, WRITE, toDevice);
            // InputBuffer[0] is ack of Addr 
            // front panel always needs a write 
            unsigned short checkSumSend(0);
            for (BYTE c : cmd)
            {
                toRead += SendByteAndReadAck(c, toDevice);
                checkSumSend += c;
            }
            static const int CHKSUM_BYTES = 2;
            SendByteAndReadAck((BYTE)checkSumSend, toDevice);
            SendByteAndReadAck((BYTE)(checkSumSend>>8), toDevice);
            toRead += CHKSUM_BYTES;
            // InputBuffer[1 and up] are i2c ack's of cmd bytes
            // Repeated START
            HighSpeedSetI2CStart(toDevice, REPEATED_START_REPEAT);
            // after START always comes address..this time we read
            toRead += SendAddrAndReadAck(WlRemoteId, READ, toDevice);

#if 1       //empirically determined that things are more reliable if we write in two parts
            ftStatus = (*Ftd2XXDynamic::FT_Write)
                (m_handle, &toDevice[0], toDevice.size(), &dwNumBytesSent);	// Send off the commands
            if (ftStatus != FT_OK || dwNumBytesSent != toDevice.size())
                continue;
            toDevice.clear();
#endif

            // InputBuffer is ack of second address write
            const int readOffset = toRead;
            // Get the answer PLUS TWO EXTRA, the chk bytes
            for (int i = expectedReturnLength + 1; i >= 0; i -= 1)
                toRead += ReadByteAndSendAck(i == 0, toDevice);
            toDevice.push_back(MPSSE_SEND_IMMEDIATE);   // flush device buffers back to PC
                                       
            HighSpeedSetI2CStop(toDevice);             // I2C STOP

            static const int BAD_COMMAND_RESPONSE_LENGTH = 2;
            toDevice.push_back(0xAA);   // to confirm sync, send a bad command at the end
            toRead += BAD_COMMAND_RESPONSE_LENGTH; // FT232H device sends back 0xFA 0xAA in response

#ifdef _DEBUG
            m_numTries = tr + 1;
            m_totalTries += 1;
            WriteStart = GetTickCount() - startTime;
#endif
            std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
            // measured length of transfer at 50K clock for 17 bytes is 9.5msec
            long long msec = (cmd.size() + CHKSUM_BYTES + expectedReturnLength) * 15 * 50000;
            msec /= (CLOCKHz * 17);
            std::chrono::steady_clock::time_point predicted = start + 
                std::chrono::milliseconds(static_cast<int>(msec));
            ftStatus = (*Ftd2XXDynamic::FT_Write)
                (m_handle, &toDevice[0], toDevice.size(), &dwNumBytesSent);	// Send off the commands
#ifdef _DEBUG
            WriteFinish = GetTickCount() - startTime;
#endif
            if (ftStatus == FT_OK && dwNumBytesSent == toDevice.size())
            {
                // ftdi device driver doesn't seem to have a way to block us, so we sleep
                std::this_thread::sleep_until(predicted);
                // Windows says it delivered our control stream to the FT232H, now
                // attempt to read what the chip had to say
                int ReadTimeoutCounter = 50; // maximum tries
                DWORD dwNumInputBuffer = 0;
                while (true)
                {
                    // it can take multiple reads to get everything back through the driver
                    ftStatus = (*Ftd2XXDynamic::FT_GetQueueStatus)(m_handle, &dwNumInputBuffer);	
                    if (ftStatus != FT_OK)
                        break;  // oops
                    // we stop when get as many as we were looking for
                    if (static_cast<int>(dwNumInputBuffer) >= toRead)
                        break;
                    if (--ReadTimeoutCounter <= 0)
                        break;
                    Sleep(static_cast<DWORD>(std::max(msec/4, 1ll)));
                }
                if (static_cast<int>(dwNumInputBuffer) >= toRead)
                {
                    DWORD dwNumBytesRead;
                    std::vector<BYTE> InputBuffer(toRead);
                    ftStatus = (*Ftd2XXDynamic::FT_Read)(m_handle, &InputBuffer[0], toRead, &dwNumBytesRead);
#ifdef _DEBUG
                    ReadFinish = GetTickCount() - startTime;
                    auto totalTime = std::chrono::steady_clock::now() - start;
                    auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(totalTime).count();
#endif
                    if (ftStatus == FT_OK)
                    {
#ifdef _DEBUG
                        prevRead = lastRead;
                        lastRead = InputBuffer;
#endif
                        if (static_cast<int>(dwNumBytesRead) > readOffset + BAD_COMMAND_RESPONSE_LENGTH)
                        {
                            if (    // in sync?
                                InputBuffer[dwNumBytesRead - 2] == 0xFA &&
                                InputBuffer[dwNumBytesRead - 1] == 0xAA)
                            {   // ft232h sent us the expected 'bad command' response
                                bool nacked = false;
                                for (int i = 0; i < readOffset; i++)
                                {
                                    if ((InputBuffer[i] & 1) != 0)
                                    {   // but the read from the i2c bus gave a nak
                                        nacked = true;
                                        break;
                                    }
                                }
                                if (!nacked)
                                {   // so far, so good
                                    std::vector<unsigned char> ret(dwNumBytesRead - readOffset);
                                    ret.assign(InputBuffer.begin() + readOffset,
                                        InputBuffer.begin() + dwNumBytesRead - BAD_COMMAND_RESPONSE_LENGTH);
                                    size_t retSze = ret.size();
                                    unsigned short sum = ret[retSze - 2];
                                    sum <<= 8;
                                    sum += ret[retSze - 1];
                                    unsigned short chk = 0;
                                    ret.resize(retSze - 2);
                                    for (auto c : ret)  // arduino sums the bytes
                                        chk += c;   //...and so do we
                                    if (ignoreChecksum || chk == sum)
                                    {
#ifdef _DEBUG
                                        m_WriteStarts.push_back(WriteStart);
                                        m_WriteFinished.push_back(WriteFinish);
                                        m_ReadFinished.push_back(ReadFinish);
#endif
                                        callerRet = ret;// success return
                                        break;
                                    }
                                    else
                                        dwNumBytesSent += 0; // error place to debug stop
                                }
                                else
                                    dwNumBytesSent += 0; //error
                            }
                            else
                                dwNumBytesSent += 0; //error
                        }
                        else
                            dwNumBytesSent += 0; //error
                    }
                    else
                        dwNumBytesSent += 0; //error
                }
                else
                    dwNumBytesSent += 0; //error
            }
        }
        { // regardless of result, command switch I2C lines to input
            std::vector<BYTE> toDevice;
            SetI2CLinesIdle(toDevice);
            DWORD dwNumBytesSent;
            FT_STATUS ftStatus = (*Ftd2XXDynamic::FT_Write)
                (m_handle, &toDevice[0], toDevice.size(), &dwNumBytesSent);	// Send off the commands
        }
        return callerRet; 
    }

    bool CFrontPanel::SyncBadCommand(BYTE b)
    {
        DWORD dwNumBytesSent;
        std::vector<BYTE> OutputBuffer;
        OutputBuffer.push_back(b);											// Add an invalid command 
                                                                            // Send off the invalid command
        FT_STATUS ftStatus = (*Ftd2XXDynamic::FT_Write)(m_handle, &OutputBuffer[0], OutputBuffer.size(), &dwNumBytesSent);

        // Check if the bytes were sent off OK
        if (ftStatus != FT_OK || OutputBuffer.size() != dwNumBytesSent)
            return false;

        // Now read the response from the FT232H. It should return error code 0xFA followed 
        //     by the actual bad command 0xAA
        // Wait for the two bytes to come back 

        DWORD dwNumInputBuffer = 0;
        int ReadTimeoutCounter = 50; // maximum tries

        while (true)
        {
            ftStatus = (*Ftd2XXDynamic::FT_GetQueueStatus)(m_handle, &dwNumInputBuffer);	// Get number of bytes in the input buffer
            if (ftStatus != FT_OK)
                return false;
            if (dwNumInputBuffer >= 2)
                break;
            if (--ReadTimeoutCounter <= 0)
                return false;
            Sleep(2);
        }

        if (dwNumInputBuffer > 512)
            return false; // there should not be anywhere near this much data
        DWORD dwNumBytesRead;
        std::vector<BYTE> InputBuffer(dwNumInputBuffer);
        ftStatus = (*Ftd2XXDynamic::FT_Read)(m_handle, &InputBuffer[0], dwNumInputBuffer, &dwNumBytesRead);
        if (ftStatus != FT_OK || dwNumBytesRead != dwNumInputBuffer)
            return false;
        if ((InputBuffer[dwNumBytesRead - 2] != BYTE(0xFA)) || (InputBuffer[dwNumBytesRead - 1] != b))
            return false;
        return true;
    }

    //////////////////////////////////////////////////////////////////////////////////////
    // Below function will setup the START condition for I2C bus communication. First, set SDA, SCL high and ensure hold time
    // requirement by device is met. Second, set SDA low, SCL high and ensure setup time requirement met. Finally, set SDA, SCL low
    ////////////////////////////////////////////////////////////////////////////////////////
    void CFrontPanel::HighSpeedSetI2CStart(std::vector<BYTE> &OutputBuffer, unsigned count)
    {
        // no i2c bus arbitration going on here...we must be the only master on the bus
        DWORD dwCount;
        // Repeat commands to ensure the minimum period of the start hold time ie 600ns is achieved
        for (dwCount = 0; dwCount < count; dwCount++)
        {
            OutputBuffer.push_back(SET_8PIN_DIRECTION_AND_STATE);
            OutputBuffer.push_back(VALUE_SCLHIGH_SDAHIGH); //Set SDA, SCL high, WP disabled by SK, DO at bit 1, 
            OutputBuffer.push_back(DIRECTION_SCLOUT_SDAOUT); //Set SK,DO pins as output with bit 1, other pins as input with bit 0
        }
        // Repeat commands to ensure the minimum period of the start setup time ie 600ns is achieved
        for (dwCount = 0; dwCount < count; dwCount++)
        {
            OutputBuffer.push_back(SET_8PIN_DIRECTION_AND_STATE);
            OutputBuffer.push_back(VALUE_SCLHIGH_SDALOW); //Set SDA low, SCL high, WP disabled by SK at bit 1, DO,
            OutputBuffer.push_back(DIRECTION_SCLOUT_SDAOUT); //Set SK,DO pins as output with bit 1, other pins as input with bit 0
        }
        OutputBuffer.push_back(SET_8PIN_DIRECTION_AND_STATE);
        OutputBuffer.push_back(VALUE_SCLLOW_SDALOW); //Set SDA, SCL low, 
        OutputBuffer.push_back(DIRECTION_SCLOUT_SDAOUT); //Set SK,DO pins as output with bit 1, other pins as input with bit 0
    }

    //////////////////////////////////////////////////////////////////////////////////////
    // Below function will setup the STOP condition for I2C bus communication. First, set SDA low, SCL high and ensure setup time
    // requirement by device is met. Second, set SDA, SCL high and ensure hold time requirement met. Finally, set SDA, SCL as input
    // to tristate the I2C bus.
    ////////////////////////////////////////////////////////////////////////////////////////
    void CFrontPanel::HighSpeedSetI2CStop(std::vector<BYTE> &OutputBuffer)
    {
        DWORD dwCount;
        // Repeat commands to ensure the minimum period of the stop setup time ie 600ns is achieved
        for (dwCount = 0; dwCount < START_STOP_REPEAT; dwCount++)
        {
            OutputBuffer.push_back(SET_8PIN_DIRECTION_AND_STATE);
            OutputBuffer.push_back(VALUE_SCLHIGH_SDALOW); //Set SDA low, SCL high, , 
            OutputBuffer.push_back(DIRECTION_SCLOUT_SDAOUT); //Set SK,DO pins as output with bit 1, other pins as input with bit 0
        }
        // Repeat commands to ensure the minimum period of the stop hold time ie 600ns is achieved
        for (dwCount = 0; dwCount < START_STOP_REPEAT; dwCount++)
        {
            OutputBuffer.push_back(SET_8PIN_DIRECTION_AND_STATE);
            OutputBuffer.push_back(VALUE_SCLHIGH_SDAHIGH); //Set SDA, SCL high, 
            OutputBuffer.push_back(DIRECTION_SCLOUT_SDAOUT); //Set SK,DO as output with bit 1, other pins as input with bit 0
        }

        SetI2CLinesIdle(OutputBuffer);
    }

    int CFrontPanel::SendAddrAndReadAck(BYTE addr, bool Read, std::vector<BYTE> &OutputBuffer)
    {
        BYTE addrToSend;
        if (Read)
            addrToSend = ((addr << 1) | 0x01);
        else
            addrToSend = ((addr << 1) & 0xFE);

        OutputBuffer.push_back(MSB_FALLING_EDGE_CLOCK_BIT_OUT);
        OutputBuffer.push_back(0x07);		// Data length of 0x0000 means 1 byte data to clock out
        OutputBuffer.push_back(addrToSend);	// Actual byte to clock out

        OutputBuffer.push_back(SET_8PIN_DIRECTION_AND_STATE);
        OutputBuffer.push_back(0x0);		// Set SCL low
        OutputBuffer.push_back(0x1);		// Set SCL as output

        OutputBuffer.push_back(MSB_RISING_EDGE_CLOCK_BIT_IN);
        OutputBuffer.push_back(0x00);	// Length of 0x00 means to scan in 1 bit

                                        // Put I2C line back to idle (during transfer) state... Clock line driven low, Data line high (open drain)
        for (int i = 0; i < ADD_DELAY_TO_I2C_WRITE; i++)
        {
            OutputBuffer.push_back(SET_8PIN_DIRECTION_AND_STATE);
            OutputBuffer.push_back(0x0);		// Set SCL low
            OutputBuffer.push_back(0x3);		// Set SK as output
                                                // AD0 (SCL) is output driven low
                                                // AD1 (DATA OUT) is output high (open drain)
                                                // AD2 (DATA IN) is input (therefore the output value specified is ignored)
        }

        return 1; // one byte expected on return
    }

    int CFrontPanel::SendByteAndReadAck(BYTE dwDataSend, std::vector<BYTE> &OutputBuffer)
    {
        OutputBuffer.push_back(MSB_FALLING_EDGE_CLOCK_BIT_OUT);
        OutputBuffer.push_back(0x07);		// Data length of 0x0000 means 1 byte data to clock out
        OutputBuffer.push_back(dwDataSend);	// Actual byte to clock out

        OutputBuffer.push_back(SET_8PIN_DIRECTION_AND_STATE);
        OutputBuffer.push_back(VALUE_SCLLOW_SDALOW);		// Set SCL low
        OutputBuffer.push_back(DIRECTION_SCLOUT_SDAIN);		// Set SCL as output

        OutputBuffer.push_back(MSB_RISING_EDGE_CLOCK_BIT_IN);
        OutputBuffer.push_back(DATA_SIZE_1BIT);	// Length of 0x00 means to scan in 1 bit

                                        // Put I2C line back to idle (during transfer) state... Clock line driven low, Data line high (open drain)
        for (int i = 0; i < ADD_DELAY_TO_I2C_WRITE; i++)
        {
            OutputBuffer.push_back(SET_8PIN_DIRECTION_AND_STATE);
            OutputBuffer.push_back(VALUE_SCLLOW_SDALOW);		// Set SCL low
            OutputBuffer.push_back(DIRECTION_SCLOUT_SDAOUT);		// Set SCL as output
                                                // AD0 (SCL) is output driven low
                                                // AD1 (DATA OUT) is output high (open drain)
                                                // AD2 (DATA IN) is input (therefore the output value specified is ignored)
        }
        return 1; // one byte expected on return
    }

    int CFrontPanel::ReadByteAndSendAck(bool Nack, std::vector<BYTE> &OutputBuffer)
    {
        for (int i = 0; i < ADD_DELAY_TO_I2C_READ; i++)
        {
            OutputBuffer.push_back(SET_8PIN_DIRECTION_AND_STATE);
            OutputBuffer.push_back(VALUE_SCLLOW_SDALOW);		// Set SCL low,
            OutputBuffer.push_back(DIRECTION_SCLOUT_SDAIN);		// Set SCL as output
        }

        // Clock one byte of data in...
        OutputBuffer.push_back(MSB_RISING_EDGE_CLOCK_BIT_IN);
        OutputBuffer.push_back(DATA_SIZE_8BITS);		// Length (high)   Length 0x0000 means clock ONE byte in 

        for (int i = 0; i < START_STOP_REPEAT; i++)
        {
            OutputBuffer.push_back(SET_8PIN_DIRECTION_AND_STATE);
            OutputBuffer.push_back(VALUE_SCLLOW_SDALOW);		// Set SCL low
            OutputBuffer.push_back(DIRECTION_SCLOUT_SDAOUT);		// Set SCL/SDA as output
        }

        // Now clock out one bit (the ACK/NAK bit). This bit has value '1' to send a NAK to the I2C Slave
        OutputBuffer.push_back(MSB_RISING_EDGE_CLOCK_BIT_OUT);
        OutputBuffer.push_back(DATA_SIZE_1BIT);		// Length of 0x00 means clock out ONE bit
        OutputBuffer.push_back(Nack ? 0xff : 0x00);		// Command will send bit 7 of this byte, 

        return 1;
    }

    void CFrontPanel::SetI2CLinesIdle(std::vector<BYTE> &OutputBuffer)
    {
        //Tristate the SCL, SDA pins
        //for (int i = 0; i < ADD_DELAY_TO_I2C_WRITE; i++)
        {
            OutputBuffer.push_back(SET_8PIN_DIRECTION_AND_STATE);
            OutputBuffer.push_back(VALUE_SCLHIGH_SDAHIGH); //values SCL/SDA are open-drain high
            OutputBuffer.push_back(DIRECTION_SCLOUT_SDAOUT); //  input vs output--all are input
        }
    }

    FT_STATUS CFrontPanel::ClearReadQueue()
    {
        DWORD dwNumInputBuffer;
        // Purge USB receive buffer ... Get the number of bytes in the FT232H receive buffer and then read them
        FT_STATUS ftStatus = (*Ftd2XXDynamic::FT_GetQueueStatus)(m_handle, &dwNumInputBuffer);
        while ((ftStatus == FT_OK) && (dwNumInputBuffer > 0))
        {
            DWORD dwNumBytesRead;
            char InputBuffer[16];
            DWORD r = std::min(static_cast<DWORD>(sizeof(InputBuffer)), dwNumInputBuffer);
            ftStatus |= (*Ftd2XXDynamic::FT_Read)(m_handle, &InputBuffer, r, &dwNumBytesRead);
            dwNumInputBuffer -= r;
        }
        return ftStatus;
    }

    CFrontPanel::FailureType_t CFrontPanel::isFrontPanel()
    {
        if (!m_initialized)
        {
            // This initialization code will be run against ANY FTDI device
            // installed in the system. To avoid pitting outputs against each
            // other on devices that we're not supposed to control, assume
            // the UART connections (TXD is out, RXD is in, DTR is out)

            FT_STATUS ftStatus = FT_OK;
            ftStatus |= (*Ftd2XXDynamic::FT_ResetDevice)(m_handle);
            ftStatus |= (*Ftd2XXDynamic::FT_SetTimeouts)(m_handle, READ_TIMEOUT_MSEC, WRITE_TIMEOUT_MSEC);
            ftStatus |= (*Ftd2XXDynamic::FT_SetUSBParameters)(m_handle, 0x10000, 0x10000);			// Set USB request transfer sizes
            ftStatus |= (*Ftd2XXDynamic::FT_SetFlowControl)(m_handle, FT_FLOW_NONE, 0, 0);
            ftStatus |= (*Ftd2XXDynamic::FT_SetChars)(m_handle, false, 0, false, 0);				// Disable event and error characters

            ftStatus |= (*Ftd2XXDynamic::FT_SetLatencyTimer)(m_handle, LATENCY_MSEC);
            ftStatus |= (*Ftd2XXDynamic::FT_SetBitMode)(m_handle, 0x0, 0x00); 					// Reset the mode to whatever is set in EEPROM
            ftStatus |= (*Ftd2XXDynamic::FT_SetBitMode)(m_handle, 0x0, 0x02);	 					// Enable MPSSE mode
            if (ftStatus != FT_OK)
                return DEV_FAILED_SETUP; // really can't talk to this device

            // #########################################################################################
            // Synchronise the MPSSE by sending bad command AA to it
            // #########################################################################################
            ftStatus |= ClearReadQueue();
            if (!SyncBadCommand(0xAA))
                return DEV_FAILED_FT232H_SYNC; // its probably not FT232H
            if (!SyncBadCommand(0xAB))
                return DEV_FAILED_FT232H_SYNC;

            // #########################################################################################
            // Configure the MPSSE settings
            // #########################################################################################

            std::vector<BYTE> OutputBuffer;

            OutputBuffer.push_back(0x9E); 		// Enable the FT232H's drive-zero mode on the lines used for I2C ...
            OutputBuffer.push_back(0x17);		// ... on the bits 0, 1 and 2 of the lower port (AD0, AD1, AD2)...include DTR
            OutputBuffer.push_back(0x00);		// ...not required on the upper port AC 0-7
                                                // Set the idle states for the AC lines
            SetI2CLinesIdle(OutputBuffer);

            OutputBuffer.push_back(0x82);	// Command to set directions of ACbus and data values for pins set as o/p
            OutputBuffer.push_back(0xFF);	// Set all 8 lines to high level (only affects pins which are output)
            OutputBuffer.push_back(0x00);   // all are inputs

            OutputBuffer.push_back(0x85);		// Ensure internal loopback is off

            OutputBuffer.push_back(0x8A); 		// Disable clock divide-by-5 for 60Mhz master clock
            OutputBuffer.push_back(0x97);		// Ensure adaptive clocking is off
            OutputBuffer.push_back(ENABLE_3PHASE_CLOCK);

            // configure the dividers to set the SCLK frequency which we will use
            // The SCLK clock frequency can be worked out by the algorithm (when divide-by-5 is off)
            // SCLK frequency  = 60MHz / (( 1 +[ (0xValueH * 256) OR 0xValueL] ) * 2)
            static const short dwClockDivisor = static_cast<short>(30000000L / (1 + CLOCKHz));
            OutputBuffer.push_back(0x86); 								// Command to set clock divisor
            OutputBuffer.push_back(dwClockDivisor & 0xFF);				// Set 0xValueL of clock divisor
            OutputBuffer.push_back((dwClockDivisor >> 8) & 0xFF);		// Set 0xValueH of clock divisor

            DWORD dwNumBytesSent;
            ftStatus = (*Ftd2XXDynamic::FT_Write)(m_handle, &OutputBuffer[0], OutputBuffer.size(), &dwNumBytesSent);	// Send off the commands
            if (ftStatus != FT_OK || dwNumBytesSent != OutputBuffer.size())
                return DEV_FAILED_I2C_SETUP;

            Sleep(20);																
            m_initialized = true;
        }
        if (m_initialized)
        {
            std::vector<BYTE>cmd(1);
            cmd[0] = COMMAND_ID_YOURSELF;
            static const int RETURN_EXPECTED = 14;
            auto res = getCmdResults(cmd, RETURN_EXPECTED, 2);
            if (res.size() == RETURN_EXPECTED && strncmp((char *)&res[0], 
                REQUIRED_ID_YOURSELF_RESPONSE, RETURN_EXPECTED) == 0)
                return DEV_OK;
        }

        return DEV_FAILED_ID_YOURSELF;
    }

    bool CFrontPanel::TestFrontPanel()
    {
        std::vector<BYTE>cmd(32), c2(1);
        cmd[0] = COMMAND_LONG_TEST;
        c2[0] = COMMAND_SHORT_TEST;
#ifdef _DEBUG
        m_totalTries = 0;
        m_WriteStarts.clear(); m_WriteFinished.clear();  m_ReadFinished.clear(); 
#endif

        DWORD start = GetTickCount();
        std::string name = GetIdString();
        for (int i = 0; i < 500; i++)
        {
            std::vector<BYTE> res;
            static const int RETURN_EXPECTED = 30;
            res = getCmdResults(cmd, RETURN_EXPECTED, 5);
            if (res.size() == RETURN_EXPECTED && strncmp((char *)&res[0],
                "ABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMN", RETURN_EXPECTED) == 0)
            {
                //Sleep(2000);
            }
            else
                return false;

            res = getCmdResults(c2, 1, 2);
            if (res.size() == 1 && res[0] == 'S')
            {
            }
            else
                return false;
        }
        start = GetTickCount() - start;
        return true;
    }

    bool CFrontPanel::ResetFrontPanel()
    {
        if (!m_initialized)
            return false;
        // The RC1101 is wired so that the output labeled D4 is wired to the Arduino reset input
        std::vector<BYTE> OutputBuffer;
        DWORD dwNumBytesSent;
        // Set the idle states for the AC lines
        OutputBuffer.push_back(SET_8PIN_DIRECTION_AND_STATE);
        OutputBuffer.push_back(0xEF);    // Set all but DTR high (only affects pins which are output)
        OutputBuffer.push_back(0x13);	//Set SK,DO pins as output  and D4
                                        // Send off the commands
        FT_STATUS ftStatus = (*Ftd2XXDynamic::FT_Write)(m_handle, &OutputBuffer[0], OutputBuffer.size(), &dwNumBytesSent);
        if (ftStatus != FT_OK || dwNumBytesSent != OutputBuffer.size())
            return false;
        OutputBuffer.clear();
        OutputBuffer.push_back(SET_8PIN_DIRECTION_AND_STATE);
        OutputBuffer.push_back(0xFF);    // Set all but DTR high (only affects pins which are output)
        OutputBuffer.push_back(0x03);	//Set SK,DO pins as output, DTR back to input
        Sleep(5); // hold dtr low for 5 msec
        ftStatus = (*Ftd2XXDynamic::FT_Write)(m_handle, &OutputBuffer[0], OutputBuffer.size(), &dwNumBytesSent);	// Send off the commands
        if (ftStatus != FT_OK || dwNumBytesSent != OutputBuffer.size())
            return false;
        return true;
    }

    std::string CFrontPanel::GetIdString()
    {
        std::string ret;
        if (m_initialized)
        {
            static const int RETURN_EXPECTED = 30;
            std::vector < BYTE> cmd(1);
            cmd[0] = COMMAND_GET_ID_STRING;
            std::vector<BYTE> res = getCmdResults(cmd, RETURN_EXPECTED, 2);
            for (auto c : res)
                if (isprint(c))
                    ret += char(c);
                else
                    break;

        }
        return ret;
    }

    void CFrontPanel::SetIdString(const std::string &n)
    {
        if (m_initialized)
        {
            static const int RETURN_EXPECTED = 1;
            std::vector<BYTE> cmd(32);
            cmd[0] = COMMAND_SET_ID_STRING;

            for (unsigned i = 1; (i < cmd.size()) && (i < n.size() + 1); i += 1)
                cmd[i] = n[i - 1];
            getCmdResults(cmd, RETURN_EXPECTED, 2);
        }
    }

    bool CFrontPanel::SetStringObject(unsigned char w, const std::string &n)
    {
        if (m_initialized)
        {
            static const int RETURN_EXPECTED = 1;
            std::vector<BYTE> cmd(32);
            cmd[0] = COMMAND_SET_DISPLAY_STRING;
            cmd[1] = w;
            unsigned j = 0;
            for (unsigned i = 2; (i < cmd.size()) && (j < n.size()); i += 1, j+=1)
                cmd[i] = n[j];
            std::vector<BYTE> res = getCmdResults(cmd, RETURN_EXPECTED, 2);
            return res.size() == RETURN_EXPECTED;
        }
        return false;
    }

    bool CFrontPanel::GetInputState(short enc[NUMBER_OF_ENCODERS], unsigned short &switches, unsigned char &encoderSwitches)
    {
        if (m_initialized)
        {
            static const int RETURN_EXPECTED = sizeof(unsigned char) + (NUMBER_OF_ENCODERS + 1) * sizeof(short);
            std::vector<BYTE> cmd(1);
            cmd[0] = COMMAND_GET_INPUT_STATE;
            std::vector<BYTE> res = getCmdResults(cmd, RETURN_EXPECTED, 2);
            if (res.size() == RETURN_EXPECTED)
            {
                unsigned i = 0;
                unsigned j = 0;
                enc[j] = res[i++];
                enc[j++] |= res[i++] << 8;
                enc[j] = res[i++];
                enc[j++] |= res[i++] << 8;
                enc[j] = res[i++];
                enc[j++] |= res[i++] << 8;
                enc[j] = res[i++];
                enc[j++] |= res[i++] << 8;
                enc[j] = res[i++];
                enc[j++] |= res[i++] << 8;
                enc[j] = res[i++];
                enc[j++] |= res[i++] << 8;
                enc[j] = res[i++];
                enc[j++] |= res[i++] << 8;
                switches = res[i++];
                switches |= res[i++] << 8;
                encoderSwitches = res[i++];
                return true;
            }
        }
        return false;
    }

    bool CFrontPanel::SetEncoderCenter(unsigned which, unsigned int encValue)
    {
        if (m_initialized)
        {
            static const int RETURN_EXPECTED = 1;
            std::vector<BYTE> cmd(6);
            cmd[0] = COMMAND_SET_ENCODER_CENTER;
            cmd[1] = (BYTE)which;
            auto temp = encValue;
            for (int i = 2; i < 6; i++)
            {
                cmd[i] = static_cast<BYTE>(temp);
                temp >>= 8;
            }
            std::vector<BYTE> res = getCmdResults(cmd, RETURN_EXPECTED, 2);
            return res.size() == RETURN_EXPECTED;
        }
        return false;
    }

    bool CFrontPanel::SetTrellisGroup(unsigned char which, unsigned short mask, unsigned short displayObjType, unsigned short objIndex)
    {
        if (m_initialized)
        {
            static const int RETURN_EXPECTED = 1;
            std::vector<BYTE> cmd(8);
            cmd[0] = COMMAND_SET_TRELLIS_GROUP;
            cmd[1] = (BYTE)which;
            cmd[2] = (BYTE)mask;
            cmd[3] = (BYTE)(mask >> 8);
            cmd[4] = (BYTE)displayObjType;
            cmd[5] = (BYTE)(displayObjType >> 8);
            cmd[6] = (BYTE)objIndex;
            cmd[7] = (BYTE)(objIndex >> 8);
            std::vector<BYTE> res = getCmdResults(cmd, RETURN_EXPECTED, 3);
            return res.size() == RETURN_EXPECTED;
        }
        return false;
    }
    
    bool CFrontPanel::SetEncoderMap(unsigned char which, unsigned short displayObjLowType, unsigned short displayObjLowIndex,
        unsigned short displayObjHighType, unsigned short displayObjHighIndex, short mult,
        int lowLimit, int highLimit)
    {
        if (m_initialized)
        {
            static const int RETURN_EXPECTED = 1;
            std::vector<BYTE> cmd(20);
            cmd[0] = COMMAND_SET_ENCODER_MAP;
            cmd[1] = (BYTE)which;
            cmd[2] = (BYTE)displayObjLowType;
            cmd[3] = (BYTE)(displayObjLowType >> 8);
            cmd[4] = (BYTE)displayObjLowIndex;
            cmd[5] = (BYTE)(displayObjLowIndex >> 8);
            cmd[6] = (BYTE)displayObjHighType;
            cmd[7] = (BYTE)(displayObjHighType >> 8);
            cmd[8] = (BYTE)displayObjHighIndex;
            cmd[9] = (BYTE)(displayObjHighIndex >> 8);
            cmd[10] = (BYTE)mult;
            cmd[11] = (BYTE)(mult >> 8);
            cmd[12] = (BYTE)lowLimit;
            cmd[13] = (BYTE)(lowLimit >> 8);
            cmd[14] = (BYTE)(lowLimit >> 16);
            cmd[15] = (BYTE)(lowLimit >> 24);
            cmd[16] = (BYTE)highLimit;
            cmd[17] = (BYTE)(highLimit >> 8);
            cmd[18] = (BYTE)(highLimit >> 16);
            cmd[19] = (BYTE)(highLimit >> 24);
            std::vector<BYTE> res = getCmdResults(cmd, RETURN_EXPECTED, 3);
            if (res.size() == RETURN_EXPECTED)
                return true;
        }
        return false;
    }

    bool CFrontPanel::PressTrellisButton(unsigned char which, unsigned char set)
    {
        if (m_initialized)
        {
            static const int RETURN_EXPECTED = 1;
            std::vector<BYTE> cmd(3);
            cmd[0] = COMMAND_PRESS_TRELLIS_BUTTON;
            cmd[1] = (BYTE)which;
            cmd[2] = (BYTE)set;
            std::vector<BYTE> res = getCmdResults(cmd, RETURN_EXPECTED, 2);
            if (res.size() == RETURN_EXPECTED)
                return true;
        }
        return false;
    }

    bool CFrontPanel::SetDisplayObjects(const SetDisplayObject toDisplay[NUM_DISPLAY_OBJECTS_IN_I2C_MESSAGE])
    {
        if (m_initialized)
        {
            static const int RETURN_EXPECTED = 1;
            static const unsigned MESSAGE_LENGTH = 1 + NUM_DISPLAY_OBJECTS_IN_I2C_MESSAGE * sizeof(SetDisplayObject);
            static_assert(MESSAGE_LENGTH <= 32, "message must fit in 32 bytes");
            std::vector<BYTE> cmd(1);
            unsigned j = 0;
            cmd[j++] = COMMAND_SET_DISPLAY_OBJECT;
            for (unsigned i = 0; i < NUM_DISPLAY_OBJECTS_IN_I2C_MESSAGE; i++)
            {
                if (toDisplay[i].objType != 0xffff)
                {
                    // as the i2c master, we can modulate the size
                    // of the message. The slave cannot.
                    cmd.resize(cmd.size() + 6);
                    cmd[j++] = (BYTE)toDisplay[i].objType;
                    cmd[j++] = (BYTE)(toDisplay[i].objType >> 8);
                    cmd[j++] = (BYTE)toDisplay[i].objIdx;
                    cmd[j++] = (BYTE)(toDisplay[i].objIdx >> 8);
                    cmd[j++] = (BYTE)(toDisplay[i].value);
                    cmd[j++] = (BYTE)(toDisplay[i].value >> 8);
                }
                else
                    break;
            }
            std::vector<BYTE> res = getCmdResults(cmd, RETURN_EXPECTED, 2);
            if (res.size() == RETURN_EXPECTED)
                return true;
        }
        return false;
    }

    bool CFrontPanel::SetTrellisBrightness(unsigned char b)
    {
        if (m_initialized)
        {
            static const int RETURN_EXPECTED = 1;
            std::vector<BYTE> cmd(2);
            cmd[0] = COMMAND_SET_TRELLIS_BRIGHTNESS;
            cmd[1] = (BYTE)b;
            std::vector<BYTE> res = getCmdResults(cmd, RETURN_EXPECTED, 2);
            if (res.size() == RETURN_EXPECTED)
                return true;
        }
        return false;
    }

    bool CFrontPanel::SetEncoderSwitchState(unsigned char b)
    {
        if (m_initialized)
        {
            static const int RETURN_EXPECTED = 1;
            std::vector<BYTE> cmd(2);
            cmd[0] = COMMAND_SET_ENCODER_SWITCH_STATE;
            cmd[1] = (BYTE)b;
            std::vector<BYTE> res = getCmdResults(cmd, RETURN_EXPECTED, 2);
            if (res.size() == RETURN_EXPECTED)
                return true;
        }
        return false;
    }

    bool CFrontPanel::ResetDisplayDefaults()
    {
        if (m_initialized)
        {
            static const int RETURN_EXPECTED = 1;
            std::vector<BYTE> cmd(1);
            cmd[0] = COMMAND_RESET_DISPLAY_DEFAULTS;
            std::vector<BYTE> res = getCmdResults(cmd, RETURN_EXPECTED, 2);
            if (res.size() == RETURN_EXPECTED)
                return true;
        }
        return false;
    }

    bool CFrontPanel::SetLcdImageFileName(const std::string &fn, std::string &result)
    {
        if (m_initialized)
        {
            static const int RETURN_EXPECTED = 2;
            std::vector<BYTE> cmd(13, ' ');
            cmd[0] = COMMAND_SET_LCD_IMAGE_NAME;
            unsigned j = 0;
            for (unsigned i = 1; (i < cmd.size()) && (j < fn.size()); )
                cmd[i++] = fn[j++];
            std::vector<BYTE> res = getCmdResults(cmd, RETURN_EXPECTED, 2);
            if (res.size() == RETURN_EXPECTED)
            {
                result.assign(res.begin(), res.end());
                return true;
            }
        }
        return false;
    }

    bool CFrontPanel::GetLcdImageVersion(int &result)
    {
        if (m_initialized)
        {
            static const int RETURN_EXPECTED = 4;
            std::vector<BYTE> cmd(1);
            cmd[0] = COMMAND_GET_LCD_IMAGE_VERSION;
            std::vector<BYTE> res = getCmdResults(cmd, RETURN_EXPECTED, 2);
            if (res.size() == RETURN_EXPECTED)
            {
                result = 0;
                for (int i = 0; i < RETURN_EXPECTED; i++)
                    result |= (0xff & res[i]) << (i * 8);
                return true;
            }
        }
        return false;
    }

}


#ifdef _DEBUGxx
    // test that our own definitions match those of the system
// compile/link error here means the typedefs copied from ftd2xx.h are incorrect!
#pragma comment(lib, "FTD2XX.lib")

namespace {
    FtdiInterface::FT_Close p1 = &::FT_Close;
    FtdiInterface::FT_CyclePort p2 = &FT_CyclePort;
    FtdiInterface::FT_OpenEx p3 = &FT_OpenEx;
    FtdiInterface::FT_CreateDeviceInfoList p4=&::FT_CreateDeviceInfoList;
    FtdiInterface::FT_GetDeviceInfoList p5 = &::FT_GetDeviceInfoList;
    FtdiInterface::FT_Write p6 = &::FT_Write;
    FtdiInterface::FT_ResetDevice p7 = &::FT_ResetDevice;
    FtdiInterface::FT_SetTimeouts p8 = &::FT_SetTimeouts;
    FtdiInterface::FT_GetQueueStatus p9 = &::FT_GetQueueStatus;
    FtdiInterface::FT_SetLatencyTimer p10 = &::FT_SetLatencyTimer;
    FtdiInterface::FT_SetBitMode p11 = &::FT_SetBitMode;
    FtdiInterface::FT_SetUSBParameters p12 = &::FT_SetUSBParameters;
    FtdiInterface::FT_SetChars p13 = &::FT_SetChars;
    FtdiInterface::FT_Read p14 = &::FT_Read;
}
#endif
