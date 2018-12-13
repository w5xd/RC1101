#include <arduino.h>
#include <genieArduino.h> // 4D systems display--unmodified.
#include <EEPROM.h>
#include "SAdafruit_Trellis.h"  // modified manufacturer's library to use twi.cpp directly 
#include "twi.h" // modified 2-wire driver for AVR

/* This is the firmware for the WriteLog Remote front panel, RC-1101.
**
** The arduino Pro Mini 5V is required. 
**    The compile-time variations for the Mega helped with development.
**
** The arduino is connected to:
**    4dsystems uLCD-32PTU display (3.2 inch) (via the serial port)
**    Adafruit Trellis 4x4 keyboard with LED indicators (via i2c)
**    ftdichip FT232H on Adafruit break-out board (via i2c)
**    up to 4 rotary encoders. via 2 arduino pins each.
**    3 of the 4 encoders may have push-to-activate switches (via 1 pin for each)
**
** There is no rig-specific functionality here. Instead, it is all "soft"
** The rig-specific functionality comes from the combination of:
** (a) rig-specific programming of the uLCD-32PTU display
** (b) rig-specific setup over the USB port via the FT-232H
**
** A Win32 dll, RadioPanelUSB, is published with this sketch. RadioPanelUSB
** is a Win32 implementation using ftdichip's published ftd2xx dll interface
** to talk to the FT232H chip. The combination achieves roughly 15 bytes
** of transfer in both directions every 15 msec.
**
** The i2c interconnect has the advantage of consuming only 2 wires to accomplish
** USB communications with the host along with 16 key button input + 16 LEDs, one
** for each button.
** 
** ...but it is complicated by the fact that the FT-232H can do i2c only as a bus
** master but CANNOT do i2c multi-master arbitration, while the Trellis operates 
** only as a bus slave. The Arduino can do either (one at a time.) The result
** is a modified version of the AVR code for twi that is unique to this sketch and in
** twi.cpp. The twi modification required like modifications to Adafruit's trellis
** library, and that modified version is also part of this ketch.
**
** Keeping the FT-232H off the i2c bus while the arduino is the master consumes
** a 3rd pin, so its not really a "2-wire" interface here.
*/

// to help debug interactions between devices, the following compile systems
// include/omit the Genie, i2c-to-pc,  trellis, and use of Serial for debugging messages
#define DO_GENIE // the 4Dsystems display
#define DO_PC_I2C // i2c to the FT-232H, arduino is slave
#define DO_TRELLIS  // i2c to the trellis. arduino is master
#define SERIAL_DEBUG
#if defined(DO_GENIE)
#undef SERIAL_DEBUG // cannot have both genie and serial debug...on Pro Mini anyway
#endif
#define NUMBER_OF_ROTARY_ENCODERS 4 // Must be 1, 2, 3, or 4
// encoder1 has no switch, but the other 3 have a second map
#define NUMBER_OF_ENCODER_MAPS ((NUMBER_OF_ROTARY_ENCODERS * 2) - 1) 

#define ENCODER3_AND_4_HANDLING 2 // 0 is poll. 1 is PCINT0, 2 is on a timer interrupt
// for reasons I have been unable to track down, the "pin change interrupt" value, 1 crashes
// the arduino randomly.

static const int32_t LOWEST_INT32 = 0x80000000;
static const int32_t HIGHEST_INT32 = 0x7fffffff;

template <uint8_t ENC_PLUS, uint8_t ENC_MINUS,
        uint8_t SWITCHMMASK>
class QuadratureEncoder
{
    // each instance of this object is for one physical
    // encoder, but holds two states, one for each 
    // each switch position.
public:
    QuadratureEncoder();
    void OnEncoder(uint8_t);
    void SetEncoderCenter(int32_t p);
    void SetEncoderCenter2(int32_t p);
    void ResetEncoderCenters() 
    {SetEncoderCenter(0); SetEncoderCenter2(0); SetLimits(); SetLimits2(); }
    int16_t GetPosition() { return offsetPosition;} // primary
    int16_t GetPosition2() { return offsetPosition2;} // secondary
    uint8_t loop(int32_t &pos, int32_t &pos2);
    void SetLimits(int32_t lowLimit=LOWEST_INT32, int32_t highLimit = HIGHEST_INT32)
    { limitLow = lowLimit; limitHigh = highLimit;}
    void SetLimits2(int32_t lowLimit=LOWEST_INT32, int32_t highLimit = HIGHEST_INT32)
    { limitLow2 = lowLimit; limitHigh2 = highLimit;}
    void update();

    enum { BITMASK = (1 << ENC_PLUS) | (1 << ENC_MINUS)};
private:
    // non are volatile because interrupts are always off when accessed
    int32_t referencePosition;
    int16_t offsetPosition;
    int32_t referencePosition2;
    int16_t offsetPosition2;
    int32_t prevLoopPos;
    int32_t prevLoopPos2;
    uint8_t lastEncoder;
    int32_t limitLow;
    int32_t limitHigh;
    int32_t limitLow2;
    int32_t limitHigh2;
};
static volatile uint8_t forceEncoderUpdate;

namespace {
    Twi::Adafruit_Trellis matrix0 = Twi::Adafruit_Trellis();
    Twi::Adafruit_TrellisSet trellis = Twi::Adafruit_TrellisSet(&matrix0);

    /* We arrange for the different processors to have the
    ** same inputs on the same ports/pins (PORTB, bits 0 through 5)
    ** on the two processor architectures so we don't need conditional compile to
    ** read the pins*/
#if defined (__AVR_ATmega2560__) // NOT FULLY SUPPORTED BELOW
    /* Mega 2560. Useful for debugging cuz has extra serial ports   
    ** https://www.arduino.cc/en/uploads/Hacking/PinMap2560big.png
    ** Mega development board assignments chosen to make the software
    ** match the target 328 hardware as close as possible. */
    const int ENCODER1_PLUS_PIN = 11; // PORTB PCINT5
    const int ENCODER1_MINUS_PIN = 10;  // PORTB PCINT4
    const int ENCODER2_PLUS_PIN = A9; // PCINT17 - PORTK bit 1
    const int ENCODER2_MINUS_PIN = A8; // PCINT16 = PORTK bit 0
    const int ENCODER3_PLUS_PIN = 50; // PORTB, bit 3
    const int ENCODER3_MINUS_PIN = 51; // PORTB, bit 2
    const int ENCODER4_MINUS_PIN = 53; // PORTB, bit 0
    const int ENCODER4_PLUS_PIN = 52; // PORTB, bit 1

    const int ENCODER4_SWITCH_PIN = A7; 
    const int ENCODER2_SWITCH_PIN = 5;
    const int ENCODER3_SWITCH_PIN = 7;

#elif defined(__AVR_ATmega328P__)
    /* PRO MINI    
    ** https://www.arduino.cc/en/Hacking/PinMapping168 */
    const int ENCODER1_PLUS_PIN = 6; // PCINT22 -- PORTD
    const int ENCODER1_MINUS_PIN = 5; // PCINT21 -- PORTD
    const int ENCODER2_PLUS_PIN = 15; //PCINT 9 -- PORTC
    const int ENCODER2_MINUS_PIN = 14; //PCINT 8 -- PORTC
    const int ENCODER3_PLUS_PIN = 11; // PCINT3 -- PORTB
    const int ENCODER3_MINUS_PIN = 10; // PCINT2 -- PORTB
    const int ENCODER4_MINUS_PIN = 8; // PortB bit 0
    const int ENCODER4_PLUS_PIN = 9;   // PortB bit 1

    const int ENCODER2_SWITCH_PIN = 12;
    const int ENCODER3_SWITCH_PIN = 7;
    const int ENCODER4_SWITCH_PIN = A7; // use this one last. its analog only

