
#include <EEPROM.h>

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific 

#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0
#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin
#define ONE_WIRE_BUS 13 // Sennsor de Temperatura


#define  BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
#define ORANGE   0xFD20

// Color definitions

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
Adafruit_GFX_Button settingsButton;

#define botaoMenos 12
#define botaoMais 11
#define botaoConfirma 10

float ultima_temperatura;
bool homeScreenOn = false;
bool configOn= false;

void setup() {

  Serial.begin(9600);

  Serial.println(F("TFT LCD test"));
  tft.begin(0x9325);

  pinMode(botaoMenos, INPUT_PULLUP);
  pinMode(botaoMais, INPUT_PULLUP);
  pinMode(botaoConfirma, INPUT_PULLUP);

  verificar_status_sistema();

   //tft.fillScreen(BLACK);
  //HomeScreen(String(2000),String(5.22),String(309), String(3.9), String(0), String(510), String(41));
  //homeScreenOn=true;
  //Tela_irrigando(String(600.10),String(5.22),String (2.83), String(3.9), "14", "06", String(510));
  //atualiza_agua_ejetada();

  //configurar_latitude();
  //configurar_coeficiente(0);
  //configurar_fase_inicial();
  //configurar_horario(0,0);//posicao do vetor e contador de horarios cadastrados
  //configurar_eficiencia();

}

void verificar_status_sistema() {
  //EEPROM.write(0, false); //para reiniciar
  bool flagStatus = EEPROM.read(0);
  if (flagStatus == true) {
    iniciar_homeScreen();
    atualizar_homeScreen(String(0), String(0), String(0), String(0), String(0), String(0), String(0), false, String(0), String(0),"");
  } else {
    homeScreenOn = false;
    configuracao();
  
  }
}


void configuracao() {

  String digitos[] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "0", ".", "[OK]"};
  String modoSistema[] = {"[Lamina Fixa]", "[Turno Fixo]"};
  unsigned int j=0;
  bool avancar = false;
  bool add = false;

 inserir_entrada(27, "Data:", "(DDMMAAAA)", digitos, true, 10, 80, j);//atualizar data e hora
 inserir_entrada(28, "Hora:", "(HHMMSS)", digitos, false, 10, 150, j);//atualizar data e hora
  
 inserir_entrada(29, "Latitude:", "(00 a 90 graus)", digitos, true, 10, 80, j);//latitude
 inserir_entrada(30, "Latitude:", "(00 a 59 min)", digitos, false, 10, 150, j);//latitude
 /*
 inserir_entrada(31, "Tamanho:", "(em m2)", digitos, false, 10, 220, j);//tamanho
 delay(2000);
 inserir_entrada(32, "Coeficiente:", "(KC)", digitos, true, 10, 80, j);//KC
 inserir_entrada(33, "Fase Feno:", "(duracao em dias)", digitos, false, 10, 150, j);//FASE

  int  cont=1;
  while (avancar == false) {

    tft.setCursor(30, 265);
    tft.setTextColor(WHITE, BLACK);
    tft.setTextSize(1);
    tft.print("(Press [OK] para avancar)");
    tft.setCursor(30, 280);
    tft.print("(Press [+] para add novo KC) ");

    while (1 == 1) {
      if (digitalRead(botaoConfirma) == LOW) {
        avancar = true;
        break;
      } else if (digitalRead(botaoMais) == LOW) {
        add = true;
        break;

      }
      delay(300);
    }
    if (avancar == true) {
       inserir_entrada(34, "Fase Inicial:", "(em dias)", digitos, true, 10, 80, j);// fase inicial

    } else if (add == true) {
      j++;//incrementa a variavel indicando que mais um kc e fase serao cadastrados
      cont++;
      inserir_entrada(32, "Coeficiente:", "(KC)", digitos, true, 10, 80, j);
      inserir_entrada(33, "Fase Feno:", "(em dias)", digitos, false, 10, 150, j);//FASE
    }
    if(cont==5){
      break;
    }
  }

  inserir_entrada(35, "Eficiencia:", "(60% a 85%)", digitos, false, 10, 150, j);//eficiencia
  delay(2000);

///definir modo do sistema: lamina fixa ou turno de rega////////////
  ////////////////////////////////////////////////////////////////////

  bool modo = inserir_entrada(40, "Modo Sistema:", "use [+] ou [-]", modoSistema, true, 10, 80, j);//modo Sistema

  if (modo==true){//modo lamina fixa
    inserir_entrada(38, "Lamina Fixa", "ETc em mm", digitos, false, 10, 150, j);//lamina fixa
  }else {//modo turno fixo
  j = 0;
  unsigned int contHR = 0;
  inserir_entrada(36, "Horas:", "h", digitos, true, 10, 80, j);//horas
  inserir_entrada(37, "Minutos:", "min", digitos, false, 10, 150, j);//minutos
 // configurar_horario(j, contHR, digitos);//posicao do vetor e contador de horarios cadastrados

  avancar = false;
  add = false;
  while (avancar == false) {
    if (contHR == 0) { //condicao para add ate 2 horarios de irrigacao

      tft.setCursor(30, 265);
      tft.setTextColor(WHITE, BLACK);
      tft.setTextSize(1);
      tft.print("(Press [OK] para avancar)");
      tft.setCursor(30, 280);
      tft.print("(Press [+] para add novo horario) ");


      while (1 == 1) {

        if (digitalRead(botaoConfirma) == LOW) {
          avancar = true;
          break;
        } else if (digitalRead(botaoMais) == LOW) {
          add = true;
          break;
        }
        delay(300);
      }
    } else {
       avancar=true;
       break;
    }
     if (add == true) {
      j++;
      contHR++;
      inserir_entrada(36, "Horas:", "00h a 23h", digitos, true, 10, 80, j);//modo Sistema
      inserir_entrada(37, "Minutos:", "00min a 59min", digitos, false, 10, 150, j);//modo Sistema
    }
   }
 
  }
      while (1 == 1) {
        tft.setTextColor(WHITE, BLACK);
        tft.setTextSize(1);
        tft.setCursor(30, 280);
        tft.print("(Press [OK] para avancar)");
        if (digitalRead(botaoConfirma) == LOW) {
          break;
        }
        delay(300);
      }

  */
      //fim da config
      bool flagStatus = true;
      EEPROM.write(0, flagStatus);
      iniciar_homeScreen();
      atualizar_homeScreen(String(0), String(0), String(0), String(0), String(0), String(0), String(0), false, String(0), String(0),"");
    
}



