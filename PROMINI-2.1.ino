#include <DallasTemperature.h>
#include <EEPROM.h>
#include <RTClib.h>
#include <TimeLib.h>
#include <TimeAlarms.h>


#define ONE_WIRE_BUS 9 // Sennsor de Temperatura
#define RelePin  8 // pino ao qual o Módulo Relé está conectado
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensorTemperatura(&oneWire);
RTC_DS1307 rtc;

int contaPulso; //Variável para a quantidade de pulsos

float temperatura;
float tmin=0.00;
float tmax=0.00;

//ENTRADAS/VARIAVEIS DO CALCULO
int latGraus;
int latMin;
float latitude;//latitude do campo em graus
float tamanho;//tamanho do campo em m2
float coeficiente[5]; //vetor de coeficiente guarda o kc da cultura
int fase_feno[5];// fase fenologica da cultura correspondente a cada kc
int fase_inicial;// fase fenologica inicial da cultura quando começou a irrigacao
int eficiencia; //85% a95% na irrigacao localizada
int horas_irrigacao[2]={-1,-1}; 
int minutos_irrigacao[2]={-1,-1};
float lamina_fixa;
int modo_sistema; //0 representa modo lamina fixa e 1 representa modo turno fixo

// calculo
float Ra=0;
float ETc=0;
float demanda_hidrica=0;
int fase_feno_atual=0;
float total_agua=0;//calculo e salvamento
float media_agua=0;////calculo e salvamento

//irrigacao
bool irrigacaoOn=false;
String tempo_irrigacao;
float vazao;
float agua_ejetada;

