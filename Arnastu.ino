/*

Urazpi Software
Rev 0.35
2015-12-20

Xabier Garmendia Iztueta
EA2EGA

Change history
Rev 0.34:
  Added output relay support, depending on level
Rev 0.35:
  Menu options simplified
  Auto Set Up at first boot
  Added automatic latch recovery
  

*/

// Libraries
#include <UC1701.h>
#include "U8glib.h"
#include <Indio.h>      //
#include <Wire.h>
//#include <EEPROM.h>

//  Static Vars

#define Serial_Number "UR-000000"
#define Sw_rev "0.37"

#define ingran_logo_width 60
#define ingran_logo_height 64
static unsigned char ingran_logo_bits[] U8G_PROGMEM= {
   0xff, 0xff, 0xff, 0x1f, 0xff, 0xff, 0xff, 0x0f, 0xff, 0xff, 0xff, 0x1f,
   0xfe, 0xff, 0xff, 0x0f, 0xff, 0xff, 0xff, 0x1f, 0xfe, 0xff, 0xff, 0x0f,
   0xff, 0xff, 0xff, 0x1f, 0xfe, 0xff, 0xff, 0x0f, 0xff, 0xff, 0x3f, 0x18,
   0x86, 0xff, 0xff, 0x0f, 0xff, 0xff, 0x1f, 0x18, 0x06, 0xfe, 0xff, 0x0f,
   0xff, 0xff, 0x0f, 0x1c, 0x0e, 0xfc, 0xff, 0x0f, 0xff, 0xff, 0x07, 0x1e,
   0x1e, 0xfc, 0xff, 0x0f, 0xff, 0xff, 0x07, 0x1f, 0x3e, 0xf8, 0xff, 0x0f,
   0xff, 0xff, 0x83, 0x1f, 0x7e, 0xf8, 0xff, 0x0f, 0xff, 0xff, 0x83, 0x1f,
   0x7e, 0xf0, 0xff, 0x0f, 0xff, 0xff, 0x83, 0x3f, 0x7f, 0xf0, 0xff, 0x0f,
   0xff, 0xff, 0x83, 0xff, 0x7f, 0xf8, 0xff, 0x0f, 0xff, 0xff, 0x87, 0xff,
   0x3f, 0xf8, 0xff, 0x0f, 0xff, 0xff, 0x07, 0xff, 0x3f, 0xf8, 0xff, 0x0f,
   0xff, 0xff, 0x0f, 0xfe, 0x0f, 0xfc, 0xff, 0x0f, 0xff, 0xff, 0x1f, 0xf0,
   0x03, 0xfe, 0xff, 0x0f, 0xff, 0xff, 0x3f, 0x00, 0x00, 0xff, 0xff, 0x0f,
   0xff, 0xff, 0x7f, 0x00, 0xc0, 0xff, 0xff, 0x0f, 0xff, 0xff, 0xff, 0x03,
   0xf0, 0xff, 0xff, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x0f,
   0xff, 0xff, 0xfb, 0xff, 0xff, 0xf7, 0xff, 0x0f, 0xff, 0xff, 0xf3, 0xff,
   0xff, 0xf3, 0xff, 0x0f, 0xff, 0xff, 0xc3, 0xff, 0xff, 0xf0, 0xff, 0x0f,
   0xff, 0xff, 0x03, 0xff, 0x3f, 0xf0, 0xff, 0x0f, 0xff, 0xff, 0x03, 0xf0,
   0x03, 0xf0, 0xff, 0x0f, 0xff, 0xff, 0x03, 0x00, 0x00, 0xf0, 0xff, 0x0f,
   0xff, 0xff, 0x03, 0x00, 0x00, 0xf0, 0xff, 0x0f, 0xff, 0xff, 0x03, 0x00,
   0x00, 0xf0, 0xff, 0x0f, 0xff, 0xff, 0x03, 0x00, 0x00, 0xf0, 0xff, 0x0f,
   0xff, 0xff, 0x03, 0x00, 0x00, 0xf0, 0xff, 0x0f, 0xff, 0xff, 0x03, 0x00,
   0x00, 0xf0, 0xff, 0x0f, 0xff, 0xff, 0x03, 0x00, 0x00, 0xf0, 0xff, 0x0f,
   0xff, 0xff, 0x03, 0x00, 0x00, 0xf0, 0xff, 0x0f, 0xff, 0xff, 0x03, 0x00,
   0x00, 0xf0, 0xff, 0x0f, 0xff, 0xff, 0x03, 0x00, 0x00, 0xf0, 0xff, 0x0f,
   0xff, 0xff, 0x03, 0x00, 0x00, 0xf0, 0xff, 0x0f, 0xff, 0xff, 0x03, 0x00,
   0x00, 0xf0, 0xff, 0x0f, 0xff, 0xff, 0x03, 0x00, 0x00, 0xf0, 0xff, 0x0f,
   0xff, 0xff, 0x03, 0x00, 0x00, 0xf0, 0xff, 0x0f, 0xff, 0xff, 0x03, 0x00,
   0x00, 0xf0, 0xff, 0x0f, 0xff, 0xff, 0x03, 0x00, 0x00, 0xf0, 0xff, 0x0f,
   0xff, 0xff, 0x03, 0x00, 0x00, 0xf0, 0xff, 0x0f, 0xff, 0xff, 0x03, 0x00,
   0x00, 0xf0, 0xff, 0x0f, 0xff, 0xff, 0x03, 0x00, 0x00, 0xf0, 0xff, 0x0f,
   0xff, 0xff, 0x03, 0x00, 0x00, 0xf0, 0xff, 0x0f, 0xff, 0xff, 0x03, 0x00,
   0x00, 0xf0, 0xff, 0x0f, 0xff, 0xff, 0x03, 0x00, 0x00, 0xf0, 0xff, 0x0f,
   0xff, 0xff, 0x03, 0x00, 0x00, 0xf0, 0xff, 0x0f, 0xff, 0xff, 0x03, 0x00,
   0x00, 0xf0, 0xff, 0x0f, 0xff, 0xff, 0x03, 0x00, 0x00, 0xf0, 0xff, 0x0f,
   0xff, 0xff, 0x03, 0x00, 0x00, 0xf0, 0xff, 0x0f, 0xff, 0xff, 0x03, 0x00,
   0x00, 0xf0, 0xff, 0x0f, 0xff, 0xff, 0x03, 0x00, 0x00, 0xf0, 0xff, 0x0f,
   0xff, 0xff, 0x03, 0x00, 0x00, 0xf0, 0xff, 0x0f, 0xff, 0xff, 0x03, 0x00,
   0x00, 0xf0, 0xff, 0x0f, 0xff, 0xff, 0x03, 0x00, 0x00, 0xf0, 0xff, 0x0f,
   0xff, 0xff, 0x03, 0x00, 0x00, 0xf0, 0xff, 0x0f, 0xff, 0xff, 0x03, 0x00,
   0x00, 0xf0, 0xff, 0x0f, 0xff, 0xff, 0x03, 0x00, 0x00, 0xf0, 0xff, 0x0f,
   0xff, 0xff, 0x03, 0x00, 0x00, 0xf0, 0xff, 0x0f, 0xff, 0x07, 0x00, 0x00,
   0x00, 0x00, 0xfe, 0x0f, 0x1f, 0x80, 0xff, 0xff, 0xff, 0x1f, 0x80, 0x0f,
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x0f };

   #define ingran_texto_width 128
