#include <Arduino.h>

// LoRa include
#include <SPI.h>
#include <RadioLib.h>

//#define TTGO_V2 1
// #define BARVINOK_TX 1
// #define RADIOMATER_BANDIT 1
#define ES900TX 1
// #define BETAFPV_MICRO 1
// #define ELRS_RX 1
// #define BETAFPV_TX_400 1


// #define CW_FROM_STARTUP 1
float default_freq = 750;
float default_pwr = 2;

#if defined(TTGO_V2)

#define LORA_CS     18
#define LORA_IRQ    26
#define LORA_RST    23

#define LORA_SCK    5
#define LORA_MISO   19
#define LORA_MOSI   27

#define LED_BUILTIN 25

const bool radio_rfo_hf = false;

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

SX1276 radio = new Module(LORA_CS, LORA_IRQ, LORA_RST);

//SX1276 radio_2 = new Module(LORA_CS_2, LORA_IRQ_2, LORA_RST_2);

void initLoRa() {
    Serial.println("Initializing LoRa....");

#if not defined(ELRS_RX)
    SPI.begin(LORA_SCK, LORA_MISO, LORA_MOSI, LORA_CS);
#else
    SPI.pins(LORA_SCK, LORA_MISO, LORA_MOSI, LORA_CS);
    SPI.begin();
#endif
    int state = radio.beginFSK();
    if (state == RADIOLIB_ERR_NONE) {
        Serial.println(F("success!"));
    } else {
        Serial.print(F("failed, code "));
        Serial.println(state);
        while (true);
    }

    // if needed, you can switch between LoRa and FSK modes
    //
    // radio.begin()       start LoRa mode (and disable FSK)
    // radio.beginFSK()    start FSK mode (and disable LoRa)

    // the following settings can also
    // be modified at run-time
    state = radio.setFrequency(default_freq);
    state = radio.setOutputPower(default_pwr, radio_rfo_hf);

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
            digitalWrite(LED_BUILTIN, HIGH);
        }
    #endif

}

void setup() {
    Serial.begin(115200);
    Serial.setTimeout(100);

    Serial.println("Setup test power tool....");

    pinMode(LED_BUILTIN, OUTPUT);

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

        int state = radio.setOutputPower(pwr, radio_rfo_hf);
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

        dacWrite(PIN_RFamp_APC2, apc);

        Serial.printf("Set DAC power control %d", apc);
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
            digitalWrite(LED_BUILTIN, LOW);
        }
        else {
            int state = radio.transmitDirect();

            if (state != RADIOLIB_ERR_NONE) {
                Serial.println(F("[SX1278] Unable to start direct transmission mode, code "));
                Serial.println(state);
            }
            else {
                Serial.println("CW ON");
                digitalWrite(LED_BUILTIN, HIGH);
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
