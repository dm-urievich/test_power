#include <Arduino.h>


#define RADIOLIB_DEBUG_BASIC 1
#define RADIOLIB_GODMODE 1

// LoRa include
#include <SPI.h>
#include <RadioLib.h>

#if defined(ESP32) || defined(RP_PICO)
#include <FastLED_NeoPixel.h>
#endif

#include <SimpleCLI.h>

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
// #define AERONETIX_TX 1
// #define EMAX_OLED_TX 1
// #define ELRS_BOX_TX 1
// #define AERONETIX_LR1121_V1_TX 1
// #define GEMINI_XROSSBAND 1
// #define AERONETIX_LR1121_V2_TX
// #define IFLIGHT_DIVERSITY_500_CENTER
// #define IFLIGHT_DIVERSITY_500_LEFT
// #define BelinRC_350_TX
// #define CYCLONE_LR1121_1W
// #define LT_433
// #define BETAFPV_TX_900_V2 1
// #define BAYCK_TX_580_LR1121 1
// #define EDIFIER_433_TX 1
// #define AERONETIX_5W_V1_TX 1
// #define AERONETIX_1W_V3_TX 1
// #define BelinRC_433_TX  1
// #define COALAS_45_TX   1
// #define NOMAD_LEFT_TX   1
// #define EMAX_2400_TX    1
// #define RANGER_MICRO_2400_TX 1
// #define ESP8285_SX1281_RX 1
// #define PICO_RADIO 1
// #define AERONETIX_1W_24_V1_TX 1
// #define BAYCKRC_GEMINI_NANO 1
// #define PICO_RADIO_EVT2 1
// #define DIY_E19 1
// #define PICO_RADIO_EVT3 1
#define BETAFPV_400_V1_1 1

// #define CW_FROM_STARTUP 1
float default_freq = 2400;
float default_pwr = 2;
int cw_is_on = 0;

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
#define LED_IS_RGB  1

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
#define LED_IS_RGB  1

const bool radio_rfo_hf = true;

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
#define LED_IS_RGB  1

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
#define LORA_RST    15

#define LORA_SCK    32
#define LORA_MISO   33
#define LORA_MOSI   25

#define LED_BUILTIN 4

const bool radio_rfo_hf = false;

#define LED_IS_RGB  1

#elif defined(EMAX_OLED_TX)

#define LORA_CS     5
#define LORA_IRQ    4
#define LORA_RST    14

#define LORA_SCK    18
#define LORA_MISO   19
#define LORA_MOSI   23

#define LED_BUILTIN 27

const bool radio_rfo_hf = false;

#define RX_EN_PIN   12
#define FAN_EN_PIN  32
#define PIN_RFamp_APC2 26
#define LED_IS_RGB  1

#elif defined(ELRS_BOX_TX)

#define LORA_CS     5
#define LORA_IRQ    16
#define LORA_RST    17

#define LORA_SCK    18
#define LORA_MISO   19
#define LORA_MOSI   23

#define LED_BUILTIN 2

const bool radio_rfo_hf = false;

#define RX_EN_PIN   26
#define TX_EN_PIN   27

#elif defined(AERONETIX_LR1121_V1_TX)

// left lr1121
#define LORA_CS     33
#define LORA_IRQ    14
#define LORA_RST    12
#define LORA_BUSY   32

// #define RADIO_DCDC  1

#define LORA_SCK    25
#define LORA_MISO   27
#define LORA_MOSI   26

#define LED_BUILTIN 4
#define LED_IS_RGB  1

const bool radio_rfo_hf = false;

#define chip_LR1121 1
#define TCXO_voltage (3.0)

#elif defined(GEMINI_XROSSBAND)

// lr1121
#define LORA_CS     27
#define LORA_IRQ    37
#define LORA_RST    26
#define LORA_BUSY   36

#define RADIO_DCDC  1

#define LORA_SCK    25
#define LORA_MISO   33
#define LORA_MOSI   32

#define LED_BUILTIN 22
#define LED_IS_RGB  1

#define FAN_EN_PIN  4

const bool radio_rfo_hf = true;

