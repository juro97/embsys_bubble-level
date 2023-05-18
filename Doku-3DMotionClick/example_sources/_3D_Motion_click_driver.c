#include "_3D_Motion_click_objects.h"
#include "_3D_Motion_click_resources.h"
#include "built_in.h"


// TFT module connections
char TFT_DataPort at LATE;
sbit TFT_RST at LATD7_bit;
sbit TFT_BLED at LATD2_bit;
sbit TFT_RS at LATD9_bit;
sbit TFT_CS at LATD10_bit;
sbit TFT_RD at LATD5_bit;
sbit TFT_WR at LATD4_bit;
char TFT_DataPort_Direction at TRISE;
sbit TFT_RST_Direction at TRISD7_bit;
sbit TFT_BLED_Direction at TRISD2_bit;
sbit TFT_RS_Direction at TRISD9_bit;
sbit TFT_CS_Direction at TRISD10_bit;
sbit TFT_RD_Direction at TRISD5_bit;
sbit TFT_WR_Direction at TRISD4_bit;
// End TFT module connections

// Touch Panel module connections
sbit DriveX_Left at LATB15_bit;
sbit DriveX_Right at LATB3_bit;
sbit DriveY_Up at LATB14_bit;
sbit DriveY_Down at LATB2_bit;
sbit DriveX_Left_Direction at TRISB15_bit;
sbit DriveX_Right_Direction at TRISB3_bit;
sbit DriveY_Up_Direction at TRISB14_bit;
sbit DriveY_Down_Direction at TRISB2_bit;
// End Touch Panel module connections

// Global variables
unsigned int Xcoord, Ycoord;
const ADC_THRESHOLD = 900;
char PenDown;
void *PressedObject;
int PressedObjectType;
unsigned int caption_length, caption_height;
unsigned int display_width, display_height;

int _object_count;
unsigned short object_pressed;
TButton_Round *local_round_button;
TButton_Round *exec_round_button;
int round_button_order;
TLabel *local_label;
TLabel *exec_label;
int label_order;
TCircleButton *local_circle_button;
TCircleButton *exec_circle_button;
int circle_button_order;


void Init_ADC() {
  AD1PCFG = 0x3FF3;
  ADC1_Init();
}
static void InitializeTouchPanel() {
  Init_ADC();
  TFT_Init_ILI9341_8bit(320, 240);

  TP_TFT_Init(320, 240, 3, 14);                                  // Initialize touch panel
  TP_TFT_Set_ADC_Threshold(ADC_THRESHOLD);                              // Set touch panel ADC threshold

  PenDown = 0;
  PressedObject = 0;
  PressedObjectType = -1;
}

void Calibrate() {
  TFT_Set_Pen(CL_WHITE, 3);
  TFT_Set_Font(TFT_defaultFont, CL_WHITE, FO_HORIZONTAL);
  TFT_Write_Text("Touch selected corners for calibration", 50, 80);
  TFT_Line(315, 1, 319, 1);
  TFT_Line(310, 10, 319, 1);
  TFT_Line(319, 5, 319, 1);
  TFT_Write_Text("first here", 230, 20);

  TP_TFT_Calibrate_Min();                      // Calibration of TP minimum
  Delay_ms(500);

  TFT_Set_Pen(CL_BLACK, 3);
  TFT_Set_Font(TFT_defaultFont, CL_BLACK, FO_HORIZONTAL);
  TFT_Line(315, 1, 319, 1);
  TFT_Line(310, 10, 319, 1);
  TFT_Line(319, 5, 319, 1);
  TFT_Write_Text("first here", 230, 20);

  TFT_Set_Pen(CL_WHITE, 3);
  TFT_Set_Font(TFT_defaultFont, CL_WHITE, FO_HORIZONTAL);
  TFT_Line(0, 239, 0, 235);
  TFT_Line(0, 239, 5, 239);
  TFT_Line(0, 239, 10, 230);
  TFT_Write_Text("now here ", 15, 200);

  TP_TFT_Calibrate_Max();                       // Calibration of TP maximum
  Delay_ms(500);
}


/////////////////////////
  TScreen*  CurrentScreen;

  TScreen                SelectSensor;
  TCircleButton          CircleButton1;
char CircleButton1_Caption[10] = "Raw Accel";

  TCircleButton          CircleButton2;
char CircleButton2_Caption[9] = "Raw Gyro";

  TCircleButton          CircleButton3;
char CircleButton3_Caption[11] = "Raw Magnet";

  TCircleButton          CircleButton4;
char CircleButton4_Caption[6] = "Accel";

  TCircleButton          CircleButton5;
char CircleButton5_Caption[8] = "Compass";

  TCircleButton          CircleButton6;
char CircleButton6_Caption[5] = "Gyro";

  TCircleButton          CircleButton7;
char CircleButton7_Caption[13] = "Inclinometer";

  TCircleButton          CircleButton8;
char CircleButton8_Caption[12] = "Orientation";

  TLabel                 Label2;
char Label2_Caption[21] = "3D MOTION click demo";

  TLine                  Line1;
  TLine                  Line2;
  TLabel                 * const code Screen1_Labels[1]=
         {
         &Label2               
         };
  TCircleButton          * const code Screen1_CircleButtons[8]=
         {
         &CircleButton1,       
         &CircleButton2,       
         &CircleButton3,       
         &CircleButton4,       
         &CircleButton5,       
         &CircleButton6,       
         &CircleButton7,       
         &CircleButton8        
         };
  TLine                  * const code Screen1_Lines[2]=
         {
         &Line1,               
         &Line2                
         };

  TScreen                RawAccel;
  TLabel                 Label3;
char Label3_Caption[23] = "Raw Accelerometer Data";

  TLabel                 X_Axis_Label;
char X_Axis_Label_Caption[4] = "X :";

  TLabel                 Y_Axis_Label;
char Y_Axis_Label_Caption[4] = "Y :";

  TLabel                 Z_Axis_Label;
char Z_Axis_Label_Caption[4] = "Z :";

  TLabel                 X_Axis_Data;
char X_Axis_Data_Caption[11] = " ";

  TLabel                 Y_Axis_Data;
char Y_Axis_Data_Caption[11] = " ";

  TLabel                 Z_Axis_Data;
char Z_Axis_Data_Caption[11] = " ";

  TButton_Round          Back;
char Back_Caption[5] = "Back";

  TButton_Round          * const code Screen2_Buttons_Round[1]=
         {
         &Back                 
         };
  TLabel                 * const code Screen2_Labels[7]=
         {
         &Label3,              
         &X_Axis_Label,        
         &Y_Axis_Label,        
         &Z_Axis_Label,        
         &X_Axis_Data,         
         &Y_Axis_Data,         
         &Z_Axis_Data          
         };

  TScreen                RawGyro;
  TLabel                 Label1;
char Label1_Caption[19] = "Raw Gyroscope Data";

  TLabel                 Label4;
char Label4_Caption[4] = "X :";

  TLabel                 Label5;
char Label5_Caption[4] = "Y :";

  TLabel                 Label6;
char Label6_Caption[4] = "Z :";

  TLabel                 Label7;
char Label7_Caption[11] = " ";

  TLabel                 Label8;
char Label8_Caption[11] = " ";

  TLabel                 Label9;
char Label9_Caption[11] = " ";

  TButton_Round          ButtonRound1;
char ButtonRound1_Caption[5] = "Back";

  TButton_Round          * const code Screen3_Buttons_Round[1]=
         {
         &ButtonRound1         
         };
  TLabel                 * const code Screen3_Labels[7]=
         {
         &Label1,              
         &Label4,              
         &Label5,              
         &Label6,              
         &Label7,              
         &Label8,              
         &Label9               
         };

  TScreen                RawMagnet;
  TLabel                 Label10;
char Label10_Caption[22] = "Raw Magnetometer Data";

  TLabel                 Label11;
char Label11_Caption[4] = "X :";

  TLabel                 Label12;
char Label12_Caption[4] = "Y :";

  TLabel                 Label13;
char Label13_Caption[4] = "Z :";

  TLabel                 Label14;
char Label14_Caption[11] = " ";

  TLabel                 Label15;
char Label15_Caption[11] = " ";

  TLabel                 Label16;
char Label16_Caption[11] = " ";

  TButton_Round          ButtonRound2;
char ButtonRound2_Caption[5] = "Back";

  TButton_Round          * const code Screen4_Buttons_Round[1]=
         {
         &ButtonRound2         
         };
  TLabel                 * const code Screen4_Labels[7]=
         {
         &Label10,             
         &Label11,             
         &Label12,             
         &Label13,             
         &Label14,             
         &Label15,             
         &Label16              
         };

  TScreen                Accel;
  TLabel                 Label17;
char Label17_Caption[19] = "Accelerometer Data";

  TLabel                 Label18;
char Label18_Caption[4] = "X :";

  TLabel                 Label19;
char Label19_Caption[4] = "Y :";

  TLabel                 Label20;
char Label20_Caption[4] = "Z :";

  TLabel                 Label21;
char Label21_Caption[11] = " ";

  TLabel                 Label22;
char Label22_Caption[11] = " ";

  TLabel                 Label23;
char Label23_Caption[11] = " ";

  TButton_Round          ButtonRound3;
char ButtonRound3_Caption[5] = "Back";

  TButton_Round          * const code Screen5_Buttons_Round[1]=
         {
         &ButtonRound3         
         };
  TLabel                 * const code Screen5_Labels[7]=
         {
         &Label17,             
         &Label18,             
         &Label19,             
         &Label20,             
         &Label21,             
         &Label22,             
         &Label23              
         };

  TScreen                Compass;
  TLabel                 Label24;
char Label24_Caption[13] = "Compass Data";

  TLabel                 Label25;
char Label25_Caption[4] = "X :";

  TLabel                 Label28;
char Label28_Caption[11] = " ";

  TButton_Round          ButtonRound4;
char ButtonRound4_Caption[5] = "Back";

  TButton_Round          * const code Screen6_Buttons_Round[1]=
         {
         &ButtonRound4         
         };
  TLabel                 * const code Screen6_Labels[3]=
         {
         &Label24,             
         &Label25,             
         &Label28              
         };

  TScreen                Gyro;
  TLabel                 Label31;
char Label31_Caption[15] = "Gyroscope Data";

  TLabel                 Label32;
char Label32_Caption[4] = "X :";

  TLabel                 Label33;
char Label33_Caption[4] = "Y :";

  TLabel                 Label34;
char Label34_Caption[4] = "Z :";

  TLabel                 Label35;
char Label35_Caption[11] = " ";

  TLabel                 Label36;
char Label36_Caption[11] = " ";

  TLabel                 Label37;
char Label37_Caption[11] = " ";

  TButton_Round          ButtonRound5;
char ButtonRound5_Caption[5] = "Back";

  TButton_Round          * const code Screen7_Buttons_Round[1]=
         {
         &ButtonRound5         
         };
  TLabel                 * const code Screen7_Labels[7]=
         {
         &Label31,             
         &Label32,             
         &Label33,             
         &Label34,             
         &Label35,             
         &Label36,             
         &Label37              
         };

  TScreen                Inclinometer;
  TLabel                 Label38;
char Label38_Caption[18] = "Inclinometer Data";

  TLabel                 Label39;
char Label39_Caption[4] = "X :";

  TLabel                 Label40;
