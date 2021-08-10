#include <Wire.h> //INCLUSÃO DA BIBLIOTECA
#include "RTClib.h" //INCLUSÃO DA BIBLIOTECA

RTC_DS3231 rtc; //OBJETO DO TIPO RTC_DS3231


bool isAlarme = false;

int botao = 7;
bool isBotao = false;

int buzzer = 10;

int led = 4;

int segundoConta = 0;
int segundoAnterior = 0;
int segundoAgora = 0;
 
void setup()
{
  if(! rtc.begin()) { // SE O RTC NÃO FOR INICIALIZADO, FAZ
    Serial.println("DS3231 não encontrado"); //IMPRIME O TEXTO NO MONITOR SERIAL
    while(1); //SEMPRE ENTRE NO LOOP
  }
  
  Serial.println("DS3231 OK!"); //IMPRIME O TEXTO NO MONITOR SERIAL
  //REMOVA O COMENTÁRIO DE UMA DAS LINHAS ABAIXO PARA INSERIR AS INFORMAÇÕES ATUALIZADAS EM SEU RTC
  //rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); //CAPTURA A DATA E HORA EM QUE O SKETCH É COMPILADO
  //rtc.adjust(DateTime(2021, 7, 30, 9, 53, 00)); //(ANO), (MÊS), (DIA), (HORA), (MINUTOS), (SEGUNDOS)
   

  pinMode(botao, INPUT_PULLUP); // define o pino do botao como entrada "INPUT"
   
  Serial.begin(9600);


  pinMode(buzzer,OUTPUT);
  pinMode(led,OUTPUT);
}
 
void loop()
{
  DateTime now = rtc.now(); //CHAMADA DE FUNÇÃO

  apresentarHoraTerminal(now);

  if((now.hour() == 05 or now.hour() == 5) && now.minute() == 40){
    isAlarme = true;
  }

  Serial.println(isAlarme);

  
  if(digitalRead(botao)== LOW) // Se o botão for pressionado
  {
    isBotao = true;
    Serial.println("ligado");
    
  }else{
    isBotao = false;
    Serial.println("desligado");
  }


  if(isAlarme){
    if(isBotao){
      digitalWrite(led,HIGH);
      segundoAgora = now.second();
      if(segundoAnterior != segundoAgora){
        segundoAnterior = segundoAgora;
        segundoConta += 1;

        Serial.println(segundoConta);
        
        if(segundoConta > 240){
          segundoConta = 0;
          isAlarme = false;
          digitalWrite(led,LOW);
        }
      }
    }else{
      tocaAlarme();
    }
  }else{
    delay(900);
  }
  
  Serial.println("  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -");
  delay(500);
}

void tocaAlarme(){
  
  digitalWrite(led,HIGH);
  tone(buzzer,2500);   
  delay(250);
   
  noTone(buzzer);
  delay(250);   

  digitalWrite(led,LOW);
  tone(buzzer,2500);   
  delay(250);
   
  noTone(buzzer);
  delay(250);  
}

void apresentarHoraTerminal(DateTime now){
  Serial.print("Data: ");
  Serial.print(now.day(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.year(), DEC);
  Serial.print(" / Dia: ");
  Serial.print(" / Horas: ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();
}
