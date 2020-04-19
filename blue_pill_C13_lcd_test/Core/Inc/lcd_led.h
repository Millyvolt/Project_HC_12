

//#include "stm32f10x.h"

//#include "stm32f4xx.h"
#include "main.h"

//#define 	SLON
//#define FREZA
//#define	LATON
//#define		LL_CUBEMX
#define		HAL_CUBEMX
#define		LCD_I2C_2004
//#define		LCD_LED_1604

//#ifdef	LATON
#define		RESET_LCD		0
#define		SET_LCD			1
#define		DB4			0x10
#define		DB5			0x20
#define		DB6			0x40
#define		DB7			0x80
//#endif	//LATON

#ifdef	SLON
#define		RS_pin		((uint16_t)0x0800)
#define		RW_pin		((uint16_t)0x0400)
#define		E_pin		((uint16_t)0x0002)
#define		DB4_pin		((uint16_t)0x1000)
#define		DB5_pin		((uint16_t)0x2000)
#define		DB6_pin		((uint16_t)0x4000)
#define		DB7_pin		((uint16_t)0x8000)
#endif	//SLON

#ifdef	FREZA
#define		RS_pin			((uint16_t)0x0008)
#define		RW_pin			((uint16_t)0x0010)
#define		E_pin				((uint16_t)0x0020)
#define		DB4_pin			((uint16_t)0x0040)
#define		DB5_pin			((uint16_t)0x0080)
#define		DB6_pin			((uint16_t)0x0100)
#define		DB7_pin			((uint16_t)0x0200)
#endif	//FREZA

#define		SET_ADDRESS			0x80
#define		LINE_1_START		0
#define		LINE_2_START		0x40
#define		LINE1						1
#define		LINE2						0


void		delay(uint32_t units);
void		delay_init(void);
void 		delay_ms(uint16_t ms);		//based on timer
void 		delay_us(uint16_t us);		//based on timer
void		WEH1602_wr_instr(uint8_t instruction);
void 		WEH1602_wr_data(uint8_t data);
void		WEH1602_init(void);
void 		WEH1602_E_strobe(void);  
void		WEH1602_wr_buffer(char *buf);
void 		WEH1602_init2(void);
void		WEH1602_clear_display(void);
void		WEH1602_clear_line(uint8_t line);
void		HD44780_init(void);		//for proteus model LM016L 
void		display_2004_i2c_init(I2C_HandleTypeDef *hi2c);
void		E_pulse(I2C_HandleTypeDef *hi2c);

//hardware depending
void 		WEH1602_RS(uint8_t set_reset);
void 		WEH1602_RW(uint8_t set_reset);
void		WEH1602_DB4(uint8_t set_reset);
void		WEH1602_DB5(uint8_t set_reset);
void 		WEH1602_DB6(uint8_t set_reset);
void 		WEH1602_DB7(uint8_t set_reset);
void		WEH1602_E(uint8_t set_reset);
//void 		WEH1602_readBF(void);