#define ingran_texto_height 40
static unsigned char ingran_texto_bits[] U8G_PROGMEM= {
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x83, 0x7f, 0xff, 0x3f,
   0xfc, 0x1f, 0x00, 0xfc, 0x03, 0xe0, 0xff, 0xcf, 0xff, 0xe7, 0xff, 0xc7,
   0xc3, 0x7f, 0xfe, 0x7f, 0xfe, 0xc7, 0x7f, 0xf8, 0xc7, 0xc3, 0xff, 0xcf,
   0xff, 0xc7, 0xff, 0xc7, 0xc3, 0x7f, 0xf8, 0x7f, 0xfe, 0xe1, 0xff, 0xfd,
   0xc7, 0x87, 0xff, 0x87, 0xff, 0x87, 0xff, 0xc7, 0xc7, 0x7f, 0xf0, 0x7f,
   0xfe, 0xf0, 0xff, 0xff, 0xc7, 0x8f, 0xff, 0x87, 0xff, 0x07, 0xff, 0xc7,
   0xc7, 0x7f, 0xe0, 0x7f, 0xfe, 0xf8, 0xff, 0xff, 0xc7, 0x8f, 0xff, 0x03,
   0xff, 0x07, 0xfe, 0xc7, 0xc7, 0x7f, 0xc0, 0x7f, 0x7e, 0xf8, 0xff, 0xff,
   0xc7, 0x8f, 0xff, 0x13, 0xff, 0x27, 0xf8, 0xcf, 0xc7, 0x7f, 0x83, 0x7f,
   0x7e, 0xf8, 0xff, 0xff, 0xc7, 0x8f, 0xff, 0x19, 0xfe, 0x67, 0xf0, 0xcf,
   0xc7, 0x7f, 0x0f, 0x7f, 0x7e, 0xfc, 0xff, 0xff, 0xc7, 0xc7, 0xff, 0x39,
   0xfe, 0xe7, 0xe0, 0xcf, 0xc7, 0x7f, 0x1e, 0x7c, 0x7e, 0xfc, 0xff, 0xff,
   0xc7, 0xe7, 0xff, 0x3c, 0xfc, 0xe7, 0xc1, 0xcf, 0xc7, 0x7f, 0x3e, 0x78,
   0x7e, 0xf8, 0x7f, 0xf8, 0x07, 0xf0, 0xff, 0x7c, 0xfc, 0xe7, 0x83, 0xcf,
   0xc7, 0x7f, 0x7e, 0x70, 0x7e, 0xf8, 0x7f, 0xf8, 0xc7, 0xf1, 0x7f, 0x3c,
   0xf8, 0xe7, 0x0f, 0xcf, 0xc7, 0x7f, 0xfe, 0x60, 0x7e, 0xf8, 0x7f, 0xf8,
   0xc7, 0xe3, 0x7f, 0x00, 0xf8, 0xe7, 0x1f, 0xec, 0xc7, 0x7f, 0xfe, 0x41,
   0x7e, 0xf0, 0x7f, 0xf8, 0xc7, 0xc7, 0x3f, 0xfe, 0xf0, 0xe7, 0x3f, 0xe8,
   0xc3, 0x3f, 0xfe, 0x07, 0xfe, 0xf0, 0x7f, 0xf8, 0xc7, 0x87, 0x3f, 0xff,
   0xf0, 0xe7, 0x7f, 0xe0, 0xc3, 0x3f, 0xfe, 0x0f, 0xfe, 0xe1, 0x7f, 0xf8,
   0xc7, 0x0f, 0x1f, 0xff, 0xf1, 0xe7, 0xff, 0xe0, 0xc3, 0x3f, 0xfe, 0x1f,
   0xfe, 0x83, 0x7f, 0xf8, 0xc7, 0x1f, 0x9e, 0xff, 0xe1, 0xe7, 0xff, 0xe1,
   0xc3, 0x3f, 0xfe, 0x3f, 0xfe, 0x07, 0x7e, 0xf8, 0xc3, 0x3f, 0x88, 0xff,
   0xc3, 0xe7, 0xff, 0xe7, 0x83, 0x3f, 0xfc, 0x7f, 0xfe, 0x3f, 0x00, 0xfc,
   0x83, 0xff, 0x80, 0xff, 0x83, 0xc3, 0xff, 0xef, 0xff, 0xff, 0xff, 0xff,
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
   0xff, 0xff, 0xff, 0x81, 0xff, 0xff, 0xf7, 0xff, 0xff, 0xff, 0xbf, 0xff,
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf9, 0xff, 0xff, 0xff, 0xff,
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf9,
   0xc3, 0x87, 0x77, 0xf8, 0xf9, 0xf1, 0xba, 0xe3, 0x87, 0xff, 0xff, 0xff,
   0xff, 0xff, 0xff, 0x81, 0x91, 0x13, 0x77, 0x72, 0xe6, 0x66, 0xb8, 0x81,
   0x13, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf9, 0xb9, 0x39, 0x77, 0x77,
   0x60, 0x60, 0xbe, 0xb9, 0x39, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf9,
   0xb9, 0x39, 0x77, 0x77, 0x7e, 0x7e, 0xbe, 0xb9, 0x39, 0xff, 0xff, 0xff,
   0xff, 0xff, 0xff, 0xf9, 0xb9, 0x33, 0x77, 0x77, 0x7e, 0x7e, 0xbe, 0xb9,
   0x13, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x83, 0xbb, 0x07, 0x77, 0xf7,
   0xf0, 0xf1, 0xbe, 0xbb, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
   0xff, 0x8f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x8f, 0xff, 0xff, 0xff,
   0xff, 0xff, 0xff, 0xff, 0xff, 0xc7, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
   0xc7, 0xff, 0xff, 0xff };


