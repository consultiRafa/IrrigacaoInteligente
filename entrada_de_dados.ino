bool inserir_entrada( int cod, String titulo, String legenda, String digitos[], bool limparTela, int posCursorX, int posCursorY, int j) {

  if(limparTela==true){
    
       tft.fillScreen(BLACK);
     settingsButton.initButton(&tft, 120, 20,
                            230, 30, WHITE, 0x03E0, WHITE,
                            "Configurando", 2);
      settingsButton.drawButton();//cabecalho

  }

  bool fim=false;
  String entradaString;
  String digito = "0";
  int pos_digito = posCursorX;
  int i = 0;

  tft.setCursor(posCursorX, posCursorY);
  tft.setTextColor(WHITE, BLACK);
  tft.setTextSize(2);
  tft.print(titulo);

  tft.setTextSize(1);
  tft.print(legenda);
  tft.setTextSize(2);
    
   if(cod!=40&&cod!=27&&cod!=28){
    tft.setTextSize(4);
    }
    
  tft.setTextColor(YELLOW, BLACK);
  tft.setCursor(posCursorX, posCursorY+30);
  tft.print("0");

  while (fim==false) {

    if (digitalRead(botaoMais) == LOW) {
      if(cod==40){//modo sistema
          if(i==2){i=0;}
          else if(i==-1){i=1;}
       }else{
          if (i == 12) {
          i = 0;
        }
        else if (i == -1) {
          i = 11;
        }
       }

      digito = digitos[i];
      i++;

      tft.fillRoundRect(pos_digito, posCursorY+30, 240, 40, 5, BLACK); //APAGANDO
      tft.setCursor(pos_digito, posCursorY+30);
      tft.print(digito);
      delay(300);
    }
    if (digitalRead(botaoMenos) == LOW) {
       if(cod==40){//modo sistema
          if(i==2){i=0;}
          else if(i==-1){i=1;}
       }else{
          if (i == 12) {
          i = 0;
        }
        else if (i == -1) {
          i = 11;
        }
       }

      digito = digitos[i];
      i--;
      
      tft.fillRoundRect(pos_digito, posCursorY+30, 240, 40, 5, BLACK); //APAGANDO
      tft.setCursor(pos_digito, posCursorY+30);
      tft.print(digito);
      delay(300);
    }
    if (digitalRead(botaoConfirma) == LOW) {

      if (digito == "[OK]") {
       if((cod==30&&entradaString.toInt()>59)||(cod==29&&entradaString.toInt() > 90)||(cod==35&&(entradaString.toInt() < 60 || entradaString.toInt() > 85)) || //latitude e eficiencia
       (cod==36&&(entradaString.toInt()>23))|| (cod==37&&(entradaString.toInt()>59))){ //horas e minutos
         
            entradaString = "";
            pos_digito = 10;
            i = 9;
            digito = digitos[i];
            tft.fillRoundRect(pos_digito, posCursorY+30, 240, 40, 5, BLACK); //APAGANDO
            tft.setCursor(pos_digito, posCursorY+30);
            tft.print(digito);//INVALIDO

       }else{// tamanho, kc, fase feno, fase inicial, 
            fim=true;
       }
      }else{
          if(cod==40){//modo sistema
            entradaString= digito;
            fim=true;
          }else{
          entradaString += digito;
          i = 0;
          digito = digitos[i];
         
            if(cod==27||cod==28){//para e data e hora pois a fonte é menor
               pos_digito += 13;
            }else{
               pos_digito += 25;
            }
          tft.fillRoundRect(pos_digito, posCursorY+30, 240, 40, 5, BLACK); //APAGANDO
          tft.setCursor(pos_digito, posCursorY+30);
          tft.print(digito);
          }
      }
       
      delay(300);
    }

  }
  tft.fillRoundRect(10, posCursorY+30, 240, 40, 5, BLACK); //APAGANDO
  tft.setCursor(10, posCursorY+30);
  tft.setTextColor(GREEN, BLACK);
 if(cod==40){//modo sistema
      tft.println(entradaString);
      Serial.println(cod);     //enviando para o outro arduino pela porta serial
        if(entradaString=="[Lamina Fixa]"){
          Serial.println(0);
          return true;
        }else if(entradaString=="[Turno Fixo]"){
          Serial.println(1);
          return false;
        }  
  
 }else{
 
    if(cod==29||cod==30||cod==33||cod==34||cod==35||cod==36||cod==37){//latitude, fase feno, fase inicial, eficiencia, horas, minutos
      tft.println(entradaString.toInt());
      Serial.println(cod);     //enviando para o outro arduino pela porta serial
      Serial.println(entradaString.toInt());//mandando só a parte inteira 
   
    }else{                               //tamanho, kc e lamina fixa
      tft.println(entradaString);
      Serial.println(cod);     //enviando para o outro arduino pela porta serial
      Serial.println(entradaString);//mandando string completa incluindo o "."
    }

    if(cod==32||cod==33||cod==36||cod==37){    //kc e fase fenologica, horas e minutos
      Serial.println(j);    //enviar indice do vetor
    }
 }
    
}
