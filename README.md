# Projeto: Uso de Interrupções para contagem em Matriz de LEDs 5x5 com Raspberry Pi Pico W

## 📌 Descrição

Este projeto tem como objetivo utilizar botões para implementar um contador numérico em uma **matriz de LEDs 5x5 WS2812**, utilizando um **Raspberry Pi Pico W**. O sistema permite controlar um contador numérico por meio de dois botões (**push buttons**).

O desenvolvimento foi realizado utilizando o simulador **Wokwi** e a placa **BitDogLab**, como parte da **Unidade 4 - Microcontroladores**, com foco no aprendizado sobre **Interrupções e Debouncing**.

## 🎯 Objetivos

✔ Usar interrupções para implementar todas as funcionalidades do sistema.
✔ Familiarizar-se com o desenvolvimento em **C** utilizando o **Pico SDK**.
✔ Utilizar o **simulador Wokwi** para prototipagem rápida.
✔ Ambientar-se a placa de desenvolvimento **BitDogLab**.



## 📺 Demonstração

- **Execução na Placa BitDogLab:** 
-**Vídeo** [Youtube](https://youtube.com/shorts/FYZtUjrifHM)

## 🛠 Tecnologia Utilizada

### 🔹 **Software**
- **Linguagem de Programação:** C
- **IDE:** Visual Studio Code
- **SDK:** Pico SDK
- **Simulador:** Wokwi
- **Controle de Versão:** Git
- **Plataforma de Hospedagem:** GitHub

### 🔹 **Hardware**
- **Placa de Desenvolvimento:** BitDogLab

## 📂 Estrutura do Projeto

```
tarefa_interrupcoes/
│── build/                  # Arquivos compilados (gerado pelo CMake)
│── CMakeLists.txt          # Configuração do CMake
│── pico_sdk_import.cmake   # Importação do SDK do Pico
│── README.md               # Documentação do projeto
│── wokwi.toml              # Configuração do simulador Wokwi
│── diagram.json            # Arquivo de configuração do circuito
│── tarefa_interrupcoes.c   # Código principal do projeto
│── tarefa_interrupcoes.pio # Arquivo referente ao uso da PIO
```

## 🎮 Funcionalidades

- **Botão A** Acresce um número no contador
- **Botão B** Decresce um número no contador
- **LED VERMELHO** Pisca cinco vezes por segundo

## 📝 Como Executar o Projeto

1️⃣ **Clone o repositório:**
```sh
 git clone https://github.com/joaossc12/tarefa_interrupcoes
 cd tarefa_interrupcoes
```

2️⃣ **Compile o projeto:**
```sh
 mkdir build && cd build
 cmake ..
 make
```

3️⃣ **Carregue o código na sua BitDogLab** e teste as funcionalidades.

