/*

  ECU.hpp - Biblioteca principal do sistema.
  Criado em 14/07/2023 pelo Lambari para implementar no J12 até novembro e servir como base para o J13.

  Essa biblioteca possui o código Procedural.

*/

#pragma once 

#include <Arduino.h>

#include "Veiculo.hpp"
#include "ImperadorGPS.hpp"
#include "ImperadorAcelerometro.hpp"
#include "ImperadorIR.hpp"
#include "ImperadorSD.hpp"
#include "ImperadorCAN.hpp"

enum Modo{
  DEV,
  CORRIDA,
  APRESENTACAO
};

class ECU
{

  private:

    Veiculo baja;
    ImperadorGPS gps;
    ImperadorAcelerometro acelerometro;
    ImperadorIR infravermelho;
    ImperadorSD sd;
    ImperadorCAN can;

    unsigned long int timer_lora;
    unsigned long int timer_atualizacoes;

    bool box_state;

    Modo modo;

    int16_t rpm;
    int8_t velocidade;
    int temperatura;
    int bateria;
    bool freio;
    Vetor3 aceleracao;
    Vetor3 giro;

    void configurar_pinos();
    void iniciar_serial();

    void adquirir_dados();
    void tratar_dados();

    void enviar_dados();
    void enviar_serial();
    void enviar_CAN();


    void receber_dados();
    void verificar_serial();
    void verificar_CAN();

    bool dev_ativo();
    bool corrida_ativo();
    bool apresentacao_ativo();

  public:

    ECU();
    ~ECU();

    void configurar();
    void executar();

    void atualizar();

    void atualizar_eixo_traseiro();
    void atualizar_rpm();

};