#include <Arduino.h>

// LoRa include
#include <SPI.h>
#include <RadioLib.h>

#include <FastLED_NeoPixel.h>

// #define TTGO_V2 1
//#define TTGO_t3_v1_6 1
// #define BARVINOK_TX 1
// #define RADIOMATER_BANDIT 1
// #define ES900TX 1
// #define BETAFPV_MICRO 1
// #define ELRS_RX 1
// #define BETAFPV_TX_400 1
// #define NAMIMNORC_900 1
// #define BAYCK_LR1121_DUAL 1
// #define BETAFPV_2400_MICRO 1
// #define HAPPYMODEL_ES24 1
// #define ES900TX_MAX 1
// #define HGLRC_900 1
#define AERONETIX_TX 1

// #define CW_FROM_STARTUP 1
float default_freq = 915;
float default_pwr = 2;

#if defined(TTGO_V2) || defined(TTGO_t3_v1_6) 

#define LORA_CS     18
#define LORA_IRQ    26
#define LORA_RST    23

#define LORA_SCK    5
#define LORA_MISO   19
#define LORA_MOSI   27

#define LED_BUILTIN 25

const bool radio_rfo_hf = false;

#if defined(TTGO_t3_v1_6)
//https://github.com/Xinyuan-LilyGO/LilyGo-LoRa-Series/blob/master/examples/ArduinoLoRa/LoRaSender/utilities.h
#define RADIO_TCXO_ENABLE           33
#endif
#elif defined(BARVINOK_TX)

// Barvinok ES900 SL Retrick
#define LORA_CS     27
#define LORA_IRQ    36
#define LORA_RST    26


//right chip
#define LORA_CS_2     13
#define LORA_IRQ_2    39
#define LORA_RST_2    21

#define LORA_SCK    25
#define LORA_MISO   33
#define LORA_MOSI   32

#define LED_BUILTIN 22
#define LED_IS_RGB  1

#define TST_FW      1

const bool radio_rfo_hf = false;

#elif defined(RADIOMATER_BANDIT)

#define LORA_CS     4
#define LORA_IRQ    22
#define LORA_RST    5

#define LORA_SCK    18
#define LORA_MISO   19
#define LORA_MOSI   23

#define LED_BUILTIN 15

const bool radio_rfo_hf = true;

#define TX_EN_PIN 33
#define FAN_EN_PIN 2

#elif defined(ES900TX)

#define LORA_CS     5
#define LORA_IRQ    26
#define LORA_RST    14

#define LORA_SCK    18
#define LORA_MISO   19
#define LORA_MOSI   23

#define LED_BUILTIN 15

const bool radio_rfo_hf = false;

#define TX_EN_PIN 12
#define FAN_EN_PIN 17
#define PIN_RFamp_APC2 25

#elif defined(BETAFPV_MICRO)

#define LORA_CS     5
#define LORA_IRQ    4
#define LORA_RST    14

#define LORA_SCK    18
#define LORA_MISO   19
#define LORA_MOSI   23

#define LED_BUILTIN 15

const bool radio_rfo_hf = true;

#define TX_EN_PIN 26
#define FAN_EN_PIN 17

#elif defined(ELRS_RX)

#define LORA_CS     15
#define LORA_IRQ    4
#define LORA_RST    2

#define LORA_SCK    14
#define LORA_MISO   12
#define LORA_MOSI   13

#define LED_BUILTIN 16

const bool radio_rfo_hf = false;

#elif defined(BETAFPV_TX_400)

#define LORA_CS     2
#define LORA_IRQ    34
#define LORA_RST    13

#define LORA_SCK    25
#define LORA_MISO   33
#define LORA_MOSI   32

#define LED_BUILTIN 15

const bool radio_rfo_hf = false;

#define FAN_EN_PIN 27

#elif defined(NAMIMNORC_900)

#define LORA_CS     5
#define LORA_IRQ    17
#define LORA_RST    21

#define LORA_SCK    18
#define LORA_MISO   19
#define LORA_MOSI   23

#define LED_BUILTIN 15

const bool radio_rfo_hf = false;

#define RX_EN_PIN 33
#define FAN_EN_PIN 2
#define PIN_RFamp_APC2 25

#elif defined(BAYCK_LR1121_DUAL)

// left lr1121
#define LORA_CS     27
#define LORA_IRQ    37
#define LORA_RST    26
#define LORA_BUSY   36


//right chip
#define LORA_CS_2     13
#define LORA_IRQ_2    34
#define LORA_RST_2    21
#define LORA_BUSY_2   39

#define RADIO_DCDC  1

