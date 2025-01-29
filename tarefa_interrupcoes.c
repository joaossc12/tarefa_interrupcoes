#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"
#include "hardware/pio.h"
#include "tarefa_interrupcoes.pio.h"

#define led_red 13
#define led_blue 12
#define led_green 11
#define led_matrix 7
#define button_a 5
#define button_b 6

#define PERIODO_US 100000  // 100ms para mudar o estado 10 vezes por segundo, portanto piscar 5 vezes.

uint32_t VERMELHO = 0x00330000; //HEXADECIMAL VERMELHO
uint32_t AZUL = 0x00003300; //HEXADECIMAL AZUL 


//Prototipação de funções
void init_pinos();
bool blink_led(struct repeating_timer *t);
void envio_matriz_led(PIO pio, uint sm, uint32_t cor, uint ref);

int main() {
    PIO pio = pio0;
    bool clk = set_sys_clock_khz(128000, false);
    uint offset = pio_add_program(pio, &tafera_interrupcoes_program);
    uint sm = pio_claim_unused_sm(pio, true);

    tafera_interrupcoes_program_init(pio, sm, offset, led_matrix);

    init_pinos();
    struct repeating_timer timer;
    add_repeating_timer_us(PERIODO_US, blink_led, NULL, &timer);

    while (true) {
        for (int i = 0; i<10; i++){
            if (i%2 == 0){
                envio_matriz_led(pio, sm, VERMELHO, i);
            }
            else{
                envio_matriz_led(pio, sm, AZUL, i);
            }
            sleep_ms(1000);

        }
    }
}

void init_pinos(){
  gpio_init(led_red);//inicializa o pino 13 do microcontrolador
  gpio_set_dir(led_red, GPIO_OUT);//configura o pino como saída
 
  gpio_init(led_blue);//inicializa o pino 12 do microcontrolador 
  gpio_set_dir(led_blue, GPIO_OUT);//configura o pino 12 como saída
 
  gpio_init(led_green); //inicializa o pino 11 do microcontrolador
  gpio_set_dir(led_green,GPIO_OUT); //configura o pino 11 como saída

  gpio_init(button_a);//inicializa o pino 5 do microcontrolador
  gpio_set_dir(button_a, GPIO_IN);//configura o pino como entrada
  gpio_pull_up(button_a); //Pull up pino 5

  gpio_init(button_b);//inicializa o pino 6 do microcontrolador
  gpio_set_dir(button_b, GPIO_IN);//configura o pino como entrada
  gpio_pull_up(button_b); //Pull up pino 6
}

void envio_matriz_led(PIO pio, uint sm, uint32_t cor, uint ref){
    uint32_t numbers[10][25] = {{0, cor, cor, cor,0,0, cor, 0, cor, 0, 0, cor, 0, cor,0, 0, cor, 0, cor,0, 0, cor, cor, cor,0}, //Numero 0
                        {0, cor, cor, cor,0,0, 0, cor, 0, 0, 0, 0, cor, 0, 0, 0, cor, cor, 0, 0,0, 0, cor, 0, 0}, //Numero 1
                        {0, cor, cor, cor,0, 0, cor, 0, 0, 0, 0, cor, cor, cor, 0, 0, 0, 0, cor,0, 0, cor, cor, cor,0}, //Numero 2
                        {0, cor, cor, cor, 0, 0, 0, 0, cor, 0,0, cor, cor, cor, 0, 0, 0, 0, cor, 0, 0, cor, cor, cor, 0}, //Numero 3
                        {0, cor, 0, 0,0,0, 0, 0, cor, 0,0, cor, cor, cor,0,0, cor, 0, cor,0,0, cor, 0, cor,0}, //Numero 4
                        {0, cor, cor, cor,0,0, 0, 0, cor, 0,0, cor, cor, cor,0,0, cor, 0, 0,0,0, cor, cor, cor,0}, //Numero 5
                        {0, cor, cor, cor,0,0, cor, 0, cor, 0,0, cor, cor, cor,0,0, 0, 0, cor,0,0, cor, cor, cor,0}, //Numero 6
                        {0, cor, 0, 0,0, 0, 0, 0, cor, 0, 0, cor, 0, 0,0,0, 0, 0, cor,0,0, cor, cor, cor,0}, //Numero 7
                        {0, cor, cor, cor,0,0, cor, 0, cor, 0, 0, cor, cor, cor,0,0, cor, 0, cor,0,0, cor, cor, cor,0}, //Numero 8
                        {0, cor, cor, cor,0,0, 0, 0, cor, 0,0, cor, cor, cor,0,0, cor, 0, cor,0,0, cor, cor, cor,0} //Numero 9
                        };
        for (int i = 0; i < 25; i++)
        {
            pio_sm_put_blocking(pio, sm, numbers[ref][i]);
        }
}

bool blink_led(struct repeating_timer *t) {
    gpio_put(led_red, !gpio_get(led_red));  // Inverte o estado do LED
    return true;  // Continua repetindo
}