/*void receber_tempo(String c, int n) {

  String tempo;
  while (c.charAt(n) != '\n') { //recebendo temperatura atual
    tempo += c.charAt(n);
    n++;
  }
  n++;

  if (homeScreenOn == false) { //atualiza só se a tela de mostrar configurcao estiver ativa
    mostrar_configuracao(tempo);
  }

}*/


void loop() {

  if ((digitalRead(botaoMais) == LOW) && (digitalRead(botaoMenos) == LOW)) { //reiniciar entradas
    if (homeScreenOn == true){
      EEPROM.write(0, false); //para reiniciar CONFIGURAÇÕES
      verificar_status_sistema();
      delay(300);
    }/*if (configOn == true){//atualizar relogio
      String digitos[] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "0", ".", "[OK]"};
      inserir_entrada(28, "Data-Hora:", "(DDMMAAAAHHMMSS)", digitos, true, 10, 80, 0);//atualizar data e hora
      delay(300);
    }*/
  } 
  if ((digitalRead(botaoConfirma) == LOW) && (digitalRead(botaoMenos) == LOW) && homeScreenOn == true) { 
    Serial.println("50");//envia comando via porta serial para reiniciar minima e máxima
    delay(300);
  }
  if ((digitalRead(botaoConfirma) == LOW) && homeScreenOn==true || (digitalRead(botaoConfirma) == LOW) && configOn==true ) { 

      if(homeScreenOn==true){
        iniciar_mostrar_config();
      }else if(configOn==true){
        iniciar_homeScreen();
      }
    delay(300);
  } 

}