U8GLIB_MINI12864 u8g(21, 20, 19, 22);

const int backlightPin = 26; // PWM output pin that the LED backlight is attached to

// Edit the following long int number
uint32_t number = 1234;

unsigned long time1,last_key_press,last_purge,purge_time,time_temp;

byte last_cycle_key;

/* EEPROM Asignsation
  
  0x00:First boot flag. After first boot, gets 0x01 value
  0x20-0x2F: Analog In Calib Points for calculating Slope and Offset (float)
  0x30:Pipe Level (float)
  0x34:Hloss correction slope (float)
  0x40:Output 1 enabled (byte)
  0x41:Output 1 if higher/lower than
  0x42:Output 1 constant to enable relay, currently not used
  0x43:Latching
  0x44:Wait First
  0x45:Auto Unlatch
  0x48:Output 1 ON Threshold (float)
  0x4C:Output 1 ON Startup Delay in seconds (float)
  0x50:Output 1 OFF Threshold (float)
  0x54:Output 1 OFF Startup Delay in seconds (float)
  0x58:Output 1 OFF Minimum time in seconds (float)
  
*/

const float offset_mA=19, slope_mA=0.005442177; //Constants for RAW ADC to mA 

float offset;
float slope;
float hloss1,hloss2,hloss_corr_slope; //Offset is 0 at Pmax, get from EEPROM 0x2C
float max_pressure; 

float level_corrected,level_corrected_0;

byte out1_en, out1_hi_lo, out1_latching, out1_autounlatch, out1_wait_first;
float out1_threshold, out1_delay,out1_OFF_threshold, out1_OFF_delay, out1_OFF_min_time;
unsigned long out1_threshold_long,out1_delay_long, out1_OFF_delay_long, out1_OFF_min_time_long;

byte out1_status=0, out1_first_cycle;
unsigned long out1_last_low_time;
unsigned long out1_last_ON_condition; //Last time that ON condition was satisfied
unsigned long out1_last_no_OFF_condition; //Last time that OFF condition was not satisfied

unsigned long out1_last_off=0, out1_last_on=0;

byte level_condition, level_OFF_condition,autounlach_condition;

/*FSM statuses
0: Normal
1: Main Menu
2: Calib Menu
21: P Sensor Menu

Install Menu
31: Store 0 Bar point
32: Store Second Point
33: Connect pipe and let settle
34: Turn off flow and let settle
35: Done
6: Calcular Perdidas

7: Load Defaults
8: Store values

40: Edit number and store in number_edit

*/

float indioadc=0;

int install_fsm=0;

int input_float_return_fsm_ok, input_float_return_fsm_skip;
float* input_float;
byte input_float_fsm=11;    //0-9 numbers, 10 OK, 11 Cancel
char number_in[15]="          ";

static byte menu_max_lines=3;
char* mainMenu[]={"Calibrar", "Instalar", "Salida","Entrada", "Info", "Reset", "Guardar","Reset Latch", "Salir"};
byte mainMenu_size=9;

char* calibMenu[]={"Tipo Sensor", "Fluido", "Tubo", "Caudal", "Profundidad", "Reset", "Salir"};
byte calibMenu_size=7;

char* outMenu[]={"Activ: ", "Tipo: ", "Esperar:", "Latch:", "T  ON", "T OFF", "Des-Latch:","Nivel OFF", "Delay OFF", "T_min OFF", "Salir"};
byte outMenu_size=11;

int FSM_status=0;

byte adc_key=0,param1=0,param2=0,menu_start=0;

float tempf01;
float ftemp2;
float pipe_level;

void load_defaults()        //Save default calib values to EEPROM
{
  float ftemp1;
  unsigned long ultemp1;
  byte btemp1;
  ftemp1=754;
  EEPROM.put( 0x20, ftemp1 ); //A_in Cal Points
  ftemp1=0;
  EEPROM.put( 0x24, ftemp1 ); //
  ftemp1=3694;
  EEPROM.put( 0x28, ftemp1 ); //
  ftemp1=61.2;
  EEPROM.put( 0x2C, ftemp1 ); //
  ftemp1=0;
  EEPROM.put( 0x30, ftemp1 ); //Pipe Level (float)
  ftemp1=0;
  EEPROM.put( 0x34, ftemp1 ); //Hloss correction slope (float)
  btemp1=0;
  EEPROM.put( 0x40, btemp1 ); //Output 1 enabled (byte)
  btemp1=0;
  EEPROM.put( 0x41, btemp1 ); //Output 1 if higher/lower than
  btemp1=4;
  EEPROM.put( 0x42, btemp1 ); //Output 1 constant to enable relay, not used currently
  btemp1=0;
  EEPROM.put( 0x43, btemp1 ); //Latching
  btemp1=0;
  EEPROM.put( 0x45, btemp1 ); //Auto Unlatch
  btemp1=0;
  EEPROM.put( 0x44, btemp1 ); //Wait First
  ftemp1=0;
  EEPROM.put( 0x48, ftemp1 ); //Output 1 ON Threshold (float)
  ftemp1=0;
  EEPROM.put( 0x4C, ftemp1 ); //Output 1 ON Startup Delay in seconds (float)
  ftemp1=0;
  EEPROM.put( 0x50, ftemp1 ); //Output 1 OFF Threshold (float)
  ftemp1=0;
  EEPROM.put( 0x54, ftemp1 ); //Output 1 OFF Startup Delay in seconds (float)
  ftemp1=0;
  EEPROM.put( 0x58, ftemp1 ); //Output 1 OFF Minimum time in seconds (float)
  
}