    const int CWDOT_PIN = 3;
    const int CWDASH_PIN = 2;
    const int FOOTTIP_PIN = 16;

#else
#error "CPU not supported"
#endif
    const int NUMTRELLISKEYS = 16; // Trellis hardware can go beyond 16, but we don't
    const int NUM_ENCODER_SWITCHES = NUMBER_OF_ROTARY_ENCODERS - 1; // All but first
    // map both trellis and encoder switches to Genie
    const int NUM_DISPLAY_MAPS = NUMTRELLISKEYS + NUM_ENCODER_SWITCHES; 

    // The TWI_MASTERSLAVE_PIN is used to keep the ft232h i2c interface
    // off the i2c bus. the ft232h does NOT do multi-master arbitration, but
    // does support a 3rd wire "wait for" hardware handshake.
    const int TWI_MASTERSLAVE_PIN = 17; //ft232H 
    const int CWOUTPUT_PIN = 13;

#if defined(DO_GENIE)
    Genie genie;
#define GENIE_SHIELD_INVERTS_RESET 0 // either zero (with no shield) or 1--with shield
    // use only open-collector pull down on the GENIE_RESET_PIN, regardless of shield
    // The genie shield hardware is only used for debugging. This inverts-reset
    // compile-time stuff is due to the fact that the shield adds an open-collector
    // that you don't get without it...
    const int GENIE_RESET_PIN = 4;  // per the 4D systems shield
#endif

    // *********************************************************************
    // 3 of the 4 encoders have momentary push-in switches. 
    // We record a 1 in encoderSwitches while the switch is pushed down
    // ...which, with our pullup wiring, is a zero on our input pin
    const uint8_t encoder1SwitchMask = 0; // there is no encoder 1 switch
    const uint8_t encoder2SwitchMask = 1;
    const uint8_t encoder3SwitchMask = 2;
    const uint8_t encoder4SwitchMask = 4;

#if NUMBER_OF_ROTARY_ENCODERS > 0
#if defined (__AVR_ATmega2560__)
    const uint8_t ENCODER1_PLUS_BITNUM = 5;
    const uint8_t ENCODER1_MINUS_BITNUM = 4; 
#elif defined(__AVR_ATmega328P__)
    const uint8_t ENCODER1_PLUS_BITNUM = 6;
    const uint8_t ENCODER1_MINUS_BITNUM = 5; 
#endif
    QuadratureEncoder<ENCODER1_PLUS_BITNUM, ENCODER1_MINUS_BITNUM, encoder1SwitchMask> 
        encoder1;
#if NUMBER_OF_ROTARY_ENCODERS > 1
    const uint8_t ENCODER2_PLUS_BITNUM = 1;
    const uint8_t ENCODER2_MINUS_BITNUM = 0;
    QuadratureEncoder<ENCODER2_PLUS_BITNUM, ENCODER2_MINUS_BITNUM, encoder2SwitchMask> 
        encoder2;
#if NUMBER_OF_ROTARY_ENCODERS > 2
    void ReadEncoders3And4(); // read these two. however
    const uint8_t ENCODER3_PLUS_BITNUM = 3;
    const uint8_t ENCODER3_MINUS_BITNUM = 2;
    QuadratureEncoder<ENCODER3_PLUS_BITNUM, ENCODER3_MINUS_BITNUM, encoder3SwitchMask> 
        encoder3;
#if NUMBER_OF_ROTARY_ENCODERS > 3
    const uint8_t ENCODER4_PLUS_BITNUM = 1;
    const uint8_t ENCODER4_MINUS_BITNUM = 0;
    QuadratureEncoder<ENCODER4_PLUS_BITNUM, ENCODER4_MINUS_BITNUM, encoder4SwitchMask> 
        encoder4;
#endif
#endif
#endif
#endif
}

// debug LED output byte
extern "C" uint8_t WhereAmI;
uint8_t WhereAmI;

namespace cw {
    void setup();
    void loop();
}

namespace PcHost {
    // The i2c address the PC will use to address us as slave
    const int myI2cSlaveAddr = 42; // the answer to the question

    // message ID's host to us.
    const byte NO_COMMAND = 0;
    const byte ID_YOURSELF = 1;
    const byte LONG_TEST = 2;
    const byte SHORT_TEST = 3;
    const byte GET_ID_STRING = 4;
    const byte SET_ID_STRING = 5;
    const byte GET_INPUT_STATE = 6;
    const byte SET_ENCODER_CENTER = 7; // one byte ID plus
    const byte SET_TRELLIS_GROUP = 8;
    const byte SET_ENCODER_MAP = 9;
    const byte SET_DISPLAY_STRING = 10;
    const byte PRESS_TRELLIS_BUTTON = 11;
    const byte SET_DISPLAY_OBJECTS = 12;
    const byte SET_TRELLIS_BRIGHTNESS = 13;
    const byte RESET_DISPLAY_DEFAULTS = 14;
    const byte SET_ENCODER_SWITCH_STATE = 15;

    struct IdString {
        char name[30];
    };
}

enum eeprom_offset { EEPROM_IDSTRING,
EEPROM_TRELLIS_BRIGHTNESS = EEPROM_IDSTRING + sizeof(PcHost::IdString)};

volatile uint8_t powerUpDefaults = 1;

#if defined(DO_TRELLIS)
namespace Trellis {
    void clear()
    {
       // light up all the LEDs in order. startup display
        for (uint8_t i = 0; i < NUMTRELLISKEYS; i++) {
            trellis.setLED(i);
            if (i > 0) trellis.clrLED(i - 1);
            trellis.writeDisplay();
            delay(50);
        }
        trellis.clrLED(NUMTRELLISKEYS - 1);
        trellis.writeDisplay();
    }
}
#endif