#define LORA_SCK    25
#define LORA_MISO   33
#define LORA_MOSI   32

#define LED_BUILTIN 22

const bool radio_rfo_hf = false;

#define chip_LR1121 1

#elif defined(BETAFPV_2400_MICRO)

#define LORA_CS     5
#define LORA_IRQ    4
#define LORA_RST    14
#define LORA_BUSY   21

#define LORA_SCK    18
#define LORA_MISO   19
#define LORA_MOSI   23

#define RX_EN_PIN   27
#define TX_EN_PIN   26

#define FAN_EN_PIN  17

#define LED_BUILTIN 16

#define chip_SX1281 1

#elif defined(HAPPYMODEL_ES24)

#define LORA_CS     5
#define LORA_IRQ    4
#define LORA_RST    14
#define LORA_BUSY   21

#define LORA_SCK    18
#define LORA_MISO   19
#define LORA_MOSI   23

#define RADIO_DCDC  1

#define RX_EN_PIN   27
#define TX_EN_PIN   26

#define FAN_EN_PIN  17

#define LED_BUILTIN 15

#define chip_SX1281 1

#elif defined(ES900TX_MAX)

#define LORA_CS     27
#define LORA_IRQ    36
#define LORA_RST    2

#define LORA_SCK    25
#define LORA_MISO   33
#define LORA_MOSI   32

#define LED_BUILTIN 22

const bool radio_rfo_hf = false;

#define RX_EN_PIN   10
#define TX_EN_PIN   14
#define FAN_EN_PIN  4
#define PIN_RFamp_APC2 26
#define LED_IS_RGB  1

#elif defined(HGLRC_900)

#define LORA_CS     26
#define LORA_IRQ    4
#define LORA_RST    14

#define LORA_SCK    2
#define LORA_MISO   19
#define LORA_MOSI   23

#define LED_BUILTIN 27

const bool radio_rfo_hf = true;

#define RX_EN_PIN   12
#define FAN_EN_PIN  32
#define LED_IS_RGB  1

#elif defined(AERONETIX_TX)

#define LORA_CS     26
#define LORA_IRQ    14
#define LORA_RST    12

#define LORA_SCK    32
#define LORA_MISO   33
#define LORA_MOSI   25

#define LED_BUILTIN 4

const bool radio_rfo_hf = false;

#define LED_IS_RGB  1

#endif

#include "cli.h"
#include <string.h>

#define CLI_BUFFER_SIZE 128
#define CLI_COMMAND_COUNT 5

#define CLI_ENTRY(command) { command ## Callback, (const CLI_BUF_VALUE_T *) #command } 

static void cliRxCallback(uint8_t data);
static void cliTxCallback(CLI_BUF_VALUE_T *buf, CLI_TX_BUF_COUNT_VALUE_T bufc);

static CLIRet_t cliCallback(void *args, CLI_ARG_COUNT_VALUE_T argc);
static CLIRet_t pCallback(void *args, CLI_ARG_COUNT_VALUE_T argc);
static CLIRet_t fCallback(void *args, CLI_ARG_COUNT_VALUE_T argc);
static CLIRet_t cwCallback(void *args, CLI_ARG_COUNT_VALUE_T argc);
static CLIRet_t aCallback(void *args, CLI_ARG_COUNT_VALUE_T argc);

CLIConfig_t l_cli_cnf;
CLIInst_t l_cli_inst;
CLI_BUF_VALUE_T l_cli_buf[CLI_BUFFER_SIZE];
CLICommand_t l_cli_commands[CLI_COMMAND_COUNT] =
{
    CLI_ENTRY(cli),
    CLI_ENTRY(p),
    CLI_ENTRY(f),
    CLI_ENTRY(cw),
    CLI_ENTRY(a),
};

#if defined(chip_LR1121)

LR1121 radio = new Module(LORA_CS, LORA_IRQ, LORA_RST, LORA_BUSY);

#elif defined(chip_SX1281)

SX1281 radio = new Module(LORA_CS, LORA_IRQ, LORA_RST, LORA_BUSY);

#else
SX1276 radio = new Module(LORA_CS, LORA_IRQ, LORA_RST);

#if defined(BARVINOK_TX)
SX1276 radio_2 = new Module(LORA_CS_2, LORA_IRQ_2, LORA_RST_2);
#endif
#endif

#if defined(LED_IS_RGB)
FastLED_NeoPixel<1, LED_BUILTIN, NEO_GRB> led;
#define RED 0xFF0000
#define GREEN 0x00FF00
#define BLUE 0x0000FF
#define LED_OFF 0x000000
#endif

