
#include "weh1602.h"
#include "main.h"



void	delay_ms_init(void)
{
//	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
//	TIM2->CR1 |= TIM_CR1_OPM;
	SystemCoreClockUpdate();								//system clock must be <= 65MHz
	TIM2->PSC = SystemCoreClock/1000 - 1;		//
	TIM2->EGR |= TIM_EGR_UG;
	while( !(TIM2->SR & TIM_SR_UIF) )
		;
	TIM2->SR &= ~TIM_SR_UIF;
}

void	delay_ms(uint16_t ms)
{
	TIM2->ARR = ms;
	TIM2->CR1 |= TIM_CR1_CEN;
	while( !(TIM2->SR & TIM_SR_UIF) )
		;
	TIM2->SR &= ~TIM_SR_UIF;
}

void 	delay_us(uint16_t us)
{
	uint16_t prsc;
	
	SystemCoreClockUpdate();
	prsc = SystemCoreClock/1000000 - 1;
	TIM3->PSC = prsc;
	
	TIM3->ARR = us;
	TIM3->CR1 |= TIM_CR1_CEN;
	while( !(TIM3->SR & TIM_SR_UIF) );
	TIM3->SR &= ~TIM_SR_UIF;
}

void	WEH1602_E_strobe(void)
{
	uint16_t tmp;
	//delay(1000);		// ~1.5ms
	//delay_ms(2);
	//for(uint16_t i=10; i>0; i--)	;
	SystemCoreClockUpdate();
	tmp = SystemCoreClock/1000000*3;		//3-6us
	for(uint16_t i=0; i<tmp; i++)	;
	WEH1602_E(SET_LCD);
	//delay(1000);		// ~1.5ms
	//delay_ms(2);
	//for(uint16_t i=10; i>0; i--)	;
	for(uint16_t i=0; i<tmp; i++)	;
	WEH1602_E(RESET_LCD);
}

void	WEH1602_wr_instr(uint8_t instr)
{
		WEH1602_RW(RESET_LCD);
		WEH1602_RS(RESET_LCD);
	
		if(instr & DB7)
				WEH1602_DB7(SET_LCD);
		else
				WEH1602_DB7(RESET_LCD);
		if(instr & DB6)
				WEH1602_DB6(SET_LCD);
		else
				WEH1602_DB6(RESET_LCD);
		if(instr & DB5)
				WEH1602_DB5(SET_LCD);
		else
				WEH1602_DB5(RESET_LCD);
		if(instr & DB4)
				WEH1602_DB4(SET_LCD);
		else
				WEH1602_DB4(RESET_LCD);
		
		WEH1602_E_strobe();
		
		instr <<= 4;
		
		if(instr & DB7)
				WEH1602_DB7(SET_LCD);
		else
				WEH1602_DB7(RESET_LCD);
		if(instr & DB6)
				WEH1602_DB6(SET_LCD);
		else
				WEH1602_DB6(RESET_LCD);
		if(instr & DB5)
				WEH1602_DB5(SET_LCD);
		else
				WEH1602_DB5(RESET_LCD);
		if(instr & DB4)
				WEH1602_DB4(SET_LCD);
		else
				WEH1602_DB4(RESET_LCD);
		
		WEH1602_E_strobe();
}