void setup() {
  Serial.begin(9600);
  sensorTemperatura.begin(); //Inicia o objeto da biblioteca do Dallas
  pinMode(2, INPUT);  //sensor de fluxo
  attachInterrupt(0, incpulso, RISING); //Configura o pino 2(Interrupção 0) interrupção

  pinMode(RelePin, OUTPUT); // Rele com a solenoide

  if(! rtc.begin()){
    Serial.println("Falha no RTC");
    while(1);
  }
  if(! rtc.isrunning()){
    Serial.println("RTC nao esta rodando");
  }

  //só precisa rodar uma vez
  //rtc.adjust(DateTime (2019, 03, 28, 13, 57, 0));//ajuste da hora 
  // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));//auto update from computer time

  DateTime now = rtc.now();
  setTime(now.hour(),now.minute(),now.second(),now.day(),now.month(),now.year());// ajustando o o relógio do sistema com o rtc

  Alarm.timerRepeat(5, atualizar_temperatura); //implementação que define que a cada x seg deve atualizar a temperatura 
  Alarm.timerRepeat(10, atualizar_homeScreen); //implementação que define que a cada x seg deve enviar os dados pela porta serial
  Alarm.timerRepeat(15, calcular_demanda_hidrica); //implementação que define que a cada x seg deve calcular a dh, Ra, Etc e Fase feno atual
  //Alarm.alarmRepeat(16,0,0, irrigar_campo); //implementação para calcular a ETc e molhar em um determinado horário. 
  Alarm.alarmRepeat(0,0,0, contar_dias_irrigacao);  //implementação para incrementar a variável que conta os dias de irrigaçao: após a meia noite a variavel incrementa na memória

  iniciar_variaveis();

}
void serialEvent(){
  String c=Serial.readString();
  receber(c);
}
void iniciar_variaveis(){
   
   EEPROM.get(81, latitude);
   EEPROM.get(85, latGraus);
   EEPROM.get(88, latMin);
   EEPROM.get(11, tamanho);
   EEPROM.get(45, fase_inicial);
   EEPROM.get(59, eficiencia);
   EEPROM.get(75, modo_sistema);
   EEPROM.get(77, lamina_fixa);
   
      int j=15;
      for(int i=0;i<5;i++){
      
           EEPROM.get(j, coeficiente[i]);
           j+=4;   
       }
      
      j=35;
       for(int i=0;i<5;i++){
           EEPROM.get(j, fase_feno[i]);
           j+=2;
       
      }
      j=61;
      for(int i=0;i<2;i++){
       
           EEPROM.get(j, horas_irrigacao[i]);
           EEPROM.get(j+4, minutos_irrigacao[i]);
           j+=2;
      }
  
  //definindo alarmes dos horarios de irrigacao
  if(modo_sistema==1){//se o sistema tiver no modo turno fixo
    Serial.println("Modo Turno Fixo Ativado");
    if(horas_irrigacao[0]>=0&&horas_irrigacao[0]<24){
       Alarm.alarmRepeat(horas_irrigacao[0],minutos_irrigacao[0],0, imprimir); 
    }if(horas_irrigacao[1]>=0&&horas_irrigacao[1]<24){
       Alarm.alarmRepeat(horas_irrigacao[1],minutos_irrigacao[1],0, imprimir); 
    }
  } 

      int flag;
      EEPROM.get(73, flag);
        if(flag!=1){
          total_agua=0;
          media_agua=0;
        }else{
           EEPROM.get(51, total_agua);
           EEPROM.get(55, media_agua); 
        }   
  
  /*Serial.println("--------------- ---------");
  Serial.print("latitude: ");
  Serial.println(latitude);
  Serial.print("Tamanho: ");
  Serial.println(tamanho);
  Serial.print("Fase Inicial: ");
  Serial.println(fase_inicial);
  Serial.print("Eficiencia: ");
  Serial.println(eficiencia);

  for(int i=0;i<5;i++){
    Serial.print("KC: ");
    Serial.print(coeficiente[i]);
    Serial.print(" F: ");
    Serial.println(fase_feno[i]);
  }
  for(int i=0;i<2;i++){
    
    Serial.print(horas_irrigacao[i]);
    Serial.print(" : ");
    Serial.println(minutos_irrigacao[i]);
  }*/
  
  
}
void imprimir(){
  Serial.println("Alarm horario");
}
void salvar_variaveis(){
  EEPROM.put(81, latitude);// byte 81 a 84
  EEPROM.put(85, latGraus);
  EEPROM.put(88, latMin);
  EEPROM.put(11, tamanho);// 
  EEPROM.put(45, fase_inicial);
  EEPROM.put(59, eficiencia);
  EEPROM.put(75, modo_sistema);
  EEPROM.put(77, lamina_fixa);
  

 int j=15;
  for(int i=0;i<5;i++){
    EEPROM.put(j, coeficiente[i]);
    j+=4;
  }

  j=35;
  for(int i=0;i<5;i++){
    EEPROM.put(j, fase_feno[i]);
    j+=2;
  }
  
  j=61;
  for(int i=0;i<2;i++){
    EEPROM.put(j, horas_irrigacao[i]);
    EEPROM.put(j+4, minutos_irrigacao[i]);
    j+=2;
  }
  
}
void reiniciar_quant_agua(){
  EEPROM.put(73, 0);  // REINICIADA
}
void salvar_quant_agua(){
  EEPROM.put(51, total_agua);
  EEPROM.put(55, media_agua);
  EEPROM.put(73, 1);  //SETANDO FLAG INDICANDO Q A MEMORIA FOI INICIADA
}
void receber(String c){
  int n=0;
  String cod;
    while(c.charAt(n)!='\n'){
      cod+=c.charAt(n);
      n++;
    }
     n++;//pulo o \n
   

   switch(cod.toInt()){

    case 27://atualizar data 
       receber_atualizacao_data(c, n);
       rtc.adjust(DateTime (2019,6,16,21,31,0));//ajuste da hora 
       break; 
     case 28://atualizar hora
       receber_atualizacao_hora(c, n);
       break; 
    case 29://receber latitude
       receber_latitudeGraus(c, n );//passar a string e a posicao que parou de ler 
       salvar_variaveis();
      //desnecessario iniciar as variaveis??
       break; 
    case 30://receber latitude
       receber_latitudeMin(c, n );//passar a string e a posicao que parou de ler 
       salvar_variaveis();
       break; 
     case 31://receber tamanho
       receber_tamanho(c, n );//passar a string e a posicao que parou de ler
       salvar_variaveis();
       //iniciar_variaveis(); 
       break; 
     case 32:
       receber_kc(c,n);
       salvar_variaveis();
       //iniciar_variaveis();
       break;
      case 33:
       receber_fase_feno(c,n);
       salvar_variaveis();
      // iniciar_variaveis();
       break;
     case 34:
       receber_fase_inicial(c,n);
       salvar_variaveis();
       //iniciar_variaveis();
       break;
     case 35:
       receber_eficiencia(c,n);
       salvar_variaveis();
       //iniciar_variaveis();
       break;
     case 36:
       receber_horas_irrigacao(c,n);
       salvar_variaveis();
       //iniciar_variaveis();
       break;
     case 37:
       receber_minutos_irrigacao(c,n);
       salvar_variaveis();
       //iniciar_variaveis();
       break;
     case 38:
       receber_lamina(c,n);
       salvar_variaveis();
       //iniciar_variaveis();
       break;
     case 40:
       receber_modo_sistema(c,n);
       salvar_variaveis();
       //iniciar_variaveis();
       break;
     case 50:
       reiniciar_temperaturas();
       break;
     //default:
        //Serial.print("default");
     
   }
 
}



