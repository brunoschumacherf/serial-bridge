---

## 📋 Descrição do Projeto

Este projeto tem como objetivo explorar e aplicar os conceitos de **comunicação serial** no microcontrolador **RP2040**, utilizando a placa de desenvolvimento **BitDogLab**. A principal tarefa é implementar a comunicação através de **UART** e **I2C**, interagindo com componentes como **LEDs comuns**, **LEDs WS2812**, **display SSD1306** e **botões**. Além disso, o projeto aborda o uso de **interrupções**, o conceito de **debouncing** nos botões e a comunicação com o **Serial Monitor**.

---

## 🔧 Requisitos

- **Interrupções (IRQ):** Utilização de interrupções para garantir a resposta rápida aos botões.
- **Debouncing:** Implementação para evitar leituras erradas dos botões.
- **Controle de LEDs:** Gerenciamento de LEDs comuns e LEDs endereçáveis (WS2812) com efeitos visuais.
- **Display SSD1306:** Exibição de informações no display via I2C.
- **Comunicação UART:** Envio de dados para o Serial Monitor.
- **Estrutura do Código:** Código modularizado com comentários e nomes em português.

---

## ⚙️ Como Rodar o Projeto

### 1. Preparando o Ambiente

- Certifique-se de que o [Pico SDK](https://datasheets.raspberrypi.com/pico/getting-started-with-pico.pdf) está instalado corretamente.
- Verifique se as dependências necessárias estão instaladas.

### 2. Clonando o Repositório

Faça o clone do repositório em sua máquina:

```bash
git clone https://github.com/brunoschumacherf/serial-bridge
```

### 3. Compilando e Enviando o Código

No VS Code, com o Pico SDK configurado, faça o seguinte:

```bash
mkdir build
cd build
cmake ..
make
```

Após a compilação, copie o arquivo `.uf2` gerado para o Raspberry Pi Pico (lembre-se de colocar a placa no modo bootloader).

### 4. Testes

- **Simulação no Wokwi:**  
  Para testar o código, você pode usar a plataforma [Wokwi](https://wokwi.com/).

---

## 📁 Entregáveis

- Código-fonte completo disponível neste repositório.
- Vídeo demonstrativo: [Vídeo]()


---

_Desenvolvido por Bruno Schumacher_

---