char Label40_Caption[4] = "Y :";

  TLabel                 Label41;
char Label41_Caption[4] = "Z :";

  TLabel                 Label42;
char Label42_Caption[11] = " ";

  TLabel                 Label43;
char Label43_Caption[11] = " ";

  TLabel                 Label44;
char Label44_Caption[11] = " ";

  TButton_Round          ButtonRound6;
char ButtonRound6_Caption[5] = "Back";

  TButton_Round          * const code Screen8_Buttons_Round[1]=
         {
         &ButtonRound6         
         };
  TLabel                 * const code Screen8_Labels[7]=
         {
         &Label38,             
         &Label39,             
         &Label40,             
         &Label41,             
         &Label42,             
         &Label43,             
         &Label44              
         };

  TScreen                Orientation;
  TLabel                 Label45;
char Label45_Caption[17] = "Orientation Data";

  TLabel                 Label46;
char Label46_Caption[4] = "X :";

  TLabel                 Label47;
char Label47_Caption[4] = "Y :";

  TLabel                 Label48;
char Label48_Caption[4] = "Z :";

  TLabel                 Label49;
char Label49_Caption[11] = " ";

  TLabel                 Label50;
char Label50_Caption[11] = " ";

  TLabel                 Label51;
char Label51_Caption[11] = " ";

  TButton_Round          ButtonRound7;
char ButtonRound7_Caption[5] = "Back";

  TLabel                 Label26;
char Label26_Caption[4] = "W :";

  TLabel                 Label27;
char Label27_Caption[11] = " ";

  TButton_Round          * const code Screen9_Buttons_Round[1]=
         {
         &ButtonRound7         
         };
  TLabel                 * const code Screen9_Labels[9]=
         {
         &Label45,             
         &Label46,             
         &Label47,             
         &Label48,             
         &Label49,             
         &Label50,             
         &Label51,             
         &Label26,             
         &Label27              
         };