void load_eeprom()          //Load calibration values from EEPROM and calculate offset and slope. 
{
  float t1, t2, t3, t4,ftemp1;

  EEPROM.get( 0x40, out1_en );
  EEPROM.get( 0x41, out1_hi_lo );
  EEPROM.get( 0x43, out1_latching );
  EEPROM.get( 0x45, out1_autounlatch );
  EEPROM.get( 0x44, out1_wait_first  );
  EEPROM.get( 0x48, out1_threshold  );
  EEPROM.get( 0x4C, out1_delay  );
  EEPROM.get( 0x50, out1_OFF_threshold  );
  EEPROM.get( 0x54, out1_OFF_delay  );
  EEPROM.get( 0x58, out1_OFF_min_time);
  ftemp1=out1_delay*1000;
  out1_delay_long=ftemp1;
  ftemp1=out1_threshold*1000;
  out1_threshold_long=ftemp1;
  ftemp1=out1_OFF_delay*1000;
  out1_OFF_delay_long=ftemp1;
  ftemp1=out1_OFF_min_time*1000;
  out1_OFF_min_time_long=ftemp1;
  
  EEPROM.get( 0x20, t1 );
  EEPROM.get( 0x24, t2 );
  EEPROM.get( 0x28, t3 );
  EEPROM.get( 0x2C, t4 );
  max_pressure=t4;
  EEPROM.get( 0x30, pipe_level ); //Load pipe level
  EEPROM.get( 0x34, hloss_corr_slope );
  slope=(t4-t2)/(t3-t1);
  offset=t1-t2/slope;
      
}

void draw(void) {
  if (FSM_status==90)
    print_intro_logo();
  if (FSM_status==91)
    print_intro_texto();
  if (FSM_status==92)
    print_intro_info();
  if (FSM_status==0)
    print_main();
  if (FSM_status==1)
    print_menu();
  if (FSM_status==2)
    calib_menu();
  if (FSM_status==3)
    out_menu();
  if (FSM_status==4)
    raw_menu();
  if (FSM_status==6)
    print_defaults();
  if (FSM_status==7)
    print_store_eprom();
  if (FSM_status==21)
    Psensor_menu();
  if (FSM_status==31)
    print_31();
  if (FSM_status==33)
    print_33();
  if (FSM_status==34)
    print_34();
  if (FSM_status==40||FSM_status==41)
    input_float_menu();
}

void setup(void) {

  byte tempb1;
  EEPROM.get( 0x00, tempb1);
  if (tempb1!=0x01)
  {
    load_defaults();
    tempb1=0x01;
    EEPROM.put( 0x00, tempb1);
  }
  
  Indio.setADCResolution(18); // Set the ADC resolution. Choices are 12bit@240SPS, 14bit@60SPS, 16bit@15SPS and 18bit@3.75SPS.

  Indio.analogReadMode(1, mA_raw);
  Indio.analogReadMode(2, mA_raw);
  Indio.analogReadMode(3, mA_raw);
  Indio.analogReadMode(4, mA_raw);
  Indio.digitalWrite(8,LOW);
  Indio.digitalMode(8,OUTPUT);  // Set CH8 as an output
  Indio.digitalWrite(1,LOW);
  Indio.digitalMode(1,OUTPUT);  // Set CH1 as an output
  Indio.digitalMode(2,INPUT);  // Set CH2 as input
  
  pinMode(23, INPUT); 
  pinMode(24, INPUT); 
  pinMode(25, INPUT); 
  
  //Serial.begin(9600);
  // flip screen, if required
  u8g.setRot180();
  
  time1=millis();
  last_key_press=millis(); 
  last_purge=10000000;
    
  out1_first_cycle=0;
  out1_status=0;

  out1_last_off=millis();
  out1_last_on=millis();
  
  
  load_eeprom();
  
  pinMode(backlightPin, OUTPUT); //set backlight pin to output 
  analogWrite(backlightPin, 0); //convert backlight intesity from a value of 0-5 to a value of 0-255 for PWM.
}

byte read_analog_buttons()
{
  if ((digitalRead(25)==1)&&(digitalRead(24)==1)&&(digitalRead(23)==1)) //No key pressed
  {
    last_cycle_key=0;
    return 0;
  }
  
  if ((digitalRead(25)==0) && (last_cycle_key==0))  //UP
  {
    delay(20);  //Debounce
    last_key_press=millis();
    last_cycle_key=3;
    return 3; 
    
  }
  else if ((digitalRead(24)==0) && (last_cycle_key==0)) //SELECT
  {
    delay(20);
    last_key_press=millis();
    last_cycle_key=1;
    return 1;
      
  }
  else if ((digitalRead(23)==0) && (last_cycle_key==0)) //DOWN
  {
    delay(20);
    last_key_press=millis(); 
    last_cycle_key=2;
    return 2;
  }
  else
  {
    return 0;  // default
  }
}