void setup()
{
#if defined(DO_GENIE)
    Serial.begin(200000);  // Serial0 @ 200000 (200K) Baud--genie docs say use 200K
    genie.Begin(Serial);   // Use Serial0 for talking to the Genie Library, and to the 4D Systems display
    static const int GENIE_STARTUP_MSEC = 4500; // When the uLCD-32PTU is set to read its program from flash, 
    // 3.5 seconds was long enough for SOME boards and not for others...an extra second should be enough?
    static const int GENIE_RESET_MSEC = 100;

    // Reset the Display (change D4 to D2 if you have original 4D Arduino Adaptor)
    // THIS IS IMPORTANT AND CAN PREVENT OUT OF SYNC ISSUES, SLOW SPEED RESPONSE ETC

    // If NOT using a 4D Arduino Adaptor, digitalWrites must be reversed as Display Reset is Active Low, and
    // the 4D Arduino Adaptors invert this signal so must be Active High.  
#if GENIE_SHIELD_INVERTS_RESET
    // the 4D Arduino Adaptors invert this signal so must be Active High.  
    pinMode(GENIE_RESET_PIN, OUTPUT);  // Set D4 on Arduino to Output (4D Arduino Adaptor V2 - Display Reset)
    digitalWrite(GENIE_RESET_PIN, HIGH);  // Reset the Display via D4
    delay(GENIE_RESET_MSEC);
    digitalWrite(GENIE_RESET_PIN, LOW);  // unReset the Display via D4
#else
    //  NOT using a 4D Arduino Adaptor, the GENIE_RESET_PIN must NOT be driven HIGH. It is a 3.3V part!
    // It has an external pullup, so drive it LOW...  
    digitalWrite(GENIE_RESET_PIN, LOW);  // Reset the Display via D4
    pinMode(GENIE_RESET_PIN, OUTPUT);  // Set D4 on Arduino to Output (4D Arduino Adaptor V2 - Display Reset)
    delay(GENIE_RESET_MSEC);
    pinMode(GENIE_RESET_PIN, INPUT);  // ... let the display pull its reset line back high
#endif
    delay(GENIE_STARTUP_MSEC); //let the display start up after the reset (This is important)

#elif defined(SERIAL_DEBUG)
    Serial.begin(9600);
    Serial.println("DEBUG");
#endif

    // encoder hardware requires pullups. 
#if NUMBER_OF_ROTARY_ENCODERS > 0
    pinMode(ENCODER1_PLUS_PIN, INPUT_PULLUP); // pullup needed only for certain hardware
    pinMode(ENCODER1_MINUS_PIN, INPUT_PULLUP); // DITTO
#if NUMBER_OF_ROTARY_ENCODERS > 1
    pinMode(ENCODER2_PLUS_PIN, INPUT_PULLUP); // pullup needed only for certain hardware
    pinMode(ENCODER2_MINUS_PIN, INPUT_PULLUP); // DITTO
#if NUMBER_OF_ROTARY_ENCODERS > 2
    pinMode(ENCODER3_PLUS_PIN, INPUT_PULLUP); // pullup needed only for certain hardware
    pinMode(ENCODER3_MINUS_PIN, INPUT_PULLUP); // DITTO
#if NUMBER_OF_ROTARY_ENCODERS > 3
    pinMode(ENCODER4_PLUS_PIN, INPUT_PULLUP); // pullup needed only for certain hardware
    pinMode(ENCODER4_MINUS_PIN, INPUT_PULLUP); // DITTO
#endif
#endif
#endif
#endif

#if defined(DO_PC_I2C)
    twi_setAddress(PcHost::myI2cSlaveAddr);
#endif
    twi_init();

#if defined(DO_TRELLIS)
    trellis.begin(0x70);  // One trellis on its default i2c addr
    byte brightness = EEPROM.read(EEPROM_TRELLIS_BRIGHTNESS);
    if (brightness == 0)
        brightness = 10;
    trellis.setBrightness(brightness);
#endif

#if defined(DO_GENIE)
    // Set the brightness/Contrast of the Display - (Not needed but illustrates how)
    // Most Displays, 1 = Display ON, 0 = Display OFF. See below for exceptions and for DIABLO16 displays.
    // For uLCD-43, uLCD-220RD, uLCD-70DT, and uLCD-35DT, use 0-15 for Brightness Control, 
    // where 0 = Display OFF, though to 15 = Max Brightness ON.
    genie.WriteContrast(1);
#endif

#if defined (__AVR_ATmega2560__)
// INPUT CHANGE INTERRUPT SETUP FOR MEGA *******************
    // rotary encoder bits to interrupt on
    // encoders 1, 3, 4 but not 2 are on PCINT0 on MEGA
    PCICR |= 1; // Enable PCINT0
#if NUMBER_OF_ROTARY_ENCODERS == 1
    PCMSK0 |= encoder1.BITMASK;
#elif NUMBER_OF_ROTARY_ENCODERS == 2
    PCMSK0 |= encoder1.BITMASK;
#elif NUMBER_OF_ROTARY_ENCODERS == 3
    PCMSK0 |= encoder1.BITMASK | encoder3.BITMASK;
#elif NUMBER_OF_ROTARY_ENCODERS == 4
    PCMSK0 |=  encoder1.BITMASK | encoder3.BITMASK | encoder4.BITMASK;
#else
#error "Unsupported number of encoders"
#endif
#if NUMBER_OF_ROTARY_ENCODERS > 1
    PCMSK2 |= encoder2.BITMASK; // on PORTK, PCINT2
    PCICR |= 4;
#endif
// END OF INPUT CHANGE INTERRUPT SETUP FOR MEGA *******************

#elif defined(__AVR_ATmega328P__)
// INPUT CHANGE INTERRUPT SETUP FOR PRO MINI *******************
    // encoder 1 is on PCINT2. 
    // encoder 2 is on PCENT1 
    // encoders 3, 4 are on PCINT0, 
#if NUMBER_OF_ROTARY_ENCODERS >= 1
    PCICR |= 4; // Enable PCINT2
    PCMSK2 = encoder1.BITMASK;
#endif
#if NUMBER_OF_ROTARY_ENCODERS >= 2
    PCICR |= 2; // Enable PCINT1
    PCMSK1 = encoder2.BITMASK; // on PORTC, PCINT1
#endif
#if ENCODER3_AND_4_HANDLING==1 // encoder 3 and 4 are POLLED cuz interrupts hung
#if NUMBER_OF_ROTARY_ENCODERS == 3
    PCICR |= 1; // Enable PCINT0
    PCMSK0 = encoder3.BITMASK;
#elif NUMBER_OF_ROTARY_ENCODERS == 4
    PCICR |= 1; // Enable PCINT0
    PCMSK0 = encoder3.BITMASK | encoder4.BITMASK;
#else
#error "Unsupported number of encoders"
#endif
#endif
// END OF INPUT CHANGE INTERRUPT SETUP FOR PRO MINI *******************
#endif

    pinMode(ENCODER2_SWITCH_PIN, INPUT_PULLUP);
    pinMode(ENCODER3_SWITCH_PIN, INPUT_PULLUP);
    // PIN A7 is analog input ONLY (on the 328)
    // set up the ADC to digitize AD7, free-running,
    // and leave it on all the time.
    ADMUX = 0x47; // Vcc reference. A7 input
    ADCSRB = 0; // trigger is "free-running"
    ADCSRA = _BV(ADEN) // enable
            | _BV(ADSC) // start first conversion
            | _BV(ADATE)   // Auto trigger.
            | 7; // prescale by 1/128
    // A7 must have an external pullup!

    cw::setup();
    pinMode(TWI_MASTERSLAVE_PIN, OUTPUT);
    digitalWrite(TWI_MASTERSLAVE_PIN, LOW); // we start out as master
#if ENCODER3_AND_4_HANDLING==0
    ReadEncoders3And4(); // init encoder polling once
#endif
    WhereAmI = 10 - 2;
}

enum I2C_state_t {I2C_MASTER=2 /* low on FT232H holding pin and slave hold time expired*/, 
        I2C_SLAVE /* high on pin, FT232H can master*/, 
        I2C_SLAVE_HOLD /* pin low, but no i2c mastering. ensure FT232H is NOT mastering */
        };
static uint8_t i2Cstate = I2C_MASTER;
static_assert(sizeof(i2Cstate) == 1, "assuming i2Cstate access is atomic");
static const int I2C_LONG_TRANSACTION_MSEC = 34; // hold TWI_MASTERSLAVE_PIN high this long
static const int I2C_SLAVE_HOLD_DEFAULT_MSEC = 3; // twi_onSlaveAddressed extends this to the above 
static const int I2C_SLAVE_EXTEND_MSEC = 2; // when we read SDA/SCL low
static const int I2C_MASTER_HOLD_MSEC = 2;
static const int I2C_SLAVE_ASSUME_FT232_WAITING_MSEC = 150;
static const int TRELLIS_POLL_MSEC = 36; // This debounces the trellis switches. 

static volatile long nextI2CtransitionTime;
static long heardI2CSdaSclLowTime;
static volatile long trellisWait;

namespace {
    const int GENIE_NAK_RETRIES = 2;
    /* This firmware treats the genie display as generic. We do a hardware
    ** reset initialization of a couple of mappings, but the map between the
    ** physical input devices and the display objects can be updated by
    ** calls through our USB interface*/
    struct GenieObject {
        GenieObject() : object(uint16_t(-1)), index(0){}
        GenieObject(uint16_t o, uint16_t i) : object(o), index(i){}
        bool isValid() const { return static_cast<int16_t>(object) != -1;}
            uint16_t object;
            uint16_t index;
    };

    // staging memory to support the PcHost::SET_DISPLAY_STRING command
    const int DISPLAY_STR_LEN = 32;
    volatile uint8_t strIndex; // 0xFF is an invalid string id
    uint8_t displayStr[DISPLAY_STR_LEN];

    // staging memory to support the PcHost::SET_DISPLAY_OBJECTS command
    struct SetDisplayObject {
        SetDisplayObject() : value(0){}
        GenieObject go;
        uint16_t   value;
    };
    static_assert(sizeof(SetDisplayObject) == 6, "6 bytes to set display");
    const int DISPLAY_OBJECT_MAX_PER_MESSAGE = 5;
    SetDisplayObject DisplaysToUpdate[DISPLAY_OBJECT_MAX_PER_MESSAGE];

    volatile uint8_t encoderSwitches; 