static void InitializeObjects() {
  SelectSensor.Color                     = 0xFFFF;
  SelectSensor.Width                     = 320;
  SelectSensor.Height                    = 240;
  SelectSensor.Buttons_RoundCount        = 0;
  SelectSensor.LabelsCount               = 1;
  SelectSensor.Labels                    = Screen1_Labels;
  SelectSensor.CircleButtonsCount        = 8;
  SelectSensor.CircleButtons             = Screen1_CircleButtons;
  SelectSensor.LinesCount                = 2;
  SelectSensor.Lines                     = Screen1_Lines;
  SelectSensor.ObjectsCount              = 11;

  RawAccel.Color                     = 0xFFFF;
  RawAccel.Width                     = 320;
  RawAccel.Height                    = 240;
  RawAccel.Buttons_RoundCount        = 1;
  RawAccel.Buttons_Round             = Screen2_Buttons_Round;
  RawAccel.LabelsCount               = 7;
  RawAccel.Labels                    = Screen2_Labels;
  RawAccel.CircleButtonsCount        = 0;
  RawAccel.LinesCount                = 0;
  RawAccel.ObjectsCount              = 8;

  RawGyro.Color                     = 0xFFFF;
  RawGyro.Width                     = 320;
  RawGyro.Height                    = 240;
  RawGyro.Buttons_RoundCount        = 1;
  RawGyro.Buttons_Round             = Screen3_Buttons_Round;
  RawGyro.LabelsCount               = 7;
  RawGyro.Labels                    = Screen3_Labels;
  RawGyro.CircleButtonsCount        = 0;
  RawGyro.LinesCount                = 0;
  RawGyro.ObjectsCount              = 8;

  RawMagnet.Color                     = 0xFFFF;
  RawMagnet.Width                     = 320;
  RawMagnet.Height                    = 240;
  RawMagnet.Buttons_RoundCount        = 1;
  RawMagnet.Buttons_Round             = Screen4_Buttons_Round;
  RawMagnet.LabelsCount               = 7;
  RawMagnet.Labels                    = Screen4_Labels;
  RawMagnet.CircleButtonsCount        = 0;
  RawMagnet.LinesCount                = 0;
  RawMagnet.ObjectsCount              = 8;

  Accel.Color                     = 0xFFFF;
  Accel.Width                     = 320;
  Accel.Height                    = 240;
  Accel.Buttons_RoundCount        = 1;
  Accel.Buttons_Round             = Screen5_Buttons_Round;
  Accel.LabelsCount               = 7;
  Accel.Labels                    = Screen5_Labels;
  Accel.CircleButtonsCount        = 0;
  Accel.LinesCount                = 0;
  Accel.ObjectsCount              = 8;

  Compass.Color                     = 0xFFFF;
  Compass.Width                     = 320;
  Compass.Height                    = 240;
  Compass.Buttons_RoundCount        = 1;
  Compass.Buttons_Round             = Screen6_Buttons_Round;
  Compass.LabelsCount               = 3;
  Compass.Labels                    = Screen6_Labels;
  Compass.CircleButtonsCount        = 0;
  Compass.LinesCount                = 0;
  Compass.ObjectsCount              = 4;

  Gyro.Color                     = 0xFFFF;
  Gyro.Width                     = 320;
  Gyro.Height                    = 240;
  Gyro.Buttons_RoundCount        = 1;
  Gyro.Buttons_Round             = Screen7_Buttons_Round;
  Gyro.LabelsCount               = 7;
  Gyro.Labels                    = Screen7_Labels;
  Gyro.CircleButtonsCount        = 0;
  Gyro.LinesCount                = 0;
  Gyro.ObjectsCount              = 8;

  Inclinometer.Color                     = 0xFFFF;
  Inclinometer.Width                     = 320;
  Inclinometer.Height                    = 240;
  Inclinometer.Buttons_RoundCount        = 1;
  Inclinometer.Buttons_Round             = Screen8_Buttons_Round;
  Inclinometer.LabelsCount               = 7;
  Inclinometer.Labels                    = Screen8_Labels;
  Inclinometer.CircleButtonsCount        = 0;
  Inclinometer.LinesCount                = 0;
  Inclinometer.ObjectsCount              = 8;

  Orientation.Color                     = 0xFFFF;
  Orientation.Width                     = 320;
  Orientation.Height                    = 240;
  Orientation.Buttons_RoundCount        = 1;
  Orientation.Buttons_Round             = Screen9_Buttons_Round;
  Orientation.LabelsCount               = 9;
  Orientation.Labels                    = Screen9_Labels;
  Orientation.CircleButtonsCount        = 0;
  Orientation.LinesCount                = 0;
  Orientation.ObjectsCount              = 10;


  CircleButton1.OwnerScreen     = &SelectSensor;
  CircleButton1.Order           = 0;
  CircleButton1.Left            = 7;
  CircleButton1.Top             = 58;
  CircleButton1.Radius          = 34;
  CircleButton1.Pen_Width       = 1;
  CircleButton1.Pen_Color       = 0x0000;
  CircleButton1.Visible         = 1;
  CircleButton1.Active          = 1;
  CircleButton1.Transparent     = 1;
  CircleButton1.Caption         = CircleButton1_Caption;
  CircleButton1.TextAlign       = _taCenter;
  CircleButton1.FontName        = Tahoma11x13_Regular;
  CircleButton1.PressColEnabled = 1;
  CircleButton1.Font_Color      = 0x0000;
  CircleButton1.Gradient        = 1;
  CircleButton1.Gradient_Orientation = 0;
  CircleButton1.Gradient_Start_Color = 0xFFFF;
  CircleButton1.Gradient_End_Color = 0xC618;
  CircleButton1.Color           = 0xC618;
  CircleButton1.Press_Color     = 0xE71C;
  CircleButton1.OnUpPtr         = 0;
  CircleButton1.OnDownPtr       = 0;
  CircleButton1.OnClickPtr      = CircleButton1OnClick;
  CircleButton1.OnPressPtr      = 0;

  CircleButton2.OwnerScreen     = &SelectSensor;
  CircleButton2.Order           = 1;
  CircleButton2.Left            = 86;
  CircleButton2.Top             = 58;
  CircleButton2.Radius          = 34;
  CircleButton2.Pen_Width       = 1;
  CircleButton2.Pen_Color       = 0x0000;
  CircleButton2.Visible         = 1;
  CircleButton2.Active          = 1;
  CircleButton2.Transparent     = 1;
  CircleButton2.Caption         = CircleButton2_Caption;
  CircleButton2.TextAlign       = _taCenter;
  CircleButton2.FontName        = Tahoma11x13_Regular;
  CircleButton2.PressColEnabled = 1;
  CircleButton2.Font_Color      = 0x0000;
  CircleButton2.Gradient        = 1;
  CircleButton2.Gradient_Orientation = 0;
  CircleButton2.Gradient_Start_Color = 0xFFFF;
  CircleButton2.Gradient_End_Color = 0xC618;
  CircleButton2.Color           = 0xC618;
  CircleButton2.Press_Color     = 0xE71C;
  CircleButton2.OnUpPtr         = 0;
  CircleButton2.OnDownPtr       = 0;
  CircleButton2.OnClickPtr      = CircleButton2OnClick;
  CircleButton2.OnPressPtr      = 0;

  CircleButton3.OwnerScreen     = &SelectSensor;
  CircleButton3.Order           = 2;
  CircleButton3.Left            = 165;
  CircleButton3.Top             = 58;
  CircleButton3.Radius          = 34;
  CircleButton3.Pen_Width       = 1;
  CircleButton3.Pen_Color       = 0x0000;
  CircleButton3.Visible         = 1;
  CircleButton3.Active          = 1;
  CircleButton3.Transparent     = 1;
  CircleButton3.Caption         = CircleButton3_Caption;
  CircleButton3.TextAlign       = _taCenter;
  CircleButton3.FontName        = Tahoma11x13_Regular;
  CircleButton3.PressColEnabled = 1;
  CircleButton3.Font_Color      = 0x0000;
  CircleButton3.Gradient        = 1;
  CircleButton3.Gradient_Orientation = 0;
  CircleButton3.Gradient_Start_Color = 0xFFFF;
  CircleButton3.Gradient_End_Color = 0xC618;
  CircleButton3.Color           = 0xC618;
  CircleButton3.Press_Color     = 0xE71C;
  CircleButton3.OnUpPtr         = 0;
  CircleButton3.OnDownPtr       = 0;
  CircleButton3.OnClickPtr      = CircleButton3OnClick;
  CircleButton3.OnPressPtr      = 0;

  CircleButton4.OwnerScreen     = &SelectSensor;
  CircleButton4.Order           = 3;
  CircleButton4.Left            = 244;
  CircleButton4.Top             = 58;
  CircleButton4.Radius          = 34;
  CircleButton4.Pen_Width       = 1;
  CircleButton4.Pen_Color       = 0x0000;
  CircleButton4.Visible         = 1;
  CircleButton4.Active          = 1;
  CircleButton4.Transparent     = 1;
  CircleButton4.Caption         = CircleButton4_Caption;
  CircleButton4.TextAlign       = _taCenter;
  CircleButton4.FontName        = Tahoma11x13_Regular;
  CircleButton4.PressColEnabled = 1;
  CircleButton4.Font_Color      = 0x0000;
  CircleButton4.Gradient        = 1;
  CircleButton4.Gradient_Orientation = 0;
  CircleButton4.Gradient_Start_Color = 0xFFFF;
  CircleButton4.Gradient_End_Color = 0xC618;
  CircleButton4.Color           = 0xC618;
  CircleButton4.Press_Color     = 0xE71C;
  CircleButton4.OnUpPtr         = 0;
  CircleButton4.OnDownPtr       = 0;
  CircleButton4.OnClickPtr      = CircleButton4OnClick;
  CircleButton4.OnPressPtr      = 0;

  CircleButton5.OwnerScreen     = &SelectSensor;
  CircleButton5.Order           = 4;
  CircleButton5.Left            = 7;
  CircleButton5.Top             = 142;
  CircleButton5.Radius          = 34;
  CircleButton5.Pen_Width       = 1;
  CircleButton5.Pen_Color       = 0x0000;
  CircleButton5.Visible         = 1;
  CircleButton5.Active          = 1;
  CircleButton5.Transparent     = 1;
  CircleButton5.Caption         = CircleButton5_Caption;
  CircleButton5.TextAlign       = _taCenter;
  CircleButton5.FontName        = Tahoma11x13_Regular;
  CircleButton5.PressColEnabled = 1;
  CircleButton5.Font_Color      = 0x0000;
  CircleButton5.Gradient        = 1;
  CircleButton5.Gradient_Orientation = 0;
  CircleButton5.Gradient_Start_Color = 0xFFFF;
  CircleButton5.Gradient_End_Color = 0xC618;
  CircleButton5.Color           = 0xC618;
  CircleButton5.Press_Color     = 0xE71C;
  CircleButton5.OnUpPtr         = 0;
  CircleButton5.OnDownPtr       = 0;
  CircleButton5.OnClickPtr      = CircleButton5OnClick;
  CircleButton5.OnPressPtr      = 0;

  CircleButton6.OwnerScreen     = &SelectSensor;
  CircleButton6.Order           = 5;
  CircleButton6.Left            = 86;
  CircleButton6.Top             = 142;
  CircleButton6.Radius          = 34;
  CircleButton6.Pen_Width       = 1;
  CircleButton6.Pen_Color       = 0x0000;
  CircleButton6.Visible         = 1;
  CircleButton6.Active          = 1;
  CircleButton6.Transparent     = 1;
  CircleButton6.Caption         = CircleButton6_Caption;
  CircleButton6.TextAlign       = _taCenter;
  CircleButton6.FontName        = Tahoma11x13_Regular;
  CircleButton6.PressColEnabled = 1;
  CircleButton6.Font_Color      = 0x0000;
  CircleButton6.Gradient        = 1;
  CircleButton6.Gradient_Orientation = 0;
  CircleButton6.Gradient_Start_Color = 0xFFFF;
  CircleButton6.Gradient_End_Color = 0xC618;
  CircleButton6.Color           = 0xC618;
  CircleButton6.Press_Color     = 0xE71C;
  CircleButton6.OnUpPtr         = 0;
  CircleButton6.OnDownPtr       = 0;
  CircleButton6.OnClickPtr      = CircleButton6OnClick;
  CircleButton6.OnPressPtr      = 0;

  CircleButton7.OwnerScreen     = &SelectSensor;
  CircleButton7.Order           = 6;
  CircleButton7.Left            = 165;
  CircleButton7.Top             = 142;
  CircleButton7.Radius          = 34;
  CircleButton7.Pen_Width       = 1;
  CircleButton7.Pen_Color       = 0x0000;
  CircleButton7.Visible         = 1;
  CircleButton7.Active          = 1;
  CircleButton7.Transparent     = 1;
  CircleButton7.Caption         = CircleButton7_Caption;
  CircleButton7.TextAlign       = _taCenter;
  CircleButton7.FontName        = Tahoma11x13_Regular;
  CircleButton7.PressColEnabled = 1;
  CircleButton7.Font_Color      = 0x0000;
  CircleButton7.Gradient        = 1;
  CircleButton7.Gradient_Orientation = 0;
  CircleButton7.Gradient_Start_Color = 0xFFFF;
  CircleButton7.Gradient_End_Color = 0xC618;
  CircleButton7.Color           = 0xC618;
  CircleButton7.Press_Color     = 0xE71C;
  CircleButton7.OnUpPtr         = 0;
  CircleButton7.OnDownPtr       = 0;
  CircleButton7.OnClickPtr      = CircleButton7OnClick;
  CircleButton7.OnPressPtr      = 0;

  CircleButton8.OwnerScreen     = &SelectSensor;
  CircleButton8.Order           = 7;
  CircleButton8.Left            = 244;
  CircleButton8.Top             = 142;
  CircleButton8.Radius          = 34;
  CircleButton8.Pen_Width       = 1;
  CircleButton8.Pen_Color       = 0x0000;
  CircleButton8.Visible         = 1;
  CircleButton8.Active          = 1;
  CircleButton8.Transparent     = 1;
  CircleButton8.Caption         = CircleButton8_Caption;
  CircleButton8.TextAlign       = _taCenter;
  CircleButton8.FontName        = Tahoma11x13_Regular;
  CircleButton8.PressColEnabled = 1;
  CircleButton8.Font_Color      = 0x0000;
  CircleButton8.Gradient        = 1;
  CircleButton8.Gradient_Orientation = 0;
  CircleButton8.Gradient_Start_Color = 0xFFFF;
  CircleButton8.Gradient_End_Color = 0xC618;
  CircleButton8.Color           = 0xC618;
  CircleButton8.Press_Color     = 0xE71C;
  CircleButton8.OnUpPtr         = 0;
  CircleButton8.OnDownPtr       = 0;
  CircleButton8.OnClickPtr      = CircleButton8OnClick;
  CircleButton8.OnPressPtr      = 0;

  Label2.OwnerScreen     = &SelectSensor;
  Label2.Order           = 8;
  Label2.Left            = 53;
  Label2.Top             = 14;
  Label2.Width           = 227;
  Label2.Height          = 25;
  Label2.Visible         = 1;
  Label2.Active          = 1;
  Label2.Caption         = Label2_Caption;
  Label2.FontName        = HandelGothic_BT21x22_Regular;
  Label2.Font_Color      = 0xF800;
  Label2.OnUpPtr         = 0;
  Label2.OnDownPtr       = 0;
  Label2.OnClickPtr      = 0;
  Label2.OnPressPtr      = 0;

  Line1.OwnerScreen     = &SelectSensor;
  Line1.Order           = 9;
  Line1.First_Point_X   = 20;
  Line1.First_Point_Y   = 46;
  Line1.Second_Point_X  = 300;
  Line1.Second_Point_Y  = 46;
  Line1.Visible         = 1;
  Line1.Pen_Width       = 1;
  Line1.Color           = 0x0000;

  Line2.OwnerScreen     = &SelectSensor;
  Line2.Order           = 10;
  Line2.First_Point_X   = 20;
  Line2.First_Point_Y   = 220;
  Line2.Second_Point_X  = 300;
  Line2.Second_Point_Y  = 220;
  Line2.Visible         = 1;
  Line2.Pen_Width       = 1;
  Line2.Color           = 0x0000;

  Label3.OwnerScreen     = &RawAccel;
  Label3.Order           = 0;
  Label3.Left            = 15;
  Label3.Top             = 40;
  Label3.Width           = 302;
  Label3.Height          = 36;
  Label3.Visible         = 1;
  Label3.Active          = 1;
  Label3.Caption         = Label3_Caption;
  Label3.FontName        = Tahoma27x33_Regular;
  Label3.Font_Color      = 0x0000;
  Label3.OnUpPtr         = 0;
  Label3.OnDownPtr       = 0;
  Label3.OnClickPtr      = 0;
  Label3.OnPressPtr      = 0;

  X_Axis_Label.OwnerScreen     = &RawAccel;
  X_Axis_Label.Order           = 1;
  X_Axis_Label.Left            = 110;
  X_Axis_Label.Top             = 101;
  X_Axis_Label.Width           = 27;
  X_Axis_Label.Height          = 28;
  X_Axis_Label.Visible         = 1;
  X_Axis_Label.Active          = 1;
  X_Axis_Label.Caption         = X_Axis_Label_Caption;
  X_Axis_Label.FontName        = Tahoma21x25_Regular;
  X_Axis_Label.Font_Color      = 0xF800;
  X_Axis_Label.OnUpPtr         = 0;
  X_Axis_Label.OnDownPtr       = 0;
  X_Axis_Label.OnClickPtr      = 0;
  X_Axis_Label.OnPressPtr      = 0;

  Y_Axis_Label.OwnerScreen     = &RawAccel;
  Y_Axis_Label.Order           = 2;
  Y_Axis_Label.Left            = 110;
  Y_Axis_Label.Top             = 124;
  Y_Axis_Label.Width           = 27;
  Y_Axis_Label.Height          = 28;
  Y_Axis_Label.Visible         = 1;
  Y_Axis_Label.Active          = 1;
  Y_Axis_Label.Caption         = Y_Axis_Label_Caption;
  Y_Axis_Label.FontName        = Tahoma21x25_Regular;
  Y_Axis_Label.Font_Color      = 0x0400;
  Y_Axis_Label.OnUpPtr         = 0;
  Y_Axis_Label.OnDownPtr       = 0;
  Y_Axis_Label.OnClickPtr      = 0;
  Y_Axis_Label.OnPressPtr      = 0;

  Z_Axis_Label.OwnerScreen     = &RawAccel;
  Z_Axis_Label.Order           = 3;
  Z_Axis_Label.Left            = 110;
  Z_Axis_Label.Top             = 146;
  Z_Axis_Label.Width           = 26;
  Z_Axis_Label.Height          = 28;
  Z_Axis_Label.Visible         = 1;
  Z_Axis_Label.Active          = 1;
  Z_Axis_Label.Caption         = Z_Axis_Label_Caption;
  Z_Axis_Label.FontName        = Tahoma21x25_Regular;
  Z_Axis_Label.Font_Color      = 0x001F;
  Z_Axis_Label.OnUpPtr         = 0;
  Z_Axis_Label.OnDownPtr       = 0;
  Z_Axis_Label.OnClickPtr      = 0;
  Z_Axis_Label.OnPressPtr      = 0;

  X_Axis_Data.OwnerScreen     = &RawAccel;
  X_Axis_Data.Order           = 4;
  X_Axis_Data.Left            = 149;
  X_Axis_Data.Top             = 100;
  X_Axis_Data.Width           = 0;
  X_Axis_Data.Height          = 28;
  X_Axis_Data.Visible         = 1;
  X_Axis_Data.Active          = 1;
  X_Axis_Data.Caption         = X_Axis_Data_Caption;
  X_Axis_Data.FontName        = Tahoma21x25_Regular;
  X_Axis_Data.Font_Color      = 0xF800;
  X_Axis_Data.OnUpPtr         = 0;
  X_Axis_Data.OnDownPtr       = 0;
  X_Axis_Data.OnClickPtr      = 0;
  X_Axis_Data.OnPressPtr      = 0;

  Y_Axis_Data.OwnerScreen     = &RawAccel;
  Y_Axis_Data.Order           = 5;
  Y_Axis_Data.Left            = 149;
  Y_Axis_Data.Top             = 124;
  Y_Axis_Data.Width           = 0;
  Y_Axis_Data.Height          = 28;
  Y_Axis_Data.Visible         = 1;
  Y_Axis_Data.Active          = 1;
  Y_Axis_Data.Caption         = Y_Axis_Data_Caption;
  Y_Axis_Data.FontName        = Tahoma21x25_Regular;
  Y_Axis_Data.Font_Color      = 0x0400;
  Y_Axis_Data.OnUpPtr         = 0;
  Y_Axis_Data.OnDownPtr       = 0;
  Y_Axis_Data.OnClickPtr      = 0;
  Y_Axis_Data.OnPressPtr      = 0;

  Z_Axis_Data.OwnerScreen     = &RawAccel;
  Z_Axis_Data.Order           = 6;
  Z_Axis_Data.Left            = 149;
  Z_Axis_Data.Top             = 146;
  Z_Axis_Data.Width           = 0;
  Z_Axis_Data.Height          = 28;
  Z_Axis_Data.Visible         = 1;
  Z_Axis_Data.Active          = 1;
  Z_Axis_Data.Caption         = Z_Axis_Data_Caption;
  Z_Axis_Data.FontName        = Tahoma21x25_Regular;
  Z_Axis_Data.Font_Color      = 0x001F;
  Z_Axis_Data.OnUpPtr         = 0;
  Z_Axis_Data.OnDownPtr       = 0;
  Z_Axis_Data.OnClickPtr      = 0;
  Z_Axis_Data.OnPressPtr      = 0;

  Back.OwnerScreen     = &RawAccel;
  Back.Order           = 7;
  Back.Left            = 266;
  Back.Top             = 4;
  Back.Width           = 50;
  Back.Height          = 25;
  Back.Pen_Width       = 1;
  Back.Pen_Color       = 0x0000;
  Back.Visible         = 1;
  Back.Active          = 1;
  Back.Transparent     = 1;
  Back.Caption         = Back_Caption;
  Back.TextAlign       = _taCenter;
  Back.FontName        = Tahoma11x13_Regular;
  Back.PressColEnabled = 1;
  Back.Font_Color      = 0x0000;
  Back.Gradient        = 1;
  Back.Gradient_Orientation = 0;
  Back.Gradient_Start_Color = 0xFFFF;
  Back.Gradient_End_Color = 0xC618;
  Back.Color           = 0xC618;
  Back.Press_Color     = 0xE71C;
  Back.Corner_Radius   = 3;
  Back.OnUpPtr         = 0;
  Back.OnDownPtr       = 0;
  Back.OnClickPtr      = BackOnClick;
  Back.OnPressPtr      = 0;

  Label1.OwnerScreen     = &RawGyro;
  Label1.Order           = 0;
  Label1.Left            = 40;
  Label1.Top             = 40;
  Label1.Width           = 255;
  Label1.Height          = 36;
  Label1.Visible         = 1;
  Label1.Active          = 1;
  Label1.Caption         = Label1_Caption;
  Label1.FontName        = Tahoma27x33_Regular;
  Label1.Font_Color      = 0x0000;
  Label1.OnUpPtr         = 0;
  Label1.OnDownPtr       = 0;
  Label1.OnClickPtr      = 0;
  Label1.OnPressPtr      = 0;

  Label4.OwnerScreen     = &RawGyro;
  Label4.Order           = 1;
  Label4.Left            = 110;
  Label4.Top             = 101;
  Label4.Width           = 27;
  Label4.Height          = 28;
  Label4.Visible         = 1;
  Label4.Active          = 1;
  Label4.Caption         = Label4_Caption;
  Label4.FontName        = Tahoma21x25_Regular;
  Label4.Font_Color      = 0xF800;
  Label4.OnUpPtr         = 0;
  Label4.OnDownPtr       = 0;
  Label4.OnClickPtr      = 0;
  Label4.OnPressPtr      = 0;

  Label5.OwnerScreen     = &RawGyro;
  Label5.Order           = 2;
  Label5.Left            = 110;
  Label5.Top             = 124;
  Label5.Width           = 27;
  Label5.Height          = 28;
  Label5.Visible         = 1;
  Label5.Active          = 1;
  Label5.Caption         = Label5_Caption;
  Label5.FontName        = Tahoma21x25_Regular;
  Label5.Font_Color      = 0x0400;
  Label5.OnUpPtr         = 0;
  Label5.OnDownPtr       = 0;
  Label5.OnClickPtr      = 0;
  Label5.OnPressPtr      = 0;

  Label6.OwnerScreen     = &RawGyro;
  Label6.Order           = 3;
  Label6.Left            = 110;
  Label6.Top             = 146;
  Label6.Width           = 26;
  Label6.Height          = 28;
  Label6.Visible         = 1;
  Label6.Active          = 1;
  Label6.Caption         = Label6_Caption;
  Label6.FontName        = Tahoma21x25_Regular;
  Label6.Font_Color      = 0x001F;
  Label6.OnUpPtr         = 0;
  Label6.OnDownPtr       = 0;
  Label6.OnClickPtr      = 0;
  Label6.OnPressPtr      = 0;

  Label7.OwnerScreen     = &RawGyro;
  Label7.Order           = 4;
  Label7.Left            = 149;
  Label7.Top             = 100;
  Label7.Width           = 50;
  Label7.Height          = 28;
  Label7.Visible         = 1;
  Label7.Active          = 1;
  Label7.Caption         = Label7_Caption;
  Label7.FontName        = Tahoma21x25_Regular;
  Label7.Font_Color      = 0xF800;
  Label7.OnUpPtr         = 0;
  Label7.OnDownPtr       = 0;
  Label7.OnClickPtr      = 0;
  Label7.OnPressPtr      = 0;

  Label8.OwnerScreen     = &RawGyro;
  Label8.Order           = 5;
  Label8.Left            = 149;
  Label8.Top             = 124;
  Label8.Width           = 50;
  Label8.Height          = 28;
  Label8.Visible         = 1;
  Label8.Active          = 1;
  Label8.Caption         = Label8_Caption;
  Label8.FontName        = Tahoma21x25_Regular;
  Label8.Font_Color      = 0x0400;
  Label8.OnUpPtr         = 0;
  Label8.OnDownPtr       = 0;
  Label8.OnClickPtr      = 0;
  Label8.OnPressPtr      = 0;

  Label9.OwnerScreen     = &RawGyro;
  Label9.Order           = 6;
  Label9.Left            = 149;
  Label9.Top             = 146;
  Label9.Width           = 50;
  Label9.Height          = 28;
  Label9.Visible         = 1;
  Label9.Active          = 1;
  Label9.Caption         = Label9_Caption;
  Label9.FontName        = Tahoma21x25_Regular;
  Label9.Font_Color      = 0x001F;
  Label9.OnUpPtr         = 0;
  Label9.OnDownPtr       = 0;
  Label9.OnClickPtr      = 0;
  Label9.OnPressPtr      = 0;

  ButtonRound1.OwnerScreen     = &RawGyro;
  ButtonRound1.Order           = 7;
  ButtonRound1.Left            = 266;
  ButtonRound1.Top             = 4;
  ButtonRound1.Width           = 50;
  ButtonRound1.Height          = 25;
  ButtonRound1.Pen_Width       = 1;
  ButtonRound1.Pen_Color       = 0x0000;
  ButtonRound1.Visible         = 1;
  ButtonRound1.Active          = 1;
  ButtonRound1.Transparent     = 1;
  ButtonRound1.Caption         = ButtonRound1_Caption;
  ButtonRound1.TextAlign       = _taCenter;
  ButtonRound1.FontName        = Tahoma11x13_Regular;
  ButtonRound1.PressColEnabled = 1;
  ButtonRound1.Font_Color      = 0x0000;
  ButtonRound1.Gradient        = 1;
  ButtonRound1.Gradient_Orientation = 0;
  ButtonRound1.Gradient_Start_Color = 0xFFFF;
  ButtonRound1.Gradient_End_Color = 0xC618;
  ButtonRound1.Color           = 0xC618;
  ButtonRound1.Press_Color     = 0xE71C;
  ButtonRound1.Corner_Radius   = 3;
  ButtonRound1.OnUpPtr         = 0;
  ButtonRound1.OnDownPtr       = 0;
  ButtonRound1.OnClickPtr      = BackOnClick;
  ButtonRound1.OnPressPtr      = 0;

  Label10.OwnerScreen     = &RawMagnet;
  Label10.Order           = 0;
  Label10.Left            = 13;
  Label10.Top             = 40;
  Label10.Width           = 305;
  Label10.Height          = 36;
  Label10.Visible         = 1;
  Label10.Active          = 1;
  Label10.Caption         = Label10_Caption;
  Label10.FontName        = Tahoma27x33_Regular;
  Label10.Font_Color      = 0x0000;
  Label10.OnUpPtr         = 0;
  Label10.OnDownPtr       = 0;
  Label10.OnClickPtr      = 0;
  Label10.OnPressPtr      = 0;

  Label11.OwnerScreen     = &RawMagnet;
  Label11.Order           = 1;
  Label11.Left            = 110;
  Label11.Top             = 101;
  Label11.Width           = 27;
  Label11.Height          = 28;
  Label11.Visible         = 1;
  Label11.Active          = 1;
  Label11.Caption         = Label11_Caption;
  Label11.FontName        = Tahoma21x25_Regular;
  Label11.Font_Color      = 0xF800;
  Label11.OnUpPtr         = 0;
  Label11.OnDownPtr       = 0;
  Label11.OnClickPtr      = 0;
  Label11.OnPressPtr      = 0;

  Label12.OwnerScreen     = &RawMagnet;
  Label12.Order           = 2;
  Label12.Left            = 110;
  Label12.Top             = 124;
  Label12.Width           = 27;
  Label12.Height          = 28;
  Label12.Visible         = 1;
  Label12.Active          = 1;
  Label12.Caption         = Label12_Caption;
  Label12.FontName        = Tahoma21x25_Regular;
  Label12.Font_Color      = 0x0400;
  Label12.OnUpPtr         = 0;
  Label12.OnDownPtr       = 0;
  Label12.OnClickPtr      = 0;
  Label12.OnPressPtr      = 0;

  Label13.OwnerScreen     = &RawMagnet;
  Label13.Order           = 3;
  Label13.Left            = 110;
  Label13.Top             = 146;
  Label13.Width           = 26;
  Label13.Height          = 28;
  Label13.Visible         = 1;
  Label13.Active          = 1;
  Label13.Caption         = Label13_Caption;
  Label13.FontName        = Tahoma21x25_Regular;
  Label13.Font_Color      = 0x001F;
  Label13.OnUpPtr         = 0;
  Label13.OnDownPtr       = 0;
  Label13.OnClickPtr      = 0;
  Label13.OnPressPtr      = 0;

  Label14.OwnerScreen     = &RawMagnet;
  Label14.Order           = 4;
  Label14.Left            = 155;
  Label14.Top             = 100;
  Label14.Width           = 50;
  Label14.Height          = 28;
  Label14.Visible         = 1;
  Label14.Active          = 1;
  Label14.Caption         = Label14_Caption;
  Label14.FontName        = Tahoma21x25_Regular;
  Label14.Font_Color      = 0xF800;
  Label14.OnUpPtr         = 0;
  Label14.OnDownPtr       = 0;
  Label14.OnClickPtr      = 0;
  Label14.OnPressPtr      = 0;

  Label15.OwnerScreen     = &RawMagnet;
  Label15.Order           = 5;
  Label15.Left            = 149;
  Label15.Top             = 124;
  Label15.Width           = 50;
  Label15.Height          = 28;
  Label15.Visible         = 1;
  Label15.Active          = 1;
  Label15.Caption         = Label15_Caption;
  Label15.FontName        = Tahoma21x25_Regular;
  Label15.Font_Color      = 0x0400;
  Label15.OnUpPtr         = 0;
  Label15.OnDownPtr       = 0;
  Label15.OnClickPtr      = 0;
  Label15.OnPressPtr      = 0;

  Label16.OwnerScreen     = &RawMagnet;
  Label16.Order           = 6;
  Label16.Left            = 149;
  Label16.Top             = 146;
  Label16.Width           = 50;
  Label16.Height          = 28;
  Label16.Visible         = 1;
  Label16.Active          = 1;
  Label16.Caption         = Label16_Caption;
  Label16.FontName        = Tahoma21x25_Regular;
  Label16.Font_Color      = 0x001F;
  Label16.OnUpPtr         = 0;
  Label16.OnDownPtr       = 0;
  Label16.OnClickPtr      = 0;
  Label16.OnPressPtr      = 0;

  ButtonRound2.OwnerScreen     = &RawMagnet;
  ButtonRound2.Order           = 7;
  ButtonRound2.Left            = 266;
  ButtonRound2.Top             = 4;
  ButtonRound2.Width           = 50;
  ButtonRound2.Height          = 25;
  ButtonRound2.Pen_Width       = 1;
  ButtonRound2.Pen_Color       = 0x0000;
  ButtonRound2.Visible         = 1;
  ButtonRound2.Active          = 1;
  ButtonRound2.Transparent     = 1;
  ButtonRound2.Caption         = ButtonRound2_Caption;
  ButtonRound2.TextAlign       = _taCenter;
  ButtonRound2.FontName        = Tahoma11x13_Regular;
  ButtonRound2.PressColEnabled = 1;
  ButtonRound2.Font_Color      = 0x0000;
  ButtonRound2.Gradient        = 1;
  ButtonRound2.Gradient_Orientation = 0;
  ButtonRound2.Gradient_Start_Color = 0xFFFF;
  ButtonRound2.Gradient_End_Color = 0xC618;
  ButtonRound2.Color           = 0xC618;
  ButtonRound2.Press_Color     = 0xE71C;
  ButtonRound2.Corner_Radius   = 3;
  ButtonRound2.OnUpPtr         = 0;
  ButtonRound2.OnDownPtr       = 0;
  ButtonRound2.OnClickPtr      = BackOnClick;
  ButtonRound2.OnPressPtr      = 0;

  Label17.OwnerScreen     = &Accel;
  Label17.Order           = 0;
  Label17.Left            = 40;
  Label17.Top             = 40;
  Label17.Width           = 241;
  Label17.Height          = 36;
  Label17.Visible         = 1;
  Label17.Active          = 1;
  Label17.Caption         = Label17_Caption;
  Label17.FontName        = Tahoma27x33_Regular;
  Label17.Font_Color      = 0x0000;
  Label17.OnUpPtr         = 0;
  Label17.OnDownPtr       = 0;
  Label17.OnClickPtr      = 0;
  Label17.OnPressPtr      = 0;

  Label18.OwnerScreen     = &Accel;
  Label18.Order           = 1;
  Label18.Left            = 110;
  Label18.Top             = 101;
  Label18.Width           = 27;
  Label18.Height          = 28;
  Label18.Visible         = 1;
  Label18.Active          = 1;
  Label18.Caption         = Label18_Caption;
  Label18.FontName        = Tahoma21x25_Regular;
  Label18.Font_Color      = 0xF800;
  Label18.OnUpPtr         = 0;
  Label18.OnDownPtr       = 0;
  Label18.OnClickPtr      = 0;
  Label18.OnPressPtr      = 0;

  Label19.OwnerScreen     = &Accel;
  Label19.Order           = 2;
  Label19.Left            = 110;
  Label19.Top             = 124;
  Label19.Width           = 27;
  Label19.Height          = 28;
  Label19.Visible         = 1;
  Label19.Active          = 1;
  Label19.Caption         = Label19_Caption;
  Label19.FontName        = Tahoma21x25_Regular;
  Label19.Font_Color      = 0x0400;
  Label19.OnUpPtr         = 0;
  Label19.OnDownPtr       = 0;
  Label19.OnClickPtr      = 0;
  Label19.OnPressPtr      = 0;

  Label20.OwnerScreen     = &Accel;
  Label20.Order           = 3;
  Label20.Left            = 110;
  Label20.Top             = 146;
  Label20.Width           = 26;
  Label20.Height          = 28;
  Label20.Visible         = 1;
  Label20.Active          = 1;
  Label20.Caption         = Label20_Caption;
  Label20.FontName        = Tahoma21x25_Regular;
  Label20.Font_Color      = 0x001F;
  Label20.OnUpPtr         = 0;
  Label20.OnDownPtr       = 0;
  Label20.OnClickPtr      = 0;
  Label20.OnPressPtr      = 0;

  Label21.OwnerScreen     = &Accel;
  Label21.Order           = 4;
  Label21.Left            = 149;
  Label21.Top             = 100;
  Label21.Width           = 50;
  Label21.Height          = 28;
  Label21.Visible         = 1;
  Label21.Active          = 1;
  Label21.Caption         = Label21_Caption;
  Label21.FontName        = Tahoma21x25_Regular;
  Label21.Font_Color      = 0xF800;
  Label21.OnUpPtr         = 0;
  Label21.OnDownPtr       = 0;
  Label21.OnClickPtr      = 0;
  Label21.OnPressPtr      = 0;

  Label22.OwnerScreen     = &Accel;
  Label22.Order           = 5;
  Label22.Left            = 149;
  Label22.Top             = 124;
  Label22.Width           = 50;
  Label22.Height          = 28;
  Label22.Visible         = 1;
  Label22.Active          = 1;
  Label22.Caption         = Label22_Caption;
  Label22.FontName        = Tahoma21x25_Regular;
  Label22.Font_Color      = 0x0400;
  Label22.OnUpPtr         = 0;
  Label22.OnDownPtr       = 0;
  Label22.OnClickPtr      = 0;
  Label22.OnPressPtr      = 0;

  Label23.OwnerScreen     = &Accel;
  Label23.Order           = 6;
  Label23.Left            = 149;
  Label23.Top             = 146;
  Label23.Width           = 50;
  Label23.Height          = 28;
  Label23.Visible         = 1;
  Label23.Active          = 1;
  Label23.Caption         = Label23_Caption;
  Label23.FontName        = Tahoma21x25_Regular;
  Label23.Font_Color      = 0x001F;
  Label23.OnUpPtr         = 0;
  Label23.OnDownPtr       = 0;
  Label23.OnClickPtr      = 0;
  Label23.OnPressPtr      = 0;

  ButtonRound3.OwnerScreen     = &Accel;
  ButtonRound3.Order           = 7;
  ButtonRound3.Left            = 266;
  ButtonRound3.Top             = 4;
  ButtonRound3.Width           = 50;
  ButtonRound3.Height          = 25;
  ButtonRound3.Pen_Width       = 1;
  ButtonRound3.Pen_Color       = 0x0000;
  ButtonRound3.Visible         = 1;
  ButtonRound3.Active          = 1;
  ButtonRound3.Transparent     = 1;
  ButtonRound3.Caption         = ButtonRound3_Caption;
  ButtonRound3.TextAlign       = _taCenter;
  ButtonRound3.FontName        = Tahoma11x13_Regular;
  ButtonRound3.PressColEnabled = 1;
  ButtonRound3.Font_Color      = 0x0000;
  ButtonRound3.Gradient        = 1;
  ButtonRound3.Gradient_Orientation = 0;
  ButtonRound3.Gradient_Start_Color = 0xFFFF;
  ButtonRound3.Gradient_End_Color = 0xC618;
  ButtonRound3.Color           = 0xC618;
  ButtonRound3.Press_Color     = 0xE71C;
  ButtonRound3.Corner_Radius   = 3;
  ButtonRound3.OnUpPtr         = 0;
  ButtonRound3.OnDownPtr       = 0;
  ButtonRound3.OnClickPtr      = BackOnClick;
  ButtonRound3.OnPressPtr      = 0;

  Label24.OwnerScreen     = &Compass;
  Label24.Order           = 0;
  Label24.Left            = 70;
  Label24.Top             = 40;
  Label24.Width           = 176;
  Label24.Height          = 36;
  Label24.Visible         = 1;
  Label24.Active          = 1;
  Label24.Caption         = Label24_Caption;
  Label24.FontName        = Tahoma27x33_Regular;
  Label24.Font_Color      = 0x0000;
  Label24.OnUpPtr         = 0;
  Label24.OnDownPtr       = 0;
  Label24.OnClickPtr      = 0;
  Label24.OnPressPtr      = 0;

  Label25.OwnerScreen     = &Compass;
  Label25.Order           = 1;
  Label25.Left            = 110;
  Label25.Top             = 101;
  Label25.Width           = 27;
  Label25.Height          = 28;
  Label25.Visible         = 1;
  Label25.Active          = 1;
  Label25.Caption         = Label25_Caption;
  Label25.FontName        = Tahoma21x25_Regular;
  Label25.Font_Color      = 0xF800;
  Label25.OnUpPtr         = 0;
  Label25.OnDownPtr       = 0;
  Label25.OnClickPtr      = 0;
  Label25.OnPressPtr      = 0;

  Label28.OwnerScreen     = &Compass;
  Label28.Order           = 2;
  Label28.Left            = 149;
  Label28.Top             = 100;
  Label28.Width           = 50;
  Label28.Height          = 28;
  Label28.Visible         = 1;
  Label28.Active          = 1;
  Label28.Caption         = Label28_Caption;
  Label28.FontName        = Tahoma21x25_Regular;
  Label28.Font_Color      = 0xF800;
  Label28.OnUpPtr         = 0;
  Label28.OnDownPtr       = 0;
  Label28.OnClickPtr      = 0;
  Label28.OnPressPtr      = 0;

  ButtonRound4.OwnerScreen     = &Compass;
  ButtonRound4.Order           = 3;
  ButtonRound4.Left            = 266;
  ButtonRound4.Top             = 4;
  ButtonRound4.Width           = 50;
  ButtonRound4.Height          = 25;
  ButtonRound4.Pen_Width       = 1;
  ButtonRound4.Pen_Color       = 0x0000;
  ButtonRound4.Visible         = 1;
  ButtonRound4.Active          = 1;
  ButtonRound4.Transparent     = 1;
  ButtonRound4.Caption         = ButtonRound4_Caption;
  ButtonRound4.TextAlign       = _taCenter;
  ButtonRound4.FontName        = Tahoma11x13_Regular;
  ButtonRound4.PressColEnabled = 1;
  ButtonRound4.Font_Color      = 0x0000;
  ButtonRound4.Gradient        = 1;
  ButtonRound4.Gradient_Orientation = 0;
  ButtonRound4.Gradient_Start_Color = 0xFFFF;
  ButtonRound4.Gradient_End_Color = 0xC618;
  ButtonRound4.Color           = 0xC618;
  ButtonRound4.Press_Color     = 0xE71C;
  ButtonRound4.Corner_Radius   = 3;
  ButtonRound4.OnUpPtr         = 0;
  ButtonRound4.OnDownPtr       = 0;
  ButtonRound4.OnClickPtr      = BackOnClick;
  ButtonRound4.OnPressPtr      = 0;

  Label31.OwnerScreen     = &Gyro;
  Label31.Order           = 0;
  Label31.Left            = 50;
  Label31.Top             = 40;
  Label31.Width           = 194;
  Label31.Height          = 36;
  Label31.Visible         = 1;
  Label31.Active          = 1;
  Label31.Caption         = Label31_Caption;
  Label31.FontName        = Tahoma27x33_Regular;
  Label31.Font_Color      = 0x0000;
  Label31.OnUpPtr         = 0;
  Label31.OnDownPtr       = 0;
  Label31.OnClickPtr      = 0;
  Label31.OnPressPtr      = 0;

  Label32.OwnerScreen     = &Gyro;
  Label32.Order           = 1;
  Label32.Left            = 110;
  Label32.Top             = 101;
  Label32.Width           = 27;
  Label32.Height          = 28;
  Label32.Visible         = 1;
  Label32.Active          = 1;
  Label32.Caption         = Label32_Caption;
  Label32.FontName        = Tahoma21x25_Regular;
  Label32.Font_Color      = 0xF800;
  Label32.OnUpPtr         = 0;
  Label32.OnDownPtr       = 0;
  Label32.OnClickPtr      = 0;
  Label32.OnPressPtr      = 0;

  Label33.OwnerScreen     = &Gyro;
  Label33.Order           = 2;
  Label33.Left            = 110;
  Label33.Top             = 124;
  Label33.Width           = 27;
  Label33.Height          = 28;
  Label33.Visible         = 1;
  Label33.Active          = 1;
  Label33.Caption         = Label33_Caption;
  Label33.FontName        = Tahoma21x25_Regular;
  Label33.Font_Color      = 0x0400;
  Label33.OnUpPtr         = 0;
  Label33.OnDownPtr       = 0;
  Label33.OnClickPtr      = 0;
  Label33.OnPressPtr      = 0;

  Label34.OwnerScreen     = &Gyro;
  Label34.Order           = 3;
  Label34.Left            = 110;
  Label34.Top             = 146;
  Label34.Width           = 26;
  Label34.Height          = 28;
  Label34.Visible         = 1;
  Label34.Active          = 1;
  Label34.Caption         = Label34_Caption;
  Label34.FontName        = Tahoma21x25_Regular;
  Label34.Font_Color      = 0x001F;
  Label34.OnUpPtr         = 0;
  Label34.OnDownPtr       = 0;
  Label34.OnClickPtr      = 0;
  Label34.OnPressPtr      = 0;

  Label35.OwnerScreen     = &Gyro;
  Label35.Order           = 4;
  Label35.Left            = 149;
  Label35.Top             = 100;
  Label35.Width           = 50;
  Label35.Height          = 28;
  Label35.Visible         = 1;
  Label35.Active          = 1;
  Label35.Caption         = Label35_Caption;
  Label35.FontName        = Tahoma21x25_Regular;
  Label35.Font_Color      = 0xF800;
  Label35.OnUpPtr         = 0;
  Label35.OnDownPtr       = 0;
  Label35.OnClickPtr      = 0;
  Label35.OnPressPtr      = 0;

  Label36.OwnerScreen     = &Gyro;
  Label36.Order           = 5;
  Label36.Left            = 149;
  Label36.Top             = 124;
  Label36.Width           = 50;
  Label36.Height          = 28;
  Label36.Visible         = 1;
  Label36.Active          = 1;
  Label36.Caption         = Label36_Caption;
  Label36.FontName        = Tahoma21x25_Regular;
  Label36.Font_Color      = 0x0400;
  Label36.OnUpPtr         = 0;
  Label36.OnDownPtr       = 0;
  Label36.OnClickPtr      = 0;
  Label36.OnPressPtr      = 0;

  Label37.OwnerScreen     = &Gyro;
  Label37.Order           = 6;
  Label37.Left            = 149;
  Label37.Top             = 146;
  Label37.Width           = 50;
  Label37.Height          = 28;
  Label37.Visible         = 1;
  Label37.Active          = 1;
  Label37.Caption         = Label37_Caption;
  Label37.FontName        = Tahoma21x25_Regular;
  Label37.Font_Color      = 0x001F;
  Label37.OnUpPtr         = 0;
  Label37.OnDownPtr       = 0;
  Label37.OnClickPtr      = 0;
  Label37.OnPressPtr      = 0;

  ButtonRound5.OwnerScreen     = &Gyro;
  ButtonRound5.Order           = 7;
  ButtonRound5.Left            = 266;
  ButtonRound5.Top             = 4;
  ButtonRound5.Width           = 50;
  ButtonRound5.Height          = 25;
  ButtonRound5.Pen_Width       = 1;
  ButtonRound5.Pen_Color       = 0x0000;
  ButtonRound5.Visible         = 1;
  ButtonRound5.Active          = 1;
  ButtonRound5.Transparent     = 1;
  ButtonRound5.Caption         = ButtonRound5_Caption;
  ButtonRound5.TextAlign       = _taCenter;
  ButtonRound5.FontName        = Tahoma11x13_Regular;
  ButtonRound5.PressColEnabled = 1;
  ButtonRound5.Font_Color      = 0x0000;
  ButtonRound5.Gradient        = 1;
  ButtonRound5.Gradient_Orientation = 0;
  ButtonRound5.Gradient_Start_Color = 0xFFFF;
  ButtonRound5.Gradient_End_Color = 0xC618;
  ButtonRound5.Color           = 0xC618;
  ButtonRound5.Press_Color     = 0xE71C;
  ButtonRound5.Corner_Radius   = 3;
  ButtonRound5.OnUpPtr         = 0;
  ButtonRound5.OnDownPtr       = 0;
  ButtonRound5.OnClickPtr      = BackOnClick;
  ButtonRound5.OnPressPtr      = 0;

  Label38.OwnerScreen     = &Inclinometer;
  Label38.Order           = 0;
  Label38.Left            = 50;
  Label38.Top             = 40;
  Label38.Width           = 220;
  Label38.Height          = 36;
  Label38.Visible         = 1;
  Label38.Active          = 1;
  Label38.Caption         = Label38_Caption;
  Label38.FontName        = Tahoma27x33_Regular;
  Label38.Font_Color      = 0x0000;
  Label38.OnUpPtr         = 0;
  Label38.OnDownPtr       = 0;
  Label38.OnClickPtr      = 0;
  Label38.OnPressPtr      = 0;

  Label39.OwnerScreen     = &Inclinometer;
  Label39.Order           = 1;
  Label39.Left            = 110;
  Label39.Top             = 101;
  Label39.Width           = 27;
  Label39.Height          = 28;
  Label39.Visible         = 1;
  Label39.Active          = 1;
  Label39.Caption         = Label39_Caption;
  Label39.FontName        = Tahoma21x25_Regular;
  Label39.Font_Color      = 0xF800;
  Label39.OnUpPtr         = 0;
  Label39.OnDownPtr       = 0;
  Label39.OnClickPtr      = 0;
  Label39.OnPressPtr      = 0;

  Label40.OwnerScreen     = &Inclinometer;
  Label40.Order           = 2;
  Label40.Left            = 110;
  Label40.Top             = 124;
  Label40.Width           = 27;
  Label40.Height          = 28;
  Label40.Visible         = 1;
  Label40.Active          = 1;
  Label40.Caption         = Label40_Caption;
  Label40.FontName        = Tahoma21x25_Regular;
  Label40.Font_Color      = 0x0400;
  Label40.OnUpPtr         = 0;
  Label40.OnDownPtr       = 0;
  Label40.OnClickPtr      = 0;
  Label40.OnPressPtr      = 0;

  Label41.OwnerScreen     = &Inclinometer;
  Label41.Order           = 3;
  Label41.Left            = 110;
  Label41.Top             = 146;
  Label41.Width           = 26;
  Label41.Height          = 28;
  Label41.Visible         = 1;
  Label41.Active          = 1;
  Label41.Caption         = Label41_Caption;
  Label41.FontName        = Tahoma21x25_Regular;
  Label41.Font_Color      = 0x001F;
  Label41.OnUpPtr         = 0;
  Label41.OnDownPtr       = 0;
  Label41.OnClickPtr      = 0;
  Label41.OnPressPtr      = 0;

  Label42.OwnerScreen     = &Inclinometer;
  Label42.Order           = 4;
  Label42.Left            = 149;
  Label42.Top             = 100;
  Label42.Width           = 50;
  Label42.Height          = 28;
  Label42.Visible         = 1;
  Label42.Active          = 1;
  Label42.Caption         = Label42_Caption;
  Label42.FontName        = Tahoma21x25_Regular;
  Label42.Font_Color      = 0xF800;
  Label42.OnUpPtr         = 0;
  Label42.OnDownPtr       = 0;
  Label42.OnClickPtr      = 0;
  Label42.OnPressPtr      = 0;

  Label43.OwnerScreen     = &Inclinometer;
  Label43.Order           = 5;
  Label43.Left            = 149;
  Label43.Top             = 124;
  Label43.Width           = 50;
  Label43.Height          = 28;
  Label43.Visible         = 1;
  Label43.Active          = 1;
  Label43.Caption         = Label43_Caption;
  Label43.FontName        = Tahoma21x25_Regular;
  Label43.Font_Color      = 0x0400;
  Label43.OnUpPtr         = 0;
  Label43.OnDownPtr       = 0;
  Label43.OnClickPtr      = 0;
  Label43.OnPressPtr      = 0;

  Label44.OwnerScreen     = &Inclinometer;
  Label44.Order           = 6;
  Label44.Left            = 149;
  Label44.Top             = 146;
  Label44.Width           = 50;
  Label44.Height          = 28;
  Label44.Visible         = 1;
  Label44.Active          = 1;
  Label44.Caption         = Label44_Caption;
  Label44.FontName        = Tahoma21x25_Regular;
  Label44.Font_Color      = 0x001F;
  Label44.OnUpPtr         = 0;
  Label44.OnDownPtr       = 0;
  Label44.OnClickPtr      = 0;
  Label44.OnPressPtr      = 0;

  ButtonRound6.OwnerScreen     = &Inclinometer;
  ButtonRound6.Order           = 7;
  ButtonRound6.Left            = 266;
  ButtonRound6.Top             = 4;
  ButtonRound6.Width           = 50;
  ButtonRound6.Height          = 25;
  ButtonRound6.Pen_Width       = 1;
  ButtonRound6.Pen_Color       = 0x0000;
  ButtonRound6.Visible         = 1;
  ButtonRound6.Active          = 1;
  ButtonRound6.Transparent     = 1;
  ButtonRound6.Caption         = ButtonRound6_Caption;
  ButtonRound6.TextAlign       = _taCenter;
  ButtonRound6.FontName        = Tahoma11x13_Regular;
  ButtonRound6.PressColEnabled = 1;
  ButtonRound6.Font_Color      = 0x0000;
  ButtonRound6.Gradient        = 1;
  ButtonRound6.Gradient_Orientation = 0;
  ButtonRound6.Gradient_Start_Color = 0xFFFF;
  ButtonRound6.Gradient_End_Color = 0xC618;
  ButtonRound6.Color           = 0xC618;
  ButtonRound6.Press_Color     = 0xE71C;
  ButtonRound6.Corner_Radius   = 3;
  ButtonRound6.OnUpPtr         = 0;
  ButtonRound6.OnDownPtr       = 0;
  ButtonRound6.OnClickPtr      = BackOnClick;
  ButtonRound6.OnPressPtr      = 0;

  Label45.OwnerScreen     = &Orientation;
  Label45.Order           = 0;
  Label45.Left            = 50;
  Label45.Top             = 40;
  Label45.Width           = 202;
  Label45.Height          = 36;
  Label45.Visible         = 1;
  Label45.Active          = 1;
  Label45.Caption         = Label45_Caption;
  Label45.FontName        = Tahoma27x33_Regular;
  Label45.Font_Color      = 0x0000;
  Label45.OnUpPtr         = 0;
  Label45.OnDownPtr       = 0;
  Label45.OnClickPtr      = 0;
  Label45.OnPressPtr      = 0;

  Label46.OwnerScreen     = &Orientation;
  Label46.Order           = 1;
  Label46.Left            = 110;
  Label46.Top             = 101;
  Label46.Width           = 27;
  Label46.Height          = 28;
  Label46.Visible         = 1;
  Label46.Active          = 1;
  Label46.Caption         = Label46_Caption;
  Label46.FontName        = Tahoma21x25_Regular;
  Label46.Font_Color      = 0xF800;
  Label46.OnUpPtr         = 0;
  Label46.OnDownPtr       = 0;
  Label46.OnClickPtr      = 0;
  Label46.OnPressPtr      = 0;

  Label47.OwnerScreen     = &Orientation;
  Label47.Order           = 2;
  Label47.Left            = 110;
  Label47.Top             = 124;
  Label47.Width           = 27;
  Label47.Height          = 28;
  Label47.Visible         = 1;
  Label47.Active          = 1;
  Label47.Caption         = Label47_Caption;
  Label47.FontName        = Tahoma21x25_Regular;
  Label47.Font_Color      = 0x0400;
  Label47.OnUpPtr         = 0;
  Label47.OnDownPtr       = 0;
  Label47.OnClickPtr      = 0;
  Label47.OnPressPtr      = 0;

  Label48.OwnerScreen     = &Orientation;
  Label48.Order           = 3;
  Label48.Left            = 110;
  Label48.Top             = 146;
  Label48.Width           = 26;
  Label48.Height          = 28;
  Label48.Visible         = 1;
  Label48.Active          = 1;
  Label48.Caption         = Label48_Caption;
  Label48.FontName        = Tahoma21x25_Regular;
  Label48.Font_Color      = 0x001F;
  Label48.OnUpPtr         = 0;
  Label48.OnDownPtr       = 0;
  Label48.OnClickPtr      = 0;
  Label48.OnPressPtr      = 0;

  Label49.OwnerScreen     = &Orientation;
  Label49.Order           = 4;
  Label49.Left            = 149;
  Label49.Top             = 100;
  Label49.Width           = 50;
  Label49.Height          = 28;
  Label49.Visible         = 1;
  Label49.Active          = 1;
  Label49.Caption         = Label49_Caption;
  Label49.FontName        = Tahoma21x25_Regular;
  Label49.Font_Color      = 0xF800;
  Label49.OnUpPtr         = 0;
  Label49.OnDownPtr       = 0;
  Label49.OnClickPtr      = 0;
  Label49.OnPressPtr      = 0;

  Label50.OwnerScreen     = &Orientation;
  Label50.Order           = 5;
  Label50.Left            = 149;
  Label50.Top             = 124;
  Label50.Width           = 50;
  Label50.Height          = 28;
  Label50.Visible         = 1;
  Label50.Active          = 1;
  Label50.Caption         = Label50_Caption;
  Label50.FontName        = Tahoma21x25_Regular;
  Label50.Font_Color      = 0x0400;
  Label50.OnUpPtr         = 0;
  Label50.OnDownPtr       = 0;
  Label50.OnClickPtr      = 0;
  Label50.OnPressPtr      = 0;

  Label51.OwnerScreen     = &Orientation;
  Label51.Order           = 6;
  Label51.Left            = 148;
  Label51.Top             = 147;
  Label51.Width           = 50;
  Label51.Height          = 28;
  Label51.Visible         = 1;
  Label51.Active          = 1;
  Label51.Caption         = Label51_Caption;
  Label51.FontName        = Tahoma21x25_Regular;
  Label51.Font_Color      = 0x001F;
  Label51.OnUpPtr         = 0;
  Label51.OnDownPtr       = 0;
  Label51.OnClickPtr      = 0;
  Label51.OnPressPtr      = 0;

  ButtonRound7.OwnerScreen     = &Orientation;
  ButtonRound7.Order           = 7;
  ButtonRound7.Left            = 266;
  ButtonRound7.Top             = 4;
  ButtonRound7.Width           = 50;
  ButtonRound7.Height          = 25;
  ButtonRound7.Pen_Width       = 1;
  ButtonRound7.Pen_Color       = 0x0000;
  ButtonRound7.Visible         = 1;
  ButtonRound7.Active          = 1;
  ButtonRound7.Transparent     = 1;
  ButtonRound7.Caption         = ButtonRound7_Caption;
  ButtonRound7.TextAlign       = _taCenter;
  ButtonRound7.FontName        = Tahoma11x13_Regular;
  ButtonRound7.PressColEnabled = 1;
  ButtonRound7.Font_Color      = 0x0000;
  ButtonRound7.Gradient        = 1;
  ButtonRound7.Gradient_Orientation = 0;
  ButtonRound7.Gradient_Start_Color = 0xFFFF;
  ButtonRound7.Gradient_End_Color = 0xC618;
  ButtonRound7.Color           = 0xC618;
  ButtonRound7.Press_Color     = 0xE71C;
  ButtonRound7.Corner_Radius   = 3;
  ButtonRound7.OnUpPtr         = 0;
  ButtonRound7.OnDownPtr       = 0;
  ButtonRound7.OnClickPtr      = BackOnClick;
  ButtonRound7.OnPressPtr      = 0;

  Label26.OwnerScreen     = &Orientation;
  Label26.Order           = 8;
  Label26.Left            = 110;
  Label26.Top             = 169;
  Label26.Width           = 34;
  Label26.Height          = 28;
  Label26.Visible         = 1;
  Label26.Active          = 1;
  Label26.Caption         = Label26_Caption;
  Label26.FontName        = Tahoma21x25_Regular;
  Label26.Font_Color      = 0xEF40;
  Label26.OnUpPtr         = 0;
  Label26.OnDownPtr       = 0;
  Label26.OnClickPtr      = 0;
  Label26.OnPressPtr      = 0;

  Label27.OwnerScreen     = &Orientation;
  Label27.Order           = 9;
  Label27.Left            = 149;
  Label27.Top             = 169;
  Label27.Width           = 50;
  Label27.Height          = 28;
  Label27.Visible         = 1;
  Label27.Active          = 1;
  Label27.Caption         = Label27_Caption;
  Label27.FontName        = Tahoma21x25_Regular;
  Label27.Font_Color      = 0xEF40;
  Label27.OnUpPtr         = 0;
  Label27.OnDownPtr       = 0;
  Label27.OnClickPtr      = 0;
  Label27.OnPressPtr      = 0;
}