void reiniciar_temperaturas(){
  EEPROM.write(8, 0);//seta 0 na flag indicando q as memorias serao reiniciadas
}
void loop() {

   //atualizar_temperatura();
   //delay(3000);
   //mostrar_relogio();
   //
   Alarm.delay(100); //implementacao para o alarm funcionar
 
}            
void atualizar_temperatura(){
     temperatura=ler_temperatura();//lendo e guardando maxima e minima
     ler_temperaturas_da_memoria();//preenchendo tmin e tmax
}

void guardar_temperatura(float temperatura){//função para guardar as temperaturas máxima e mínima na memória EEPROM

  //verificar se ja existem leituras
  int flag=EEPROM.read(8);
  //flag=0;                 //reiniciar manualmente
  if(flag==1){           //ja existem valores de temperatura na memoria, realizar comparações
    float tmin=0.00;
    float tmax=0.00;
    EEPROM.get(0, tmin);
    EEPROM.get(4, tmax);
    if(temperatura<tmin){
      EEPROM.put(0, temperatura);
      
    }
    if(temperatura>tmax){
      EEPROM.put(4, temperatura);
     
    }
    
  }else{//não existem valores de temperatura na memoria, preencher tmin e tmax com o valor lido
     EEPROM.put(0, temperatura);//setando valor de tmin
     EEPROM.put(4, temperatura);//setando valor da tmax
     //int valor=1;
     EEPROM.write(8, 1); //setar a flag com valor '1' indicando que tmin e tmax foram inciadas
    
  }
  
}

float ler_temperatura(){//função para o sensor de temperatura realizar a coleta dos dados
  
  sensorTemperatura.requestTemperatures();
  float leitura=sensorTemperatura.getTempCByIndex(0);
  guardar_temperatura(leitura); // chama a função para guardar as novas leituras, se for necessário
 // ler_temperaturas_da_memoria(); // ler temperatura da memória
  return leitura;
}

float ler_temperaturas_da_memoria(){//função para ler as temperaturas máxima e mínima salvas na memória EEPROM
  int flag;
  flag=EEPROM.read(8);
  if(flag!=1){//nao ha temperaturas registradas
    float tmin=0.00;
   float tmax=0.00;
  }else{
  EEPROM.get(0, tmin);
  EEPROM.get(4, tmax); 
  }
}

