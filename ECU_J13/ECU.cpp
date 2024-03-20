/*

  ECU.cpp - Principal código do sistema.
  Criado em 14/07/2023 pelo Lambari para implementar no J12 até novembro e servir como base para o J13.

  Essa biblioteca possui o código Procedural.

*/

#include "ECU.hpp"
#include "defines.hpp"

ECU::ECU():
  gps(BAUD_GPS, RX_GPS, TX_GPS),
  modo(CORRIDA),
  box_state(false),
  timer_lora(millis()),
  timer_atualizacoes(millis())
{}

ECU::~ECU()
{}

void ECU::configurar()
{
  iniciar_serial();
  Serial.println("Sistema Baja Imperador");
  //if(dev_ativo())
    Serial.println("Serial iniciada");
  delay(1000);
  configurar_pinos();
  //Serial.println("Pinos Configurados");

  //acelerometro.iniciar();
  infravermelho.iniciar();
  can.iniciar();
}

void ECU::executar  ()
{
  //if(dev_ativo()) 
    //Serial.println("Atualizando");
  atualizar();
  //if(dev_ativo()) 
    //Serial.println("Enviando Dados");
  adquirir_dados();
  tratar_dados();
  enviar_dados();
  //if(dev_ativo()) 
    //Serial.println("Verificando Seriais");
  verificar_serial();
  //if(dev_ativo()) 
    //Serial.println("Enviando CAN novamente");
  enviar_CAN();
}




void ECU::configurar_pinos()
{
  pinMode(PINO_RODAD, INPUT);
  pinMode(PINO_RODAE, INPUT);
  pinMode(PINO_EIXOT, INPUT);
  pinMode(PINO_RPM, INPUT);

  pinMode(PINO_FREIO, INPUT);

  pinMode(PINO_LED, OUTPUT);

  digitalWrite(PINO_LED, HIGH);
}

void ECU::iniciar_serial()
{
  Serial.begin(BAUD_SERIAL);
  Serial2.begin(BAUD_SERIAL, SERIAL_8N1, RX_LORA, TX_LORA );
}

void ECU::enviar_dados()
{
  //Serial.println("Envio Lora");
  enviar_serial();
  //Serial.println("Envio CAN");
  enviar_CAN();
}

int modulo_int(int a){
  return a < 0 ? -a : a;
}

void ECU::adquirir_dados(){
  rpm = baja.get_rpm()/10;
  velocidade = baja.get_velocidade();
  temperatura = (int) (infravermelho.get_temperatura_objeto()*10);
  bateria = (int) (baja.get_bateria()*10);
  freio = baja.get_freio();
  
  //aceleracao = acelerometro.get_aceleracao();
  //giro = acelerometro.get_giro();

}

void ECU::tratar_dados(){
  // Tratamento dos dados

  if(rpm < 0)
    rpm = 0;
  rpm %= 1000;

  if(velocidade < 0)
    velocidade = 0;
  else if(velocidade > 99)
    velocidade = 99;
  velocidade %= 100;

  temperatura %= 10000;

  if(bateria < 0)
    bateria = 0;
  bateria %= 1000;
}

