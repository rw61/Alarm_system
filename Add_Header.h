#include <stm32f4xx.h>
#include <stm32f411xe.h>
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

///////////////    И Н И Ц И А Л И З А Ц И Я     ////////////////////// 
														
////////////// В Х О Д ////////////////////
		
 void inputsInit(void)
 {
	  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
	  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
	
	 CLEAR_BIT(GPIOB->MODER, GPIO_MODER_MODER1_0 | GPIO_MODER_MODER1_1 | GPIO_MODER_MODER2_0 | GPIO_MODER_MODER2_1 |
	 GPIO_MODER_MODER14_0 | GPIO_MODER_MODER14_1 | GPIO_MODER_MODER4_0 | GPIO_MODER_MODER4_1 |
	 GPIO_MODER_MODER5_0 | GPIO_MODER_MODER5_1 | GPIO_MODER_MODER10_0 | GPIO_MODER_MODER10_1); // сброс битов нулевого и первого в регистре MODER ЗНАЧИТ, что пины PA2,PA3,PB3,PB4,PB5,PB10 настроены на input , на этих пинах будут кнопки
											
	 MODIFY_REG(GPIOB->PUPDR, GPIO_PUPDR_PUPDR1_0 | GPIO_PUPDR_PUPDR2_0 | GPIO_PUPDR_PUPDR5_0 | GPIO_PUPDR_PUPDR10_0 | GPIO_PUPDR_PUPDR14_0 | GPIO_PUPDR_PUPDR4_0,
	 GPIO_PUPDR_PUPDR1_1 | GPIO_PUPDR_PUPDR2_1 |	GPIO_PUPDR_PUPDR5_1 | GPIO_PUPDR_PUPDR10_1 | GPIO_PUPDR_PUPDR14_1 | GPIO_PUPDR_PUPDR4_1 );  //установка пина в состоянии PULL_DOWN, тоесть на пине будет 0. комбинация PUPDR -10(первый бит один- нулевой бит ноль) 
	
////////// Кнопка USER Batton на плате Nucleo ////////////////
	 
	 CLEAR_BIT(GPIOC->MODER, GPIO_MODER_MODER13_0 | GPIO_MODER_MODER13_1); 
         MODIFY_REG(GPIOC->PUPDR, GPIO_PUPDR_PUPDR13_1, GPIO_PUPDR_PUPDR13_0);
	 GPIOC->BSRR |= GPIO_BSRR_BS13;
 }
 ////////////// В Ы Х О Д ////////////////////
 
 void outputsInit(void)
 {
	 //// L E D   N U C L E O  P A 5////
	 
	 MODIFY_REG(GPIOA->MODER, GPIO_MODER_MODER5_1, GPIO_MODER_MODER5_0); 
	 
	//// L E D  O N  T H E  B O A R D ////
	 
	 MODIFY_REG(GPIOB->MODER, GPIO_MODER_MODER8_1 | GPIO_MODER_MODER9_1 , GPIO_MODER_MODER8_0 | GPIO_MODER_MODER9_0); //01 - GP OUTPUT MODE
	 SET_BIT(GPIOB->BSRR, GPIO_BSRR_BS_8 | GPIO_BSRR_BS_9);
	 CLEAR_BIT(GPIOB->OTYPER, GPIO_OTYPER_OT8 | GPIO_OTYPER_OT9); //0 - PUSH PULL 
	 MODIFY_REG(GPIOB->OSPEEDR, GPIO_OSPEEDER_OSPEEDR8_1 | GPIO_OSPEEDER_OSPEEDR9_1, GPIO_OSPEEDER_OSPEEDR8_0 | GPIO_OSPEEDER_OSPEEDR9_0); //MED SPEED
	 MODIFY_REG(GPIOB->PUPDR, GPIO_PUPDR_PUPDR8_1 | GPIO_PUPDR_PUPDR9_1, GPIO_PUPDR_PUPDR8_0 | GPIO_PUPDR_PUPDR9_0); //pull_UPP - 01, подтяжка к 1
	 
 }
 																		/////////////  О Б Р А Б О Т К А  К Н О П К И   //////////////
 int PushButtonB(uint16_t pinNumber)
 {
     Button_StateB = READ_BIT(GPIOB->IDR, pinNumber);
     result = Debounce_HandlerB(Button_StateB);
							
        if(result == 0)
	{return 1;	//если кнопка нажата - возвращаем 1}
	else
	return 0;									
	 }
	
 int PushButtonC(uint16_t pinNumber)
 {	           
	 Button_StateC = READ_BIT(GPIOC->IDR, pinNumber);
	 result = Debounce_HandlerB(Button_StateC);
	 if(result == 0)
	{
	 return 1;					//если кнопка нажата - возвращаем 1
	}													
	else						
	return 0;						
}								
//////////////  Д Р Е Б Е З Г  К О Н Т А К Т О В   ////////////
 
 uint8_t Debounce_HandlerB(uint16_t Button_State)							
{							
	if(Button_State != 0)								
											
 {
   if(Button_Count < 3)				//если счетчик меньше заданного значения
	{					//прибавляем единицу к счетчику и снова проверяем его значение
		Button_Count++;
		return 1;					
	}																	
  
else					        //когда счётчик будет больше заданного значения
{ 
  if(Button_State != 0)		//проверяем ещё раз состояние кнопки
	return 0;							//если состояние равно 0 , возвращаем 1
}														
	}	
		else			
		{ return1;
		}
		return 1;
				}				

 void buttonsUpdate(void)
 {
	enterButton = PushButtonC(PINC13);
	StateOfButt1 = PushButtonB(PIN1);
	StateOfButt2 = PushButtonB(PIN2);
	StateOfButt10 = PushButtonB(PIN10);
	StateOfButt14 = PushButtonB(PIN14);
	StateOfButt5 = PushButtonB(PIN5);
	StateOfButt4 = PushButtonB(PIN4);
 }
				
				
 void alarmActivationUpdate(void)
 {
	if(StateOfButt1 || StateOfButt2)	                    //активация любого из датчиков активирует сигналку
	{
	SET_BIT(GPIOA->BSRR, GPIO_BSRR_BS_5);
	alarmLed = 1;                   //СЧИТЫВАЕМ СОСТОЯНИЕ ПИНА ЛАМПЫ PB8, ЗАПИСЫВАЕМ В ПЕРЕМЕННУЮ
	}
 }


 void alarmDeactivationUpdate(void)
 {
 if(alarmLed && !enterButton)
	{
         if(incorrectCodeLed == 0)
	  {
	       if(StateOfButt10 && StateOfButt4 && !StateOfButt14 && !StateOfButt5)
		{
		 SET_BIT(GPIOA->BSRR, GPIO_BSRR_BR_5);
		 alarmLed = 0; 
		}
		else
		{
		SET_BIT(GPIOB->BSRR, GPIO_BSRR_BR8);
		incorrectCodeLed = 1;
		wrong_deactivation_attempts++;
		}
	}
	 else
	{
	  if(wrong_deactivation_attempts<5)
	{
	 if(StateOfButt4 && !StateOfButt10 && !StateOfButt14 && !StateOfButt5)
	{
	  SET_BIT(GPIOB->BSRR, GPIO_BSRR_BS8);
	   incorrectCodeLed = 0;
	  wrong_deactivation_attempts = 0;
	}
																	
	 else
	{
	  wrong_deactivation_attempts++;
	  }
	   }	
 else
{
   SET_BIT(GPIOB->BSRR, GPIO_BSRR_BR9);
																
}														
														
														
  }
   }
 }

				
	
				
