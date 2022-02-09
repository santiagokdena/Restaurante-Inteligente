#include <LiquidCrystal.h> //include LCD library (standard library)
#include <Keypad.h> //include keypad library - first you must install library (library link in the video description)
//notas musicales
#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978
#define REST      0
int thisNote = 0;
//velocidad de la musica
int tempo = 105;
//pin buzzer
int buzzer =9;
// notes of the moledy followed by the duration.
// a 4 means a quarter note, 8 an eighteenth , 16 sixteenth, so on
// !!negative numbers are used to represent dotted notes,
// so -4 means a dotted quarter note, that is, a quarter plus an eighteenth!!
int melody[] = {

  // Pacman
  // Score available at https://musescore.com/user/85429/scores/107109
  NOTE_B4, 16, NOTE_B5, 16, NOTE_FS5, 16, NOTE_DS5, 16, //1
  NOTE_B5, 32, NOTE_FS5, -16, NOTE_DS5, 8, NOTE_C5, 16,
  NOTE_C6, 16, NOTE_G6, 16, NOTE_E6, 16, NOTE_C6, 32, NOTE_G6, -16, NOTE_E6, 8,

  NOTE_B4, 16,  NOTE_B5, 16,  NOTE_FS5, 16,   NOTE_DS5, 16,  NOTE_B5, 32,  //2
  NOTE_FS5, -16, NOTE_DS5, 8,  NOTE_DS5, 32, NOTE_E5, 32,  NOTE_F5, 32,
  NOTE_F5, 32,  NOTE_FS5, 32,  NOTE_G5, 32,  NOTE_G5, 32, NOTE_GS5, 32,  NOTE_A5, 16, NOTE_B5, 8
};

// sizeof gives the number of bytes, each int value is composed of two bytes (16 bits)
// there are two values per note (pitch and duration), so for each note there are four bytes
int notes = sizeof(melody) / sizeof(melody[0]) / 2;

// this calculates the duration of a whole note in ms
int wholenote = (60000 * 4) / tempo;

int divider = 0, noteDuration = 0;


//pines de motor,piezo y leds
const int motor=10;
const int leds=11;
//EasyBuzzer.setPin(piezo);