#define chip_LR1121 1

#elif defined(AERONETIX_LR1121_V2_TX)

// left lr1121
#define LORA_CS     33
#define LORA_IRQ    14
#define LORA_RST    15
#define LORA_BUSY   32

#define LORA_SCK    25
#define LORA_MISO   27
#define LORA_MOSI   26

#define LED_BUILTIN 4
#define LED_IS_RGB  1

const bool radio_rfo_hf = true;

#define chip_LR1121 1

#elif defined(IFLIGHT_DIVERSITY_500_CENTER)

#define LORA_CS     27
#define LORA_IRQ    36
#define LORA_RST    26

//right chip
#define LORA_CS_2     13

#define LORA_SCK    25
#define LORA_MISO   33
#define LORA_MOSI   32

#define LED_BUILTIN 22
#define LED_IS_RGB  1

#define RX_EN_PIN   10
#define TX_EN_PIN   14

const bool radio_rfo_hf = false;

#elif defined(IFLIGHT_DIVERSITY_500_LEFT)

#define LORA_CS     13
#define LORA_IRQ    39
#define LORA_RST    21

//center chip
#define LORA_CS_2     27

#define LORA_SCK    25
#define LORA_MISO   33
#define LORA_MOSI   32

#define LED_BUILTIN 22
#define LED_IS_RGB  1

#define RX_EN_PIN   9
#define TX_EN_PIN   15

const bool radio_rfo_hf = false;

#elif defined(BelinRC_350_TX)

#define LORA_CS     5
#define LORA_IRQ    4
#define LORA_RST    14

#define LORA_SCK    18
#define LORA_MISO   19
#define LORA_MOSI   23

#define LED_BUILTIN 27

const bool radio_rfo_hf = false;

#define RX_EN_PIN   12
#define TX_EN_PIN   2
#define FAN_EN_PIN  32
#define PIN_RFamp_APC2 26
#define LED_IS_RGB  1

#elif defined(CYCLONE_LR1121_1W)

// lr1121
#define LORA_CS     5
#define LORA_IRQ    39
#define LORA_RST    22
#define LORA_BUSY   38

#define RADIO_DCDC  1

#define LORA_SCK    18
#define LORA_MISO   19
#define LORA_MOSI   23

#define LED_BUILTIN 21
#define LED_IS_RGB  1

#define FAN_EN_PIN  4

const bool radio_rfo_hf = true;

#define chip_LR1121 1

#elif defined(LT_433)

#define LORA_CS     17
#define LORA_IRQ    26
#define LORA_RST    14

#define LORA_SCK    18
#define LORA_MISO   19
#define LORA_MOSI   23

#define LED_BUILTIN 27

#define RX_EN_PIN   13
#define TX_EN_PIN   12
const bool radio_rfo_hf = false;

#define FAN_EN_PIN  16

#elif defined(BETAFPV_TX_900_V2)

#define LORA_CS     2
#define LORA_IRQ    34
#define LORA_RST    13

#define LORA_SCK    25
#define LORA_MISO   33
#define LORA_MOSI   32

#define LED_BUILTIN 22
#define LED_IS_RGB  1

const bool radio_rfo_hf = false;
#define PIN_RFamp_APC2 26

#define FAN_EN_PIN 27

#elif defined(EDIFIER_433_TX)

#define LORA_CS     5
#define LORA_IRQ    4
#define LORA_RST    14

#define LORA_SCK    18
#define LORA_MISO   19
#define LORA_MOSI   23

#define LED_BUILTIN 27

const bool radio_rfo_hf = false;

#define FAN_EN_PIN  32
#define PIN_RFamp_APC2 26
#define LED_IS_RGB  1

#elif defined(AERONETIX_5W_V1_TX)

// left lr1121
#define LORA_CS     33
#define LORA_IRQ    14
#define LORA_RST    13
#define LORA_BUSY   32

#define LORA_SCK    25
#define LORA_MISO   27
#define LORA_MOSI   26

#define LED_BUILTIN 23
#define LED_IS_RGB  1

const bool radio_rfo_hf = true;