String mostrar_relogio(){
  
  String daysOfTheWeek[7] = {"Domingo", "Segunda", "Terca", "Quarta", "Quinta", "Sexta", "Sabado"};
  DateTime now = rtc.now();

  String tempo(String(now.day())+"/"+String(now.month())+"/"+String(now.year())+" "+String(now.hour())+":"+String(now.minute())+":"+String(now.second())+":"+" "+daysOfTheWeek[now.dayOfTheWeek()]);
  return tempo;
 // Serial.println("20");//codigo para enviar tempo
 // Serial.println(tempo);
  

}
void contar_dias_irrigacao(){// função que faz a contagem de dias na memória EEPROM
  int flag;
  EEPROM.get(71, flag);// ler valor da flag
    if(flag!=1){ // se flag diferente de 1, memória ainda não iniciada
       EEPROM.write(69, 1); //contagem de dias inicia 
      EEPROM.write(71, 1);//seta a flag com valor 1, indicando que a memoria foi iniciada
    }else {       // memória já iniciada
       int contDias;
       EEPROM.get(69, contDias);
       contDias++;
       EEPROM.write(69, contDias); //incrementa a contagem de dias
    }
 }
  void reiniciar_contador_dias_irrigacao(){
     EEPROM.write(71, 0); 
 }

 float selecionar_KC(){
      //busca na memória quantidade de dias transcorrido na irrigação
      int contDiasIrrigacao;
      int flag;
      EEPROM.get(71, flag);
        if(flag!=1){
          contDiasIrrigacao=0;
        }else{
          EEPROM.get(69,contDiasIrrigacao);
        }     
     //double coeficientes_cultura[3]={0.69, 0.90, 1.04}; //coeficientes da cultura equivalentes a cada fase fenologica
     //int fases_fenologicas[3]={35,60,80}; //fase fenologicas em dias 
     int fase_feno_atual=fase_inicial + contDiasIrrigacao; //fase fenologica atual
     
     for(int i=0;i<sizeof(fase_feno);i++){
      if(fase_feno_atual<=fase_feno[i]){
        float KC=coeficiente[i];
        //Serial.println(contDiasIrrigacao);
        //Serial.println(KC);
        return KC;
      }
     }
     
}
float calcular_Ra(){
  int dia_juliano = calcular_diaJuliano(); //J na equação de Ra
  float dr_terra_sol; //distancia relatica terra sol
  float pi = 3.14159;
  float declinacao_solar; //declinação solar
  float angulo_hr_por_do_sol;// angulo horario do por do sol
  float latitude_radianos; //latitude em radianos
  //float Ra; //estimativa de radiacao
   
  
    dr_terra_sol=1+(0.033*cos((2*pi/365)*dia_juliano)); //calculando a distancia relativa terra sol
    declinacao_solar=0.4093*sin(((2*pi*dia_juliano)/365)-1.405);//calculando declinação solar
    latitude_radianos=(pi/180)* latitude *(-1);//convertendo latitude em graus para radianos e multiplicando por -1, considerando a latitude negativa para o Hemisfério Sul
    angulo_hr_por_do_sol=acos(-tan(latitude_radianos)*tan(declinacao_solar)); //calculando o angulo horario do por do sol, para o hemisfério sul a latitude tem sinal negativo
  //ESTIMANDO Ra NO TOPO DA ATMOSFERA
   Ra=37.586*dr_terra_sol*((angulo_hr_por_do_sol*sin(latitude_radianos)*sin(declinacao_solar))+(cos(latitude_radianos)*cos(declinacao_solar)*sin(angulo_hr_por_do_sol)));//calculando Ra estimada
   //Serial.println(" \nRadiacao: ");
   //Serial.print(Ra);
   //Serial.print(" MJ m dia ou ");
   //Serial.print(Ra*0.408);
  // Serial.print(" mm/dia ");
  Ra=(0.408*Ra); // (0.408*Ra) convertendo de MJ para mm por dia
  return Ra;
 
}
float calcular_ETo(){ //função para calcular a ( ETo ) Evapotranspiração de Referência
  
  float Ra= calcular_Ra(); //estimativa de radiacao
  float ETo;   
  
//CALCULANDO ETo
  float tmin=0.00;
  float tmax=0.00;
  EEPROM.get(0, tmin);  //pegando a temperatura mínima do dia registrada na memória
  EEPROM.get(4, tmax);  //pegando a temperatura máxima do dia registrada na memória
  float tmed=(tmax+tmin)/2;
  ETo=0.0023*(tmed+17.8)*(pow((tmax-tmin),0.5))*Ra; // 
  //Serial.println("\nEto: ");
 // Serial.print(ETo);
  //Serial.print(" mm/dia ");
  return ETo;
}