void led_set_color(uint32_t color);

void ledOn()
{
#if defined(LED_IS_RGB)    
    led_set_color(GREEN);
#else
    digitalWrite(LED_BUILTIN, HIGH);
#endif
}

void ledOff()
{
#if defined(LED_IS_RGB)    
    led_set_color(LED_OFF);
#else
    digitalWrite(LED_BUILTIN, LOW);
#endif
}

void led_set_color(uint32_t color)
{
#if defined(LED_IS_RGB)
    led.setPixelColor(0, color);
	led.show();
#else
    if (color) {
        ledOn();
    }
    else {
        ledOff();
    }
#endif    
}

void initLoRa() {
    Serial.println("Initializing LoRa....");

    pinMode(LORA_RST, OUTPUT);
    digitalWrite(LORA_RST, LOW);
    delay(20);
    digitalWrite(LORA_RST, HIGH);
    delay(200);

    // pinMode(LORA_BUSY, INPUT);
    // while (digitalRead(LORA_BUSY)) {
    //     Serial.println(F("busy"));
    //     delay(10);
    // }


#if not defined(ELRS_RX)
    SPI.begin(LORA_SCK, LORA_MISO, LORA_MOSI, LORA_CS);
    // SPI.setFrequency(400000);
#else
    SPI.pins(LORA_SCK, LORA_MISO, LORA_MOSI, LORA_CS);
    SPI.begin();
#endif
#if defined RADIO_TCXO_ENABLE
    pinMode(RADIO_TCXO_ENABLE, OUTPUT);
    digitalWrite(RADIO_TCXO_ENABLE, HIGH);
#endif

#if defined (chip_LR1121)
    // int state = radio.setRegulatorDCDC();
    // if (state == RADIOLIB_ERR_NONE) {
    //     Serial.println(F("setRegulatorDCDC success!"));
    // } else {
    //     Serial.print(F("setRegulatorDCDC failed, code "));
    //     Serial.println(state);
    //     while (true);
    // }

    int state = radio.beginGFSK();
#elif defined(chip_SX1281)
    int state = radio.beginGFSK();
#else
    int state = radio.beginFSK();
#endif
    
    if (state == RADIOLIB_ERR_NONE) {
        Serial.println(F("success!"));
    } else {
        Serial.print(F("failed, code "));
        Serial.println(state);
        led_set_color(BLUE);
        while (true);
    }

// for test purpose
#if defined(BARVINOK_TX)
    int state2 = radio_2.beginFSK();

    if (state2 == RADIOLIB_ERR_NONE) {
        Serial.println(F("success!"));
    } else {
        Serial.print(F("failed, code "));
        Serial.println(state);
        led_set_color(RED);
        while (true);
    }

    led_set_color(GREEN);
#endif

    // if needed, you can switch between LoRa and FSK modes
    //
    // radio.begin()       start LoRa mode (and disable FSK)
    // radio.beginFSK()    start FSK mode (and disable LoRa)

    // the following settings can also
    // be modified at run-time
    state = radio.setFrequency(default_freq);

#if defined(chip_SX1281)
    state = radio.setOutputPower(default_pwr);
#else    
    state = radio.setOutputPower(default_pwr, radio_rfo_hf);
#endif    

    // state = radio.transmitDirect();
    // if (state != RADIOLIB_ERR_NONE) {
    //     Serial.println(F("[SX1278] Unable to start direct transmission mode, code "));
    //     Serial.println(state);
    // }

    #if defined(CW_FROM_STARTUP)
        state = radio.transmitDirect();

        if (state != RADIOLIB_ERR_NONE) {
            Serial.println(F("[SX1278] Unable to start direct transmission mode, code "));
            Serial.println(state);
        }
        else {
            Serial.println("CW ON");
            ledOn();
        }
    #endif

}

void setup() {
    Serial.begin(115200);
    Serial.setTimeout(100);

    Serial.println("Setup test power tool....");

    pinMode(LED_BUILTIN, OUTPUT);

#if defined(LED_IS_RGB)
    led.begin();  // initialize strip (required!)
	led.setBrightness(255);

#endif

    initLoRa();

    l_cli_cnf.buf = l_cli_buf;
    l_cli_cnf.bufc = CLI_BUFFER_SIZE;
    l_cli_cnf.commands = l_cli_commands;
    l_cli_cnf.commandc = CLI_COMMAND_COUNT;
    l_cli_cnf.tx = cliTxCallback;

    CLIInit(&l_cli_inst, l_cli_cnf);


    #if defined(TX_EN_PIN)
        pinMode(TX_EN_PIN, OUTPUT);
        digitalWrite(TX_EN_PIN, HIGH);
    #endif

    #if defined(RX_EN_PIN)
        pinMode(RX_EN_PIN, OUTPUT);
        digitalWrite(RX_EN_PIN, LOW);
    #endif

    #if defined(FAN_EN_PIN)
        pinMode(FAN_EN_PIN, OUTPUT);
        digitalWrite(FAN_EN_PIN, HIGH);
    #endif

    #if defined(PIN_RFamp_APC2)
        dacWrite(PIN_RFamp_APC2, 30);
    #endif

}