#define chip_LR1121 1

#elif defined(AERONETIX_1W_V3_TX)

// left lr1121
#define LORA_CS     33
#define LORA_IRQ    14
#define LORA_RST    13
#define LORA_BUSY   32

#define LORA_SCK    25
#define LORA_MISO   27
#define LORA_MOSI   26

#define LED_BUILTIN 23
#define LED_IS_RGB  1

const bool radio_rfo_hf = false;

#define chip_LR1121 1

#elif defined(BelinRC_433_TX)

#define LORA_CS     5
#define LORA_IRQ    4
#define LORA_RST    14

#define LORA_SCK    18
#define LORA_MISO   19
#define LORA_MOSI   23

#define LED_BUILTIN 27

const bool radio_rfo_hf = false;

#define RX_EN_PIN   2
#define TX_EN_PIN   12
#define FAN_EN_PIN  32
#define PIN_RFamp_APC2 26
#define LED_IS_RGB  1

#elif defined(COALAS_45_TX)

#define LORA_CS     15
#define LORA_IRQ    18
#define LORA_RST    5

#define LORA_SCK    14
#define LORA_MISO   12
#define LORA_MOSI   13

#define LED_BUILTIN 21
#define LED_IS_RGB  1

const bool radio_rfo_hf = false;

#define PIN_RFamp_APC2 25

#elif defined(NOMAD_LEFT_TX)

#define LORA_CS     27
#define LORA_IRQ    37
#define LORA_RST    15
#define LORA_BUSY   36

#define LORA_SCK    25
#define LORA_MISO   33
#define LORA_MOSI   32

#define LORA_CS_2   13

#define FAN_EN_PIN  2

#define LED_BUILTIN 22
#define LED_IS_RGB  1

const bool radio_rfo_hf = true;

#define PIN_RFamp_APC2 26
#define RADIO_DCDC  1

#define chip_LR1121 1

#elif defined(EMAX_2400_TX)

#define LORA_CS     5
#define LORA_IRQ    4
#define LORA_RST    14
#define LORA_BUSY   21

#define LORA_SCK    18
#define LORA_MISO   19
#define LORA_MOSI   23

#define RX_EN_PIN   27
#define TX_EN_PIN   26

#define FAN_EN_PIN  22

#define LED_BUILTIN 12
#define LED_IS_RGB  1

#define chip_SX1281 1

#elif defined(RANGER_MICRO_2400_TX)

#define LORA_CS     4
#define LORA_IRQ    21
#define LORA_RST    5
#define LORA_BUSY   22

#define LORA_SCK    18
#define LORA_MISO   19
#define LORA_MOSI   23

#define RX_EN_PIN   32
#define TX_EN_PIN   33

#define FAN_EN_PIN  27

#define LED_BUILTIN 15
#define LED_IS_RGB  1

#define chip_SX1281 1

#elif defined(ESP8285_SX1281_RX)

#define LORA_CS     15
#define LORA_IRQ    4
#define LORA_RST    2
#define LORA_BUSY   5

#define LORA_SCK    14
#define LORA_MISO   12
#define LORA_MOSI   13

#define LED_BUILTIN 16

#define chip_SX1281 1

#elif defined(PICO_RADIO)

#define LORA_CS     17
#define LORA_IRQ    15
#define LORA_RST    12
#define LORA_BUSY   14

#define LORA_CS_2   27
#define LORA_RST_2  22
#define LORA_IRQ_2  5

#define LORA_SCK    18
#define LORA_MISO   16
#define LORA_MOSI   19

#define LED_BUILTIN 25

#define RADIO_TCXO_ENABLE      13

#define chip_SX1281 1
#define B5_PICO 1

#elif defined(AERONETIX_1W_24_V1_TX)

// left lr1121
#define LORA_CS     33
#define LORA_IRQ    14
#define LORA_RST    13
#define LORA_BUSY   32

#define LORA_SCK    25
#define LORA_MISO   27
#define LORA_MOSI   26

#define LED_BUILTIN 23
#define LED_IS_RGB  1

const bool radio_rfo_hf = false;

