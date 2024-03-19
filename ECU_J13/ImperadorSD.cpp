/*

  SD_IMPERADOR.hpp - Arquivo para programação da biblioteca SD_IMPERADOR.
  Criado em 14/07/2023 pelo Lambari para implementar no J12 até novembro e servir como base para o J13.

*/

#include "ImperadorSD.hpp"
#include "defines.hpp"

const uint8_t BASE_NAME_SIZE = sizeof(FILE_BASE_NAME) - 1;
char diretorio[] = FILE_BASE_NAME "00.csv";

ImperadorSD::ImperadorSD():
numero_arquivo(0)
{}

ImperadorSD::~ImperadorSD(){}

bool ImperadorSD::iniciar(){

  Serial.println("Iniciando SD");

  pinMode(15, OUTPUT);
  SPI.begin(SD_CLK, SD_MISO, SD_MOSI, SD_CS);


  if(!SD.begin()){
    Serial.println("Card Mount Failed");
    return false;
  }

  
  iniciado = true;

  uint8_t cardType = SD.cardType();

  
  if(cardType == CARD_NONE){
      Serial.println("No SD card attached");
      return false;
  }


  while (SD.exists(diretorio)) {
    
    if (diretorio[BASE_NAME_SIZE + 1] != '9') {
      diretorio[BASE_NAME_SIZE + 1]++;
    } else if (diretorio[BASE_NAME_SIZE] != '9') {
      diretorio[BASE_NAME_SIZE + 1] = '0';
      diretorio[BASE_NAME_SIZE]++;
    } else {
      Serial.println(F("Não foi possivel criar o nome de arquivo."));
      return false;
    }
  }
    Serial.println(diretorio);
    writeFile(SD, diretorio, "Tempo;Velocidade;RPM;Freio;Temperatura;Bateria;Aceleracao X;Aceleracao Y;Aceleracao Z\n");
    Serial.println("SD INICIADO");
    numero_arquivo = atoi(&diretorio[BASE_NAME_SIZE]);

  return true;

}

void ImperadorSD::listDir(fs::FS &fs, const char * dirname, uint8_t levels){
    Serial.printf("Listing directory: %s\n", dirname);

    File root = fs.open(dirname);
    if(!root){
        Serial.println("Failed to open directory");
        return;
    }
    if(!root.isDirectory()){
        Serial.println("Not a directory");
        return;
    }

    File file = root.openNextFile();
    while(file){
        if(file.isDirectory()){
            Serial.print("  DIR : ");
            Serial.println(file.name());
            if(levels){
                listDir(fs, file.path(), levels -1);
            }
        } else {
            Serial.print("  FILE: ");
            Serial.print(file.name());
            Serial.print("  SIZE: ");
            Serial.println(file.size());
        }
        file = root.openNextFile();
    }
}

void ImperadorSD::readFile(fs::FS &fs, const char * path){
    Serial.printf("Reading file: %s\n", path);

    File file = fs.open(path);
    if(!file){
        Serial.println("Failed to open file for reading");
        return;
    }

    Serial.print("Read from file: ");
    while(file.available()){
        Serial.write(file.read());
    }
    file.close();
}

void ImperadorSD::writeFile(fs::FS &fs, const char * path, const char * message){
    Serial.printf("Writing file: %s\n", path);
    Serial.printf("Mensagem: %s\n", message);

    File file = fs.open(path, FILE_WRITE);
    if(!file){
        Serial.println("Failed to open file for writing");
        return;
    }
    if(file.print(message)){
        //Serial.println("File written");
    } else {
        //Serial.println("Write failed");
    }
    file.close();
}

void ImperadorSD::appendFile(fs::FS &fs, const char * path, const char * message){
    Serial.printf("Appending to file: %s\n", path);

    File file = fs.open(path, FILE_APPEND);
    if(!file){
        Serial.println("Failed to open file for appending");
        return;
    }
    if(file.print(message)){
        //Serial.println("Message appended");
    } else {
        //Serial.println("Append failed");
    }
    file.close();
}

void ImperadorSD::escrever_dados(String dados){
  appendFile(SD, diretorio, dados.c_str());
}

int ImperadorSD::get_arquivo_atual(){
  return numero_arquivo;
}