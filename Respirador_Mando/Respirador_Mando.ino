#include <FlashAsEEPROM.h>
#include <FlashStorage.h>
#include <LoRa.h>
#include <Adafruit_ZeroTimer.h>
#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include <Fonts/FreeSans12pt7b.h>
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSans18pt7b.h>
#include <Fonts/Org_01.h>
#include <Adafruit_SleepyDog.h>

#define WIRELESS
#define LORA_TEST 0

#define TFT_DC 32
#define TFT_CS 48
#define TFT_RST 2
#define pulse 45
#define usb_sel 14
#define W_LED 3
#define V_MEASURE 15
#define V_REF 16
#define RF_ENABLE 9
#define VBAT_CONTROL 44
#define VCHARGE_CONTROL 43
#define READ_5V 41
#define RF_SET 11
#define TFT_LED 4
#define BUZZ 6
#define MUX_ENABLE 54

#define PUSHED digitalRead(pulse)
#define NOT_PUSHED digitalRead(pulse) == 0
#define SerialBus Serial1

//-------------------//

int graphicWait = 3000;
byte CMV, previousCMV;
int PIPpressure = 35, minPIPpressure = 25, maxPIPpressure = 45, PIPpressureAlarm = PIPpressure;
int oscRate = 15, minOscRate = 8, maxOscRate = 30, oscRateAlarm = oscRate;
int cc = 400, minCc = 200, maxCc = 1000, ccAlarm = cc;
int expRatio = 2, minExpRatio = 1, maxExpRatio = 5;
int PTrigger = -2, minPTrigger = -5, maxPTrigger = 0;

//-------------------//

int xRes = 215, yRes = 221;
int xOffset = 5, yOffset = 310;
int xAxis = 220, yAxis = 285;
int graphicX = xAxis - xOffset;
int graphicY = yAxis - yOffset;
int axisWidth = 3;
int scale = 2;
int stripLineWidth = 1;
int stripLineDensity = 5, stripLineCount = 0;
int PIPpressureXPos = 170;
int oscRateXPos = 155;
int expRatioXPos = 230;
int ccXPos = 210;
int PTriggerXPos = 165;

//-------------------//

int graphicPos;
int graphicBuffer[300], inputBuffer[300], graphicBefore[300];
int newData, maxData = 0, minData = 99;
bool CMV_set;
int PIPpressure_set, oscRate_set, cc_set, expRatio_set, PTrigger_set;
byte bufferIndex = 0;
byte Start_capture = 0;
unsigned long data_generation_ref = 0;
int graph_index = 0;
float angle;
byte start_confirmed = 0;

//-------------------//



int n_serie_int = 744;            //especificar el numero de serie
byte set_n_serie = 1;
int ajuste_bateria = 1100;
byte set_ajuste_bateria = 0;
byte producto = 0;
const int sentido_giro_encoder = -1; //sentido giro encoder
const int eeprom_size = 1024;
int millivolts_0 = 0;


#ifdef WIRELESS
#define SERIAL  Serial2
const byte wireless = 1;
byte wireless_com = 1;
#elif defined(WIRED)
#define SERIAL  Serial1
const byte wireless = 0;
byte wireless_com = 0;
#endif

/* Working parameters start */
long long stopping_speed[3] = {200000, 200000, 200000};
long tl_speed_travel = 2400;
long tl_speed_pan = 5000;
int min_speed_travel = 100000;
int max_speed_travel = 2000;
const long long min_speed_pan = 400000;
const long long max_speed_pan = 2000;
const long long max_manual_speed_pan = 5000; //VEL MAX PAN (2000 para el drv8825), (2000 para el tmc2100)
int first_speed_to_avoid = 41;
int last_speed_to_avoid = 41;
int sentido_travel = 1; //sentido inicio travel
long long accel_time_min = 2000000;
const int acc_const = 40000;
float r = 0.2; // Factor para modificar la curva de distribuciÃ³n de velocidades. 0 = Poca resolucion en velocidades bajas. 10 = distribuciÃ³n similar a la antigua distribucion en serie geometrica.
const float r_pan = -0.2; // Factor para modificar la curva de distribuciÃ³n de velocidades. 0 = Poca resolucion en velocidades bajas. 10 = distribuciÃ³n similar a la antigua distribucion en serie geometrica.
float wheel_factor = 9.0;
/* Working parameters end */



