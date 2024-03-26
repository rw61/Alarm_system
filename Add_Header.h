#include <stm32f4xx.h>
#include <stm32f411xe.h>
#include "functions.c"
#include "Init_butt_leds.c"
void rccInit(void);
void inputsInit(void);
void outputsInit(void);

void alarmActivationUpdate(void);
void alarmDeactivationUpdate(void);
void buttonsUpdate(void);
uint8_t Debounce_HandlerB(uint16_t);
uint8_t Debounce_HandlerC(uint16_t);
int PushButtonB(uint16_t);
int PushButtonC(uint16_t);

static uint16_t Button_StateB = 0;				//состояние кнопки
static uint16_t Button_StateC = 0;
static uint32_t Button_Count = 0;			  //счетчик
static uint8_t result = 0;							//результат на выходе
static int alarmLed = 0;
static int StateOfButt1 = 0;
static int StateOfButt2 = 0;
static int StateOfButt10 = 0;
static int StateOfButt4 = 0;
static int StateOfButt14 = 0;
static int StateOfButt5 = 0;
static int enterButton = 0;

static int wrong_deactivation_attempts = 0;
uint32_t led_ON(uint32_t);
static uint32_t ledStatus = 0;
static int incorrectCodeLed = 0;
static int systemBlocked = 0;
static int buttonPressed = 0;


#define PIN1 GPIO_IDR_IDR_1
#define PIN2 GPIO_IDR_IDR_2
#define PIN14 GPIO_IDR_IDR_14
#define PIN4 GPIO_IDR_IDR_4
#define PIN5 GPIO_IDR_IDR_5
#define PIN10 GPIO_IDR_IDR_10
#define PINC13 GPIO_IDR_IDR_13 
#define LED_BLUE GPIO_ODR_ODR8



				
	
				