static char IsInsideObject (unsigned int X, unsigned int Y, unsigned int Left, unsigned int Top, unsigned int Width, unsigned int Height) { // static
  if ( (Left<= X) && (Left+ Width - 1 >= X) &&
       (Top <= Y)  && (Top + Height - 1 >= Y) )
    return 1;
  else
    return 0;
}


#define GetRoundButton(index)         CurrentScreen->Buttons_Round[index]
#define GetLabel(index)               CurrentScreen->Labels[index]
#define GetCircleButton(index)        CurrentScreen->CircleButtons[index]
#define GetLine(index)                CurrentScreen->Lines[index]


void DrawRoundButton(TButton_Round *Around_button) {
    if (Around_button->Visible == 1) {
      if (object_pressed == 1) {
        object_pressed = 0;
        TFT_Set_Brush(Around_button->Transparent, Around_button->Press_Color, Around_button->Gradient, Around_button->Gradient_Orientation,
                      Around_button->Gradient_End_Color, Around_button->Gradient_Start_Color);
      }
      else {
        TFT_Set_Brush(Around_button->Transparent, Around_button->Color, Around_button->Gradient, Around_button->Gradient_Orientation,
                      Around_button->Gradient_Start_Color, Around_button->Gradient_End_Color);
      }
      TFT_Set_Pen(Around_button->Pen_Color, Around_button->Pen_Width);
      TFT_Rectangle_Round_Edges(Around_button->Left + 1, Around_button->Top + 1,
        Around_button->Left + Around_button->Width - 2,
        Around_button->Top + Around_button->Height - 2, Around_button->Corner_Radius);
      TFT_Set_Font(Around_button->FontName, Around_button->Font_Color, FO_HORIZONTAL);
      TFT_Write_Text_Return_Pos(Around_button->Caption, Around_button->Left, Around_button->Top);
      if (Around_button->TextAlign == _taLeft)
        TFT_Write_Text(Around_button->Caption, Around_button->Left + 4, (Around_button->Top + ((Around_button->Height - caption_height) / 2)));
      else if (Around_button->TextAlign == _taCenter)
        TFT_Write_Text(Around_button->Caption, (Around_button->Left + (Around_button->Width - caption_length) / 2), (Around_button->Top + ((Around_button->Height - caption_height) / 2)));
      else if (Around_button->TextAlign == _taRight)
        TFT_Write_Text(Around_button->Caption, Around_button->Left + (Around_button->Width - caption_length - 4), (Around_button->Top + (Around_button->Height - caption_height) / 2));
    }
}