void	WEH1602_init(void)
{
	
	delay_ms(100);		//must be > 500ms
	
	WEH1602_E(RESET_LCD);
	WEH1602_RW(RESET_LCD);
	WEH1602_RS(RESET_LCD);
	WEH1602_DB4(RESET_LCD);
	WEH1602_DB5(RESET_LCD);
	WEH1602_DB6(RESET_LCD);
	WEH1602_DB7(RESET_LCD);
	for(uint8_t i=0; i<5; i++)
	{
			delay_ms(10);		// ~50ms
			WEH1602_E_strobe();
	}
	
	//function set
	WEH1602_DB5(SET_LCD);
	for(uint8_t i=0; i<2; i++)
	{
			delay_ms(10);		// ~50ms
			WEH1602_E_strobe();
	}
	WEH1602_DB7(SET_LCD);
	delay_ms(10);		// ~50ms
	WEH1602_E_strobe();
	//WEH1602_readBF();
	delay_ms(10);		// ~50ms
	
	//display on/off control
	WEH1602_DB5(RESET_LCD);
	WEH1602_E_strobe();
	WEH1602_DB4(SET_LCD);
	WEH1602_DB5(SET_LCD);
	WEH1602_DB6(SET_LCD);
	WEH1602_DB7(SET_LCD);
	WEH1602_E_strobe();
	//WEH1602_readBF();
	delay_ms(10);		// ~50ms
	
	//display clear
	WEH1602_DB4(RESET_LCD);
	WEH1602_DB5(RESET_LCD);
	WEH1602_DB6(RESET_LCD);
	WEH1602_DB7(RESET_LCD);
	WEH1602_E_strobe();
	WEH1602_DB4(SET_LCD);
	WEH1602_E_strobe();
	delay_ms(10);		// ~50ms
	
	//return home
	WEH1602_DB4(RESET_LCD);
	WEH1602_E_strobe();
	WEH1602_DB5(SET_LCD);
	WEH1602_E_strobe();
	//WEH1602_readBF();
	delay_ms(10);		// ~50ms
	
	//entry mode set
	WEH1602_DB5(RESET_LCD);
	WEH1602_E_strobe();
	WEH1602_DB6(SET_LCD);
	WEH1602_DB5(SET_LCD);
	WEH1602_DB4(RESET_LCD);		//display doesn't shift
	WEH1602_E_strobe();
	//WEH1602_readBF();
	delay_ms(10);		// ~50ms
	
	
			//display off
		WEH1602_DB4(RESET_LCD);
		WEH1602_DB5(RESET_LCD);
		WEH1602_DB6(RESET_LCD);
		WEH1602_DB7(RESET_LCD);
		WEH1602_E_strobe();
		WEH1602_DB4(RESET_LCD);
		WEH1602_DB5(RESET_LCD);
		WEH1602_DB6(RESET_LCD);
		WEH1602_DB7(SET_LCD);
		WEH1602_E_strobe();
		//WEH1602_readBF();
		delay_ms(10);		// ~50ms
	
		//display clear
		WEH1602_DB4(RESET_LCD);
		WEH1602_DB5(RESET_LCD);
		WEH1602_DB6(RESET_LCD);
		WEH1602_DB7(RESET_LCD);
		WEH1602_E_strobe();
		WEH1602_DB4(SET_LCD);
		WEH1602_E_strobe();
		delay_ms(10);		// ~50ms
		
		//return home
		WEH1602_DB4(RESET_LCD);
		WEH1602_E_strobe();
		WEH1602_DB5(SET_LCD);
		WEH1602_E_strobe();
		//WEH1602_readBF();
		delay_ms(10);		// ~50ms
		
		//display on
		WEH1602_DB5(RESET_LCD);
		WEH1602_E_strobe();
		WEH1602_DB4(RESET_LCD);
		WEH1602_DB5(RESET_LCD);
		WEH1602_DB6(SET_LCD);
		WEH1602_DB7(SET_LCD);
		WEH1602_E_strobe();
		//WEH1602_readBF();
		delay_ms(10);		// ~50ms
		
}