byte key_pos = 0, key_sequence = 0;
int key_dir = 1;


byte dual_range = 0;
int turntable_factor = 6500;
byte stop_m_running = 0;
int move_total = 0;
byte stop_m_direction = 2;
const long min_speed_mt = 4000000;
const int timer_autoapagado = 600000;
const int manual_speed_interval = 104;
bool bulb;
byte grey_text = 0;
const int MAX_PROGRAM = 2;
bool NuevoPrograma;
const int t_min[3] = {250, 250, 250};
const float interval_margin = 1;
unsigned long stop_m_pos_update_time = 0;
byte high_speed = 0;
byte disable_pan = 0;
const byte max_tramos = 21;
long long max_speed_jump = 150;
double Kt = 16000.0;            //Constante relacionado con la conversion de tiempos en velocidad (lineales)
double Kacc = 120.0;            //Constante relacionado con la conversion de tiempos en velocidad (aceleracion)

const unsigned char noxon_logo [] = { 0x01, 0xE0, 0x00, 0x00, 0x1F, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xF0, 0x00, 0x03, 0xF0, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x07, 0xFF, 0x00, 0x00, 0x01, 0xF8, 0x00, 0x03, 0xF8, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x0F, 0xFF, 0x80, 0x00, 0x03, 0xFC, 0x00, 0x03, 0xFC, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x3F, 0xFF, 0x80, 0x00, 0x03, 0xFE, 0x00, 0x03, 0xFE, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x7F, 0xFF, 0x00, 0x00, 0x03, 0xFF, 0x00, 0x03, 0xFF, 0x00, 0x00, 0x3F, 0x80, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x01, 0xFF, 0x80, 0x03, 0xFF, 0x80, 0x00, 0x3F, 0x80, 0x01, 0xFF, 0xFC, 0x00, 0x00, 0x00, 0xFF, 0xC0, 0x03, 0xFF, 0xC0, 0x00, 0x3F, 0x80, 0x01, 0xFF, 0x80, 0x00, 0xF0, 0x00, 0x7F, 0xE0, 0x03, 0xFF, 0xE0, 0x00, 0x3F, 0x80, 0x03, 0xFE, 0x00, 0x01, 0xF8, 0x00, 0x3F, 0xF0, 0x03, 0xFF, 0xF0, 0x00, 0x3F, 0x80, 0x03, 0xFC, 0x00, 0x03, 0xF8, 0x00, 0x1F, 0xF8, 0x03, 0xFF, 0xF8, 0x00, 0x3F, 0x80, 0x07, 0xF8, 0x00, 0x03, 0xFC, 0x00, 0x0F, 0xFC, 0x03, 0xFF, 0xFC, 0x00, 0x3F, 0x80, 0x07, 0xF0, 0x00, 0x03, 0xFC, 0x00, 0x07, 0xFE, 0x03, 0xF7, 0xFE, 0x00, 0x3F, 0x80, 0x0F, 0xF0, 0x00, 0x01, 0xFE, 0x00, 0x03, 0xFF, 0x03, 0xF3, 0xFF, 0x00, 0x3F, 0x80, 0x0F, 0xE0, 0x00, 0x01, 0xFE, 0x00, 0x01, 0xFF, 0x03, 0xF1, 0xFF, 0x80, 0x3F, 0x80, 0x0F, 0xE0, 0x00, 0x00, 0xFE, 0x00, 0x00, 0xFF, 0x03, 0xF0, 0xFF, 0xC0, 0x3F, 0x80, 0x0F, 0xE0, 0x00, 0x00, 0xFE, 0x00, 0x00, 0x7F, 0x03, 0xF0, 0x7F, 0xE0, 0x3F, 0x80, 0x0F, 0xE0, 0x00, 0x00, 0xFE, 0x00, 0x00, 0x3F, 0x03, 0xF0, 0x3F, 0xF0, 0x3F, 0x80, 0x0F, 0xE0, 0x00, 0x00, 0xFE, 0x00, 0x00, 0x1F, 0x03, 0xF0, 0x1F, 0xF8, 0x3F, 0x80, 0x0F, 0xE0, 0x00, 0x00, 0xFE, 0x00, 0x00, 0x3F, 0x03, 0xF0, 0x0F, 0xFC, 0x3F, 0x80, 0x0F, 0xE0, 0x00, 0x00, 0xFE, 0x00, 0x00, 0x7F, 0x03, 0xF0, 0x07, 0xFE, 0x3F, 0x80, 0x0F, 0xE0, 0x00, 0x00, 0xFE, 0x00, 0x00, 0xFF, 0x03, 0xF0, 0x03, 0xFF, 0x3F, 0x80, 0x0F, 0xE0, 0x00, 0x01, 0xFE, 0x00, 0x01, 0xFF, 0x03, 0xF0, 0x01, 0xFF, 0xBF, 0x80, 0x0F, 0xF0, 0x00, 0x01, 0xFC, 0x00, 0x03, 0xFF, 0x03, 0xF0, 0x00, 0xFF, 0xFF, 0x80, 0x07, 0xF8, 0x00, 0x03, 0xFC, 0x00, 0x07, 0xFE, 0x03, 0xF0, 0x00, 0x7F, 0xFF, 0x80, 0x07, 0xF8, 0x00, 0x07, 0xFC, 0x00, 0x0F, 0xFC, 0x03, 0xF0, 0x00, 0x3F, 0xFF, 0x80, 0x03, 0xFC, 0x00, 0x07, 0xF8, 0x00, 0x1F, 0xF8, 0x03, 0xF0, 0x00, 0x1F, 0xFF, 0x80, 0x03, 0xFF, 0x00, 0x1F, 0xF8, 0x00, 0x3F, 0xF0, 0x03, 0xF0, 0x00, 0x0F, 0xFF, 0x80, 0x01, 0xFF, 0xC0, 0x7F, 0xF0, 0x00, 0x7F, 0xE0, 0x03, 0xF0, 0x00, 0x07, 0xFF, 0x80, 0x01, 0xFF, 0xFF, 0xFF, 0xE0, 0x00, 0xFF, 0xC0, 0x03, 0xF0, 0x00, 0x03, 0xFF, 0x80, 0x00, 0xFF, 0xFF, 0xFF, 0xC0, 0x01, 0xFF, 0x80, 0x03, 0xF0, 0x00, 0x01, 0xFF, 0x80, 0x00, 0x7F, 0xFF, 0xFF, 0x80, 0x03, 0xFF, 0x00, 0x03, 0xF0, 0x00, 0x00, 0xFF, 0x80, 0x00, 0x3F, 0xFF, 0xFF, 0x00, 0x03, 0xFE, 0x00, 0x03, 0xF0, 0x00, 0x00, 0x7F, 0x80, 0x00, 0x0F, 0xFF, 0xFE, 0x00, 0x03, 0xFC, 0x00, 0x03, 0xF0, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x03, 0xFF, 0xF8, 0x00, 0x01, 0xF8, 0x00, 0x01, 0xE0, 0x00, 0x00, 0x1F, 0x00, 0x00, 0x00, 0xFF, 0xC0, 0x00, 0x00, 0xF0, 0x00};