void ECU::enviar_serial()
{
  unsigned long int intervalo_envio = dev_ativo() || corrida_ativo() ? 1000 : 3000;

  if(millis() - timer_lora < intervalo_envio) return;

  timer_lora = millis();

  // Tratamento das Strings

  String s_rpm = String(rpm);
  String s_velocidade = String(velocidade);

  String s_temperatura = temperatura < 0 ? "-" : "+";
  s_temperatura += String(modulo_int(temperatura));

  String s_bateria = String(modulo_int(bateria));
  


  // Formato da String V**R***F*T+****B***

  switch(modo){
    case DEV:

      Serial.print("V");
      Serial.print(s_velocidade);
      Serial.print("R");
      Serial.print(s_rpm);
      Serial.print("F");
      Serial.print(freio);
      Serial.print("T");
      Serial.print(temperatura);
      Serial.print("B");
      Serial.println(s_bateria);

    
      Serial2.print("V");
      Serial2.print(s_velocidade); 
      Serial2.print("R");
      Serial2.print(s_rpm);
      Serial2.print("F");
      Serial2.print(freio);
      Serial2.print("T");
      Serial2.print(s_temperatura);
      Serial2.print("B");
      Serial2.println(s_bateria);
      
      break;
  
    case CORRIDA:
  
      Serial2.print("V");
      Serial2.print(s_velocidade);
      Serial2.print("R");
      Serial2.print(s_rpm);
      Serial2.print("F");
      Serial2.print(freio);
      Serial2.print("T");
      Serial2.print(s_temperatura);
      Serial2.print("B");
      Serial2.println(s_bateria);

      break;
  
    case APRESENTACAO:
  
      Serial.print("V");
      Serial.print(s_velocidade);
      Serial.print("R");
      Serial.print(s_rpm);
      Serial.print("F");
      Serial.print(freio);
      Serial.print("T");
      Serial.print(s_temperatura);
      Serial.print("B");
      Serial.println(s_bateria);

      Serial2.print("V");
      Serial2.print(s_velocidade);
      Serial2.print("R");
      Serial2.print(s_rpm);
      Serial2.print("F");
      Serial2.print(freio);
      Serial2.print("T");
      Serial2.print(s_temperatura);
      Serial2.print("B");
      Serial2.println(s_bateria);

      break;
  }

  //!dev
  
  

}

void ECU::enviar_CAN()
{ 
  //Serial.println("Enviando RVF");
  can.enviar_rvf(rpm*10, velocidade, freio);
  delay(50);  
  //Serial.println("Enviando BTB");
  can.enviar_btb(float(bateria/10.0) + 1.0, double((temperatura)/10), box_state);
  delay(50);

  can.enviar_dhm(sd.get_arquivo_atual(), gps.get_hora(), gps.get_minutos());
}

void ECU::verificar_serial(){
  if(Serial.available())
  {
    String comando = Serial.readString();

    //Serial.println("Comando Recebido = " + comando);

    if(comando == "DEV" || comando == "dev")
    {
      modo = DEV;
      Serial.println("DEV MODE ativado");
    }
    else if(comando == "CORRIDA" || comando == "corrida")
    {
      modo = CORRIDA;
    }
    else if(comando == "BOX")
    {
      box_state = !box_state;
      box_state ? Serial.println("BOX ON") : Serial.println("BOX OFF");
    }
    else if(comando == "APRESENTACAO" || comando == "apresentacao")
    {
      modo = APRESENTACAO;
    }
  }

  if(Serial2.available())
  {
    String comando = Serial2.readString();


    if(comando == "DEV" || comando == "dev")
    {
      modo = DEV;
      Serial2.println("DEV MODE ativado");
    }
    else if(comando == "BOX")
    {
      box_state = !box_state;
      box_state ? Serial2.println("BOX ON") : Serial2.println("BOX OFF");
    }
    else if(comando == "APRESENTACAO" || comando == "apresentacao")
    {
      modo = APRESENTACAO;
    }
    else if(comando == "CORRIDA" || comando == "corrida")
    {
      modo = CORRIDA;
    }
  }
}


void ECU::verificar_CAN(){}


void ECU::atualizar()
{
  baja.atualizar_freio();

  if(millis() - timer_atualizacoes < 1000)
    return;

  timer_atualizacoes = millis();

  gps.atualizar();
  infravermelho.atualizar();
  baja.atualizar();
  //acelerometro.atualizar();
}

bool ECU::dev_ativo()
{
  return modo == DEV;
}

bool ECU::corrida_ativo()
{
  return modo == CORRIDA;
}

bool ECU::apresentacao_ativo()
{
  return modo == APRESENTACAO;
}

void ECU::atualizar_eixo_traseiro()
{
  baja.calcular_velocidade();
} 

void ECU::atualizar_rpm()
{
  baja.calcular_rpm();
}