void	WEH1602_init2(void)
{
	
	delay(6000000);		//>500ms
	
	WEH1602_E(RESET_LCD);
	WEH1602_RW(RESET_LCD);
	WEH1602_RS(RESET_LCD);
	WEH1602_DB4(RESET_LCD);
	WEH1602_DB5(RESET_LCD);
	WEH1602_DB6(RESET_LCD);
	WEH1602_DB7(RESET_LCD);
	for(uint8_t i=0; i<5; i++)
	{
			delay(50000);		// ~50ms
			WEH1602_E_strobe();
	}
	
	//function set
	WEH1602_DB5(SET_LCD);
	for(uint8_t i=0; i<3; i++)
	{
			delay(50000);		// ~50ms
			WEH1602_E_strobe();
	}
//	WEH1602_DB7(SET_LCD);
//	delay(500000);		// ~50ms
//	WEH1602_E_strobe();
	
	//WEH1602_readBF();
	delay(50000);		// ~50ms
	
	//display on/off control
	WEH1602_DB5(RESET_LCD);
	WEH1602_E_strobe();
	WEH1602_DB4(SET_LCD);
	WEH1602_DB5(SET_LCD);
	WEH1602_DB6(SET_LCD);
	WEH1602_DB7(SET_LCD);
	WEH1602_E_strobe();
	//WEH1602_readBF();
	delay(50000);		// ~50ms
	
	//display clear
	WEH1602_DB4(RESET_LCD);
	WEH1602_DB5(RESET_LCD);
	WEH1602_DB6(RESET_LCD);
	WEH1602_DB7(RESET_LCD);
	WEH1602_E_strobe();
	WEH1602_DB4(SET_LCD);
	WEH1602_E_strobe();
	//WEH1602_readBF();
	delay(500000);		// ~50ms
	
	//return home
	WEH1602_DB4(RESET_LCD);
	WEH1602_E_strobe();
	WEH1602_DB5(SET_LCD);
	WEH1602_E_strobe();
	//WEH1602_readBF();
	delay(50000);		// ~50ms
	
	//entry mode set
	WEH1602_DB5(RESET_LCD);
	WEH1602_E_strobe();
	WEH1602_DB6(SET_LCD);
	WEH1602_DB5(SET_LCD);
	WEH1602_DB4(RESET_LCD);
	WEH1602_E_strobe();
	//WEH1602_readBF();
	delay(50000);		// ~50ms
	
	
			//display off
		WEH1602_DB4(RESET_LCD);
		WEH1602_DB5(RESET_LCD);
		WEH1602_DB6(RESET_LCD);
		WEH1602_DB7(RESET_LCD);
		WEH1602_E_strobe();
		WEH1602_DB4(RESET_LCD);
		WEH1602_DB5(RESET_LCD);
		WEH1602_DB6(RESET_LCD);
		WEH1602_DB7(SET_LCD);
		WEH1602_E_strobe();
		//WEH1602_readBF();
		delay(50000);		// ~50ms
	
		//display clear
		WEH1602_DB4(RESET_LCD);
		WEH1602_DB5(RESET_LCD);
		WEH1602_DB6(RESET_LCD);
		WEH1602_DB7(RESET_LCD);
		WEH1602_E_strobe();
		WEH1602_DB4(SET_LCD);
		WEH1602_E_strobe();
		delay(500000);		// ~50ms
		
		//return home
		WEH1602_DB4(RESET_LCD);
		WEH1602_E_strobe();
		WEH1602_DB5(SET_LCD);
		WEH1602_E_strobe();
		//WEH1602_readBF();
		delay(50000);		// ~50ms
		
		//display on
		WEH1602_DB5(RESET_LCD);
		WEH1602_E_strobe();
		WEH1602_DB4(SET_LCD);
		WEH1602_DB5(SET_LCD);
		WEH1602_DB6(SET_LCD);
		WEH1602_DB7(SET_LCD);
		WEH1602_E_strobe();
		//WEH1602_readBF();
		delay(50000);		// ~50ms
	
}

void	delay(uint32_t units)
{
	while(--units);
} 

void	WEH1602_wr_data(uint8_t data)
{
		WEH1602_RW(RESET_LCD);
		WEH1602_RS(SET_LCD);
	
		if(data & DB7)
				WEH1602_DB7(SET_LCD);
		else
				WEH1602_DB7(RESET_LCD);
		if(data & DB6)
				WEH1602_DB6(SET_LCD);
		else
				WEH1602_DB6(RESET_LCD);
		if(data & DB5)
				WEH1602_DB5(SET_LCD);
		else
				WEH1602_DB5(RESET_LCD);
		if(data & DB4)
				WEH1602_DB4(SET_LCD);
		else
				WEH1602_DB4(RESET_LCD);
		
		WEH1602_E_strobe();
		
		data <<= 4;
		
		if(data & DB7)
				WEH1602_DB7(SET_LCD);
		else
				WEH1602_DB7(RESET_LCD);
		if(data & DB6)
				WEH1602_DB6(SET_LCD);
		else
				WEH1602_DB6(RESET_LCD);
		if(data & DB5)
				WEH1602_DB5(SET_LCD);
		else
				WEH1602_DB5(RESET_LCD);
		if(data & DB4)
				WEH1602_DB4(SET_LCD);
		else
				WEH1602_DB4(RESET_LCD);
		
		WEH1602_E_strobe();
}



void WEH1602_RS(uint8_t set_reset)
{
		if(set_reset)
			#ifdef	LL_CUBEMX
			LL_GPIO_SetOutputPin(RS_GPIO_Port, RS_Pin);
			#endif	//LL_CUBEMX
			#ifdef	HAL_CUBEMX
			HAL_GPIO_WritePin(RS_GPIO_Port, RS_Pin, GPIO_PIN_SET);
			#endif	//HALCUBEMX
		else
			#ifdef	LL_CUBEMX
			LL_GPIO_ResetOutputPin(RS_GPIO_Port, RS_Pin);
			#endif	//LL_CUBEMX
			#ifdef	HAL_CUBEMX
			HAL_GPIO_WritePin(RS_GPIO_Port, RS_Pin, GPIO_PIN_RESET);
			#endif	//HALCUBEMX
}