unsigned long driver_serial_number = 0;
unsigned long sync_tries = 0;
byte check_wire = 0;
byte numResonancia = 0; //introducir nÃºmeros de resonancia
byte velResonancia[10] = {30, 31, 32, 33, 34, 35, 36, 37, 38, 39,}; //introducir empezando desde la izquierda el nÃºmero de la velocidad en la que hay resonancia
byte incResonancia[10] = {100, 100, 100, 100, 100, 100, 80, 60, 40, 20,}; //introducir empezando desde la izquierda el nÃºmero de la incremento de velocidad (entre 5 y 20 suele ser notable)
byte stopping = 0;

#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF
#define LIGHT_ORANGE 0xBB41
#define COLOR_MSG 0x94B2
#define COLOR_MENU BLACK
#define COLOR_BAR  BLACK
#define COLOR_SELECTED WHITE
#define COLOR_CHOSEN 0xBB41
#define COLOR_HEADING WHITE
#define COLOR_ARROW 0x0000
#define COLOR_BATTERY BLACK
#define COLOR_BATTERY_LEFT BLACK
#define COLOR_LINE GREY
#define GREY 0x52AA
#define MAXENCODERS 1

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

volatile int encstate[MAXENCODERS];
volatile int encflag[MAXENCODERS];
boolean A_set[MAXENCODERS];
boolean B_set[MAXENCODERS];
volatile int16_t encoderpos[MAXENCODERS];
volatile unsigned long  encodertimer = millis(); // acceleration measurement
int encoderpinA[MAXENCODERS] = {49}; // pin array of all encoder A inputs
int encoderpinB[MAXENCODERS] = {50}; // pin array of all encoder B inputs
unsigned int lastEncoderPos[MAXENCODERS];
const byte width_select = 7;
const byte width_heading = 34;
const byte width_space = 2;
const byte width_back = 50;
const byte side_gap = 4;
const byte letter_height = 26;
const byte triang = 4;
const byte triang_pos = 6;
const byte radius = 10;
const byte circle = 8;
const byte logo = 40;
const byte battery_lenght = 50;
const byte battery_height = 6;
const byte battery_gap = 2;
const byte battery_margin = 20;
const byte battery_round = 4;
const byte arrow_height = 6;
const byte arrow_tail = 5;
byte disable_tilt = 0;
int ypos;
bool print_text = 1;
bool tl_preview;
long info_tramos[max_tramos + 1][16];
byte modo;
byte tl_ramp;
byte divs;
byte hours, minutes;
bool first_draw;
const byte return_speed = 38;
unsigned long manual_speed_rate_control = 0;
bool startup = 1;
bool battery_cleared = 0;
bool battery_charging = 0;
bool battery_present = 0;
byte inhibit_first_resync = 1;
char battery_lvl = 'w';
char battery_lvl0 = 'w';
byte battery_lvl_rectangle = 0;
byte battery_lvl_rectangle_draw = 0;
long last_battery_draw = 0;
unsigned int speed2;
long data, instant_read;
byte page, page0;
byte text_size;
bool selected;
bool pos_text[8];
byte ramp_state = 0;
bool zero_mode = 0;
bool tl_power = 0;
volatile int move;
long last_pulsed;
char* words[15];
byte bar_pos;
int oldPosition;
int newPosition;
byte rectangles;
byte manualMode;
byte Click = 0;
byte disable_mux_flag = 1;
const byte time_back_draw = 100;
const byte time_back_wait = 255;
const byte pos_clip[2] = {163, 220};
long min_interval;
const byte pos_dist = 130;
const byte pos_dolly = 175;
unsigned char letter;
bool mode;
long travel_counter, pan_counter, tilt_counter;
byte movements = 3;
long travel_ref_0, travel_ref_1, pan_ref_0, pan_ref_1, tilt_ref_0, tilt_ref_1;;
byte tl_data[6], data_min, data_max;
int tl_distance;
byte distance_number[4];
bool tl_direction;
byte clip_fps = 28;
byte xpos;
byte length;
long stops, stops_done;
const byte height = 4;
const byte separation = 25;
const byte height_bar = 73;
const byte height_letter = 65;
const byte rect_length = 80;
bool manual_state = 0;
long blinking, espera2;
unsigned long last_something;
bool state_blink;
const int time_blink = 500;
byte battery_lvl_rectangle_data[6], battery_lvl_data[6];
bool initialize_battery = 1;
bool initialize_int_battery = 1;
byte missed;
byte going_to_zero = 0;
unsigned int speed;
const byte limit_speed = 40; //40
bool moved;
bool int_length, int_length_0;
byte next;
float factor;
bool pulsed, pulsed_before;
int time_lock = 60000;
bool state_asleep = 1;
bool auto_lock = 0;
byte remote_auto_off = 0;
byte controller_auto_off = 0;
int manual_speed;
byte end2;
byte sequence = 0;
byte counter;
bool repeat;
long long ramp;
bool am_move[6];
long check_EEPROM;
const byte language = 0;
byte delay_start;
bool keep_am = 0;
bool keep_tl = 0;
byte color_led;
int voltage, current;
int battery_lines, battery_lines0;
byte battery_div = 4;
float battery_percentage = 0.96;
float battery_sure;
long long min_ramp, max_speed;
bool errorPan;
byte bar_offset = 0;
byte scrolling_down = 0;
byte virtual_rectangles = 0;
byte d;
bool stopped;
bool first_draw_battery = 1;
bool state_tl;
byte bar_limit;
byte shots_height;
byte text_height;
bool allow_shot;
bool battery_blink;
long last_battery_blink;
bool waiting;
bool dimmed = 0;
byte beep_on = 0;
int exp_ramp_duration, exp_ramp_finish, final_exp, initial_exp;
byte info_paradas[max_tramos];
byte max_speeds[max_tramos];
byte min_speeds[max_tramos];
long long pan_speeds[40];
long long travel_speeds[40];
long long speeds[40][3];
long info_abs_speed[max_tramos][3];
long info_speed_inverse[max_tramos][3];
byte info_master[max_tramos];
byte info_slave[max_tramos];
long long accel_time[max_tramos];
long long info_distancia[max_tramos][3];
byte n_tramos = 2;
char output_msg[70];
byte output_msg_index;
long timeout;
int crashed = 0;
int crashed0 = 0;
int sentido = 1;
int sentido_tmp = 1;
byte low_batt = 0;
unsigned long low_batt_blink_timer = 0;
byte blink_low_batt = 1;

