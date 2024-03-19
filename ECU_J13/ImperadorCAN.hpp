/*
 *
 *	ImperadorCAN.hpp - Biblioteca feita para utilizar o protocolo de comunicação CAN nas ECUs do veículo.
 *
 *	Código escrito pelo Lambari.
 *
 */

#pragma once

#include <Arduino.h>

class ImperadorCAN
{
  private:

    bool iniciado;

    int tentativas_reconexao;

	public:

		ImperadorCAN();
		~ImperadorCAN();

    bool iniciar();

		void enviar_rvf(int16_t rpm, int8_t velocidade, bool freio);
    void enviar_btb(float tensao_bateria, double valor_temperatura, bool box);
    void enviar_dhm(int diretorio, int hora, int minutos);

		String receber();

    bool get_iniciado();
};