void WEH1602_RW(uint8_t set_reset)
{
		if(set_reset)
			#ifdef	LL_CUBEMX
			LL_GPIO_SetOutputPin(RW_GPIO_Port, RW_Pin);
			#endif	//LL_CUBEMX
			#ifdef	HAL_CUBEMX
			HAL_GPIO_WritePin(RW_GPIO_Port, RW_Pin, GPIO_PIN_SET);
			#endif	//HALCUBEMX
		else
			#ifdef	LL_CUBEMX
			LL_GPIO_ResetOutputPin(RW_GPIO_Port, RW_Pin);
			#endif	//LL_CUBEMX
			#ifdef	HAL_CUBEMX
			HAL_GPIO_WritePin(RW_GPIO_Port, RW_Pin, GPIO_PIN_RESET);
			#endif	//HALCUBEMX
}

void WEH1602_DB4(uint8_t set_reset)
{
		if(set_reset)
			#ifdef	LL_CUBEMX
			LL_GPIO_SetOutputPin(DB4_GPIO_Port, DB4_Pin);
			#endif	//LL_CUBEMX
			#ifdef	HAL_CUBEMX
			HAL_GPIO_WritePin(DB4_GPIO_Port, DB4_Pin, GPIO_PIN_SET);
			#endif	//HALCUBEMX
		else
			#ifdef	LL_CUBEMX
			LL_GPIO_ResetOutputPin(DB4_GPIO_Port, DB4_Pin);
			#endif	//LL_CUBEMX
			#ifdef	HAL_CUBEMX
			HAL_GPIO_WritePin(DB4_GPIO_Port, DB4_Pin, GPIO_PIN_RESET);
			#endif	//HALCUBEMX
}

void WEH1602_DB5(uint8_t set_reset)
{
		if(set_reset)
			#ifdef	LL_CUBEMX
			LL_GPIO_SetOutputPin(DB5_GPIO_Port, DB5_Pin);
			#endif	//LL_CUBEMX
			#ifdef	HAL_CUBEMX
			HAL_GPIO_WritePin(DB5_GPIO_Port, DB5_Pin, GPIO_PIN_SET);
			#endif	//HALCUBEMX
		else
			#ifdef	LL_CUBEMX
			LL_GPIO_ResetOutputPin(DB5_GPIO_Port, DB5_Pin);
			#endif	//LL_CUBEMX
			#ifdef	HAL_CUBEMX
			HAL_GPIO_WritePin(DB5_GPIO_Port, DB5_Pin, GPIO_PIN_RESET);
			#endif	//HALCUBEMX
}

void WEH1602_DB6(uint8_t set_reset)
{
		if(set_reset)
			#ifdef	LL_CUBEMX
			LL_GPIO_SetOutputPin(DB6_GPIO_Port, DB6_Pin);
			#endif	//LL_CUBEMX
			#ifdef	HAL_CUBEMX
			HAL_GPIO_WritePin(DB6_GPIO_Port, DB6_Pin, GPIO_PIN_SET);
			#endif	//HALCUBEMX
		else
			#ifdef	LL_CUBEMX
			LL_GPIO_ResetOutputPin(DB6_GPIO_Port, DB6_Pin);
			#endif	//LL_CUBEMX
			#ifdef	HAL_CUBEMX
			HAL_GPIO_WritePin(DB6_GPIO_Port, DB6_Pin, GPIO_PIN_RESET);
			#endif	//HALCUBEMX
}

void WEH1602_DB7(uint8_t set_reset)
{
		if(set_reset)
			#ifdef	LL_CUBEMX
			LL_GPIO_SetOutputPin(DB7_GPIO_Port, DB7_Pin);
			#endif	//LL_CUBEMX
			#ifdef	HAL_CUBEMX
			HAL_GPIO_WritePin(DB7_GPIO_Port, DB7_Pin, GPIO_PIN_SET);
			#endif	//HALCUBEMX
		else
			#ifdef	LL_CUBEMX
			LL_GPIO_ResetOutputPin(DB7_GPIO_Port, DB7_Pin);
			#endif	//LL_CUBEMX
			#ifdef	HAL_CUBEMX
			HAL_GPIO_WritePin(DB7_GPIO_Port, DB7_Pin, GPIO_PIN_RESET);
			#endif	//HALCUBEMX
}

void	WEH1602_E(uint8_t set_reset)
{
		if(set_reset)
			#ifdef	LL_CUBEMX
			LL_GPIO_SetOutputPin(E_GPIO_Port, E_Pin);
			#endif	//LL_CUBEMX
			#ifdef	HAL_CUBEMX
			HAL_GPIO_WritePin(E_GPIO_Port, E_Pin, GPIO_PIN_SET);
			#endif	//HALCUBEMX
		else
			#ifdef	LL_CUBEMX
			LL_GPIO_ResetOutputPin(E_GPIO_Port, E_Pin);
			#endif	//LL_CUBEMX
			#ifdef	HAL_CUBEMX
			HAL_GPIO_WritePin(E_GPIO_Port, E_Pin, GPIO_PIN_RESET);
			#endif	//HALCUBEMX
}

