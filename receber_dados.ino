
void serialEvent() {
  String c = Serial.readString();
  receber2(c);
  //tft.print(c);
}

void receber2(String c) {
  int n = 0;
  unsigned int cont=0;
  
  String temperatura, tmin, tmax, Ra, ETc, dh, ffa, total_agua, media_agua, tempo, latGraus, tam, kcString, faseString, eficiencia, horarioString, 
  contDias, modoSistema, lamina, latMin ;
  //String vazao, tempo_irrigacao, agua_ejetada, dh_restante;

  while(cont<20){
        while (c.charAt(n) != '\n') { //recebendo temperatura atual
          if( cont==0){temperatura += c.charAt(n);}
          else if(cont==1){tmax += c.charAt(n);}
          else if(cont==2){tmin += c.charAt(n);}
          else if(cont==3){Ra+= c.charAt(n);}
          else if(cont==4){ETc += c.charAt(n);}
          else if(cont==5){dh += c.charAt(n);}
          else if(cont==6){ffa += c.charAt(n);}
          else if(cont==7){total_agua += c.charAt(n);}
          else if(cont==8){media_agua += c.charAt(n);}
          else if(cont==9){tempo += c.charAt(n);}
          else if(cont==10){latGraus += c.charAt(n);}
          else if(cont==11){tam += c.charAt(n);}
          else if(cont==12){kcString += c.charAt(n);}
          else if(cont==13){faseString += c.charAt(n);}
          else if(cont==14){eficiencia += c.charAt(n);}
          else if(cont==15){horarioString += c.charAt(n);}
          else if(cont==16){contDias += c.charAt(n);}
          else if(cont==17){modoSistema += c.charAt(n);}
          else if(cont==18){lamina += c.charAt(n);}
          else if(cont==19){latMin += c.charAt(n);}
          
          
          
          n++;
        }
        n++;
        cont++;
      }
        n++;
     
          if (homeScreenOn == true) { //atualiza só se a home tiver ativa
            bool controle_temperatura;
          if (temperatura.toFloat() >= ultima_temperatura) {
            controle_temperatura=true;
          } else {
            controle_temperatura=false;
          }
          ultima_temperatura = temperatura.toFloat();
          atualizar_homeScreen(total_agua, ETc, media_agua, Ra, temperatura, tmax, tmin, controle_temperatura, dh, ffa, lamina);
        }else if(configOn==true){
          atualizar_mostrar_config(tempo, latGraus,tam, kcString, faseString, eficiencia, horarioString, contDias, lamina, latMin);
        }
  
}