    uint8_t encoderSwitchState;

    const uint8_t DEBOUNCE_HIGH_COUNT = 3;
    uint8_t encoder2SwitchHighCount;
    uint8_t encoder3SwitchHighCount;
    uint8_t encoder4SwitchHighCount;

    void debounceEncoderSwitch(bool isHigh, uint8_t mask, uint8_t &highCount)
    {
        if (isHigh)
        {   // assuming the low to high transition bounces more
            if (highCount < DEBOUNCE_HIGH_COUNT)
                highCount += 1;
            else
                encoderSwitchState &= ~mask;
        }
        else
        {   // going low lets go of the contact and should not bounce much
            highCount = 0;
            encoderSwitchState |= mask;
        }
    }
    //*********************************************************************
}

namespace EncoderDisplay {
    /* The hardware has 4 encoders. Each can be mapped to updated
    ** either one or two genie objects. Two are required if the
    ** display goes to more than 9999... like frequency display*/
    struct MapEntry {
        GenieObject low10e4;
        GenieObject high10e4;
        int16_t     multiplier;
        void Apply(int32_t enc)
        {   // update the LCD display digits per our table entries
#if defined(DO_GENIE)
            noInterrupts();
            GenieObject low(low10e4);
            GenieObject high(high10e4);
            interrupts();
            if (low.isValid())
            {
                enc *= multiplier;
                if (high.isValid())
                {   // we have a pair of table entries with 4 digits each
                    int top = enc / 10000; 
                    int bot = enc % 10000;                                    
                    for (int i = 0; i < GENIE_NAK_RETRIES; i++)
                    {
                        bool repeat = false;
                        if (GENIE_NAK == genie.WriteObject(low.object, low.index, bot))
                            repeat = true;
                        if (GENIE_NAK == genie.WriteObject(high.object, high.index, top))
                            repeat = true;
                        if (!repeat)
                            break;
                    }
                }
                else if (low.isValid()) // we just have one entry. fits in 9999. or +/- 999
                {
                     for (int i = 0; i < GENIE_NAK_RETRIES; i++)
                    {
                        if (GENIE_NAK !=genie.WriteObject(low.object, low.index, enc))
                            break;
                     }
                }
            }
#endif
        }
        MapEntry(const GenieObject &low, const GenieObject &high)
            : low10e4(low), high10e4(high), multiplier(1)
        {}
        MapEntry() : multiplier(1) {}
    };

    MapEntry encoderMaps[NUMBER_OF_ENCODER_MAPS] = {
        MapEntry( GenieObject(GENIE_OBJ_LED_DIGITS, 0), GenieObject(GENIE_OBJ_LED_DIGITS, 1)),
        MapEntry( GenieObject(GENIE_OBJ_LED_DIGITS, 2), GenieObject(GENIE_OBJ_LED_DIGITS, 3)),
    };
    static_assert(sizeof(MapEntry) == 10, "10 bytes per encoder table entry--see SET_ENCODER_MAP"); 
    void resetDisplayDefaults()
    {
        int i = 0;
        encoderMaps[i++] = MapEntry( GenieObject(GENIE_OBJ_LED_DIGITS, 0), GenieObject(GENIE_OBJ_LED_DIGITS, 1));
        encoderMaps[i++] = MapEntry( GenieObject(GENIE_OBJ_LED_DIGITS, 2), GenieObject(GENIE_OBJ_LED_DIGITS, 3));
        for (; i < NUMBER_OF_ENCODER_MAPS; i++)
            encoderMaps[i] = MapEntry();
        encoder1.ResetEncoderCenters();
        encoder2.ResetEncoderCenters();
        encoder3.ResetEncoderCenters();
        encoder4.ResetEncoderCenters();
    }
}

#if defined(DO_TRELLIS)
namespace Trellis {
    uint16_t switches; // read but not written by interrupts. so not volatile
    uint16_t GroupMasks[NUMTRELLISKEYS] = {
        0x7,   // default...
        0x7,   // the bottom 3 bits are a radio button group
        0x7,   // that read out SSB, CW, RTTY
    };
    static_assert(sizeof(GroupMasks) == 32, "32 bytes for the entire set of group masks");
    static const int STR_MODE_OBJECT = 1;
    GenieObject Display[NUM_DISPLAY_MAPS] = {
        {GENIE_OBJ_STRINGS, STR_MODE_OBJECT}, // the bottom 3 map to 
        {GENIE_OBJ_STRINGS, STR_MODE_OBJECT}, // a particular genie object
        {GENIE_OBJ_STRINGS, STR_MODE_OBJECT},
    };
    static_assert(sizeof(GenieObject) == 4, "4 bytes per Trellis group entry");

    void resetDisplayDefaults()
    {   // called from interrupt
        unsigned i = 0;
        for (; i < NUMTRELLISKEYS; i++)
            GroupMasks[i] = i < 3 ? 0x7 : 0;
        i = 0;
        while (i < 3)
            Display[i++] = {GENIE_OBJ_STRINGS, STR_MODE_OBJECT};
        while (i < NUM_DISPLAY_MAPS)
            Display[i++] = {};
    }

    // a single trellis entry is one mask (2 bytes) plus a GenieObject (4 bytes)
    // The pchost need only that 6 bytes for ONE of the entries and we, the
    // Arduino, replicate those 6 into all the entries corresponding to the mask
    void matchEntriesToMask(uint8_t which)
    {
        uint16_t mask = GroupMasks[which];
        uint16_t thisKey = 1;
        for (uint8_t i = 0; (i < NUMTRELLISKEYS) && (mask > thisKey); i++, thisKey <<= 1)
        {
            if (i == which)
                continue;   // same one we came in with
            if (!(mask & thisKey))
                continue;
            GroupMasks[i] = mask;
            Display[i] = Display[which];
        }
    }

    void justPressed(uint8_t i) // 0 to NUMTRELLISKEYS - 1
    {
        noInterrupts(); // copy outside interrupts
        uint16_t iMask = GroupMasks[i];
        GenieObject display(Display[i]);
        interrupts();
        uint16_t pressedButtonMask=1<<i;
        uint8_t radioValue = 0;
        if (iMask == 0)
        {   // the default. the switch is not part of a radio group
            switches ^= pressedButtonMask;
            if (switches & pressedButtonMask)
            {
                radioValue = 1;
                trellis.setLED(i);
            }
            else
                trellis.clrLED(i);
        }
        else    // user would expect all members of group have same mapping...
        {   // the switch is part of a radio group with N bits set
            uint16_t groupMask = 1;
            for (uint8_t j=0; j < NUMTRELLISKEYS && groupMask < iMask; j++, groupMask <<= 1)
            {
                if (!(iMask & groupMask))
                    continue;   // not a member of this group
                // turn OFF each switch LED in the group
                switches &= ~groupMask;
                trellis.clrLED(j);
                // calculate (result 0 to N-1) which of N was pressed
                if (pressedButtonMask > groupMask)
                    radioValue += 1; 
            }
            // turn ON the switch that was pressed
            switches |= pressedButtonMask;
            trellis.setLED(i);
        }
#if defined(DO_GENIE)
        if (display.isValid())
        {   // we have a mapping from this switch to the genie...use it
            for (int i = 0; i < GENIE_NAK_RETRIES; i++)
            {
                if (GENIE_NAK != genie.WriteObject(display.object, 
                        display.index, radioValue))
                        break;
            }
        }
#endif
    }
    
    uint8_t pressButton = 0xff;
    uint8_t setButton;
    uint8_t setTrellisBrightness;
}
#endif