#define chip_LR1121 1

#elif defined(BAYCKRC_GEMINI_NANO)

#define LORA_CS     27
#define LORA_IRQ    37
#define LORA_RST    26
#define LORA_BUSY   36

#define LORA_SCK    25
#define LORA_MISO   33
#define LORA_MOSI   32

#define LORA_CS_2   15

#define FAN_EN_PIN  4

#define LED_BUILTIN 12
#define LED_IS_RGB  1

const bool radio_rfo_hf = false;

#define RADIO_DCDC  1

#define chip_LR1121 1
#define SET_RF_SWITCH 1

#elif defined(PICO_RADIO_EVT2)

#define LORA_CS     17
#define LORA_IRQ    15
#define LORA_RST    12
#define LORA_BUSY   14

#define LORA_CS_2   27
#define LORA_RST_2  22
#define LORA_IRQ_2  21
#define LORA_BUSY_2 20

#define LORA_SCK    18
#define LORA_MISO   16
#define LORA_MOSI   19

#define LED_BUILTIN 25

#define RADIO_TCXO_ENABLE      13

#define chip_SX1281 1
#define B5_PICO 1

#elif defined(DIY_E19)

#define LORA_CS     5
#define LORA_IRQ    26
#define LORA_RST    14

#define LORA_SCK    18
#define LORA_MISO   19
#define LORA_MOSI   23

#define LED_BUILTIN 27

const bool radio_rfo_hf = false;

#define RX_EN_PIN   13
#define TX_EN_PIN   12
#define FAN_EN_PIN  32

// #define LED_IS_RGB  1

#elif defined(PICO_RADIO_EVT3)

#define LORA_CS     5
#define LORA_IRQ    11
#define LORA_RST    12
#define LORA_BUSY   10

#define LORA_CS_2   27
#define LORA_RST_2  15
#define LORA_IRQ_2  14
#define LORA_BUSY_2 13

#define LORA_SCK    6
#define LORA_MISO   4
#define LORA_MOSI   7

#define LED_BUILTIN 25
#define LED_IS_RGB  1

#define chip_SX1281 1
#define B5_PICO 1

#elif defined(BETAFPV_400_V1_1)

#define LORA_CS     2
#define LORA_IRQ    34
#define LORA_RST    13

#define LORA_SCK    25
#define LORA_MISO   33
#define LORA_MOSI   32

#define LED_BUILTIN 22
#define LED_IS_RGB  1

#define PIN_RFamp_APC2 26

const bool radio_rfo_hf = false;

#define FAN_EN_PIN 27

#endif

// Create CLI Object
SimpleCLI cli;

// Commands
Command freqCmd;
Command pwrCmd;
Command analogCmd;
Command cwCmd;

void fCallback(cmd* c);
void pCallback(cmd* c);
void aCallback(cmd* c);
void cwCallback(cmd* c);
void errorCallback(cmd_error* e);

#if defined(chip_LR1121)

LR1121 radio = new Module(LORA_CS, LORA_IRQ, LORA_RST, LORA_BUSY);

#elif defined(chip_SX1281)

SX1281 radio = new Module(LORA_CS, LORA_IRQ, LORA_RST, LORA_BUSY);

#if defined(B5_PICO)
SX1281 radio_2 = new Module(LORA_CS_2, LORA_IRQ_2, LORA_RST_2);
#endif

#else
SX1276 radio = new Module(LORA_CS, LORA_IRQ, LORA_RST);

#if defined(BARVINOK_TX)
SX1276 radio_2 = new Module(LORA_CS_2, LORA_IRQ_2, LORA_RST_2);
#endif
#endif

#if defined(LED_IS_RGB)
FastLED_NeoPixel<1, LED_BUILTIN, NEO_GRB> led;
#endif

#define RED 0xFF0000
#define GREEN 0x00FF00
#define BLUE 0x0000FF
#define LED_OFF 0x000000

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

#if defined(LORA_CS_2)    
    pinMode(LORA_CS_2, OUTPUT);
    digitalWrite(LORA_CS_2, HIGH);
#endif

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