//keypad rows and columns,Keymap
const byte rows = 4; 
const byte cols = 4;
char keyMap [rows] [cols] = { //define the cymbols on the buttons of the keypad

  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
//pins of the keypad
byte rowPins [rows] = {8, 7, 6, 5}; 
byte colPins [cols] = {4, 3, 2, 1};
//inicializacion de instancia myKeypad
Keypad myKeypad = Keypad( makeKeymap(keyMap), rowPins, colPins, rows, cols);
//inicializacion de instancia LiquidCrystal
LiquidCrystal lcd (A0, A1, A2, A3, A4, A5); // pins of the LCD. (RS, E, D4, D5, D6, D7)
//ram memory 
String memory[10][4];
//lista de cantidad por comida
String cantidad[]={"","","",""};
//variables de la maquina de estados
//estado inicial
int state=0;
//bienvenida
int delaytime=1000;
int Li          = 16;
int Lii         = 0; 
//menu
int menu = 1; 
long alerttime,starttime;
int turno_salida,turno_entrada=0;

void setup(){
  lcd.begin(16, 2);  // start the LCD library
  alerttime=9000000; //duración máxima de la lcd funcionando (150min)
  
}
//---------------------------------- 
void asignar_turno(String(&myarray)[10][4],String cantidad[4])
{
  turno_entrada++;
  for (int i=0;i<4;i++){
    myarray [turno_salida][i]=cantidad[i];}
}
//---------------------------------- 
void loop(){
 	starttime=millis();       
   if((starttime-alerttime)>10000){//cuando ya esta lista la orden
     state=2;
     lcd.clear();
     lcd.setCursor(0,0);
     lcd.print("Recoge tu pedido");
     lcd.setCursor(0,1);
     lcd.print("Turno ");
     lcd.print(turno_salida+1);
   }
   
  switch(state){ //maquina de estados del programa
    case 0:  //bienvenida

       if(myKeypad.getKey()){
          state=1;
          updateMenu(menu);
          break;
        }

        lcd.setCursor(0,0);             
        lcd.print(" Haz tu pedido!");  
        lcd.setCursor(0, 1);
        lcd.print(Scroll_LCD_Left("Bienvenido/a al restaurante S&T&M"));
        delay(100);   
        break;

     case 1: //menu
    {
          char option=myKeypad.getKey();
          if(option=='C'){ //down
              menu++;
              updateMenu(menu);
              delay(100);
            	break;
              }
          else if(option=='A'){ //up
              menu--;
              updateMenu(menu);
              delay(100);
            break;
          }
          else if(option>='1'){ // digitos validos: '1' al '9'
              lcd.write(option);
              cantidad[menu-1]=option;
              delay(100);
         }  
        
          else if(option=='*'){
              alerttime=starttime; //inicia el conteo 
              asignar_turno(memory,cantidad); //asigna turno y cantidades a la mem ram
              lcd.clear();
              lcd.setCursor(0,0);
              lcd.print("Pedido Enviado");
              lcd.setCursor(0,1);
              lcd.print("Turno ");
              lcd.print(turno_entrada);
              delay(3000);  
              //cantidad[] tiene los valores por default
              for(int i=0;i<4;i++){
                cantidad[i]="";}
              state=0;
              break;
         }
      break;
    }
    case 2: //entrega de pedido
    	alerttime=9000000;  
    if(thisNote<notes * 2){
          divider = melody[thisNote + 1];
          if (divider > 0) {// regular note
            noteDuration = (wholenote) / divider;
          }
      	  else if (divider < 0) {// dotted notes 
            noteDuration = (wholenote) / abs(divider);
            noteDuration *= 1.5; // increases the duration in half for dotted notes
          }
          tone(buzzer, melody[thisNote], noteDuration * 0.9);
          delay(noteDuration);
          noTone(buzzer);
          thisNote=thisNote+2;
    }
    else{
      state=0;
    }
    	
    if(myKeypad.getKey()){
      analogWrite(motor,255); //accionamiento del motor
      //accionamiento del buzzer
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Que lo disfrutes");
      delay(1000);
      //mostrar pedidos
      if(memory[turno_salida][0]!=""){
        lcd.setCursor(0,1);
        lcd.print(memory[turno_salida][0]);
        lcd.print(" Hamburguesas   ");
        delay(2500);
      }
      if(memory[turno_salida][1]!=""){
        lcd.setCursor(0,1);
        lcd.print(memory[turno_salida][1]);
        lcd.print(" Salchipapas    ");
        delay(2500);
      }
      if(memory[turno_salida][2]!=""){
        lcd.setCursor(0,1);
        lcd.print(memory[turno_salida][2]);
        lcd.print(" Perros         ");
        delay(2500);
      }
      if(memory[turno_salida][3]!=""){
        lcd.setCursor(0,1);
        lcd.print(memory[turno_salida][3]);
        lcd.print(" Pizzas         ");
        delay(2500);
      }
      turno_salida++; //turno de salida aumenta
      state=0;
      analogWrite(motor,0);
      //	EasyBuzzer.stopBeep();
    }
    break;

    
 }
}
//funcion para el efecto de corrimiento en el display
String Scroll_LCD_Left(String StrDisplay){
  String result;
  String StrProcess = "                " + StrDisplay + "                ";
  result = StrProcess.substring(Lii,Li);
  Li++;
  Lii++;
  if (Li>StrProcess.length()){
    Li=16;
    Lii=0;
  }
  return result;
}

void updateMenu(int menu) {
  String str_menu="      MENU      ";
  lcd.clear();
  lcd.print(str_menu);
  lcd.setCursor(0, 1);
  switch (menu) {
    case 0:
      menu = 1;
      break;
    case 1:{
      lcd.print("Hamburguesas:");
      lcd.print(cantidad[0]);
      break;}
    case 2:
      lcd.print("Salchipapas:");
      lcd.print(cantidad[1]);
      break;
    case 3:
      lcd.print("Perro Caliente:");
      lcd.print(cantidad[2]);
      break;
    case 4:
      lcd.print("Pizza:");
      lcd.print(cantidad[3]);
      break;
    case 5:
      menu = 4;
      break;
  }
}


