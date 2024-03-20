# ECU Central - Equipe Imperador

Repositório destinado para o desenvolvimento e controle de versões dos códigos utilizados na ECU Central. Além disso, também é utilizado para o controle das versões dos diagramas elétricos da ECU central.


## Documentação 
Toda a parte de código foi desenvolvida orientada a objetos, facilitando sua documentação. Permite uma maior legibilidade do código, permitindo uma rápida adaptção de futuros novos integrantes no subsistema.

O código é desenvolvido em C++ por ser uma linguagem de ótima performance, sendo ensinada para quase todos os cursos que possuem programação na UTFPR Curitiba. Além disso, é a linguagem mais utilizada pelo mercado para sistemas embarcados, dessa forma sendo a escolha óbvia para o sistema.

### UML

![UML Ecu Central](https://github.com/Equipe-Imperador/ECU-Central/assets/89215941/1e1fdc41-9700-4a10-ab44-3c7bdcad9b61)


## Descrição dos Códigos
Abaixo está contido uma descrição breve sobre o funcionamento do código e sobre as funções de cada Classe do código.

### Código
O código consistem em uma classe ECU a qual gere todas as demais. Simula basicamente a estrutura física do nosso sistema em código. 

### ECU
A ECU requisita todos os dados paras as demais classes, e a partir disso, trata todos os dados e envia para o software de telemetria via LoRa e para o painel via CAN. Também salva os dados no cartão SD.

### Veículo
A Classe veículo realiza toda a obtenção dos dados a partir de sensores básicos do carro. São sensores analógicos, sendo esses a bateria do carro, velocidade, RPM e freio. Foi criada justamente para ser a representante de todos os sensores os quais não são digitais.

### Infravermelho
A Classe infravermelho é responsável por realizar a interface com o sensor infravermleho. Adquire a temperatura da correia da CVT por estar fixada logo na PCVT.

### GPS
A classe GPS possui como principal funcionalidade a aquisição de dados essenciais para o carro, como a velocidade por GPS, localização latitudinal e longitudinal, além do horário preciso do relógio do satélite. 

### Acelerômetro
A classe acelerômetro faz a interface com o sensor instalado no centro de gravidade do veículo. Conseguimos a aceleração nos três eixos além do giro de cada eixo.

### CAN
A classe CAN realiza a interface com a biblioteca CAN.h. Essa por sua vez realiza o envio dos dados para o painel via os módulos CAN.

### SD
A classe SD foi criada para fazer uma interface simples com a biblioteca FS.h da ESP32. Salva todos os arquivos em um SD de forma simples.

# Equipe Imperador
Este projeto foi desenvolvido pela equipe Imperador, subsistema de eletrônica. Todos os códigos de terceiros que foram utilizados no projeto são Open Source, entretanto, com modificações realizadas para atender da melhor forma a equipe. Caso pretenda utilizar partes do nosso projeto as quais são de nossa autoria, pedimos que apenas dê os devidos créditos. 

### Integrantes de eletrônica
Rossi
Lambari
Mary
Marco Polo