#if defined(RP_PICO)
    SPI.begin();
    SPI.setMISO(LORA_MISO);
    SPI.setMOSI(LORA_MOSI);
    SPI.setSCK(LORA_SCK);
#elif not defined(ESP8285)
    SPI.begin(LORA_SCK, LORA_MISO, LORA_MOSI, LORA_CS);
    // SPI.setFrequency(400000);
#else
    SPI.pins(LORA_SCK, LORA_MISO, LORA_MOSI, LORA_CS);
    SPI.begin();
#endif
#if defined RADIO_TCXO_ENABLE
    pinMode(RADIO_TCXO_ENABLE, OUTPUT);
    digitalWrite(RADIO_TCXO_ENABLE, HIGH);
    delay(10);
#endif

#if defined (chip_LR1121)
    int state = 0;
    #if defined (RADIO_DCDC)
        state = radio.setRegulatorDCDC();
        if (state == RADIOLIB_ERR_NONE) {
            Serial.println(F("setRegulatorDCDC success!"));
        } else {
            Serial.print(F("setRegulatorDCDC failed, code "));
            Serial.println(state);
            while (true);
        }
    
    #endif

#ifdef TCXO_voltage
    state = radio.setTCXO(TCXO_voltage, 5000);

    if (state == RADIOLIB_ERR_NONE) {
        Serial.println(F("success!"));
    } else {
        Serial.print(F("failed, code "));
        Serial.println(state);
    }
#endif 

#ifdef SET_RF_SWITCH
    radio.setDioAsRfSwitch(0b00001111, 0b00000000, 0b00000100, 0b00001000, 0b00001000, 0b00000010, 0, 0b00000001);
#endif

    state = radio.beginGFSK();

#elif defined(chip_SX1281)
    int state = radio.beginGFSK();


    #if defined(B5_PICO)
    radio_2.beginGFSK();
    #endif
#else
    int state = radio.beginFSK();
