/*

  SD_IMPERADOR.hpp - Arquivo para declaração da biblioteca SD_IMPERADOR.
  Criado em 14/07/2023 pelo Lambari para implementar no J12 até novembro e servir como base para o J13.

*/



#pragma once

#include <Arduino.h>

#include <SD.h>
#include <SPI.h>
#include <FS.h>
#include "ImperadorAcelerometro.hpp"

#define FILE_BASE_NAME "/Teste"
#define FREQUENCIA "20"


class ImperadorSD
{

  private:
    
    File arquivo;

    void escrever(const char* path, const char* mensagem);

    bool iniciado;

    void listDir(fs::FS &fs, const char * dirname, uint8_t levels);
    void readFile(fs::FS &fs, const char * path);
    void writeFile(fs::FS &fs, const char * path, const char * message);
    void appendFile(fs::FS &fs, const char * path, const char * message);

    int numero_arquivo;

  public:
    
    ImperadorSD();
    ~ImperadorSD();

    bool iniciar();

    void escrever_dados(String dados);
    int get_arquivo_atual();
  
};

