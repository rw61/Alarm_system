#include "Add_Header.h"

/////////////  О Б Р А Б О Т К А  К Н О П К И   //////////////
 int PushButtonB(uint16_t pinNumber){
     Button_StateB = READ_BIT(GPIOB->IDR, pinNumber);
     result = Debounce_HandlerB(Button_StateB);
			 if(result == 0){
         return 1;	            //если кнопка нажата - возвращаем 1}
         else
	       return 0;}
	
 int PushButtonC(uint16_t pinNumber){	           
	 Button_StateC = READ_BIT(GPIOC->IDR, pinNumber);
	 result = Debounce_HandlerB(Button_StateC);
	 if(result == 0){
	 return 1;}					//если кнопка нажата - возвращаем 1										
  	else						
	   return 0;}								
//////////////  Д Р Е Б Е З Г  К О Н Т А К Т О В   ////////////
 uint8_t Debounce_HandlerB(uint16_t Button_State)	{							
	if(Button_State != 0) {
   if(Button_Count < 3){					//если счетчик меньше заданного значения
			Button_Count++;		          //прибавляем единицу к счетчику и снова проверяем его значение
		  return 1;	}	 
   else{					        //когда счётчик будет больше заданного значения
      if(Button_State != 0)		//проверяем ещё раз состояние кнопки
      return 0;}							//если состояние равно 0 , возвращаем 1
 		}													
    	else{
        return1;}
	return 1;}
								
 void buttonsUpdate(void) {            //опрос кнопок
	enterButton = PushButtonC(PINC13);StateOfButt1 = PushButtonB(PIN1);StateOfButt2 = PushButtonB(PIN2);
	StateOfButt10 = PushButtonB(PIN10);StateOfButt14 = PushButtonB(PIN14);StateOfButt5 = PushButtonB(PIN5);StateOfButt4 = PushButtonB(PIN4);
 }

 void alarmActivationUpdate(void){
	if(StateOfButt1 || StateOfButt2)	{ //активация любого из датчиков активирует сигналку
	SET_BIT(GPIOA->BSRR, GPIO_BSRR_BS_5);
	alarmLed = 1;  }}                 
	
 void alarmDeactivationUpdate(void){
  if(alarmLed && !enterButton){
    if(incorrectCodeLed == 0){
	    if(StateOfButt10 && StateOfButt4 && !StateOfButt14 && !StateOfButt5){
		  SET_BIT(GPIOA->BSRR, GPIO_BSRR_BR_5);
		  alarmLed = 0; }
		else{
		  SET_BIT(GPIOB->BSRR, GPIO_BSRR_BR8);
		  incorrectCodeLed = 1;
		  wrong_deactivation_attempts++;}}
	  else{
	   if(wrong_deactivation_attempts<5){
	    if(StateOfButt4 && !StateOfButt10 && !StateOfButt14 && !StateOfButt5){
	      SET_BIT(GPIOB->BSRR, GPIO_BSRR_BS8);
	      incorrectCodeLed = 0;
	      wrong_deactivation_attempts = 0;}														
	    else{ 
        wrong_deactivation_attempts++;}}	
    else{                                //пять и более попыток ввести неправильный код
   SET_BIT(GPIOB->BSRR, GPIO_BSRR_BR9);}
  }}}																										
 