char n_serie[7];

int txt_color = 0;
int millivolts;
int int_battery_lvl0 = 20;
int int_battery_lvl = 20;
byte int_page0;
byte int_battery_cleared = 0;
byte battery_start = 1;
byte charging = 0;
bool charge_blink;
long last_supply_check = 0;
unsigned long battery_disable_timer = 0;

unsigned int tt_interval = 2;
long acc_level = 0;
bool multi_tramo_going;
byte sub_toggle_d = 0;
long velocidades[max_tramos];
long tramos_relativos[max_tramos];
long tramos_relativos_abs[max_tramos];
byte recording = 0;
byte playing = 0;
int ppm_rate = 100;

char distancia_0[2] = "9";
char distancia_1[2] = "9";
char distancia_2[2] = "9";
char distancia_3[2] = "9";

char timespeed_0[2] = "9";
char timespeed_1[2] = "9";
char timespeed_2[2] = "9";
char timespeed_3[2] = "9";
char timespeed_4[2] = "9";


char espera_0[2] = "9";
char espera_1[2] = "9";
char espera_2[2] = "9";
char espera_3[2] = "9";

byte battery_msg_disabled = 0;

long travel_abs, travel_abs_change;
int tramo = 0;
byte offset;

byte program;
long zero;
byte brightness = 8;
byte set_brightness0 = 8;
byte msg_n0 = 0;
byte success_0 = 1;
byte resync = 0;
byte resync2 = 0;
byte scrolling = 0;

