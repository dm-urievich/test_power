#include <Arduino.h>

// LoRa include
#include <SPI.h>
#include <RadioLib.h>

#define TTGO_V2 1

#if defined(TTGO_V2)

#define LORA_CS     18
#define LORA_IRQ    26
#define LORA_RST    23

#define LORA_SCK    5
#define LORA_MISO   19
#define LORA_MOSI   27

#define LED_BUILTIN 25

#else

/*    "radio_dio0": 4,
    "radio_miso": 19,
    "radio_mosi": 23,
    "radio_nss": 5,
    "radio_rst": 14,
    "radio_sck": 18,*/
#define LORA_CS     5
#define LORA_IRQ    4
#define LORA_RST    14

#define LORA_SCK    18
#define LORA_MISO   19
#define LORA_MOSI   23

#endif 

#include "cli.h"
#include <string.h>

#define CLI_BUFFER_SIZE 128
#define CLI_COMMAND_COUNT 4

#define CLI_ENTRY(command) { command ## Callback, (const CLI_BUF_VALUE_T *) #command } 

static void cliRxCallback(uint8_t data);
static void cliTxCallback(CLI_BUF_VALUE_T *buf, CLI_TX_BUF_COUNT_VALUE_T bufc);

static CLIRet_t cliCallback(void *args, CLI_ARG_COUNT_VALUE_T argc);
static CLIRet_t pwrCallback(void *args, CLI_ARG_COUNT_VALUE_T argc);
static CLIRet_t freqCallback(void *args, CLI_ARG_COUNT_VALUE_T argc);
static CLIRet_t cwCallback(void *args, CLI_ARG_COUNT_VALUE_T argc);

CLIConfig_t l_cli_cnf;
CLIInst_t l_cli_inst;
CLI_BUF_VALUE_T l_cli_buf[CLI_BUFFER_SIZE];
CLICommand_t l_cli_commands[CLI_COMMAND_COUNT] =
{
    CLI_ENTRY(cli),
    CLI_ENTRY(pwr),
    CLI_ENTRY(freq),
    CLI_ENTRY(cw),
};

float default_freq = 915;
float default_pwr = 1;

SX1276 radio = new Module(LORA_CS, LORA_IRQ, LORA_RST);

void initLoRa() {
    Serial.println("Initializing LoRa....");
    SPI.begin(LORA_SCK, LORA_MISO, LORA_MOSI, LORA_CS);

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
    state = radio.setOutputPower(default_pwr);

    // state = radio.transmitDirect();
    // if (state != RADIOLIB_ERR_NONE) {
    //     Serial.println(F("[SX1278] Unable to start direct transmission mode, code "));
    //     Serial.println(state);
    // }
}

void setup() {
    Serial.begin(115200);
    Serial.setTimeout(100);

    Serial.println("Setup test power tool....");

    initLoRa();
    
    pinMode(LED_BUILTIN, OUTPUT);


    l_cli_cnf.buf = l_cli_buf;
    l_cli_cnf.bufc = CLI_BUFFER_SIZE;
    l_cli_cnf.commands = l_cli_commands;
    l_cli_cnf.commandc = CLI_COMMAND_COUNT;
    l_cli_cnf.tx = cliTxCallback;

    CLIInit(&l_cli_inst, l_cli_cnf);

}

void loop() {
    // Check if user typed something into the serial monitor
    while (Serial.available()) {
       cliRxCallback(Serial.read());
    }

    CLIHandle(&l_cli_inst);
}


static CLIRet_t pwrCallback(void *args, CLI_ARG_COUNT_VALUE_T argc)
{
    void *buf = NULL;
    CLIArg_t arg = {0U}; 

    buf = CLIArgParse(&l_cli_inst, &arg, args, argc);

    if (buf) {
        int pwr = atoi((char*)buf);

        int state = radio.setOutputPower(pwr);
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


static CLIRet_t freqCallback(void *args, CLI_ARG_COUNT_VALUE_T argc)
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
