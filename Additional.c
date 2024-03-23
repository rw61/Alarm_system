#include <stm32f4xx.h>
#include <stm32f411xe.h>
void rccInit(void);
void inputsInit(void);
void outputsInit(void);
void alarmActivationUpdate(int);
void alarmDeactivationUpdate(int);
uint8_t Debounce_Handler(uint16_t);
void GasDetector(void);
void OverTempDetector(void);
static uint16_t Button_State = 0;				//состояние кнопки
static uint32_t Button_Count = 0;			  //счетчик
static uint8_t result = 0;							//результат на выходе




														///////////////    И Н И Ц И А Л И З А Ц И Я     ////////////////////// 
		
 void inputsInit(void)
 {
	 
	 RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
	 
	 CLEAR_BIT(GPIOB->MODER, GPIO_MODER_MODER1_0 | GPIO_MODER_MODER1_1 | GPIO_MODER_MODER2_0 | GPIO_MODER_MODER2_1 | // сброс битов нулевого и первого в регистре MODER ЗНАЧИТ, что пины PA2,PA3,PB3,PB4,PB5,PB10 настроены на input , на этих пинах будут кнопки
													 GPIO_MODER_MODER3_0 | GPIO_MODER_MODER3_1 | GPIO_MODER_MODER4_0 | GPIO_MODER_MODER4_1 |
													 GPIO_MODER_MODER5_0 | GPIO_MODER_MODER5_1 | GPIO_MODER_MODER10_0 | GPIO_MODER_MODER10_1);
	 
	 										
	 MODIFY_REG(GPIOB->PUPDR, GPIO_PUPDR_PUPDR1_0 | GPIO_PUPDR_PUPDR2_0 | GPIO_PUPDR_PUPDR3_0 | GPIO_PUPDR_PUPDR4_0 |
														GPIO_PUPDR_PUPDR5_0 | GPIO_PUPDR_PUPDR10_0, 
														GPIO_PUPDR_PUPDR1_1 | GPIO_PUPDR_PUPDR2_1 | GPIO_PUPDR_PUPDR3_1 | GPIO_PUPDR_PUPDR4_1 |
														GPIO_PUPDR_PUPDR5_1 | GPIO_PUPDR_PUPDR10_1);  //установка пина в состоянии PULL_DOWN, тоесть на пине будет 0. комбинация PUPDR -10(первый бит один- нулевой бит ноль) 
	 
 }
 void outputsInit(void)
 {
	 RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	 
	 GPIOA->MODER &= ~GPIO_MODER_MODER5_1;					//moder5 = 01 - general purpouse output mode
	 GPIOA->MODER |= GPIO_MODER_MODER5_0;
	 
	 
	 GPIOA->BSRR|= GPIO_BSRR_BR5;
 }
 
																				/////////////   Д А Т Ч И К И    //////////////
 
 void GasDetector(void)
 {
	            Button_State = READ_BIT(GPIOB->IDR, GPIO_IDR_IDR_1);
							result = Debounce_Handler(Button_State);
							
							if(result == 1)
								{
									GPIOA->BSRR |= GPIO_BSRR_BR5;
								}
							else
								{
									GPIOA->BSRR |= GPIO_BSRR_BS5;
								}
 }
 
 void OverTempDetector(void)
 {
	            Button_State = READ_BIT(GPIOB->IDR, GPIO_IDR_IDR_2);
							result = Debounce_Handler(Button_State);
							
							if(result == 1)
								{
									GPIOA->BSRR |= GPIO_BSRR_BR5;
								}
							else
								{
									GPIOA->BSRR |= GPIO_BSRR_BS5;
								}
 }
 
 
																	//////////////  Д Р Е Б Е З Г  К О Н Т А К Т О В   ////////////
 
 uint8_t Debounce_Handler(uint16_t Button_State)
				{
					if(Button_State == 0)
					{
						if(Button_Count < 3)				//если счетчик меньше заданного значения
							{													//прибавляем единицу к счетчику и снова проверяем его значение
							Button_Count++;
							return 0;
							}
						else												//когда счётчик будет больше заданного значения
							{ 
								if(Button_State == 0)		//проверяем ещё раз состояние кнопки
								{
								return 1;							//если состояние равно 0 , возвращаем 1
								}
							}
					}	
						else										//Button_State = 1
						{
							if(Button_Count > 0)
							{
								Button_Count--;
								return 1;
							}
							else
							{
								if(Button_State != 0)
								{
									return 0;
								}
							}
					}			
				return 0;						
				}