void DrawLabel(TLabel *ALabel) {
  if (ALabel->Visible == 1) {
    TFT_Set_Font(ALabel->FontName, ALabel->Font_Color, FO_HORIZONTAL);
    TFT_Write_Text(ALabel->Caption, ALabel->Left, ALabel->Top);
  }
}

void DrawCircleButton(TCircleButton *ACircle_button) {
  if (ACircle_button->Visible == 1) {
    if (object_pressed == 1) {
      object_pressed = 0;
      TFT_Set_Brush(ACircle_button->Transparent, ACircle_button->Press_Color, ACircle_button->Gradient, ACircle_button->Gradient_Orientation,
                    ACircle_button->Gradient_End_Color, ACircle_button->Gradient_Start_Color);
    }
    else {
      TFT_Set_Brush(ACircle_button->Transparent, ACircle_button->Color, ACircle_button->Gradient, ACircle_button->Gradient_Orientation,
                    ACircle_button->Gradient_Start_Color, ACircle_button->Gradient_End_Color);
    }
    TFT_Set_Pen(ACircle_button->Pen_Color, ACircle_button->Pen_Width);
    TFT_Circle(ACircle_button->Left + ACircle_button->Radius,
               ACircle_button->Top + ACircle_button->Radius,
               ACircle_button->Radius);
    TFT_Set_Font(ACircle_button->FontName, ACircle_button->Font_Color, FO_HORIZONTAL);
    TFT_Write_Text_Return_Pos(ACircle_button->Caption, ACircle_button->Left, ACircle_button->Top);
    if (ACircle_button->TextAlign == _taLeft)
      TFT_Write_Text(ACircle_button->Caption, ACircle_button->Left + 4, (ACircle_button->Top + ACircle_button->Radius - caption_height / 2));
    else if (ACircle_button->TextAlign == _taCenter)
      TFT_Write_Text(ACircle_button->Caption, (ACircle_button->Left + (ACircle_button->Radius*2 - caption_length) / 2), (ACircle_button->Top + ACircle_button->Radius - caption_height / 2));
    else if (ACircle_button->TextAlign == _taRight)
      TFT_Write_Text(ACircle_button->Caption, ACircle_button->Left + (ACircle_button->Radius*2 - caption_length - 4), (ACircle_button->Top + ACircle_button->Radius - caption_height / 2));
  }
}

