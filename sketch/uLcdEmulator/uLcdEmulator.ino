/* running on Arduino Mega, connect these three pins to the LCD connector 
** on the RC-1101 PCB:
**      Mega            RC-1101
**        21             RES
**        20             GND
**        19             TXD
**        18             RXD
**
**        23            RESET on removed FT232 socket
*/
/*
Typical printout:
Start reset sequence
2351 LCD reset asserted
state change to Reset Asserted
51 state change to Reset Released
RC1101-LCD READY
Incoming fn="RunFlash.4xe"
10022009 OK
state change to Running
1902 Write Contrast
0 Write Object type=10 idx=0
**
** total is about 4.5 seconds....
** the Arduino is processing the i2c at about 2.4 seconds
** This sketch responds to its reset from the arduino instantaneously...
** The physical LCD display takes about 4 seconds to respond 
** and get to its "RC1101-LCD READY" report (its not processing genie yet)
** 
** The timeline with real parts is this:
** Arduino RESET to LCD reset is about 2.5 seconds
** LCD reset to "RC1101-LCD READY" is a measured 2.9 seconds
** The wlRemoteGeneric sketch notes that READY message and holds 1.9 seconds
** before sending any commands to the LCD.
*/


const int GROUND_PIN = 20;
const int RESET_INPUT = 21;
const int RC1101_RESET_OUTPUT = 23;

const long LOADER_BAUD = 9600;
const long RUN_BAUD = 200000;

enum emState_t {RESET_IN_ASSERTED, RESET_IN_RELEASED, RUNNING} emState = RESET_IN_RELEASED;

static const char * const StateNames[] =
{"Reset Asserted", "Reset Released", "Running"};

void setup()
{
    Serial.begin(9600);
    Serial.println("uLcdEmulator");
    Serial1.begin(LOADER_BAUD);
    pinMode(RESET_INPUT, INPUT_PULLUP);
    digitalWrite(GROUND_PIN, LOW);
    pinMode(GROUND_PIN, OUTPUT);
    pinMode(RC1101_RESET_OUTPUT, INPUT);
    digitalWrite(RC1101_RESET_OUTPUT, LOW);
}

void setState(emState_t n)
{
    if (emState != n)
    {
        Serial.print("state change to "); 
        Serial.println(StateNames[n]);
    }
    emState = n;
}

const int LCD_BUF_LEN = 80;
const int FILENAME_LEN = 12;
static char LcdInBuf[LCD_BUF_LEN];
int charsInLcdBuf;

const int DBG_BUF_LEN = 80;
static char DbgInBuf[DBG_BUF_LEN];
int charsInDbgBuf;

bool ProcessLcd(int diff)
{
    bool ret = false;
    while (charsInLcdBuf > 0)
    {
        int used = 0;
        switch (LcdInBuf[0])
        {
        case 0x4: //write contrast
            if (charsInLcdBuf < 3)
                return ret;
            used = 3;
            Serial1.write(6);
            Serial.print(diff);
            Serial.println(" Write Contrast");
            break;
        case 0x1:
            if (charsInLcdBuf < 6)
                return ret;
            used = 6;
            Serial1.write(6);
            Serial.print(diff);
            Serial.print(" Write Object type=");
            Serial.print((int) LcdInBuf[1]);
            Serial.print(" idx="); 
            Serial.println((int) LcdInBuf[2]);
            break;
        default:
            Serial.print(diff);
            Serial.print(" Unknown command 0x");
            Serial.println((int)(unsigned char)LcdInBuf[0], HEX);
            used = charsInLcdBuf;
            break;
        }
        if (used > 0)
        {
            ret = true;
            if (used < charsInLcdBuf)
                memmove(LcdInBuf, LcdInBuf + used, charsInLcdBuf - used);
            charsInLcdBuf -= used;
        }
    }
    return ret;
}

unsigned long runStateTransition;
void LcdRun()
{
    Serial.print("Incoming fn=\"");
    for (int i = 0; i < charsInLcdBuf; i++)
        Serial.print(LcdInBuf[i]);
    Serial.println("\"");
    charsInLcdBuf = 0;

    static const char *Msg = "10022009 OK";
    Serial.println(Msg);
    Serial1.print(Msg); Serial1.print('\n');
    Serial1.flush();
    Serial1.end();
    Serial1.begin(RUN_BAUD);
    setState(RUNNING);
    runStateTransition = millis();
}

unsigned long inReleasedSince = 0x8000000;
void ProcessDbg()
{
    if (charsInDbgBuf > 0)
    {
        if (toupper(DbgInBuf[0]) == 'R')
        {
            Serial.println("Start reset sequence");
            inReleasedSince = millis();
            pinMode(RC1101_RESET_OUTPUT, OUTPUT);
            delay(50);
            pinMode(RC1101_RESET_OUTPUT, INPUT);
        }
    }
    charsInDbgBuf = 0;
}


void loop()
{
    while (Serial.available() > 0)
    {
        auto c = Serial.read();
        if (c == '\r')
            ProcessDbg();
        else if (charsInDbgBuf < DBG_BUF_LEN)
            DbgInBuf[charsInDbgBuf++] = c;
    }

    auto res = digitalRead(RESET_INPUT);
    auto now = millis();

    if (res == LOW)
    {
        if (emState != RESET_IN_ASSERTED)
        {
            Serial1.end();
            Serial1.begin(LOADER_BAUD);
            Serial.print(now - inReleasedSince);
            Serial.println(" LCD reset asserted");
            inReleasedSince = now;
        }
        setState(RESET_IN_ASSERTED);
    }
    else
    {
        switch(emState)
        {
        case RESET_IN_ASSERTED:
            Serial.print((int)(now - inReleasedSince));
            Serial.print(' ');
            setState(RESET_IN_RELEASED);
            // fall through

        case RESET_IN_RELEASED:
            if (now - inReleasedSince > 1000)
            {
                static const char *Msg = "RC1101-LCD READY";
                inReleasedSince = now;
                Serial.println(Msg);
                Serial1.print(Msg); Serial1.print('\n');
                charsInLcdBuf = 0;
            }
            while (Serial1.available() > 0)
            {
                auto c = Serial1.read();
                if (isspace(c))
                    LcdRun();
                else if (charsInLcdBuf < LCD_BUF_LEN)
                {
                    LcdInBuf[charsInLcdBuf++] = c;
                    if (charsInLcdBuf >= FILENAME_LEN)
                        LcdRun();
                }
            }
            break;

        case RUNNING:
            while (Serial1.available() > 0)
            {
                auto c = Serial1.read();
                int diff = now - runStateTransition;
                if (charsInLcdBuf < LCD_BUF_LEN)
                    LcdInBuf[charsInLcdBuf++] = c;                
                if (ProcessLcd(diff))
                     runStateTransition = now;
            }
            break;
        }

    }
}