void loop()
{
    WhereAmI = 10 - 3;
    if (i2Cstate == I2C_MASTER && powerUpDefaults)
    {
        Trellis::clear();
        for (int i = 0; i < GENIE_NAK_RETRIES; i++)
        {   // set the main form
            if (GENIE_NAK != genie.WriteObject(10, 0, 1))
                break;
        }    
        powerUpDefaults = 0;
    }

    long now = millis();
    noInterrupts(); // if encoder interrupts occur while we update

    uint8_t switchBefore = encoderSwitchState;
    debounceEncoderSwitch(ADCH > 0, // encoder4 is HIGH as read through ADC
        encoder4SwitchMask, encoder4SwitchHighCount);
    debounceEncoderSwitch(digitalRead(ENCODER3_SWITCH_PIN) == HIGH, 
        encoder3SwitchMask, encoder3SwitchHighCount);
    debounceEncoderSwitch(digitalRead(ENCODER2_SWITCH_PIN) == HIGH,
        encoder2SwitchMask, encoder2SwitchHighCount);
    uint8_t switchAfter = encoderSwitchState;
#if ENCODER3_AND_4_HANDLING==0
    ReadEncoders3And4();
#endif
    interrupts();
    WhereAmI = 10 - 4;

    uint8_t changedSwitches = switchBefore ^ switchAfter;
    if (changedSwitches != 0)
    {   // if decoder switch changed
        uint8_t mask = 1;
        for (int j = 0; j < NUM_ENCODER_SWITCHES; j++, mask <<= 1)
        {   // see if the genie display wants to be updated for it.
            if (changedSwitches & mask == 0)
                continue; //wasn't this one
            noInterrupts();
            GenieObject display = Trellis::Display[j + NUMTRELLISKEYS];
            interrupts();
            if (!(mask & switchAfter))
                continue; // switch just turned OFF. do nothing
            // switch just turned on
            encoderSwitches ^= mask; // toggle its entry
            if (display.isValid())
            {
                for (int i = 0; i < GENIE_NAK_RETRIES; i++)
                {
                    if (GENIE_NAK != genie.WriteObject(display.object, display.index, (mask & encoderSwitches) ? 1 : 0))
                        break;
                }
            }
        }
    }
    WhereAmI = 10 - 5;

#if defined (SERIAL_DEBUG)
    {
        static uint8_t lastencoderSwitches;
        if (lastencoderSwitches != encoderSwitches)
        {
            Serial.print("encoder switches: ");
            Serial.println(encoderSwitches, HEX);
            lastencoderSwitches = encoderSwitches;
        }
    }
#endif
    switch (i2Cstate)
    {
    case I2C_MASTER:
        {   // TWI_MASTERSLAVE_PIN is LOW: we're free to be i2c master
            // ...but we only get into this state after holding LOW for
            // I2C_MASTER_HOLD_MSEC. Other master has that much time
            // after we drop TWI_MASTERSLAVE_PIN
            // to address us before we decide they are not coming.
            bool switchToSlaveNow = false;
#if defined(DO_TRELLIS) 
            if (now -( trellisWait + TRELLIS_POLL_MSEC) >= 0)
            {
                trellisWait = now;
                if (trellis.readSwitches())
                {
                    // go through every button
                    for (uint8_t i=0; i < NUMTRELLISKEYS; i++)
                    {
                        if (trellis.justPressed(i))
                            Trellis::justPressed(i);
                    }
                    trellis.writeDisplay();
                }
                switchToSlaveNow = true;
            }
#endif
            noInterrupts();
            if (switchToSlaveNow ||
                (now - nextI2CtransitionTime) >= 0)
            {
                digitalWrite(TWI_MASTERSLAVE_PIN, HIGH);
                i2Cstate = I2C_SLAVE;
#if defined(DO_PC_I2C)
                twi_setAddress(PcHost::myI2cSlaveAddr);
#endif
                nextI2CtransitionTime = millis() + I2C_LONG_TRANSACTION_MSEC;
            }
            interrupts();
        }
        break;

    case I2C_SLAVE:
        // We previously raised the TWI_MASTERSLAVE_PIN to HIGH.
        // we'll only stay there I2C_SLAVE_HOLD_DEFAULT_MSEC unless
        // we get addressed as a slave,
        // or if SDR or SCL read low while we think 
        noInterrupts();
        if (now - nextI2CtransitionTime >= 0)
        {   // lower TWI_MASTERSLAVE_PIN
            digitalWrite(TWI_MASTERSLAVE_PIN, LOW);
            nextI2CtransitionTime = now + I2C_SLAVE_HOLD_DEFAULT_MSEC;
            i2Cstate = I2C_SLAVE_HOLD;
            heardI2CSdaSclLowTime = now;
        }
        interrupts();
        break;

    case I2C_SLAVE_HOLD:
        // stay here long enough to ensure the other master is
        // not going to address us after we lowered TWI_MASTERSLAVE_PIN
        noInterrupts();
        if ((digitalRead(SDA) == LOW) || (digitalRead(SCL) == LOW))
        {   // here is the possibility that we DID get addressed,
            // but our hardware failed to pick it up.
            heardI2CSdaSclLowTime = now;
        }
        if (now - nextI2CtransitionTime >= 0)
        {   // past our "normal" deadline
            if (!(now - I2C_SLAVE_EXTEND_MSEC - heardI2CSdaSclLowTime >= 0))
            {   // but SDA or SDA was low... make sure its not US pulling low...
                pinMode(SDA, INPUT_PULLUP);
                pinMode(SCL, INPUT_PULLUP);
                if (now - I2C_SLAVE_ASSUME_FT232_WAITING_MSEC - nextI2CtransitionTime >= 0)
                {
                    // assume FT232 is hung
                    digitalWrite(TWI_MASTERSLAVE_PIN, HIGH);
                    i2Cstate = I2C_SLAVE;
                    nextI2CtransitionTime = millis() + I2C_LONG_TRANSACTION_MSEC;
                    // go to slave state
                }
            }
            else
            {
                i2Cstate = I2C_MASTER;
                nextI2CtransitionTime = now + I2C_MASTER_HOLD_MSEC;
            }
        }
        interrupts();
        break;
    }
    WhereAmI = 10 - 6;

#if defined(DO_TRELLIS)
    if (i2Cstate == I2C_MASTER)
    {
        noInterrupts();
        uint8_t button = Trellis::pressButton;
        Trellis::pressButton = 0xFF;
        uint8_t setButton = Trellis::setButton;
        uint8_t brightness = Trellis::setTrellisBrightness;
        Trellis::setTrellisBrightness = 0;
        interrupts();
        if (button != 0xFF)
        {
            uint16_t mask = 1 << button;
            if (setButton)
                Trellis::switches &= ~mask;
            else
                Trellis::switches |= mask;
            trellis.readSwitches();
            Trellis::justPressed(button);
            trellis.writeDisplay();
        }
        if (brightness != 0)
        {
            EEPROM.update(EEPROM_TRELLIS_BRIGHTNESS, brightness);
            // it seems counterintuitive, but this IS the order required:
            trellis.writeDisplay();
            trellis.setBrightness(brightness);
        }
    }
#endif
    WhereAmI = 10 - 7;
#if defined(DO_GENIE)
    {
        static char buf[DISPLAY_STR_LEN];
        noInterrupts();
        uint8_t idx = strIndex;
        if (idx != 0xFF)
        {
            memcpy(buf, displayStr, 
                min(sizeof(buf), sizeof(displayStr)));
            strIndex = 0xFF;
        }
        interrupts();
        if (idx != 0xFF)
        {
            for (int i = 0; i < GENIE_NAK_RETRIES; i++)
            {
                if (GENIE_NAK != genie.WriteStr(idx, buf))
                    break;
            }
        }
    }
#endif
    WhereAmI = 10 - 8;
    // check each encoder for a change and update the genie
    {
#if NUMBER_OF_ROTARY_ENCODERS > 0
    int i =0;
    int32_t enc1, enc2;
    uint8_t r = encoder1.loop(enc1, enc2);
    if (r&1)
        EncoderDisplay::encoderMaps[i].Apply(enc1);
    i += 1;
    // encoder1 has no switch
#if NUMBER_OF_ROTARY_ENCODERS > 1
    r = encoder2.loop(enc1, enc2);
    if (r&1)
        EncoderDisplay::encoderMaps[i].Apply(enc1);
    if (r&2)
        EncoderDisplay::encoderMaps[i + NUMBER_OF_ROTARY_ENCODERS - 1].Apply(enc2);
    i += 1;
#if NUMBER_OF_ROTARY_ENCODERS > 2
    r = encoder3.loop(enc1, enc2);
    if (r&1)
        EncoderDisplay::encoderMaps[i].Apply(enc1);
    if (r&2)
        EncoderDisplay::encoderMaps[i + NUMBER_OF_ROTARY_ENCODERS - 1].Apply(enc2);
    i += 1;
#if NUMBER_OF_ROTARY_ENCODERS > 3
    r = encoder4.loop(enc1, enc2);
    if (r&1)
        EncoderDisplay::encoderMaps[i].Apply(enc1);
    if (r&2)
        EncoderDisplay::encoderMaps[i + NUMBER_OF_ROTARY_ENCODERS - 1].Apply(enc2);
#endif
#endif
#endif
#endif
    }
    WhereAmI = 10  - 9;
    for (uint8_t k = 0; k < DISPLAY_OBJECT_MAX_PER_MESSAGE; k++)
    {
        noInterrupts();
        SetDisplayObject so(DisplaysToUpdate[k]);
        DisplaysToUpdate[k].go.object = 0xffff; // mark finished
        interrupts();
        if (!so.go.isValid())
            break;
        for (int i = 0; i < GENIE_NAK_RETRIES; i++)
        {
            if (GENIE_NAK != genie.WriteObject(so.go.object, so.go.index, so.value))
                break;
        }
    }
    //cw::loop(); diagnostics... LED output even if loop frozen
}   // loop();

