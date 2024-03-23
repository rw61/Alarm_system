#include <stm32f4xx.h>
#include <stm32f411xe.h>
#include "Add_Header.h"



		int main(void)
		{
				inputsInit();
				outputsInit();
			
			
			while(1)
				{
					  buttonsUpdate();
						alarmActivationUpdate();
						alarmDeactivationUpdate();
							
						
			}
		}
													
	
	
 
															

 