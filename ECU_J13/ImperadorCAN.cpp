/*
 *
 * ImperadorCAN.cpp
 *
 */

#include "defines.hpp"
#include "ImperadorCAN.hpp"
#include "CAN.h"

ImperadorCAN::ImperadorCAN():
  iniciado(false),
  tentativas_reconexao(0)
{
}

ImperadorCAN::~ImperadorCAN(){}

bool ImperadorCAN::iniciar()
{
  CAN.setPins(RX_CAN, TX_CAN);
  if(!CAN.begin(500E3))
  {
    Serial.println("CAN Falhou para inciar");
    iniciado = false;
    return false;
  }

  Serial.println("CAN OK");
  iniciado = true;
  return true;
}

void ImperadorCAN::enviar_rvf(int16_t rpm, int8_t velocidade, bool freio)
{
    if(!iniciado){
        return;
    }

    //Serial.println("Enviando Pacote RV");

    //Serial.println("Velocidade: " + String(velocidade) + "RPM: " + String(rpm));

    CAN.beginPacket(0x12);
    
    String rpm_string = String(rpm);
    String vel_string = String(velocidade);

    //Serial.println("For R");


    // FORMATO DA STRING ENVIADA Rrrrrvvf

    while (rpm_string.length() < 4)
    {
      rpm_string = "0" + rpm_string;
    }

    if(vel_string.length() < 2) vel_string = "0" + vel_string;

    CAN.write('R');

    for(int i = 0; i < 4; i++)
    {
        //Serial.print(rpm_string[i]);
        CAN.write(rpm_string[i]);
    }

    //Serial.println("For V");

    for(int i = 0; i < 2; i++)
    {
        //Serial.print(vel_string[i]);
        CAN.write(vel_string[i]);
    }

    //Serial.println();

    freio ? CAN.write('1') : CAN.write('0');

   //Serial.println("Finalizando");

    iniciado = CAN.endPacket();

    //Serial.println("Envio Finalizado");
}


void ImperadorCAN::enviar_btb(float tensao_bateria, double valor_temperatura, bool box)
{
    if(!iniciado){
        return;      
    }
    //Serial.println("Enviando BTB");

    CAN.beginPacket(0x12);

    //FORMATO DA STRING ENVIADA Cbbbtttb
    
    String bateria_string = String(tensao_bateria*10, 0);
    String temperatura_string = String(valor_temperatura, 0);
    char box_char;

    //Serial.println("ITOA");

    while(bateria_string.length() < 3)
    {
      bateria_string = "0" + bateria_string;
    }

    while(temperatura_string.length() < 3)
    {
      temperatura_string = "0" + temperatura_string;
    }

    //Serial.println("For B");

    CAN.write('C'); //Indetificador do pacote - C: Críticos

    for (int i = 0; i < 3; i++)
    {
        CAN.write(bateria_string[i]);           
    }    

    //Serial.println("For T");

    for(int i = 0; i < 3; i++)
    {
        CAN.write(temperatura_string[i]);
    }

    //Serial.print("Formatando Box");


    box ? box_char = '1' : box_char = '0';

    CAN.write(box_char);

    //Serial.println("Finalizando Packet");

    iniciado = CAN.endPacket();

    //Serial.println("Finalizado");
}

void ImperadorCAN::enviar_dhm(int diretorio, int hora, int minutos){
    if(!iniciado){
        return;      
    }
    //Serial.println("Enviando BTB");

    CAN.beginPacket(0x12);

    //FORMATO DA STRING ENVIADA Cbbbtttb
    
    String s_diretorio = String(diretorio);
    String s_hora = String(hora);
    String s_minutos = String(minutos);

    //Serial.println("ITOA");

    if(s_diretorio.length() < 2)
      s_diretorio = "0" + s_diretorio;

    if(s_hora.length() < 2)
      s_hora = "0" + s_hora;

    if(s_minutos.length() < 2)
      s_minutos = "0" + s_minutos;

    //Serial.println("For B");

    CAN.write('S'); //Indetificador do pacote - C: Críticos

    for (int i = 0; i < 2; i++)
    {
        CAN.write(s_diretorio[i]);           
    }    

    //Serial.println("For T");

    for(int i = 0; i < 2; i++)
    {
        CAN.write(s_hora[i]);
    }

    for(int i = 0; i < 2; i++)
    {
        CAN.write(s_minutos[i]);
    }

    //Serial.print("Formatan

    //Serial.println("Finalizando Packet");

    iniciado = CAN.endPacket();

    //Serial.println("Finalizado");
}


String ImperadorCAN::receber()
{
    if(!iniciado) return "error";

    String retorno;

    while(CAN.available())
    {
        retorno.concat(CAN.read());
    }

    return retorno;
}

bool ImperadorCAN::get_iniciado(){return iniciado;}