void DrawLine(TLine *Aline) {
  if (Aline->Visible == 1) {
    TFT_Set_Pen(Aline->Color, Aline->Pen_Width);
    TFT_Line(Aline->First_Point_X, Aline->First_Point_Y, Aline->Second_Point_X, Aline->Second_Point_Y);
  }
}

void DrawScreen(TScreen *aScreen) {
 unsigned int order;
  unsigned short round_button_idx;
  TButton_Round *local_round_button;
  unsigned short label_idx;
  TLabel *local_label;
  unsigned short circle_button_idx;
  TCircleButton *local_circle_button;
  unsigned short line_idx;
  TLine *local_line;
  char save_bled, save_bled_direction;

  object_pressed = 0;
  order = 0;
  round_button_idx = 0;
  label_idx = 0;
  circle_button_idx = 0;
  line_idx = 0;
  CurrentScreen = aScreen;

  if ((display_width != CurrentScreen->Width) || (display_height != CurrentScreen->Height)) {
    save_bled = TFT_BLED;
    save_bled_direction = TFT_BLED_Direction;
    TFT_BLED_Direction = 0;
    TFT_BLED           = 0;
    TFT_Init_ILI9341_8bit(CurrentScreen->Width, CurrentScreen->Height);
    TP_TFT_Init(CurrentScreen->Width, CurrentScreen->Height, 3, 14);                                  // Initialize touch panel
    TP_TFT_Set_ADC_Threshold(ADC_THRESHOLD);                              // Set touch panel ADC threshold
    TFT_Fill_Screen(CurrentScreen->Color);
    display_width = CurrentScreen->Width;
    display_height = CurrentScreen->Height;
    TFT_BLED           = save_bled;
    TFT_BLED_Direction = save_bled_direction;
  }
  else
    TFT_Fill_Screen(CurrentScreen->Color);


  while (order < CurrentScreen->ObjectsCount) {
    if (round_button_idx < CurrentScreen->Buttons_RoundCount) {
      local_round_button = GetRoundButton(round_button_idx);
      if (order == local_round_button->Order) {
        round_button_idx++;
        order++;
        DrawRoundButton(local_round_button);
      }
    }

    if (label_idx < CurrentScreen->LabelsCount) {
      local_label = GetLabel(label_idx);
      if (order == local_label->Order) {
        label_idx++;
        order++;
        DrawLabel(local_label);
      }
    }

    if (circle_button_idx < CurrentScreen->CircleButtonsCount) {
      local_circle_button = GetCircleButton(circle_button_idx);
      if (order == local_circle_button->Order) {
        circle_button_idx++;
        order++;
        DrawCircleButton(local_circle_button);
      }
    }

    if (line_idx < CurrentScreen->LinesCount) {
      local_line = GetLine(line_idx);
      if (order == local_line->Order) {
        line_idx++;
        order++;
        DrawLine(local_line);
      }
    }

  }
}