int calcular_diaJuliano(){
  int diasNoMes[] = {31,28,31,30,31,30,31,31,30,31,30,31};
  int diaJuliano=0;
  DateTime now = rtc.now();

  //acrescentando mais um dia a fevereiro caso o ano seja bissexto. Regra do Ano bissexto: multiplo de 4 e não-multiplo de 100 ou multiplo de 4 e multiplo de 400
  if (now.year()%4  == 0) {
    if (now.year()%100 != 0) {
      diasNoMes[1] = 29;
    }
    else {
      if (now.year()%400 == 0) {
        diasNoMes[1] = 29;
      }
    }
   }
  
    for(int i=0;i<now.month()-1;i++){
      diaJuliano=diaJuliano + diasNoMes[i];
      
    }    
   diaJuliano=diaJuliano + now.day();

   //Serial.print(diaJuliano);
    return diaJuliano;   
}
int quant_dias_irrigacao(){
  
   int flag, contDiasIrrigacao;
      EEPROM.get(71, flag);
        if(flag!=1){
          contDiasIrrigacao=0;
        }else{
          EEPROM.get(69,contDiasIrrigacao);
        }     
        return contDiasIrrigacao;
}
void calcular_demanda_hidrica(){
  
  float ETo=calcular_ETo();//Evapotranspiração de Referência em mm/dia
  float KC=selecionar_KC(); // Chamando função para selecionar o Coeficiente da Cultura passando a fase fenologica atual da cultura no momento do 1o dia da irrigação 
  ETc=ETo*KC;
  float Li=(100*ETc)/eficiencia;  //Lâmina dágua para cada evento, dada em mm/m² (cada mm/m² equivale a 1L de água)
  demanda_hidrica=Li*tamanho;// Quantidade de Litros de água necessários na irrigação
     
  fase_feno_atual=fase_inicial + quant_dias_irrigacao(); //fase fenologica atual
    
  if(modo_sistema==true){//se o sistema tiver no modo lamina fixa
    if(ETc>=lamina_fixa){
      irrigar();
    }   
  }
  

 
}
void atualizar_homeScreen(){

  String kcString, faseString, horarioString;
  
  for(int i=0;i<5;i++){
    kcString+=String(coeficiente[i])+String(" ");
    faseString+=String(fase_feno[i])+String(" ");
  }
  for(int i=0;i<2;i++){
    horarioString+=String(horas_irrigacao[i])+String(":")+String(minutos_irrigacao[i])+String(" ");
  }
  
  Serial.println(temperatura,1);
  Serial.println(tmax,1);
  Serial.println(tmin,1);
  Serial.println(Ra);
  Serial.println(ETc);
  Serial.println(demanda_hidrica);
  Serial.println(fase_feno_atual);
  Serial.println(total_agua);
  Serial.println(media_agua);
  Serial.println(mostrar_relogio());
  Serial.println(latGraus);
  Serial.println(tamanho);
  Serial.println(kcString);
  Serial.println(faseString);
  Serial.println(eficiencia);
  Serial.println(horarioString);
  Serial.println(quant_dias_irrigacao());
  Serial.println(modo_sistema);
  if(modo_sistema==1){
    Serial.println(0);
  }else{
    Serial.println(lamina_fixa,1);
  }
   Serial.println(latMin);
 /* if(irrigacaoOn==true){
      //Serial.println("20");//codigo
       Serial.println(vazao,1); //vazao
       Serial.println(tempo_irrigacao);//tempo passado
       Serial.println(agua_ejetada,1);//quant agua ejetada
       Serial.println((demanda_hidrica-agua_ejetada),1);//demanda hidrica restante
      
  }else{
       Serial.println(0); //vazao
       Serial.println(0);//tempo passado
       Serial.println(0);//quant agua ejetada
       Serial.println(0);//demanda hidrica restante
  }*/
}
