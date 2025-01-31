#include <stdio.h>
#include "pico/stdlib.h"
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
volatile uint32_t color = 0x33000000; //INICIA COM LED NA COR VERDE
volatile int8_t control_led = 0;

//Prototipação de funções
void init_pinos();
bool blink_led(struct repeating_timer *t);
void envio_matriz_led(PIO pio, uint sm, uint32_t cor, uint ref);
static void callback_button(uint gpio, uint32_t events);

int main() {
    PIO pio = pio0;
    bool clk = set_sys_clock_khz(128000, false);
    uint offset = pio_add_program(pio, &tafera_interrupcoes_program);
    uint sm = pio_claim_unused_sm(pio, true);
    tafera_interrupcoes_program_init(pio, sm, offset, led_matrix);
    init_pinos();
    struct repeating_timer timer;
    add_repeating_timer_us(PERIODO_US, blink_led, NULL, &timer);
    gpio_set_irq_enabled_with_callback(button_a, GPIO_IRQ_EDGE_FALL, true, &callback_button);
    gpio_set_irq_enabled(button_b, GPIO_IRQ_EDGE_FALL, true);

    stdio_init_all();

    while (true) {

        envio_matriz_led(pio, sm, color, control_led);
        sleep_ms(1000);
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

static void callback_button(uint gpio, uint32_t events) {
    static absolute_time_t last_time_A = 0; // Tempo do último evento do botão A
    static absolute_time_t last_time_B = 0; // Tempo do último evento do botão B
    absolute_time_t now = get_absolute_time();

    if (gpio == button_a) { // Interrupção do botão A
        if (absolute_time_diff_us(last_time_A, now) > 200000) { // Debounce de 500ms
            control_led ++;
            if(control_led > 9){
                control_led = 0;
            }
            color = AZUL;
            last_time_A = now; // Atualiza o tempo do último evento do botão A
        }
    } else if (gpio == button_b) { // Interrupção do botão B
        if (absolute_time_diff_us(last_time_B, now) > 200000) { // Debounce de 50ms
            control_led --;
            if (control_led < 0){
                control_led = 9;
            }
            color = VERMELHO;
            last_time_B = now; // Atualiza o tempo do último evento do botão B
        }
    }
}