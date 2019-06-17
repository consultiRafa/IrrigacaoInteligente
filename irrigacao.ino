void irrigar(){
  irrigacaoOn=true;
  controle_fluxo_de_agua(demanda_hidrica);//chamar a funcao q controla a saida de agua e acionar a valvula, passando a demanda de agua a ser ejetada
  reiniciar_temperaturas(); //reiniciar tmin e tmax após a irrigacao
}


void controle_fluxo_de_agua(float demanda_hidrica){//função para controlar a vazão de água no sistema. Quando a quantidade de água necessária passar pelo sensor, a válvula solenóide deve ser desligada. 
  int a=0;
   vazao; //Variável para armazenar o valor em L/min
  float media = 0; //Variável para fazer a média
  int i = 0; //Variável para segundos
  int minutos = 00; //Variável para minutos
  agua_ejetada = 0; //Variável para Quantidade de agua
  float miliLitros = 0; //Variavel para Conversão
  //bool piscaBolinha=true;
  
  digitalWrite(RelePin, LOW); //aciona a valvula
  while(a==0){
      contaPulso = 0;//Zera a variável
      sei(); //Habilita interrupção
      delay (1000); //Aguarda 1 segundo
      cli(); //Desabilita interrupção
      
      vazao = contaPulso / 5.5; //Converte para L/min
       media = media + vazao; //Soma a vazão para o calculo da media
      i++;
 
      miliLitros = vazao / 60;
      miliLitros = miliLitros*0.775; //fator de calibragem do setor. Ajuste de -225ml para cada 1L. Inicialmente o sensor tava superestimando a quantidade de agua que saia
      agua_ejetada = agua_ejetada + miliLitros ;

       tempo_irrigacao=String(minutos)+":"+String(i);
     
     // Neste conjunto de linhas fizemos a média das leituras obtidas a cada 1 minuto
     if (i == 60){
     minutos++;
     //Serial.println(minutos);
      
     if (minutos >= 60)
     {
     minutos = 0;
     }
     media = media / 60; //faz a média
    
     media = 0; //Zera a variável media para uma nova contagem
     i = 0; //Zera a variável i para uma nova contagem
     }
    
     if(agua_ejetada>=demanda_hidrica){ // condição estabelece que a válvula solenoide deve fechar quando o volume de água ejetado for igual a calculado necessário
      digitalWrite(RelePin, HIGH);  //desativa a valvula
      a++;                    //incrementa a variável 'a' para que o laço termine
      
     }
 }
  irrigacaoOn=false;
  total_agua+=agua_ejetada;//atualizando total de agua gasto no cultivo   
  unsigned int contador_irrigacao=quant_dias_irrigacao();
   if(contador_irrigacao==0){contador_irrigacao=1;}
  media_agua=total_agua/contador_irrigacao;//atualizando media de agua gasta por dia no cultivo
  salvar_quant_agua();
  
}
void incpulso ()
{
 contaPulso++; //Incrementa a variável de pulsos
}