void Get_Object(unsigned int X, unsigned int Y) {
  round_button_order  = -1;
  label_order         = -1;
  circle_button_order = -1;
  //  Buttons with Round Edges
  for ( _object_count = 0 ; _object_count < CurrentScreen->Buttons_RoundCount ; _object_count++ ) {
    local_round_button = GetRoundButton(_object_count);
    if (local_round_button->Active == 1) {
      if (IsInsideObject(X, Y, local_round_button->Left, local_round_button->Top,
                         local_round_button->Width, local_round_button->Height) == 1) {
        round_button_order = local_round_button->Order;
        exec_round_button = local_round_button;
      }
    }
  }

  //  Labels
  for ( _object_count = 0 ; _object_count < CurrentScreen->LabelsCount ; _object_count++ ) {
    local_label = GetLabel(_object_count);
    if (local_label->Active == 1) {
      if (IsInsideObject(X, Y, local_label->Left, local_label->Top,
                         local_label->Width, local_label->Height) == 1) {
        label_order = local_label->Order;
        exec_label = local_label;
      }
    }
  }

  //  Circle Buttons
  for ( _object_count = 0 ; _object_count < CurrentScreen->CircleButtonsCount ; _object_count++ ) {
    local_circle_button = GetCircleButton(_object_count);
    if (local_circle_button->Active == 1) {
      if (IsInsideObject(X, Y, local_circle_button->Left, local_circle_button->Top,
                        (local_circle_button->Radius * 2), (local_circle_button->Radius * 2)) == 1) {
        circle_button_order = local_circle_button->Order;
        exec_circle_button = local_circle_button;
      }
    }
  }

  _object_count = -1;
  if (round_button_order >  _object_count )
    _object_count = round_button_order;
  if (label_order >  _object_count )
    _object_count = label_order;
  if (circle_button_order >  _object_count )
    _object_count = circle_button_order;
}