byte go_to_zero = 0;
long prev_pos;


// timer
#define ENCODER_RATE 1000    // in microseconds; 
Adafruit_ZeroTimer timer3 = Adafruit_ZeroTimer(3);
void TC3_Handler() {
  Adafruit_ZeroTimer::timerHandler(3);
}

int time[10];
int date[10];
char version[13];
char version_driver[13] = "No presente";
bool reading_tl;
bool startup_led_control = 1;
byte channel = 0x14;
int turntable_shots = 0;
int turntable_angle = 0;
long turntable_travel = 0;
byte turntable_speed = 0;
unsigned long last_checked_wire = 0;
int min_tt_interval = 2;
long stops_when_paused = 0;
int battery_lvl_tmp = 0;
byte not_dimmed = 1;
void setup() {
  init_rf();
  enable_mux();
  tft.setTextWrap(0);
  pinMode(usb_sel, OUTPUT);
  digitalWrite(usb_sel, HIGH);
  pinMode(RF_SET, INPUT);
  pinMode(RF_ENABLE, OUTPUT);
  digitalWrite(RF_ENABLE, HIGH);
  pinMode(READ_5V, INPUT);
  pinMode(VBAT_CONTROL, OUTPUT);
  digitalWrite(VBAT_CONTROL, LOW);
  pinMode(VCHARGE_CONTROL, OUTPUT);
  digitalWrite(VCHARGE_CONTROL, LOW);
  pinMode(V_REF, INPUT);
  pinMode(V_MEASURE, INPUT);
  pinMode(pulse, INPUT_PULLDOWN);
  tft.begin();
  tft.setRotation(0);
  tft.fillScreen(BLACK);
  initEncoders();
  analogWrite(TFT_LED, 100);

  Serial1.begin(115200);
  Serial2.begin(115200);
  initEEPROM();

  startup = 0;
  startup_led_control = 0;
  tft.fillScreen(BLACK);

  Watchdog.enable(4000);
  boot();
}
void loop() {}


