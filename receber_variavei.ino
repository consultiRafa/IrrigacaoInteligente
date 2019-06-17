
void receber_atualizacao_hora(String c, int n){

 String horas, minutos, segundos;
  while(c.charAt(n)!='\n'){
     if(n<=3){
      horas+=c.charAt(n);
     }else if(n>=4&&n<=5){
       minutos+=c.charAt(n);
     }else if (n>=6&&n<=7){
       segundos+=c.charAt(n);
     }
    n++;//pulando o '\n'
}

//DateTime now = rtc.now();
//rtc.adjust(DateTime (now.year(), now.month(), now.day(),horas.toInt(), minutos.toInt(), segundos.toInt()));//ajuste da hora 
}
void receber_atualizacao_data(String c, int n){

 String dia, mes, ano;
  while(c.charAt(n)!='\n'){
     if(n<=3){
      dia+=c.charAt(n);
     }else if(n>=4&&n<=5){
       mes+=c.charAt(n);
     }else if (n>=6&&n<=9){
       ano+=c.charAt(n);
     }
    n++;//pulando o '\n'
}

//DateTime now = rtc.now();
//rtc.adjust(DateTime (ano.toInt(), mes.toInt(), dia.toInt(),now.hour(), now.minute(), now.second()));//ajuste da hora 
}
void receber_latitudeGraus(String c, int n){

 String valor;
  while(c.charAt(n)!='\n'){
      valor+=c.charAt(n);
       n++;
    }
    n++;//pulando o '\n'
    latGraus=valor.toInt();
    latitude=latGraus;
   
}

void receber_latitudeMin(String c, int n){

 String valor;
  while(c.charAt(n)!='\n'){
      valor+=c.charAt(n);
       n++;
    }
    n++;//pulando o '\n'
      latMin=valor.toInt();
      if(latMin!=0){
        float aux =latMin/60;
        latitude+=aux;
      }
      
}

void receber_tamanho(String c, int n){

 String valor;
  while(c.charAt(n)!='\n'){
      valor+=c.charAt(n);
       n++;
    }
    n++;//pulando o '\n'
    tamanho=valor.toFloat();
   
}

void receber_lamina(String c, int n){

 String valor;
  while(c.charAt(n)!='\n'){
      valor+=c.charAt(n);
       n++;
    }
    n++;//pulando o '\n'
    lamina_fixa=valor.toFloat();
   
}


void receber_modo_sistema(String c, int n){

 String valor;
  while(c.charAt(n)!='\n'){
      valor+=c.charAt(n);
       n++;
    }
    n++;//pulando o '\n'
    modo_sistema=valor.toInt();
    
}

void receber_kc(String c, int n){

 int i;
 String valor;
 String kc_recebido;


  while(c.charAt(n)!='\n'){//recebo a posicao do vetor
      valor+=c.charAt(n);
       n++;
    }
    n++;//pulando o '\n'
   
    kc_recebido=valor;
    valor="";
     
    while(c.charAt(n)!='\n'){//recebo primeiro o kc
      valor+=c.charAt(n);
       n++;
    }
    n++;//pulando o '\n'
    i=valor.toInt();
   
    coeficiente[i]=kc_recebido.toFloat();
    
}

void receber_fase_feno(String c, int n){
 int i;
 String valor;
 String fase_recebida;


  while(c.charAt(n)!='\n'){//recebo a posicao do vetor
      valor+=c.charAt(n);
       n++;
    }
    n++;//pulando o '\n'
   
    fase_recebida=valor;
    valor="";
     
    while(c.charAt(n)!='\n'){//recebo primeiro o kc
      valor+=c.charAt(n);
       n++;
    }
    n++;//pulando o '\n'
    i=valor.toInt();
   
    fase_feno[i]=fase_recebida.toInt();
}
    

void receber_horas_irrigacao(String c, int n){

 int i;
 String horas_recebida;
 String valor;
  while(c.charAt(n)!='\n'){//recebo a posicao do vetor
      valor+=c.charAt(n);
       n++;
    }
    n++;//pulando o '\n'

    horas_recebida=valor;

    valor="";
    while(c.charAt(n)!='\n'){//recebo primeiro as horas
      valor+=c.charAt(n);
       n++;
    }
    n++;//pulando o '\n'
    i=valor.toInt();
    horas_irrigacao[i]=horas_recebida.toInt();//hora

}


void receber_minutos_irrigacao(String c, int n){

 int i;
 String  minutos_recebido;
 String valor;
  while(c.charAt(n)!='\n'){//recebo a posicao do vetor
      valor+=c.charAt(n);
       n++;
    }
    n++;//pulando o '\n'

    minutos_recebido=valor;

    valor="";
    while(c.charAt(n)!='\n'){//recebo primeiro as horas
      valor+=c.charAt(n);
       n++;
    }
    n++;//pulando o '\n'
    i=valor.toInt();
    minutos_irrigacao[i]=minutos_recebido.toInt();//min

}

void receber_fase_inicial(String c, int n){

 String valor;
  while(c.charAt(n)!='\n'){
      valor+=c.charAt(n);
       n++;
    }
    n++;//pulando o '\n'
    fase_inicial=valor.toInt();

    //Serial.println("--");
    //Serial.println(fase_inicial);
}
void receber_eficiencia(String c, int n){

 String valor;
  while(c.charAt(n)!='\n'){
      valor+=c.charAt(n);
       n++;
    }
    n++;//pulando o '\n'
    eficiencia=valor.toInt();

    //Serial.println("--");
    //Serial.println(eficiencia);
}
