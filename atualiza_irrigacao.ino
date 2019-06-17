void atualiza_tela_irrigacao(String vazao, String tempo, String agua_ejetada, String dh_restante) {


  tft.setCursor(10, 50);
  tft.setTextColor(WHITE, BLACK);
  tft.setTextSize(2);
  tft.print("Vazao");
  tft.setTextColor(YELLOW, BLACK);
  tft.print("(" + vazao + " L/seg)");

  tft.setCursor(10, 80);
  tft.setTextSize(5);
  tft.setTextColor(WHITE, BLACK);
  tft.fillRoundRect(10, 80, 180, 50, 5, BLACK); //APAGANDO O QUANT AGUA  ANTERIOR
  tft.setCursor(10, 80);
  tft.print(agua_ejetada);

  tft.setCursor(10, 130);
  tft.setTextSize(2);
  tft.setTextColor(WHITE, BLACK);
  tft.print("Tempo:" + tempo);

  tft.setCursor(10, 280);
  tft.setTextSize(2);
  tft.setTextColor(WHITE, BLACK);
  tft.print("DH:" + dh_restante + " L");


   tft.setCursor(10, 165);
   tft.setTextColor(RED);
   tft.print("Valvula Aberta");
   
  /*if (bolinha == 1) {
    tft.fillCircle(170, 285, 10, BLUE);//bolinha aparece
   

  } else {

    tft.fillCircle(170, 285, 10, BLACK);//bolinha some
    tft.fillRoundRect(9, 163, 180, 31, 5, BLACK); //apagando msg  valvula

  }*/
}

/*void atualiza_agua_ejetada(){

  float total=litrosEjetados+quantAguaEjetadaTotal;
  for(int i=1;total>quantAguaEjetadaTotal;i++){
    quantAguaEjetadaTotal+=litrosEjetados/20;

    tft.setCursor(10,80);
    tft.setTextSize(5);
    tft.setTextColor(WHITE,BLACK);
    tft.fillRoundRect(10,80,180,50,5,BLACK);//APAGANDO O QUANT AGUA  ANTERIOR
    tft.setCursor(10,80);
    tft.print(String(quantAguaEjetadaTotal).charAt(0));
    tft.print(String(quantAguaEjetadaTotal).charAt(1));
    tft.print(String(quantAguaEjetadaTotal).charAt(2));
    tft.print(String(quantAguaEjetadaTotal).charAt(3));
    tft.print(String(quantAguaEjetadaTotal).charAt(4));
    tft.print(String(quantAguaEjetadaTotal).charAt(5));
    delay(100);

  }
  }*/

  /*else if (digitalRead(botaoConfirma) == LOW) {
    if (homeScreenOn == true) {
      homeScreenOn = false;
      mostrar_configuracao("");//passando valor do tempo vazio
    } else {
      homeScreenOn = true;
      HomeScreen(String(2000), String(5.22), String(309), String(3.9), String(ultima_temperatura), String(510), String(41));
    }
    delay(300);
  }*/

  /*
    float leitura=ler_temperatura();

    ler_temperaturas_da_memoria();
    if(leitura>=auxTemperatura){
    atualiza_temperatura(String(leitura),String(tmin), String(tmax), true);
    }else{
    atualiza_temperatura(String(leitura),String(tmin), String(tmax), false);
    }
    auxTemperatura=leitura;
    delay(3000);

  */


  /**
    litrosEjetados=0;
    int minutos=0;
    int segundos=0;

    float demandaHidricaInicial=510;
    float demandaHidrica;
    bool bolinha=false;
    Serial.println(demandaHidrica);
    for(float i=1;demandaHidrica>=0;i++){
    segundos++;
    if(segundos>=60){
     segundos=0;
     minutos++;
    }
    litrosEjetados+=1.22;
    demandaHidrica=demandaHidricaInicial-litrosEjetados;
    if(bolinha==false){
     bolinha=true;
    }else{
     bolinha=false;
    }
    String segundosString;
    String minutosString;
    if(segundos<=9){
    segundosString=String("0"+String(segundos));
    }else{
     segundosString=String(String(segundos));
    }
    if(minutos<=9){
    minutosString=String("0"+String(minutos));
    }else{
     minutosString=String(String(minutos));
    }
    atualiza_tela_irrigacao(String(litrosEjetados),String(demandaHidrica), minutosString, segundosString, "1.38",bolinha);
    delay(1000);
    }

    // fim_irrigacao(litrosEjetados);
  */


/* void Tela_irrigando(String quantAguaEjetada,String ETc,String vazao, String Ra, String minutos, String segundos, String demandaHidrica){

   tft.fillScreen(BLACK);

   settingsButton.initButton(&tft, 120,20,    // x, y, w, h, outline, fill, text
                     230,30, WHITE, 0x03E0, WHITE,
                     "Irrigando", 2);

   settingsButton.drawButton();//cabeÃ§alho

   tft.setCursor(10, 50);
   tft.setTextColor(WHITE, BLACK);
   tft.setTextSize(2);
   tft.print("Vazao");
   tft.setTextColor(YELLOW, BLACK);
   tft.print("("+vazao+" L/seg)");

   tft.setCursor(10,80);
   tft.setTextSize(5);
   tft.setTextColor(WHITE,BLACK);
   tft.print(quantAguaEjetada + "L");
   tft.setCursor(185,120);
   tft.setTextSize(1);
   tft.print("(Ejetado)");

   tft.setCursor(10,130);
   tft.setTextSize(2);
   tft.setTextColor(WHITE,BLACK);
   tft.print("Tempo:"+minutos+":"+segundos);
   //tft.setCursor(10,150);
   //tft.print("Temp:"+temperatura+" C");

   tft.setCursor(10, 200);
   tft.setTextColor(WHITE, BLACK);
   tft.setTextSize(2);
   tft.print("ETc");
   tft.setTextColor(GREEN, BLACK);
   tft.print("(Ra: "+Ra+" mm)");

   tft.setCursor(10,280);
   tft.setTextSize(2);
   tft.setTextColor(WHITE,BLACK);
   tft.print("DH:"+demandaHidrica+" L");


   //tft.fillCircle(170, 135, 10, GREEN);

   tft.setCursor(10,230);
   tft.setTextSize(5);
   tft.setTextColor(WHITE,BLACK);
   tft.print(ETc + " mm");

   tft.fillCircle(170, 285, 10, BLUE);
  }*/
/*void mostrar_configuracao(String tempo) {

  tft.fillScreen(BLACK);

  settingsButton.initButton(&tft, 120, 20,   // x, y, w, h, outline, fill, text
                            230, 30, WHITE, 0x03E0, WHITE,
                            "JUDITE", 2);

  settingsButton.drawButton();//cabeÃ§alho
  tft.setCursor(10, 50);
  tft.setTextColor(YELLOW, BLACK);
  tft.setTextSize(2);
  tft.print(tempo);

}*/