void loadEEPROM() {
  switch (program) {
    case 1:
    case 2:
      n_tramos = EEPROMread(84 + (program - 1) * 84);
      if (n_tramos < 0 || n_tramos > 20) {
        n_tramos = 0;
      }
      acc_level = EEPROMread(85 + (program - 1) * 84);
      if (acc_level == 0 || acc_level > 10) {
        acc_level = 1;
      }
      zero = EEPROMreadLong(86 + (program - 1) * 84);
      for (int i = 0; i < max_tramos - 1; i++) {
        info_tramos[i + 1][0] = EEPROMreadLong(88 + i * 2 + (program - 1) * 84);
        info_tramos[i + 1][2] = EEPROMread(128 + i + (program - 1) * 84);
        info_tramos[i + 1][3] = EEPROMread(148 + i + (program - 1) * 84);
      }
      break;
  }
  info_tramos[0][0] = zero;
  for (int i = 0; i < max_tramos - 1; i++) {
    if ( info_tramos[i][0] >= 0 ) {
      info_tramos[i][4] = info_tramos[i][0] / 4000.0 * wheel_factor + 0.1;
      info_tramos[i][5] = 3000;
    }
    else {
      info_tramos[i][4] = info_tramos[i][0] / 4000.0 * wheel_factor - 0.1;
      info_tramos[i][5] = 3000;
    }
  }
}

void updateEEPROM() {
  zero = info_tramos[0][0];
  switch (program) {
    case 1:
    case 2:
      if ( n_tramos != EEPROMread(84 + (program - 1) * 84)) {
        EEPROMwrite(84 + (program - 1) * 84, n_tramos);
      }
      if ( acc_level != EEPROMread(85 + (program - 1) * 84)) {
        EEPROMwrite(85 + (program - 1) * 84, acc_level);
      }
      if ( zero != EEPROMreadLong(86 + (program - 1) * 84)) {
        EEPROMWriteLong(86 + (program - 1) * 84, zero);
      }
      for (int i = 0; i < max_tramos - 1; i++) {
        if ( info_tramos[i + 1][0] != EEPROMreadLong(88 + i * 2 + (program - 1) * 84)) {
          EEPROMWriteLong(88 + i * 2 + (program - 1) * 84, info_tramos[i + 1][0]);
        }
        if ( info_tramos[i + 1][2] != EEPROMread(128 + i + (program - 1) * 84)) {
          EEPROMwrite(128 + i + (program - 1) * 84, info_tramos[i + 1][2]);
        }
        if ( info_tramos[i + 1][3] != EEPROMread(148 + i + (program - 1) * 84)) {
          EEPROMwrite(148 + i + (program - 1) * 84, info_tramos[i + 1][3]);
        }
      }
      break;/*
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
      if(program > 5) {
        EEPROM.PageBase0 = 0x801F000;
        EEPROM.PageBase1 = 0x801F400;
        EEPROM.PageSize  = 0x400;
        EEPROM.init();
      }
      else {
        EEPROM.PageBase0 = 0x801E800;
        EEPROM.PageBase1 = 0x801EC00;
        EEPROM.PageSize  = 0x400;
        EEPROM.init();
      }
      if( n_tramos != EEPROMread((program % 3) * 84)) {
        EEPROMwrite((program % 3) * 84, n_tramos);
      }
      if( acc_level != EEPROMread(1 + (program % 3) * 84)) {
         EEPROMwrite(1 + (program % 3) * 84, acc_level);
      }
      if( zero != EEPROMreadLong(2 + (program % 3) * 84)) {
        EEPROMWriteLong(2 + (program % 3) * 84, zero);
      }
      for(int i = 0; i < max_tramos - 1; i++) {
        if( info_tramos[i + 1][0] != EEPROMreadLong(4 + i * 2 + (program % 3) * 84)) {
          EEPROMWriteLong(4 + i * 2 + (program % 3) * 84, info_tramos[i + 1][0]);
        }
        if( info_tramos[i + 1][2] != EEPROMread(44 + i + (program % 3) * 84)) {
          EEPROMwrite(44 + i + (program % 3) * 84, info_tramos[i + 1][2]);
        }
        if( info_tramos[i + 1][3] != EEPROMread(64 + i + (program % 3) * 84)) {
          EEPROMwrite(64 + i + (program % 3) * 84, info_tramos[i + 1][3]);
        }
      }
      EEPROM.PageBase0 = 0x801F800;
      EEPROM.PageBase1 = 0x801FC00;
      EEPROM.PageSize  = 0x400;
      EEPROM.init();
      break;  */
  }
}

void initEEPROM() {


}

void init_speeds() {
}

void recap() {

}

byte read_char() {
  return (SerialX_read());
}