namespace PcHost {
    byte LastCommand = NO_COMMAND;
}

/* we get a call from i2c at the end of the incoming
** command message. Extend the wait time for the trellis
** according to how long we know the response will be. This
** allows faster polling, on average, as most messages
** are shorter than the max. */
static const uint8_t CommandResponseMsec[] =
{
    1,     // 0 NO_COMMAND 
    25,	   // ID_YOURSELF 30 bytes
    25,    // LONG_TEST  30 bytes
    0,     // SHORT_TEST
    25,    // GET_ID_STRING 30 bytes
    1,     // 5 SET_ID_STRING
    12,    // GET_INPUT_STATE 17 bytes
    1,     // SET_ENCODER_CENTER
    1,     // SET_TRELLIS_GROUP
    1,     // SET_ENCODER_MAP
    1,     // 10 SET_DISPLAY_STRING
    1,     // PRESS_TRELLIS_BUTTON
    1,     // SET_DISPLAY_OBJECTS
    1,     // 13 SET_TRELLIS_BRIGHTNESS
    1,     // RESET_DISPLAY_DEFAULTS
    1,     // SET_ENCODER_SWITCH_STATE
};	
// set the wait time for known response length
// with 55KHz clock on the PC side, the GET_INPUT_STATE 
// is shifted out in 10msec.

// these twi_ calls are from the modified twi library with interrupts disabled 
void twi_onSlaveReceive(uint8_t*p, int c)
{
    // previousCommand is a hack to avoid double-executing commands
    // that are retried because our ack to the PC failed to get through
    static uint8_t previousCommand;
    if (c > 2)
    {   // c is number of characters in message.
        uint16_t checksum(0);
        uint8_t *toSum = p;
        for (int jj = c-2; jj>0; jj -= 1)
            checksum += *toSum++;
        if ((uint8_t)checksum != *toSum)
            return;
        *toSum++ = 0; // trailing zero byte
        if ((uint8_t)(checksum >> 8) != *toSum++)
            return;
        uint8_t command = *p;
        c -= 2;
        PcHost::LastCommand = command;
        if ((i2Cstate != I2C_MASTER) &&
            (command < sizeof(CommandResponseMsec)))
        {
            long w = millis(); // now
            w += CommandResponseMsec[command];
            nextI2CtransitionTime = w;
            i2Cstate = I2C_SLAVE_HOLD;
            digitalWrite(TWI_MASTERSLAVE_PIN, LOW);
        }
        // commands that send data, we can deal with here.
        switch (*p)
        {
        case PcHost::SET_ID_STRING:
            {
                if (c > sizeof(PcHost::IdString))
                {
                    PcHost::IdString *v = reinterpret_cast<PcHost::IdString*>(&p[1]);
                    EEPROM.put(0, *v);
                }
            }
            break;

        case PcHost::SET_ENCODER_CENTER:
            if (c >= 6)
            {
                int32_t center = *reinterpret_cast<int32_t *>(&p[2]);
                switch (p[1])
                {
                case 0:  encoder1.SetEncoderCenter(center);
                    break;
                case 1:  encoder2.SetEncoderCenter(center);
                    break;
                case 2:  encoder3.SetEncoderCenter(center);
                    break;
                case 3:  encoder4.SetEncoderCenter(center);
                    break;
                case 4:  encoder2.SetEncoderCenter2(center);
                    break;
                case 5:  encoder3.SetEncoderCenter2(center);
                    break;
                case 6:  encoder4.SetEncoderCenter2(center);
                    break;
                }
            }
            break;

        case PcHost::SET_ENCODER_MAP:
            static const int ENCODER_HANDLER_MESSAGE_LEN = 1 /*command id*/
                            + 1 /*which encoder */
                            + sizeof(EncoderDisplay::MapEntry)
                            + sizeof(int32_t) + sizeof(int32_t); /* lowLimit and highLimit*/
            if (c >= ENCODER_HANDLER_MESSAGE_LEN)
            {
                uint8_t which = p[1];
                if (which >= NUMBER_OF_ENCODER_MAPS)
                    break;
                EncoderDisplay::encoderMaps[which].low10e4.object = *reinterpret_cast<uint16_t *>(&p[2]);
                EncoderDisplay::encoderMaps[which].low10e4.index = *reinterpret_cast<uint16_t *>(&p[4]);
                EncoderDisplay::encoderMaps[which].high10e4.object = *reinterpret_cast<uint16_t *>(&p[6]);
                EncoderDisplay::encoderMaps[which].high10e4.index = *reinterpret_cast<uint16_t *>(&p[8]);
                EncoderDisplay::encoderMaps[which].multiplier = *reinterpret_cast<int16_t *>(&p[10]);
                const int32_t *lowLimit = reinterpret_cast<int32_t*>(&p[12]);
                const int32_t *highLimit = reinterpret_cast<int32_t*>(&p[16]);
                switch (which)
                {   // mark appropriate encoder for forced genie update
                case 0:
                    encoder1.update();
                    break;

                case 1:
                    encoder2.SetLimits(*lowLimit, *highLimit);
                    encoder2.update();
                    break;
                case 4:
                    encoder2.SetLimits2(*lowLimit, *highLimit);
                    encoder2.update();
                    break;

                case 2:
                    encoder3.SetLimits(*lowLimit, *highLimit);
                    encoder3.update();
                    break;
                case 5:
                    encoder3.SetLimits2(*lowLimit, *highLimit);
                    encoder3.update();
                    break;

                case 3:
                    encoder4.SetLimits(*lowLimit, *highLimit);
                    encoder4.update();
                    break;
                case 6:
                    encoder4.SetLimits2(*lowLimit, *highLimit);
                    encoder4.update();
                    break;
                }
            }
            break;
        
        case PcHost::SET_TRELLIS_GROUP:
            static const int TRELLIS_GROUP_MESSAGE_LEN = 1 /*command id*/
                            + 1 /*which button */
                            + sizeof(Trellis::GroupMasks[0]) + sizeof(GenieObject);
            if (c >= TRELLIS_GROUP_MESSAGE_LEN)
            {
                uint8_t which = p[1];
                if (which >= NUM_DISPLAY_MAPS) // 16 trellis keys plus 3 encoder switches
                    break; // oops. parameter out of range
                Trellis::Display[which].object = *reinterpret_cast<uint16_t *>(&p[4]);
                Trellis::Display[which].index = *reinterpret_cast<uint16_t *>(&p[6]);
                if (which >= NUMTRELLISKEYS) // the genie maps apply to trellis+encoder
                    break;  // but masks apply only to trellis...
                Trellis::GroupMasks[which] = *reinterpret_cast<uint16_t *>(&p[2]);
                Trellis::matchEntriesToMask(which); // copy attributes according GroupMask
                // note that ADDING buttons to radio group requires only calling once on
                // one of the group. REMOVING them requires call on EACH with a zero mask
            }
            break;

        case PcHost::SET_DISPLAY_STRING:
            if (c > 1)
            {
                strIndex = p[1];
                memset(displayStr, 0, sizeof(displayStr));
                memcpy(displayStr, p+2, c-2);
            }
            break;

        case PcHost::PRESS_TRELLIS_BUTTON:
            if (c >= 3)
            {
                Trellis::pressButton = p[1];
                Trellis::setButton = p[2];
            }
            break;

        case PcHost::SET_DISPLAY_OBJECTS:
            {
                SetDisplayObject *q = reinterpret_cast<SetDisplayObject *>(&p[1]);
                unsigned neededBytes = 1 + sizeof(SetDisplayObject);
                for (uint8_t k = 0; 
                    c >= neededBytes && k < DISPLAY_OBJECT_MAX_PER_MESSAGE; 
                    k++, neededBytes += sizeof(SetDisplayObject))
                {
                    DisplaysToUpdate[k] = *q++;
                }
            }
            break;

        case PcHost::SET_TRELLIS_BRIGHTNESS:
            if (c >= 2)
                Trellis::setTrellisBrightness = p[1];
            break;

        case PcHost::RESET_DISPLAY_DEFAULTS:
            if ((c == 1)  && (previousCommand != PcHost::RESET_DISPLAY_DEFAULTS))
            {
                EncoderDisplay::resetDisplayDefaults();
                Trellis::resetDisplayDefaults();
                encoderSwitches = 0;
                powerUpDefaults = 1;
            }
            break;

        case PcHost::SET_ENCODER_SWITCH_STATE:
            if (c == 2)
                encoderSwitches = p[1];
            break;

        default:
            break;
        }
        previousCommand = *p;
    }
}