void next_FSM()
{
  
  
  adc_key = read_analog_buttons();
  
  if (FSM_status!=0)  //Menu watchdog
  {
    time_temp=millis();
    if (time_temp-last_key_press>=100000)
      FSM_status=0;
  }
  
  switch (FSM_status) {
    
  case 80:
    time_temp=millis();
    if (time_temp-purge_time>=700)  //  0.7 second
    {
      FSM_status=0;         //Back to Main
      Indio.digitalWrite(8,LOW);
    }
    break;
  
  case 0:    // We are in Display Screen
      time_temp=millis();
    if (time_temp-last_purge>=86400000)
    {
      FSM_status=80;          //Purge
      last_key_press=time_temp;   //reset last key press counter, otherwhile would activate Menu Watchdog
      last_purge=time_temp;
      purge_time=time_temp;
      Indio.digitalWrite(8,HIGH);
    }
    switch (adc_key) {
    case 1: //Enter pressed
    FSM_status=1; //Enter Main Menu
    param1=1;
    menu_start=1;
    break;
    }
      break;
    
  case 1:    // We are in Main Menu
    switch (adc_key) {
    case 1: //Enter pressed
    switch (param1) {
    case 1:   //Calib
      FSM_status=2; //Enter Calib Menu
      param1=1;
      menu_start=1;
      break;
    case 2:   //Install
      FSM_status=31;  //Enter Install Menu
      install_fsm=11;
      break;
    case 3:   //Out Menu
      FSM_status=3; //Out Menu
      break;
    case 4:   //Raw Inputs
      FSM_status=4; //Show Raw Inputs
      break;
    case 5:   //Info
      time1=millis();
      FSM_status=92;  //Show Raw Inputs
      break;
    case 6:   //Load Defaults
      FSM_status=6; //
      param1=1;
      menu_start=1;
      break;
    case 7:   //Save to EEPROM
      FSM_status=7; //
      param1=1;
      menu_start=1;
      break;
    case 8:   //Exit
       if (out1_latching==1&&((out1_status==1)&&(level_condition==0)))
        out1_status=0;
      break;
    case 9:   //Exit
      FSM_status=0; //Back to Display Screen
      param1=1;
      menu_start=1;
      break;
    }
    break;
    case 2: //Enter pressed
    if (param1<(mainMenu_size))
    {
      param1++;
      if (param1>(menu_start+menu_max_lines))
        menu_start++;
    }
    break;
    case 3: //Enter pressed
    if (param1>1)
    {
      param1--;
      if (param1<menu_start)
        menu_start--;
    }
    break;
    }
      break;
    
    
    case 2:    // We are in Calib Menu
    switch (adc_key) {
    case 1: //Enter pressed
    switch (param1) {
    case 5:   //Edit pipe level
      dtostrf(pipe_level, 0, 2, number_in);
      input_float=&pipe_level;
      input_float_return_fsm_ok=2;
      input_float_return_fsm_skip=2;
      FSM_status=40; //Jump to float editor
      break;
    case 6:   //Load Defaults
      load_defaults();
      break;
    case 7:   //Exit
      FSM_status=1; //Back to Main Menu
      param1=1;
      menu_start=1;
      break;
    }
    break;
    case 2: //Enter pressed
    if (param1<(calibMenu_size))
    {
      param1++;
      if (param1>(menu_start+menu_max_lines))
        menu_start++;
    }
    break;
    case 3: //Enter pressed
    if (param1>1)
    {
      param1--;
      if (param1<menu_start)
        menu_start--;
    }
    break;
    }
      break;
  
  case 3:    // We are in Out Menu
    switch (adc_key) {
    case 1: //Enter pressed
    switch (param1) {
    case 1:   //Activate
      if (out1_en==0)
      {
        out1_en=1;
        EEPROM.put( 0x40, out1_en );
      }
      else
      {
        out1_en=0;
        EEPROM.put( 0x40, out1_en );
      }
      break;
    case 2:   //Out1 Hi-Lo
      if (out1_hi_lo==0)
      {
        out1_hi_lo=1;
        EEPROM.put( 0x41, out1_hi_lo );
      }
      else
      {
        out1_hi_lo=0;
        EEPROM.put( 0x41, out1_hi_lo );
      }
      break;
    case 3:   //Out1 Latch
      if (out1_wait_first==0)
      {
        out1_wait_first=1;
        EEPROM.put( 0x44, out1_wait_first );
      }
      else
      {
        out1_wait_first=0;
        EEPROM.put( 0x44, out1_wait_first );
      }
      break;
    case 4:   //Out1 Latching
      if (out1_latching==0)
      {
        out1_latching=1;
        EEPROM.put( 0x43, out1_latching );
      }
      else
      {
        out1_latching=0;
        EEPROM.put( 0x43, out1_latching );
      }
      break;
    case 5:   //Edit thrershold
      dtostrf(out1_threshold, 0, 2, number_in);
      input_float=&out1_threshold;
      input_float_return_fsm_ok=3051;
      input_float_return_fsm_skip=3;
      FSM_status=40; //Jump to float editor
      break;
    case 6:   //Edit delay
      dtostrf(out1_delay, 0, 2, number_in);
      input_float=&out1_delay;
      input_float_return_fsm_ok=3052;
      input_float_return_fsm_skip=3;
      FSM_status=40; //Jump to float editor
      break;
    case 7:   //Out1 Auto_UnLatching
      if (out1_autounlatch==0)
      {
        out1_autounlatch=1;
        EEPROM.put( 0x45, out1_autounlatch );
      }
      else
      {
        out1_autounlatch=0;
        EEPROM.put( 0x45, out1_autounlatch );
      }
      break;
    case 8:   //Edit OFF thrershold
      dtostrf(out1_OFF_threshold, 0, 2, number_in);
      input_float=&out1_OFF_threshold;
      input_float_return_fsm_ok=3053;
      input_float_return_fsm_skip=3;
      FSM_status=40; //Jump to float editor
      break;
    case 9:   //Edit OFF Delay
      dtostrf(out1_OFF_delay, 0, 2, number_in);
      input_float=&out1_OFF_delay;
      input_float_return_fsm_ok=3054;
      input_float_return_fsm_skip=3;
      FSM_status=40; //Jump to float editor
      break;
    case 10:    //Edit OFF_min_time
      dtostrf(out1_OFF_min_time, 0, 2, number_in);
      input_float=&out1_OFF_min_time;
      input_float_return_fsm_ok=3055;
      input_float_return_fsm_skip=3;
      FSM_status=40; //Jump to float editor
      break;
    case 11:    //Exit
      FSM_status=1; //Back to Main Menu
      param1=1;
      menu_start=1;
      break;
    }
    break;
    case 2: //Enter pressed
    if (param1<(outMenu_size))
    {
      param1++;
      if (param1>(menu_start+menu_max_lines))
        menu_start++;
    }
    break;
    case 3: //Enter pressed
    if (param1>1)
    {
      param1--;
      if (param1<menu_start)
        menu_start--;
    }
    break;
    }
      break;
    
  case 3051:    // Store threshold value in EEPROM
    EEPROM.put( 0x48, out1_threshold );
    FSM_status=3;
    break;
    
  case 3052:    // Store threshold value in EEPROM
    EEPROM.put( 0x4C, out1_delay );
    ftemp2=out1_delay*1000;
    out1_delay_long=ftemp2;
    FSM_status=3;
    break;
    
  case 3053:    // Store threshold value in EEPROM
    EEPROM.put( 0x50, out1_OFF_threshold );
    FSM_status=3;
    break;
    
  case 3054:    // Store threshold value in EEPROM
    EEPROM.put( 0x54, out1_OFF_delay );
    ftemp2=out1_delay*1000;
    out1_delay_long=ftemp2;
    FSM_status=3;
    break;
    
  case 3055:    // Store threshold value in EEPROM
    EEPROM.put( 0x58, out1_OFF_min_time );
    ftemp2=out1_delay*1000;
    out1_delay_long=ftemp2;
    FSM_status=3;
    break;
  
  case 31:    // Install Menu Set 0 Bar
      switch (adc_key) {
    case 1:
      if (install_fsm==10)
      {
        float ftemp10;
        ftemp10=(Indio.analogRead(1));
            EEPROM.put( 0x20, ftemp10 );
        load_eeprom();
      }
      FSM_status=32;  //Jump to case 32
    break;
    case 2: //UP pressed
    if (install_fsm==11)
      install_fsm=10;
    else
      install_fsm=11;
    break;
    case 3: //DOWN pressed
    if (install_fsm==11)
      install_fsm=10;
    else
      install_fsm=11;
    break;
    }
    break;
    
    
    case 32:    // Install Menu Set Second Point
    EEPROM.get( 0x2C, tempf01 );
    dtostrf(tempf01, 0, 2, number_in);
    input_float=&tempf01;
    input_float_return_fsm_ok=321; //Next FSM in return from FSM
    input_float_return_fsm_skip=33; //Next FSM in return from FSM
    FSM_status=40; //Jump to float editor
    break;
    
    case 321:    // Install Menu Set Second Point Store
    EEPROM.put( 0x2C, tempf01 );
    float ftemp10;
    ftemp10=(Indio.analogRead(1));
      EEPROM.put( 0x28, ftemp10 );
    load_eeprom();
    FSM_status=33;
    break;
    
    case 33:    // Connect Pipe and left settle
      switch (adc_key) {
      case 1:
        if (install_fsm==10)
        {
          hloss1=((Indio.analogRead(1)-offset)*slope);
          FSM_status=34;  //Jump to case 34
        }
        else
          FSM_status=0; //Jump to case 34
      break;
      case 2: //UP pressed
      if (install_fsm==11)
        install_fsm=10;
      else
        install_fsm=11;
      break;
      case 3: //DOWN pressed
      if (install_fsm==11)
        install_fsm=10;
      else
        install_fsm=11;
      break;
      }
      break;
    break;
    
    case 34:    // Close reg and left settle
      switch (adc_key) {
      case 1:
        if (install_fsm==10)
        {
          float t4;
          EEPROM.get( 0x2C, t4 ); //Load max pressure
          hloss2=((Indio.analogRead(1)-offset)*slope);
          hloss_corr_slope=(hloss1-hloss2)/(t4-hloss1);
          EEPROM.put( 0x34, hloss_corr_slope ); //Load max pressure         
        }
        FSM_status=0; //Jump to case 32
      break;
      case 2: //UP pressed
      if (install_fsm==11)
        install_fsm=10;
      else
        install_fsm=11;
      break;
      case 3: //DOWN pressed
      if (install_fsm==11)
        install_fsm=10;
      else
        install_fsm=11;
      break;
      }
      break;
    break;
  
  case 4:    // Load DEF
      if (adc_key==1)
    {
      FSM_status=1; //Enter Main Menu
    }
      break;
  
  case 6:    // Load DEF
      load_defaults();
      load_eeprom();
      FSM_status=0; //Enter Main Menu
      break;
    
    
  case 7:    // SAVE to EEPROM
      EEPROM.put( 0x30, pipe_level );
      FSM_status=0; //Enter Main Menu
      break;
    
    case 40:    // Enter a new number
    switch (adc_key) {
    case 1: //Enter pressed
    if (input_float_fsm==11)
        FSM_status=input_float_return_fsm_skip;
    else if (input_float_fsm==10) //OK Pressed
    {
      *input_float=atof(number_in);
      FSM_status=input_float_return_fsm_ok;
    }
    else
      FSM_status=41;
    break;
    case 2: //UP pressed
    if (input_float_fsm==11)
      input_float_fsm=0;
    else
      input_float_fsm++;
    break;
    case 3: //DOWN pressed
    if (input_float_fsm==0)
      input_float_fsm=11;
    else
      input_float_fsm--;
    break;
    } 
    
    break;
    
  case 41:    // Enter a char
    switch (adc_key) {
    case 1: //Enter pressed
      FSM_status=40;
    break;
    case 2: //UP pressed
    if (number_in[input_float_fsm]=='-')
      number_in[input_float_fsm]='9';
    else if (number_in[input_float_fsm]=='.')
    {
      if (input_float_fsm==0)
        number_in[input_float_fsm]='-';
      else
        number_in[input_float_fsm]='9';
    }
    else if (number_in[input_float_fsm]=='0')
      number_in[input_float_fsm]='.';
    else if (number_in[input_float_fsm]>='1'&&number_in[input_float_fsm]<='9')
      number_in[input_float_fsm]--;
    else
      number_in[input_float_fsm]='0';
    break;
    case 3: //DOWN pressed
    if (number_in[input_float_fsm]==' ')
      number_in[input_float_fsm]='0';
    if (number_in[input_float_fsm]=='.')
      number_in[input_float_fsm]='0';
    else if (number_in[input_float_fsm]=='-')
      number_in[input_float_fsm]='.';
    else if (number_in[input_float_fsm]=='9')
    {
      if (input_float_fsm==0)
        number_in[input_float_fsm]='-';
      else
        number_in[input_float_fsm]='.';
    }
    else if (number_in[input_float_fsm]<='8'&&number_in[input_float_fsm]>='0')
      number_in[input_float_fsm]++;
    else
      number_in[input_float_fsm]='0';
      
    break;
    } 
    
    break;
    
    
    
  case 90:    // your hand is nowhere near the sensor
      time_temp=millis();
    if (time_temp-time1>=1500)
    {
      FSM_status=91;  //Enter Main Menu
      time1=time_temp;
    }
      break;
  case 91:    // your hand is nowhere near the sensor
      time_temp=millis();
    if (time_temp-time1>=1500)
    {
      FSM_status=92;  //Enter Main Menu
      time1=time_temp;
    }
  case 92:    // your hand is nowhere near the sensor
      time_temp=millis();
    if (time_temp-time1>=1500)
    {
      FSM_status=0; //Enter Main Menu
      time1=time_temp;
    }
      break;
  } 
  
} 