void	WEH1602_wr_buffer(char *buf)
{
	while(*buf)
		WEH1602_wr_data(*buf++);
}

void	WEH1602_clear_display(void)
{
	WEH1602_RW(RESET_LCD);
	WEH1602_RS(RESET_LCD);
	
	WEH1602_DB4(RESET_LCD);
	WEH1602_DB5(RESET_LCD);
	WEH1602_DB6(RESET_LCD);
	WEH1602_DB7(RESET_LCD);
	WEH1602_E_strobe();
	WEH1602_DB4(SET_LCD);
	WEH1602_E_strobe();
	delay_ms(10);		// 6.2ms
}

void		WEH1602_clear_line(uint8_t line)
{
	if(line)
		WEH1602_wr_instr(SET_ADDRESS|LINE_1_START);
	else
		WEH1602_wr_instr(SET_ADDRESS|LINE_2_START);
	for(uint8_t i=0; i<16; i++)
	WEH1602_wr_data(' ');
}

void	HD44780_init(void)		//for proteus model LM016L
{
	delay_ms(100);

	WEH1602_RS(RESET_LCD);
	WEH1602_RW(RESET_LCD);
	WEH1602_DB7(RESET_LCD);
	WEH1602_DB6(RESET_LCD);
	WEH1602_DB5(SET_LCD);
	WEH1602_DB4(SET_LCD);
	WEH1602_E_strobe();		//Function set

	delay_ms(10);

	WEH1602_E_strobe();		//Function set

	delay_ms(1);

	WEH1602_E_strobe();		//Function set

	delay_ms(1);

	WEH1602_DB4(RESET_LCD);
	WEH1602_E_strobe();		//Function set 4 bit

	delay_ms(5);

	WEH1602_E_strobe();		//Function set 4 bit
	WEH1602_DB7(SET_LCD);	//2 lines
	WEH1602_E_strobe();		// font??

	delay_ms(5);

	WEH1602_DB5(RESET_LCD);
	WEH1602_DB7(RESET_LCD);
	WEH1602_E_strobe();
	WEH1602_DB7(SET_LCD);
	WEH1602_E_strobe();		//display off

	delay_ms(5);

	WEH1602_DB7(RESET_LCD);
	WEH1602_E_strobe();
	WEH1602_DB4(SET_LCD);
	WEH1602_E_strobe();		//display clear

	delay_ms(5);

	WEH1602_DB4(RESET_LCD);
	WEH1602_E_strobe();
	WEH1602_DB6(SET_LCD);		//entry mode
	WEH1602_DB5(SET_LCD);				//increment DDRAM
	WEH1602_E_strobe();

	WEH1602_DB6(RESET_LCD);
	WEH1602_DB5(RESET_LCD);
	WEH1602_E_strobe();		//display control
	WEH1602_DB7(SET_LCD);
	WEH1602_DB6(SET_LCD);				//display on
	WEH1602_DB5(SET_LCD);				//cursor on
	WEH1602_DB4(SET_LCD);				//blinking
	WEH1602_E_strobe();

}

//void WEH1602_readBF(void)
//{
//
//	uint8_t flag = 1;
//
//	GPIO_InitTypeDef temp_GPIO_Init;
//
//	GPIO_WriteBit(GPIOB, GPIO_Pin_10, Bit_SET);					//R/W = 1
//	GPIO_WriteBit(GPIOB, GPIO_Pin_11, Bit_RESET);				//RS  = 0
//
//	temp_GPIO_Init.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
//	temp_GPIO_Init.GPIO_Speed = GPIO_Speed_10MHz;
//	temp_GPIO_Init.GPIO_Mode = GPIO_Mode_IPD;
//	GPIO_Init(GPIOB, &temp_GPIO_Init);
//
//	while (flag)
//	{
//		WEH1602_E_strobe();
//		if (!(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15)))
//			flag = 0;
//		WEH1602_E_strobe();
//		delay(200);
//	}
//
//	temp_GPIO_Init.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
//	temp_GPIO_Init.GPIO_Speed = GPIO_Speed_2MHz;
//	temp_GPIO_Init.GPIO_Mode = GPIO_Mode_Out_PP;
//	GPIO_Init(GPIOB, &temp_GPIO_Init);
//
//}