void twi_onSlaveTransmit()
{	// a checksum is added to the end of the message by the twi library
    // we can only send 30 bytes max out of 32
    static const int XBUFFER_LEN = 30;
    static uint8_t xbuffer[XBUFFER_LEN];
    static const char id[] = "WriteLog 12.33";
    switch (PcHost::LastCommand)
    {
    case PcHost::ID_YOURSELF:
        twi_transmit(id, sizeof(id) - 1);
        break;

    case PcHost::LONG_TEST:
        {
            char c = 'A';
            for (int i = 0; i < sizeof(xbuffer); i++)
            {
                if (c > 'Z')
                    c = 'A';
                xbuffer[i] = c++;
            }
            twi_transmit(xbuffer, sizeof(xbuffer));
        }
        break;

    case PcHost::SHORT_TEST:
        xbuffer[0] = 'S';
        twi_transmit(xbuffer, 1);
        break;

    case PcHost::GET_ID_STRING:
        {
            PcHost::IdString *v = reinterpret_cast<PcHost::IdString*>(&xbuffer[0]);
            EEPROM.get(0, *v);
            twi_transmit(xbuffer, sizeof(xbuffer));
        }
        break;

    case PcHost::GET_INPUT_STATE:
        {
            uint8_t i = 0;
#if NUMBER_OF_ROTARY_ENCODERS > 0
            int16_t pos = encoder1.GetPosition();
            xbuffer[i++] = static_cast<uint8_t>(pos);
            xbuffer[i++] = static_cast<uint8_t>(pos >> 8);
#if NUMBER_OF_ROTARY_ENCODERS > 1
            pos = encoder2.GetPosition();
            xbuffer[i++] = static_cast<uint8_t>(pos);
            xbuffer[i++] = static_cast<uint8_t>(pos >> 8);
#if NUMBER_OF_ROTARY_ENCODERS > 2
            pos = encoder3.GetPosition();
            xbuffer[i++] = static_cast<uint8_t>(pos);
            xbuffer[i++] = static_cast<uint8_t>(pos >> 8);
#if NUMBER_OF_ROTARY_ENCODERS > 3
            pos = encoder4.GetPosition();
            xbuffer[i++] = static_cast<uint8_t>(pos);
            xbuffer[i++] = static_cast<uint8_t>(pos >> 8);
#endif
#endif
#endif
#endif
#if NUMBER_OF_ROTARY_ENCODERS > 1
            pos = encoder2.GetPosition2();
            xbuffer[i++] = static_cast<uint8_t>(pos);
            xbuffer[i++] = static_cast<uint8_t>(pos >> 8);
#if NUMBER_OF_ROTARY_ENCODERS > 2
            pos = encoder3.GetPosition2();
            xbuffer[i++] = static_cast<uint8_t>(pos);
            xbuffer[i++] = static_cast<uint8_t>(pos >> 8);
#if NUMBER_OF_ROTARY_ENCODERS > 3
            pos = encoder4.GetPosition2();
            xbuffer[i++] = static_cast<uint8_t>(pos);
            xbuffer[i++] = static_cast<uint8_t>(pos >> 8);
#endif
#endif
#endif
#if defined(DO_TRELLIS)
            static_assert(sizeof(Trellis::switches) == 2, "Switches fit into 2 bytes");
            xbuffer[i++] = static_cast<uint8_t>(Trellis::switches);
            xbuffer[i++] = static_cast<uint8_t>(Trellis::switches >> 8);
#endif
            xbuffer[i++] = encoderSwitches;
            twi_transmit(xbuffer, i);
        }
        break;

    default:
        xbuffer[0] = '1';
        twi_transmit(xbuffer,1);	
        break;
    }

    PcHost::LastCommand = PcHost::NO_COMMAND;
}

void twi_onSlaveAddressed()
{
    if (i2Cstate != I2C_MASTER)
    {
        long w = millis(); // now
        w += I2C_LONG_TRANSACTION_MSEC; // hold in slave for length of longest incoming message
        nextI2CtransitionTime = w;
        i2Cstate = I2C_SLAVE_HOLD;
        digitalWrite(TWI_MASTERSLAVE_PIN, LOW);
    }
}

template <uint8_t ENC_PLUS, uint8_t ENC_MINUS, uint8_t SM>
QuadratureEncoder<ENC_PLUS, ENC_MINUS,SM>::QuadratureEncoder()
:referencePosition(0), offsetPosition(0), referencePosition2(0)
,offsetPosition2(0), prevLoopPos(0),prevLoopPos2(0),lastEncoder(0)
,limitLow(LOWEST_INT32),limitHigh(HIGHEST_INT32)
,limitLow2(LOWEST_INT32), limitHigh2(HIGHEST_INT32)
{}

template <uint8_t ENC_PLUS, uint8_t ENC_MINUS, uint8_t SM>
void QuadratureEncoder<ENC_PLUS, ENC_MINUS,SM>::OnEncoder(uint8_t curVal)
{
    curVal &= BITMASK;
    // optimization: 3 XORs, one shift, two ANDs, and two test/branch for zero
    uint8_t changed = curVal ^ lastEncoder;
    if (changed == BITMASK)
    {   // both changed...don't try to process
        lastEncoder = curVal;
        return;
    }
    if (changed != 0)
    { // one changed
        static_assert(ENC_PLUS > ENC_MINUS, "encoder bits assumed in this order");
        uint8_t alignedMinus = curVal << (ENC_PLUS - ENC_MINUS);
        bool primary = (SM & encoderSwitches) == 0;
        if (((changed ^ alignedMinus ^ curVal) & (1 << ENC_PLUS)) == 0)
        {
            if (primary)
                offsetPosition += 1;
            else
                offsetPosition2 += 1;
        }
        else
        {
            if (primary)
                offsetPosition -= 1;
            else
                offsetPosition2 -= 1;
        }
        lastEncoder = curVal;
    }
}