void loop() {
    // Check if user typed something into the serial monitor
    while (Serial.available()) {
        uint8_t a;
        a = Serial.read();
       cliRxCallback(a);
    }

    CLIHandle(&l_cli_inst);
}


static CLIRet_t pCallback(void *args, CLI_ARG_COUNT_VALUE_T argc)
{
    void *buf = NULL;
    CLIArg_t arg = {0U}; 

    buf = CLIArgParse(&l_cli_inst, &arg, args, argc);

    if (buf) {
        int pwr = atoi((char*)buf);

        #if defined(chip_SX1281)
            int state = radio.setOutputPower(default_pwr);
        #else    
            int state = radio.setOutputPower(default_pwr, radio_rfo_hf);
        #endif

        if (state != RADIOLIB_ERR_NONE) {
            Serial.println(F("Selected output power is invalid for this module!"));
            Serial.println(state);
        }
        else {
            int state = radio.transmitDirect();
            Serial.printf("set power to %d", pwr);
        }
    }
    else {
        Serial.println("please provide power value in dBm");
    }

    return CLI_OK;
}


static CLIRet_t fCallback(void *args, CLI_ARG_COUNT_VALUE_T argc)
{
    void *buf = NULL;
    CLIArg_t arg = {0U}; 

    buf = CLIArgParse(&l_cli_inst, &arg, args, argc);

    if (buf) {
        int freq = atoi((char*)buf);

        int state = radio.setFrequency(freq);
        if (state != RADIOLIB_ERR_NONE) {
            Serial.println(F("Selected frequency is invalid for this module!"));
            Serial.println(state);
        }
        else {
            Serial.printf("Set frequency %d", freq);
        }
    }
    else {
        Serial.println("please set freq <freq value>");
    }
	return CLI_OK;
}

static CLIRet_t aCallback(void *args, CLI_ARG_COUNT_VALUE_T argc)
{
    void *buf = NULL;
    CLIArg_t arg = {0U}; 

    buf = CLIArgParse(&l_cli_inst, &arg, args, argc);

    if (buf) {
        int apc = atoi((char*)buf);
#if defined(PIN_RFamp_APC2)
        dacWrite(PIN_RFamp_APC2, apc);

        Serial.printf("Set DAC power control %d", apc);
#else
        Serial.printf("Analog control not supported");
#endif
    }
    else {
        Serial.println("please set dac power control <dac value>");
    }
	return CLI_OK;
}

static CLIRet_t cwCallback(void *args, CLI_ARG_COUNT_VALUE_T argc)
{
    void *buf = NULL;
    CLIArg_t arg = {0U}; 

    buf = CLIArgParse(&l_cli_inst, &arg, args, argc);

    if (buf) {
        int cw = atoi((char*)buf);
        if (cw == 0) {
            int state = radio.standby();
            Serial.println("CW OFF");
            ledOff();
        }
        else {
            int state = radio.transmitDirect();

            if (state != RADIOLIB_ERR_NONE) {
                Serial.println(F("[SX1278] Unable to start direct transmission mode, code "));
                Serial.println(state);
            }
            else {
                Serial.println("CW ON");
                ledOn();
            }
        }
    }
    else {
        Serial.println("please set CW 0/1");
    }

    return CLI_OK;
}



static void cliRxCallback(uint8_t data)
{
    CLIInsert(&l_cli_inst, data);
}

static void cliTxCallback(CLI_BUF_VALUE_T *buf, CLI_TX_BUF_COUNT_VALUE_T bufc)
{
    Serial.write(buf, bufc);
}

static CLIRet_t cliCallback(void *args, CLI_ARG_COUNT_VALUE_T argc)
{
    void *buf = NULL;
    CLIArg_t arg = {0U}; 

    do
    {
        buf = CLIArgParse(&l_cli_inst, &arg, args, argc);

        if (buf)
        {
            cliTxCallback((CLI_BUF_VALUE_T *) buf, strlen((char *) buf));
        }
    } while (buf != NULL);
    
    return CLI_OK;
}
