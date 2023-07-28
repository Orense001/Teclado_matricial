#include <keypad.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "esp_log.h"
#include <stdlib.h>

const static char *TAG="matriz";
uint8_t cont=0;
#define LED_PIN 2
#define PUSH_BUTTON_PIN_4    (4) //Botón 4 para la interrupción
int8_t estado_led=0;
int8_t num1, num2;
char n[]={"19"};
char m[1]; //Sirve para almacenar el número ingresado por teclado tipo char

void isr_handler(void *args);

void app_main() {
    /// keypad pinout
    ///                     R1  R2  R3  R4  C1  C2  C3  C4 
    gpio_num_t keypad[8] = {13, 12, 14, 27, 26, 25, 33, 32};
    printf("1.--- Config ---\n");
    
    keypad_initalize(keypad); /// Initialize keyboard

    /*-----------------LED como salida-------------*/
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);
    /*---------------------------------------------*/

    /*------------Habilitamon interrupciones-----------------*/
    gpio_set_direction(PUSH_BUTTON_PIN_4, GPIO_MODE_INPUT);
    gpio_set_pull_mode(PUSH_BUTTON_PIN_4, GPIO_PULLUP_ONLY);
    gpio_set_intr_type(PUSH_BUTTON_PIN_4, GPIO_INTR_NEGEDGE); // falling edge
    gpio_install_isr_service(0);
    gpio_isr_handler_add(PUSH_BUTTON_PIN_4, isr_handler, NULL);
    /*--------------------------------------------------------*/

    while(true)
    {
        gpio_set_level(LED_PIN, estado_led);
        //printf("Contador es: %c\n", cont);

        unsigned keypressed = keypad_getkey();  /// gets from key queue
        //char m[]={keypressed}; //es otra manera de almacenar el caracter del teclado
        *m = keypressed; //Lo convertimos a puntero al caracter del teclado para usar el atoi

        if(keypressed != '\0')
        {
            //ESP_LOGI (TAG, "event = %04x", keypressed);
            printf("Pressed key: %c\n", keypressed);
            //fgets(keypressed, sizeof(keypressed),stdin);
            num1= atoi(m);
            num2= num1+31;
            printf("Suma %c\n", num2);
        }

        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
    
}

void isr_handler(void *args)
{
    cont++;
    estado_led = !estado_led;
}