template <uint8_t P, uint8_t M, uint8_t SM>
void QuadratureEncoder<P, M, SM>::SetEncoderCenter(int32_t p)
{   // primary virtual
    referencePosition = p;
    offsetPosition = 0;
    prevLoopPos = // force on-screen update
        referencePosition + offsetPosition + 1;
}
template <uint8_t P, uint8_t M, uint8_t SM>
void QuadratureEncoder<P, M, SM>::SetEncoderCenter2(int32_t p)
{   // secondary virtual
    referencePosition2 = p;
    offsetPosition2 = 0;
    prevLoopPos2 = // force on-screen update
        referencePosition2 + offsetPosition2 + 1;
}
template <uint8_t EP, uint8_t EM, uint8_t SM>
uint8_t QuadratureEncoder<EP,EM,SM>::loop(int32_t &pos, int32_t &pos2)
{
        noInterrupts();
        // main virtual encoder
        pos = referencePosition;
        pos += offsetPosition;
        if (pos > limitHigh)
        {
            offsetPosition = static_cast<int16_t>(limitHigh - referencePosition);
            pos = limitHigh;
        } else if (pos < limitLow)
        {
            offsetPosition = static_cast<int16_t>(limitLow - referencePosition);
            pos = limitLow;
        }
        uint8_t ret = (prevLoopPos == pos) ? 0 : 1;
        prevLoopPos = pos;

        // the secondary virtual encoder
        pos2 = referencePosition2;
        pos2 += offsetPosition2;
        if (pos2 > limitHigh2)
        {
            offsetPosition2 = static_cast<int16_t>(limitHigh2 - referencePosition2);
            pos2 = limitHigh2;
        }
        else if (pos2 < limitLow2)
        {
            offsetPosition2 = static_cast<int16_t>(limitLow2 - referencePosition2);
            pos2 = limitLow2;
        }
        if (prevLoopPos2 != pos2)
            ret |= 2;
        prevLoopPos2 = pos2;
        uint8_t u = forceEncoderUpdate;
        if (SM & u)
            ret |= 3;
        u &= ~SM;
        forceEncoderUpdate = u;
        interrupts();
        return ret;
}
template <uint8_t EP, uint8_t EM, uint8_t SWITCHMASK>
void QuadratureEncoder<EP,EM,SWITCHMASK>::update()
{
    forceEncoderUpdate |= SWITCHMASK;
}

namespace cw {
    int CwState = 3;
    void loop()
    {
        static long CwTimer;
        static uint8_t toFlash;
        static uint8_t print;

        long now = millis(); // refresh current time cuz stuff above takes time

        if ((int)(now - CwTimer) > 0)
        {
            if (CwState < (toFlash+1)<<1)
            {   // for toFlash:
                //       0 -> 0,1
                //       1 -> 0,1,2,3
                //       2 -> 0,1,2,3,4,5
                CwState += 1;
                if (print > 4) // flash quickly at 4 and above
                    CwTimer = now + 50;
                else
                    CwTimer = now + 300 + ((CwState % 8 == 0) ? 100 : 0);
            } else
            {
                CwState = 0;
                CwTimer = now + 1000;
                if (print == 0)
                {
                    toFlash = WhereAmI;
                    print = 1;
                }
                else if (print == 1)
                {
                    toFlash = static_cast<uint8_t>(i2Cstate);
                    print = 2;
                }
                else if (print == 2)
                {
                    toFlash = encoderSwitches;
                    print = 3;
                }
                else if (print == 3)
                {
                    toFlash = 0x7 & ((Trellis::switches) >> 8);
                    print = 4;
                }
                else if (print == 4)
                {
                    toFlash = 0;
                    print = 5;
                    CwTimer += 3000;
                }
                else if (print == 5)
                {
                    print = 0;
                    CwTimer = now;
                }
            }
        }    
    }
    void on512usec()
    {
        // FIXME
        cw::loop(); // DIAGNOSTICS only
#if 0
        static uint8_t osc;
        static const uint8_t CYCLES_ON = 1;
        static const uint8_t CYCLES_OFF = 6;
        static bool isOn;
        if ((1 & CwState) == 0)
        {
            isOn = false;
            digitalWrite(CWOUTPUT_PIN, LOW);
            osc = CYCLES_ON;
        }
        else
        {
            osc -= 1;
            if (osc == 0)
            {
                digitalWrite(CWOUTPUT_PIN, isOn ? LOW : HIGH);
                isOn = !isOn;
                osc = isOn ? CYCLES_ON : CYCLES_OFF;
            }
        }
#else
        digitalWrite(CWOUTPUT_PIN, (1 & CwState) == 0 ? LOW : HIGH);
#endif
    }

#if (F_CPU == 16000000ul)
    const uint8_t USEC_512_COUNT = 0x80u; // 256 counts at 2uSec
#elif (F_CPU == 8000000ul)
    const uint8_t USEC_512_COUNT = 0x40u; // 128 counts at 4uSec
#else
#error Only support 16MHz and 8Mhz clocks
#endif

#if defined  (__AVR_ATmega328P__) || defined (__AVR_ATmega2560__)
    // target hardware is Pro Mini, which is 328P
    uint8_t tcnt2Setting()
    {
        return static_cast<uint8_t>(0x100u - USEC_512_COUNT); //counts up from here, interrupt at == 0
    }

    ISR(TIMER2_OVF_vect)
    {
        TCNT2 = tcnt2Setting();
        on512usec();
#if ENCODER3_AND_4_HANDLING==2
        ReadEncoders3And4();
#endif
    }
    void setup()
    {
        digitalWrite(CWOUTPUT_PIN, LOW);
        pinMode(CWOUTPUT_PIN, OUTPUT);
        noInterrupts();
        /* Normal timer operation.*/
        TCCR2A = 0x00;
        /* The timer counter register.         */
        TCNT2 = tcnt2Setting();
        TCCR2B = 0x04; // 64 prescale, which is 2usec per TCNT2 @ 16MHz
        /* Enable the timer overflow interrupt. */
        TIMSK2 = 0x01;
        TIFR2 = 1; // Clear any current overflow flag
        interrupts();
    }
#else
#error "Unsupported CPU for timer2"
#endif
}

#if NUMBER_OF_ROTARY_ENCODERS > 0
#if defined  (__AVR_ATmega328P__)
#if NUMBER_OF_ROTARY_ENCODERS > 2
#if ENCODER3_AND_4_HANDLING==1 // POLL portB cuz we get a crash on interrupts
ISR (PCINT0_vect) 
{
    // one of pins PCINT0 through PCINT7 has changed
    ReadEncoders3And4();
}
#endif
#endif

namespace {
#if NUMBER_OF_ROTARY_ENCODERS > 2
    void ReadEncoders3And4()
     {  // initialize encoders
        uint8_t v = PINB;
        encoder3.OnEncoder(v);
#if NUMBER_OF_ROTARY_ENCODERS > 3
        encoder4.OnEncoder(v);
#endif
    }
#endif
}

ISR (PCINT1_vect) 
{
    // one of pins PCINT8 through PCINT15 has changed
    uint8_t vc = PINC;
    encoder2.OnEncoder(vc);
}

ISR (PCINT2_vect) 
{
    // one of pins PCINT16 through PCINT23 has changed
    uint8_t vc = PIND;
    encoder1.OnEncoder(vc);
}

#elif defined (__AVR_ATmega2560__)
ISR (PCINT0_vect) 
{
    // one of pins PCINT0 through PCINT7 has changed
    uint8_t v = PINB;
    encoder1.OnEncoder(v);
#if NUMBER_OF_ROTARY_ENCODERS > 2
    encoder3.OnEncoder(v);
#if NUMBER_OF_ROTARY_ENCODERS > 3
    encoder4.OnEncoder(v);
#endif
#endif
}
ISR (PCINT2_vect) 
{
    // one of pins PCINT16 through PCINT23 has changed
    uint8_t vc = PINK;
    encoder2.OnEncoder(vc);
}
#endif
#endif