long read_string() {
  if (SerialX_available()) {
    data = 0;
    instant_read = 0;
    if (SerialX_peek() == '-') {
      SerialX_read();
    }
    while (SerialX_available() && instant_read != ',') {
      instant_read = read_char();
      if (instant_read != ',' && instant_read >= '0' && instant_read <= '9') {
        data = data * 10 + (instant_read - 48);
      }
      delay(5);
    }
    while (SerialX_available() && !reading_tl && page != 12) {
      instant_read = read_char();
      delay(1);
    }
    return (data);
  }
  else {
    return (0);
  }
}


void reset() {

}

void int2string(long integer) {
  if (integer == 0) {
    output_msg[output_msg_index] = '0';
  }
  else {
    if (integer < 0) {
      output_msg[output_msg_index] = '-';
      output_msg_index++;
      integer *= -1;

    }
    long factor = 1000000000;
    while (integer / factor == 0) {
      factor /= 10;
    }
    output_msg[output_msg_index] = (char)(integer / factor + 48);
    while (factor != 1) {
      factor /= 10;
      output_msg_index++;
      output_msg[output_msg_index] = (char)((integer / factor) % 10 + 48);
    }
  }
  output_msg_index++;
  output_msg[output_msg_index] = ',';
}




void powerOnConfirm() {
  tft.fillScreen(WHITE);
  tftsetTextColorWrapper(BLACK);
  tftdrawCentreStringWrap("Turn the knob", 120, 120, 4);
  tft.drawRect(30, 160, 180, 5, BLACK);
}


void boot() {
  intro();
}

void intro() {
  page = 0;
  start_confirmed = 0;
  CMV = 0;
  tft.fillScreen(BLACK);
  tftsetTextColorWrapper(WHITE);
  tft.setTextSize(10);
  tftdrawCentreStringWrap("PUSH TO", 120, 140, 4);
  tftdrawCentreStringWrap("START", 120, 180, 4);
  for (int i = 255; i > 100; i--) {
    analogWrite(TFT_LED, i);
    delayMicroseconds(2000);
  }
  while (PUSHED) {
    updateData();
  }
  delay(40);
  while (NOT_PUSHED) {
    updateData();
  }
  start_confirmed = 1;
  CMV = 1;
  if (SerialBus == Serial1) {
    digitalWrite(usb_sel, LOW);
  }
  SerialBus.begin(115200);
  menu();
}

void stopConfirmation() {

  tft.fillScreen(BLACK);
  tftsetTextColorWrapper(WHITE);
  tftdrawCentreStringWrap("DO YOU WANT", 120, 140, 4);
  tftdrawCentreStringWrap("TO STOP?", 120, 180, 4);
  while (PUSHED) {
    updateData();
  }
  delay(250);
  while (NOT_PUSHED) {
    updateData();
  }
  menu();
}

void EEPROMwrite(int jj, int xx, byte size) {
  EEPROM.write(jj, xx);
}

void enable_mux() {
  pinMode( MUX_ENABLE, OUTPUT );
  digitalWrite( MUX_ENABLE, LOW );
}

void disable_mux() {
  pinMode( MUX_ENABLE, OUTPUT );
  digitalWrite( MUX_ENABLE, HIGH );
}

void tft_led_on() {
  for (int i = 0; i < 255; i++) {
    analogWrite( TFT_LED, 255 - i );
    delay(4);
  }
}

void battery_power_on() {
  if (digitalRead(READ_5V) == 0) {
    powerOnConfirm();
    tft_led_on();
    pinMode(VBAT_CONTROL, OUTPUT);
    digitalWrite(VBAT_CONTROL, LOW);
    long startup_time = millis();
    byte start_bar = 0;
    while (1) {
      if (myEncoderRead() != oldPosition) {
        oldPosition = myEncoderRead();
        startup_time = millis();
        start_bar++;
        tft.fillRect(30, 160, start_bar * 30, 5, BLACK);
        if (start_bar >= 6) {
          break;
        }
      }
      else if ( millis() - startup_time > 1000) {
        digitalWrite(VBAT_CONTROL, LOW);
        digitalWrite(VCHARGE_CONTROL, LOW);
        pinMode(RF_ENABLE, OUTPUT);
        digitalWrite(RF_ENABLE, LOW);
        digitalWrite(TFT_LED, HIGH);
        enable_mux();
        while (1) {}
      }
    }
  }
}