void print_menu()
{
  u8g.setFont(u8g_font_unifontr);
  u8g.setPrintPos(20, 13);   u8g.print("Menu");
  
  u8g.setFont(u8g_font_unifontr);
  byte i=menu_start;
  while (i<(4+menu_start))
  {
  u8g.setPrintPos(5, 25+(i-menu_start)*11);
  if (param1==(i))
    u8g.print("#");
  else
    u8g.print(" ");
  u8g.print(i);
  u8g.print(".");
  u8g.print(mainMenu[i-1]);
  i++;
  }
  
}

void print_31()
{
  u8g.setFont(u8g_font_ncenR08);
  u8g.setPrintPos(5, 20);   u8g.print("Desconecte el");
  u8g.setPrintPos(5, 30);   u8g.print("tubo de aire");
  
  u8g.setFont(u8g_font_unifontr);
  if (install_fsm==10)
  {
    u8g.setPrintPos(5, 58);   u8g.print(" __");
  }
  if (install_fsm==11)
  {
    u8g.setPrintPos(5, 58);   u8g.print("     ____");
  }
  
  u8g.setPrintPos(5, 55);   u8g.print(" OK  Skip");
  
}

void print_33()

{
  float indioadc;
  
  u8g.setFont(u8g_font_ncenR08);
  u8g.setPrintPos(5, 9);   u8g.print("Conecte el tubo de");
  u8g.setPrintPos(5, 18);   u8g.print("aire y espere a que");
  u8g.setPrintPos(5, 27);   u8g.print("el nivel se estabilice");
  
  indioadc=((Indio.analogRead(1)-offset)*slope);
  
  u8g.setPrintPos(5, 40);   u8g.print("Nivel: "); u8g.print(indioadc,2);
  
  u8g.setFont(u8g_font_unifontr);
  if (install_fsm==10)
  {
    u8g.setPrintPos(5, 58);   u8g.print(" __");
  }
  if (install_fsm==11)
  {
    u8g.setPrintPos(5, 58);   u8g.print("     ____");
  }
  
  u8g.setPrintPos(5, 55);   u8g.print(" OK  Skip");
  
}