#endif
    
    if (state == RADIOLIB_ERR_NONE) {
        Serial.println(F("radio.begin success!"));
    } else {
        Serial.print(F("radio.begin failed, code "));
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
    #if defined(B5_PICO)
    if (state != RADIOLIB_ERR_NONE) {
        state = radio_2.setOutputPower(default_pwr);
    }
    #endif
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

    cli.setOnError(errorCallback); // Set error Callback

    freqCmd = cli.addSingleArgCmd("f", fCallback);
    pwrCmd = cli.addSingleArgCmd("p", pCallback);
    analogCmd = cli.addSingleArgCmd("a", aCallback);
    cwCmd = cli.addSingleArgCmd("cw", cwCallback);

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

String serial_input;

void loop() {
    // Check if user typed something into the serial monitor
    if (Serial.available()) {
        char input = (char) Serial.read();//Serial.readString();
        Serial.print(input);
        serial_input += input;

        if (serial_input.endsWith(String('\n'))) {
            // Parse the user input into the CLI
            cli.parse(serial_input);

            if (cli.errored()) {
                CommandError cmdError = cli.getError();

                Serial.print("ERROR: ");
                Serial.println(cmdError.toString());

                if (cmdError.hasCommand()) {
                    Serial.print("Did you mean \"");
                    Serial.print(cmdError.getCommand().toString());
                    Serial.println("\"?");
                }
            }

            serial_input = "";
        }
    }
}

// Callback function for freq command
void fCallback(cmd* c) {
    Command cmd(c); // Create wrapper object

    // Get first (and only) Argument
    Argument arg = cmd.getArgument(0);

    if (arg.isSet()) {

        // Get value of argument
        String argVal = arg.getValue();
        int freq = argVal.toInt();

        if (freq) {
            #ifdef TCXO_voltage
            radio.setTCXO(TCXO_voltage, 5000);
            #endif
            
            int state = radio.setFrequency(freq);
            #if defined(B5_PICO)
                radio_2.setFrequency(freq+2);
            #endif
            if (state != RADIOLIB_ERR_NONE) {
                Serial.println(F("Selected frequency is invalid for this module!"));
                Serial.println(state);
            }
            else {
                Serial.printf("Set frequency %d\n", freq);

                #if defined(chip_LR1121) || defined(chip_SX1281)
                if (cw_is_on == 1 || cw_is_on == 4) {
                    radio.standby();
                    delay(20);
                    radio.transmitDirect();
                }

                #if defined(B5_PICO)
                    if (cw_is_on == 3 || cw_is_on == 4) {
                        radio_2.standby();
                        delay(20);
                        radio_2.transmitDirect();
                    }
                #endif
                #endif
            }
        }
    }
    else {
        Serial.println("please set freq <freq value>");
    }
}

// Callback function for power command
void pCallback(cmd* c) {
    Command cmd(c); // Create wrapper object

    // Get first (and only) Argument
    Argument arg = cmd.getArgument(0);

    if (arg.isSet()) {
        // Get value of argument
        String argVal = arg.getValue();
        int pwr = argVal.toInt();

        #if defined(chip_SX1281)
            int state = radio.setOutputPower(pwr);
            #if defined(B5_PICO)
            if (state != RADIOLIB_ERR_NONE) {
                state = radio_2.setOutputPower(pwr);
            }
            #endif
        #else    
            int state = radio.setOutputPower(pwr, radio_rfo_hf);
        #endif

        if (state != RADIOLIB_ERR_NONE) {
            Serial.println(F("Selected output power is invalid for this module!"));
            Serial.println(state);
        }
        else {
            Serial.printf("set power to %d\n", pwr);
        }
    }
    else {
        Serial.println("please provide power value in dBm");
    }
}

void cwCallback(cmd* c) {
    Command cmd(c); // Create wrapper object

    // Get first (and only) Argument
    Argument arg = cmd.getArgument(0);

    if (arg.isSet()) {
        String argVal = arg.getValue();
        int cw = argVal.toInt();

        switch (cw) {
            case(0) : {
                int state = radio.standby();
                Serial.println("CW OFF");
                ledOff();
                cw_is_on = 0;
                break;
            }
            case(1) : {
                int state = radio.transmitDirect();
                if (state != RADIOLIB_ERR_NONE) {
                    Serial.println(F("[SX1278] Unable to start direct transmission mode, code "));
                    Serial.println(state);
                }
                else {
                    Serial.println("CW ON");
                    ledOn();
                    cw_is_on = 1;
                }
                break;
            }
            #if defined(B5_PICO)
            case(2) : {
                int state = radio_2.standby();
                Serial.println("CW radio 2 OFF");
                ledOff();
                cw_is_on = 0;
                break;
            }
            case(3) : {
                int state = radio_2.transmitDirect();
                if (state != RADIOLIB_ERR_NONE) {
                    Serial.println(F("[SX1278] Unable to start direct transmission mode, code "));
                    Serial.println(state);
                }
                else {
                    Serial.println("CW radio 2 ON");
                    ledOn();
                    cw_is_on = 3;
                }
                break;
            }
            case(4) : {
                int state = radio.transmitDirect();
                state = radio_2.transmitDirect();
                if (state != RADIOLIB_ERR_NONE) {
                    Serial.println(F("[SX1278] Unable to start direct transmission mode, code "));
                    Serial.println(state);
                }
                else {
                    Serial.println("CW radio 1 and 2 ON");
                    ledOn();
                    cw_is_on = 4;
                }
                break;
            }
            #endif
        }
    }
    else {
        Serial.println("please set CW 0/1");
    }
}

void aCallback(cmd* c) {
    Command cmd(c); // Create wrapper object

    // Get first (and only) Argument
    Argument arg = cmd.getArgument(0);

    if (arg.isSet()) {
        String argVal = arg.getValue();
        int apc = argVal.toInt();
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
}

// Callback in case of an error
void errorCallback(cmd_error* e) {
    CommandError cmdError(e); // Create wrapper object

    Serial.print("ERROR: ");
    Serial.println(cmdError.toString());

    if (cmdError.hasCommand()) {
        Serial.print("Did you mean \"");
        Serial.print(cmdError.getCommand().toString());
        Serial.println("\"?");
    }
}