void Process_TP_Press(unsigned int X, unsigned int Y) {
  exec_round_button   = 0;
  exec_label          = 0;
  exec_circle_button  = 0;

  Get_Object(X, Y);

  if (_object_count != -1) {
    if (_object_count == round_button_order) {
      if (exec_round_button->Active == 1) {
        if (exec_round_button->OnPressPtr != 0) {
          exec_round_button->OnPressPtr();
          return;
        }
      }
    }

    if (_object_count == label_order) {
      if (exec_label->Active == 1) {
        if (exec_label->OnPressPtr != 0) {
          exec_label->OnPressPtr();
          return;
        }
      }
    }

    if (_object_count == circle_button_order) {
      if (exec_circle_button->Active == 1) {
        if (exec_circle_button->OnPressPtr != 0) {
          exec_circle_button->OnPressPtr();
          return;
        }
      }
    }

  }
}

void Process_TP_Up(unsigned int X, unsigned int Y) {

  switch (PressedObjectType) {
    // Round Button
    case 1: {
      if (PressedObject != 0) {
        exec_round_button = (TButton_Round*)PressedObject;
        if ((exec_round_button->PressColEnabled == 1) && (exec_round_button->OwnerScreen == CurrentScreen)) {
          DrawRoundButton(exec_round_button);
        }
        break;
      }
      break;
    }
    // Circle Button
    case 5: {
      if (PressedObject != 0) {
        exec_circle_button = (TCircleButton*)PressedObject;
        if ((exec_circle_button->PressColEnabled == 1) && (exec_circle_button->OwnerScreen == CurrentScreen)) {
          DrawCircleButton(exec_circle_button);
        }
        break;
      }
      break;
    }
  }

  exec_label          = 0;

  Get_Object(X, Y);


  if (_object_count != -1) {
  // Buttons with Round Edges
    if (_object_count == round_button_order) {
      if (exec_round_button->Active == 1) {
        if (exec_round_button->OnUpPtr != 0)
          exec_round_button->OnUpPtr();
        if (PressedObject == (void *)exec_round_button)
          if (exec_round_button->OnClickPtr != 0)
            exec_round_button->OnClickPtr();
        PressedObject = 0;
        PressedObjectType = -1;
        return;
      }
    }

  // Labels
    if (_object_count == label_order) {
      if (exec_label->Active == 1) {
        if (exec_label->OnUpPtr != 0)
          exec_label->OnUpPtr();
        if (PressedObject == (void *)exec_label)
          if (exec_label->OnClickPtr != 0)
            exec_label->OnClickPtr();
        PressedObject = 0;
        PressedObjectType = -1;
        return;
      }
    }

  // Circle Buttons
    if (_object_count == circle_button_order) {
      if (exec_circle_button->Active == 1) {
        if (exec_circle_button->OnUpPtr != 0)
          exec_circle_button->OnUpPtr();
        if (PressedObject == (void *)exec_circle_button)
          if (exec_circle_button->OnClickPtr != 0)
            exec_circle_button->OnClickPtr();
        PressedObject = 0;
        PressedObjectType = -1;
        return;
      }
    }

  }
  PressedObject = 0;
  PressedObjectType = -1;
}

void Process_TP_Down(unsigned int X, unsigned int Y) {

  object_pressed      = 0;
  exec_round_button   = 0;
  exec_label          = 0;
  exec_circle_button  = 0;

  Get_Object(X, Y);

  if (_object_count != -1) {
    if (_object_count == round_button_order) {
      if (exec_round_button->Active == 1) {
        if (exec_round_button->PressColEnabled == 1) {
          object_pressed = 1;
          DrawRoundButton(exec_round_button);
        }
        PressedObject = (void *)exec_round_button;
        PressedObjectType = 1;
        if (exec_round_button->OnDownPtr != 0) {
          exec_round_button->OnDownPtr();
          return;
        }
      }
    }

    if (_object_count == label_order) {
      if (exec_label->Active == 1) {
        PressedObject = (void *)exec_label;
        PressedObjectType = 2;
        if (exec_label->OnDownPtr != 0) {
          exec_label->OnDownPtr();
          return;
        }
      }
    }

    if (_object_count == circle_button_order) {
      if (exec_circle_button->Active == 1) {
        if (exec_circle_button->PressColEnabled == 1) {
          object_pressed = 1;
          DrawCircleButton(exec_circle_button);
        }
        PressedObject = (void *)exec_circle_button;
        PressedObjectType = 5;
        if (exec_circle_button->OnDownPtr != 0) {
          exec_circle_button->OnDownPtr();
          return;
        }
      }
    }

  }
}

void Check_TP() {
  if (TP_TFT_Press_Detect()) {
    if (TP_TFT_Get_Coordinates(&Xcoord, &Ycoord) == 0) {
    // After a PRESS is detected read X-Y and convert it to Display dimensions space
      Process_TP_Press(Xcoord, Ycoord);
      if (PenDown == 0) {
        PenDown = 1;
        Process_TP_Down(Xcoord, Ycoord);
      }
    }
  }
  else if (PenDown == 1) {
    PenDown = 0;
    Process_TP_Up(Xcoord, Ycoord);
  }
}

void Init_MCU() {
  AD1PCFG = 0x3FF3;
  JTAGEN_bit = 0;
  TFT_Set_Default_Mode();
  TP_TFT_Set_Default_Mode();
}

void Start_TP() {
  Init_MCU();

  InitializeTouchPanel();

  Delay_ms(1000);
  TFT_Fill_Screen(0);
  Calibrate();
  TFT_Fill_Screen(0);

  InitializeObjects();
  display_width = SelectSensor.Width;
  display_height = SelectSensor.Height;
  DrawScreen(&SelectSensor);
}