void print_34()

{
  float indioadc;
  
  u8g.setFont(u8g_font_ncenR08);
  u8g.setPrintPos(5, 9);   u8g.print("Cierre el regulador");
  u8g.setPrintPos(5, 18);   u8g.print("rojo y espere a que");
  u8g.setPrintPos(5, 27);   u8g.print("el nivel se estabilice");
  
  indioadc=((Indio.analogRead(1)-offset)*slope);
  
  u8g.setPrintPos(5, 40);   u8g.print("Nivel: "); u8g.print(indioadc,2);
  
  u8g.setFont(u8g_font_unifontr);
  if (install_fsm==10)
  {
    u8g.setPrintPos(5, 58);   u8g.print(" __");
  }
  if (install_fsm==11)
  {
    u8g.setPrintPos(5, 58);   u8g.print("     ____");
  }
  
  u8g.setPrintPos(5, 55);   u8g.print(" OK  Skip");
  
}

void input_float_menu()
{
  u8g.setFont(u8g_font_unifontr);
  u8g.setPrintPos(5, 15);   u8g.print("Enter number");
  
  byte j=0;
  if (input_float_fsm<10)
  {
  u8g.setPrintPos(5, 38);
  while (j<10)
  {
    if (j==input_float_fsm)
      u8g.print("_");
    else
      u8g.print(" ");  
    j++;
  }
  }
  u8g.setPrintPos(5, 35);   u8g.print(number_in);
  
  if (input_float_fsm==10)
  {
    u8g.setPrintPos(5, 58);   u8g.print(" __");
  }
  if (input_float_fsm==11)
  {
    u8g.setPrintPos(5, 58);   u8g.print("     ____");
  }
  
  u8g.setPrintPos(5, 55);   u8g.print(" OK  Skip");
  
}

void Psensor_menu()
{
  u8g.setFont(u8g_font_unifontr);
  u8g.setPrintPos(20, 13);   u8g.print("P Sensor");
  
  u8g.setFont(u8g_font_unifontr);
  byte i=menu_start;
  while (i<(4+menu_start))
  {
  u8g.setPrintPos(5, 25+(i-menu_start)*11);
  if (param1==(i))
    u8g.print("#");
  else
    u8g.print(" ");
  u8g.print(i);
  u8g.print(".");
  u8g.print(calibMenu[i-1]);
  i++;
  }
}


void calib_menu()
{
  u8g.setFont(u8g_font_unifontr);
  u8g.setPrintPos(20, 13);   u8g.print("Calib Menu");
  
  u8g.setFont(u8g_font_unifontr);
  byte i=menu_start;
  while (i<(4+menu_start))
  {
  u8g.setPrintPos(5, 25+(i-menu_start)*11);
  if (param1==(i))
    u8g.print("#");
  else
    u8g.print(" ");
  u8g.print(i);
  u8g.print(".");
  u8g.print(calibMenu[i-1]);
  i++;
  }
}

void print_main()
{
  
  u8g.setFont(u8g_font_unifontr);
  u8g.setPrintPos(20, 11);   u8g.print("Arnastu MK1");
  
  u8g.setFont(u8g_font_profont29r);
  u8g.setPrintPos(10, 40);   u8g.print(level_corrected,2);
  
  u8g.setFont(u8g_font_unifontr);
  
  if (level_condition==1)
  {
  u8g.setPrintPos(20, 55);   u8g.print("C");  
  }
  else
  {
  u8g.setPrintPos(20, 55);   u8g.print(" ");   
  }
  
  if (out1_first_cycle==0)
  {
  u8g.setPrintPos(30, 55);   u8g.print("P");  
  }
  else
  {
  u8g.setPrintPos(30, 55);   u8g.print(" ");   
  }
  
  if (out1_status==1)
  {
  u8g.setPrintPos(40, 55);   u8g.print("S");  
  }
  else
  {
  u8g.setPrintPos(40, 55);   u8g.print(" ");   
  }
  if (out1_latching==1&&((out1_status==1)&&(level_condition==0)))
  {
  u8g.setPrintPos(50, 55);   u8g.print("L");  
  }
  else
  {
  u8g.setPrintPos(50, 55);   u8g.print(" ");   
  }
  if (Indio.digitalRead(2)==1)  //Input 2 
  {
  u8g.setPrintPos(60, 55);   u8g.print("2");  
  }
  else
  {
  u8g.setPrintPos(60, 55);   u8g.print(" ");   
  }
}


void print_store_eprom()
{
  u8g.setFont(u8g_font_unifontr);
  u8g.setPrintPos(20, 11);   u8g.print("Values Saved");
}

void print_defaults()
{
  u8g.setFont(u8g_font_unifontr);
  u8g.setPrintPos(20, 11);   u8g.print("Defaults");
  u8g.setPrintPos(20, 20);   u8g.print("Loaded");
}


void raw_menu()
{
  float adc_val=Indio.analogRead(1);
  u8g.setFont(u8g_font_unifontr);
  u8g.setPrintPos(20, 11);   u8g.print("Raw In 1");
  u8g.setPrintPos(1, 25);   u8g.print("ADC: "); u8g.print(adc_val);
  u8g.setPrintPos(1, 36);   u8g.print( "mA: "); u8g.print((adc_val-offset_mA)*slope_mA,3);
}