void atualizar_homeScreen(String totalAgua, String ETc, String mediaAgua, String Ra, String temperatura, String tmax, String tmin,
bool controleTemperatura, String demandaHidrica, String FFA,  String lamina) {


  tft.setCursor(10, 50);
  tft.setTextColor(WHITE, BLACK);
  tft.setTextSize(2);
  tft.print("Agua");
  tft.setTextColor(YELLOW, BLACK);
  tft.print("(Media: " + mediaAgua + " L)");

  tft.setCursor(10, 80);
  tft.setTextSize(5);
  tft.setTextColor(WHITE, BLACK);
  tft.print(totalAgua + "L");
  tft.setCursor(185, 120);
  tft.setTextSize(1);
  tft.print("(Total)");

  tft.setCursor(10, 130);
  tft.setTextSize(2);
  tft.setTextColor(WHITE, BLACK);
  tft.print("FFA:" + FFA + " dias");
  
  tft.setTextSize(3);
  tft.setTextColor(WHITE, BLACK);
  tft.setCursor(10, 157);
  tft.print(temperatura);
  tft.setCursor(100, 157);
  tft.print("C");

  tft.setCursor(135, 150);
  tft.setTextSize(2);
  tft.setTextColor(WHITE, RED);
  tft.print(tmax);
  tft.print("C");

  tft.setCursor(135, 170);
  tft.setTextSize(2);
  tft.setTextColor(WHITE, BLUE);
  tft.print(tmin);
  tft.print("C");

  tft.fillRoundRect(80, 170, 15, 15, 5, BLACK); //APAGANDO O INDICADOR ANTERIOR
  if (controleTemperatura == false) {
    tft.fillTriangle(83, 180, 93, 170, 93, 180, BLUE); //DESCEU
  } else {
    tft.fillTriangle(83, 180, 83, 170, 93, 170, ORANGE); //SUBIU
  }


  tft.setCursor(10, 200);
  tft.setTextColor(WHITE, BLACK);
  tft.setTextSize(2);
  tft.print("ETc");
  tft.setTextColor(GREEN, BLACK);
  tft.print("(Ra: " + Ra + " mm)");

  tft.setCursor(10, 280);
  tft.setTextSize(2);
  tft.setTextColor(WHITE, BLACK);
  tft.print("DH:" + demandaHidrica + " L");


  tft.fillCircle(170, 135, 10, GREEN);


  tft.setCursor(10, 230);
  tft.setTextSize(5);
  tft.setTextColor(WHITE, BLACK);
  tft.print(ETc + " mm");

  tft.fillCircle(170, 285, 10, BLUE);

  if(lamina.toInt()!=0){
    tft.setCursor(135, 220);
    tft.setTextSize(1);
    tft.setTextColor(RED, BLACK);
    tft.print("LFI:"+lamina+"mm");
  }

}
void iniciar_homeScreen(){

  tft.fillScreen(BLACK);
  homeScreenOn = true;
  configOn=false;
  settingsButton.initButton(&tft, 120, 20,   // x, y, w, h, outline, fill, text
                            230, 30, WHITE, 0x03E0, WHITE,
                            "Home", 2);
  settingsButton.drawButton();//cabeÃ§alho
}

void iniciar_mostrar_config(){

  tft.fillScreen(BLACK);
  homeScreenOn = false;
  configOn=true;
  settingsButton.initButton(&tft, 120, 20,   // x, y, w, h, outline, fill, text
                            230, 30, WHITE, 0x03E0, WHITE,
                            "Config", 2);
  settingsButton.drawButton();//cabeÃ§alho
}

void atualizar_mostrar_config(String tempo, String latGraus, String tam, String kcString, String faseString, String eficiencia,
String horarioString, String contDias, String lamina, String latMin){

  tft.setCursor(10, 50);
  tft.setTextSize(1);
  tft.setTextColor(YELLOW, BLACK);
  tft.print("    "+tempo);
  tft.setCursor(10, 70);
  tft.print("      Ativo ha  "+contDias+"  dias");

  tft.setCursor(10, 90);
  tft.setTextSize(2);
  tft.setTextColor(WHITE, BLACK);
  tft.print("Lat: " + latGraus + "G "+latMin+"M");
  tft.setCursor(10, 110);
  tft.print("Tam: " + tam + " m2");
  tft.setTextSize(1);
  tft.setCursor(10, 140);
  tft.print("KCs: " + kcString);
  tft.setCursor(10, 160);
  tft.print("Fases: " + faseString);
  tft.setTextSize(2);
  tft.setCursor(10, 190);
  tft.print("Ei: " + eficiencia+"%");
  if(lamina.toInt()==0){
    tft.setCursor(10, 220);
    tft.print("Modo: Turno Fixo");
    tft.setCursor(10, 250);
    tft.print("Hor: " + horarioString);
  }else{
    tft.setCursor(10, 220);
    tft.print("Modo: Lamina Fixa");
    tft.setCursor(10, 250);
    tft.print("LFI: " + lamina+" mm");
  }

   /* tft.setCursor(10, 270);
    tft.setTextColor(WHITE, BLACK);
    tft.setTextSize(1);
    tft.print("(Press [+] e [-] p/ atualizar relogio)");
    
   */
  
}