void print_si_no(byte entrada)
{
  if (entrada==1)
      u8g.print("SI");
  else
    u8g.print("NO");
}

void out_menu()
{
  u8g.setFont(u8g_font_unifontr);
  u8g.setPrintPos(20, 13);   u8g.print("Out 1 Menu");
  
  u8g.setFont(u8g_font_unifontr);
  byte i=menu_start;
  while (i<(4+menu_start))
  {
  u8g.setPrintPos(5, 25+(i-menu_start)*11);
  if (param1==(i))
    u8g.print("#");
  else
    u8g.print(" ");
  u8g.print(i);
  u8g.print(".");
  u8g.print(outMenu[i-1]);
  switch (i){
  case 1:
    print_si_no(out1_en);
    break;
  case 2:
    if (out1_hi_lo==1)
      u8g.print("Mayor");
    else
      u8g.print("Menor");
    break;
  case 3:
    print_si_no(out1_wait_first);
    break;
  case 4:
    print_si_no(out1_latching);
    break;  
  case 7:
    print_si_no(out1_autounlatch);
    break;  
  }
  i++;
  }
}

void print_intro_logo()
{
  u8g.drawBox(0,0,34,64);
  u8g.drawXBMP( 34, 0, ingran_logo_width, ingran_logo_height, ingran_logo_bits);
  u8g.drawBox(94,0,128,64);
}

void print_intro_texto()
{
  u8g.drawBox(0,0,128,12);
  u8g.drawXBMP( 0, 12, ingran_texto_width, ingran_texto_height, ingran_texto_bits);
  u8g.drawBox(0,52,128,64);
}

void print_intro_info()
{
  u8g.setFont(u8g_font_unifontr);
  u8g.setPrintPos(0, 11);   u8g.print("INFO");
  u8g.setPrintPos(0, 22);   u8g.print("S/N: "); u8g.print(Serial_Number);
  u8g.setPrintPos(0, 33);   u8g.print("SW Rev: "); u8g.print(Sw_rev);
  u8g.setPrintPos(0, 44);   u8g.print("hl_c: "); u8g.print(hloss_corr_slope,3);
  out1_last_low_time=millis();
}

void out1_logic()
{
  time_temp=millis();
  if (out1_status==1)
  {

    if ((time_temp - out1_last_off)>out1_threshold*1000){
      out1_status=0;
      out1_last_on=millis();

  }

  }

  else
  {

    if ((time_temp - out1_last_on)>out1_delay_long){
      out1_status=1;
      out1_last_off=millis();

    }
    }
  
  if (out1_status==1)
    Indio.digitalWrite(1,HIGH);
  else
    Indio.digitalWrite(1,LOW);    

}

/*void out1_logic()
{
  
  // Si se cumple la regla establecida para el ON
  if (out1_hi_lo==1)  //Si Mayor que
  {
    if(level_corrected>out1_threshold)
    {
      level_condition=1;
      out1_last_ON_condition=millis();
    }
    else
      level_condition=0;
  }
  else  //Si menor que
  {
    if(level_corrected<out1_threshold)
    {
      level_condition=1;
      out1_last_ON_condition=millis();
    }
    else
      level_condition=0;
  }
  
  // Si se cumple la regla establecida para el OFF
  if (out1_hi_lo==1)  //Si Mayor que
  {
    if(level_corrected<out1_OFF_threshold)
    {
      level_OFF_condition=1;
    }
    else
      level_OFF_condition=0;
  }
  else  //Si menor que
  {
    if(level_corrected>out1_OFF_threshold)
    {
      level_OFF_condition=1;
    }
    else
      level_OFF_condition=0;
  }
  
  if (out1_wait_first==1&&out1_first_cycle==0)  //Si esta activado el sistema de espera
  {
    if (level_condition==0) //Esperar hasta que la condicion no se cumpla
    {
      out1_first_cycle=1; //Activar el flag
      out1_last_low_time=millis();
    }
  }
  else    //Si el sistema de espera está desactivado
    out1_first_cycle=1; //Activar el flag
  
  if (out1_first_cycle==1)
  {
    if (level_OFF_condition==1)
    {
      time_temp=millis();
      if ((time_temp-out1_last_no_OFF_condition>=out1_OFF_delay_long)&&(time_temp-out1_last_ON_condition>=out1_OFF_min_time_long))
      {
        autounlach_condition=1;
      }
      else
      {
        autounlach_condition=0;
      }
    }
    else
    {
      out1_last_no_OFF_condition=millis();
      autounlach_condition=0;
    }
    
    if (level_condition==1)
    {
      time_temp=millis();
      if (time_temp-out1_last_low_time>=out1_delay_long)
      {
        out1_status=1;
      }
      else if (out1_latching!=1)
      {
        out1_status=0;
      }
    }
        
    else
    {
      out1_last_low_time=millis();
      if (out1_latching==1)
      {
        if (Indio.digitalRead(2)==1)  //if unlatching button pressed
        {
          out1_status=0;
        }
        
        if (out1_autounlatch==1 && autounlach_condition==1)   //If autounlatching is enabled and autounlatch condition satisfies
        {
          out1_status=0;
        }
      }
      else
      {
      out1_status=0;
      }
    }
  }
  
  if (out1_status==1&&out1_en)
    Indio.digitalWrite(1,HIGH);
  else
    Indio.digitalWrite(1,LOW);

}
*/

void a_out(){
    
    byte l=0;
    
    Indio.analogWriteMode(l+1, mA);
    float level_out = 4+level_corrected_0*0.2666;
    if (level_out > 20)     // Check 20 mA overflow
        level_out = 20;
    Indio.analogWrite(l+1, level_out, false); 
    
}

// Arduino loop procedure
void loop() {
  
  indioadc=((Indio.analogRead(1)-offset)*slope);
  level_corrected=indioadc-((max_pressure-indioadc)*hloss_corr_slope);
  level_corrected_0=level_corrected;
  level_corrected=level_corrected-pipe_level;
  
  a_out();
  
  // picture loop
  u8g.firstPage();  
  do {
    draw();
  } while( u8g.nextPage() );
  
  // rebuild the picture after some delay
  delay(50);
  
  out1_logic(); //Run Output 1 logic;
  
  next_FSM();
  
}
