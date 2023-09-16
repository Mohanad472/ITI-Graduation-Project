#include "HAL/LCD/LCD_int.h"
#include "MCAL/GPIO/GPIO_int.h"
#include "MCAL/RCC/RCC_int.h"
#include "HAL/LCD/LCD_cfg.h"
#include "MCAL/SYSTICK/STK_int.h"
#include "HAL/KEYPAD/KEYPAD_int.h"
#include "MCAL/EXTI/EXTI_int.h"
#include "MCAL/NVIC/NVIC_int.h"

u16 G_u16timer = 0;
u8 toggle = 0;
u8 G_u8Start_flag = 0;
u8 G_u8Displayed_Time[4] = {0,0,0,0};
u8 G_u8Pause_Flag_1st_time = 0;
u8 G_u8Pause_Flag_2nd_time = 0;

void flash_display();
void counter_display();

void task_sw1()
{
	if(G_u8Pause_Flag_1st_time)
	{
		G_u8Pause_Flag_2nd_time = 1;
	}
	else
	{
		G_u8Pause_Flag_1st_time = 1;
	}
}

void task_sw2()
{
	G_u8Pause_Flag_1st_time = 0;
	G_u8Start_flag = 1;
}

void task_sw3()
{
	while(!MGPIO_u8GetPinValue(GPIO_PORTB, GPIO_PIN2));			// Wait until door is closed
}

void main()
{
	MRCC_vInit();
	MRCC_vEnableClock(AHB1_, RCC_GPIOA_EN);
	MRCC_vEnableClock(AHB1_, RCC_GPIOB_EN);
	MRCC_vEnableClock(APB2_, RCC_SYSCFG_EN);
	MSTK_vInit();
	GPIO_cfg_t LCD_GPIO_RS = {GPIO_MODE_OUTPUT, GPIO_OUTTYPE_PUSHPULL, 0, GPIO_OUTSPEED_MEDIUM, 0 , LCD_PORT, RS_PIN };
	GPIO_cfg_t LCD_GPIO_RW = {GPIO_MODE_OUTPUT, GPIO_OUTTYPE_PUSHPULL, 0, GPIO_OUTSPEED_MEDIUM, 0 , LCD_PORT, RW_PIN };
	GPIO_cfg_t LCD_GPIO_E =	 {GPIO_MODE_OUTPUT, GPIO_OUTTYPE_PUSHPULL, 0, GPIO_OUTSPEED_MEDIUM, 0 , LCD_PORT, E_PIN };
	GPIO_cfg_t LCD_GPIO_D0 = {GPIO_MODE_OUTPUT, GPIO_OUTTYPE_PUSHPULL, 0, GPIO_OUTSPEED_MEDIUM, 0 , LCD_PORT, D0_PIN };
	GPIO_cfg_t LCD_GPIO_D1 = {GPIO_MODE_OUTPUT, GPIO_OUTTYPE_PUSHPULL, 0, GPIO_OUTSPEED_MEDIUM, 0 , LCD_PORT, D1_PIN };
	GPIO_cfg_t LCD_GPIO_D2 = {GPIO_MODE_OUTPUT, GPIO_OUTTYPE_PUSHPULL, 0, GPIO_OUTSPEED_MEDIUM, 0 , LCD_PORT, D2_PIN };
	GPIO_cfg_t LCD_GPIO_D3 = {GPIO_MODE_OUTPUT, GPIO_OUTTYPE_PUSHPULL, 0, GPIO_OUTSPEED_MEDIUM, 0 , LCD_PORT, D3_PIN };
	GPIO_cfg_t LCD_GPIO_D4 = {GPIO_MODE_OUTPUT, GPIO_OUTTYPE_PUSHPULL, 0, GPIO_OUTSPEED_MEDIUM, 0 , LCD_PORT, D4_PIN };
	GPIO_cfg_t LCD_GPIO_D5 = {GPIO_MODE_OUTPUT, GPIO_OUTTYPE_PUSHPULL, 0, GPIO_OUTSPEED_MEDIUM, 0 , LCD_PORT, D5_PIN };
	GPIO_cfg_t LCD_GPIO_D6 = {GPIO_MODE_OUTPUT, GPIO_OUTTYPE_PUSHPULL, 0, GPIO_OUTSPEED_MEDIUM, 0 , LCD_PORT, D6_PIN };
	GPIO_cfg_t LCD_GPIO_D7 = {GPIO_MODE_OUTPUT, GPIO_OUTTYPE_PUSHPULL, 0, GPIO_OUTSPEED_MEDIUM, 0 , LCD_PORT, D7_PIN };
	MGPIO_vInit(&LCD_GPIO_RS);
	MGPIO_vInit(&LCD_GPIO_RW);
	MGPIO_vInit(&LCD_GPIO_E);
	MGPIO_vInit(&LCD_GPIO_D0);
	MGPIO_vInit(&LCD_GPIO_D1);
	MGPIO_vInit(&LCD_GPIO_D2);
	MGPIO_vInit(&LCD_GPIO_D3);
	MGPIO_vInit(&LCD_GPIO_D4);
	MGPIO_vInit(&LCD_GPIO_D5);
	MGPIO_vInit(&LCD_GPIO_D6);
	MGPIO_vInit(&LCD_GPIO_D7);

	HLCD_vInit();
	MSTK_vSetBusyWait(1000);

	GPIO_cfg_t KEYPAD_B4 = {GPIO_MODE_INPUT, 0, GPIO_INPUTTYPE_PULLUP, GPIO_OUTSPEED_MEDIUM, 0 , GPIO_PORTB, GPIO_PIN4};
	GPIO_cfg_t KEYPAD_B5 = {GPIO_MODE_INPUT, 0, GPIO_INPUTTYPE_PULLUP, GPIO_OUTSPEED_MEDIUM, 0 , GPIO_PORTB, GPIO_PIN5};
	GPIO_cfg_t KEYPAD_B6 = {GPIO_MODE_INPUT, 0, GPIO_INPUTTYPE_PULLUP, GPIO_OUTSPEED_MEDIUM, 0 , GPIO_PORTB, GPIO_PIN6};
	GPIO_cfg_t KEYPAD_B7 = {GPIO_MODE_INPUT, 0, GPIO_INPUTTYPE_PULLUP, GPIO_OUTSPEED_MEDIUM, 0 , GPIO_PORTB, GPIO_PIN7};
	GPIO_cfg_t KEYPAD_B8 = {GPIO_MODE_OUTPUT, GPIO_OUTTYPE_PUSHPULL, 0, GPIO_OUTSPEED_MEDIUM, 0 , GPIO_PORTB, GPIO_PIN8};
	GPIO_cfg_t KEYPAD_B9 = {GPIO_MODE_OUTPUT, GPIO_OUTTYPE_PUSHPULL, 0, GPIO_OUTSPEED_MEDIUM, 0 , GPIO_PORTB, GPIO_PIN9};
	GPIO_cfg_t KEYPAD_B10 = {GPIO_MODE_OUTPUT, GPIO_OUTTYPE_PUSHPULL, 0, GPIO_OUTSPEED_MEDIUM, 0 , GPIO_PORTB, GPIO_PIN10};
	GPIO_cfg_t KEYPAD_B3 = {GPIO_MODE_OUTPUT, GPIO_OUTTYPE_PUSHPULL, 0, GPIO_OUTSPEED_MEDIUM, 0 , GPIO_PORTB, GPIO_PIN3};
	MGPIO_vInit(&KEYPAD_B8);
	MGPIO_vInit(&KEYPAD_B9);
	MGPIO_vInit(&KEYPAD_B10);
	MGPIO_vInit(&KEYPAD_B3);
	MGPIO_vInit(&KEYPAD_B4);
	MGPIO_vInit(&KEYPAD_B5);
	MGPIO_vInit(&KEYPAD_B6);
	MGPIO_vInit(&KEYPAD_B7);

	GPIO_cfg_t SW1_PIN = {GPIO_MODE_INPUT, 0, GPIO_INPUTTYPE_PULLUP, GPIO_OUTSPEED_MEDIUM, 0 , GPIO_PORTB, GPIO_PIN0};
	GPIO_cfg_t SW2_PIN = {GPIO_MODE_INPUT, 0, GPIO_INPUTTYPE_PULLUP, GPIO_OUTSPEED_MEDIUM, 0 , GPIO_PORTB, GPIO_PIN1};
	GPIO_cfg_t SW3_PIN = {GPIO_MODE_INPUT, 0, GPIO_INPUTTYPE_PULLUP, GPIO_OUTSPEED_MEDIUM, 0 , GPIO_PORTB, GPIO_PIN2};
	MGPIO_vInit(&SW1_PIN);
	MGPIO_vInit(&SW2_PIN);
	MGPIO_vInit(&SW3_PIN);

	MEXTI_vEnableLine(EXTI_LINE0, EXTI_FALLING);
	MEXTI_vEnableLine(EXTI_LINE1, EXTI_FALLING);
	MEXTI_vEnableLine(EXTI_LINE2, EXTI_FALLING);
	MEXTI_vSelectionLineCfg(PB, GPIO_PIN0);
	MEXTI_vSelectionLineCfg(PB, GPIO_PIN1);
	MEXTI_vSelectionLineCfg(PB, GPIO_PIN2);
	MEXTI_vSetCallBack(EXTI_LINE0, task_sw1);
	MEXTI_vSetCallBack(EXTI_LINE1, task_sw2);
	MEXTI_vSetCallBack(EXTI_LINE2, task_sw3);
	MNVIC_vSetPriorityCfg(NVIC_8_GROUPS);
	MNVIC_vSetPriority(NVIC_EXTI0, 0, 0);
	MNVIC_vSetPriority(NVIC_EXTI1, 1, 1);
	MNVIC_vSetPriority(NVIC_EXTI2, 2, 1);
	MNVIC_vEnableInterruptPeripheral(NVIC_EXTI0);
	MNVIC_vEnableInterruptPeripheral(NVIC_EXTI1);
	MNVIC_vEnableInterruptPeripheral(NVIC_EXTI2);

	while(1)
	{
		G_u8Pause_Flag_1st_time = 0;
		G_u8Pause_Flag_2nd_time = 0;
		while(!HKEYPAD_vGetPressedKey());
		HLCD_vSendCmd(LCD_CLEAR);
		HLCD_vSendCmd(LCD_CURSOR_FIRST_LINE);
		if(HKEYPAD_vGetPressedKey() == 'A')
		{
			HLCD_vWriteString("Popcorn", sizeof("Popcorn"));
			G_u16timer = 60;
			while(G_u16timer > 0)
			{
				while(G_u8Pause_Flag_1st_time && !G_u8Pause_Flag_2nd_time);		// wait at first time pause;
				if(G_u8Pause_Flag_2nd_time == 1)
				{
					HLCD_vSendCmd(LCD_CLEAR);
					break;									// exit at second time pause
				}
				counter_display();
				G_u16timer--;
				if(G_u16timer == 0)
				{
					MSTK_vSetBusyWait(250000000);
					flash_display();
				}
			}
			HLCD_vSendCmd(LCD_CLEAR);
		}
		else if(HKEYPAD_vGetPressedKey() == 'B')
		{
			HLCD_vWriteString("Beef Weight", sizeof("Beef Weight"));
			while(HKEYPAD_vGetPressedKey() == 'A' || HKEYPAD_vGetPressedKey() == 'B'
					|| HKEYPAD_vGetPressedKey() == 'C' || HKEYPAD_vGetPressedKey() == 'D' || HKEYPAD_vGetPressedKey() == 0);
			G_u16timer = HKEYPAD_vGetPressedKey();
			G_u16timer = G_u16timer - '0';
			HLCD_vCursor_Position(2,1);
			HLCD_vWriteChar(G_u16timer + '0');
			MSTK_vSetBusyWait(250000000);
			HLCD_vSendCmd(LCD_CLEAR);
			G_u16timer*=30;
			while(G_u16timer > 0)
			{
				while(G_u8Pause_Flag_1st_time && !G_u8Pause_Flag_2nd_time);		// wait at first time pause;
				if(G_u8Pause_Flag_2nd_time == 1)
				{
					HLCD_vSendCmd(LCD_CLEAR);
					break;									// exit at second time pause
				}
				counter_display();
				G_u16timer--;
				if(G_u16timer == 0)
				{
					MSTK_vSetBusyWait(250000000);
					flash_display();
				}
			}
			HLCD_vSendCmd(LCD_CLEAR);
		}
		else if(HKEYPAD_vGetPressedKey() == 'C')
		{
			HLCD_vWriteString("Chicken Weight", sizeof("Chicken Weight"));
			while(HKEYPAD_vGetPressedKey() == 'A' || HKEYPAD_vGetPressedKey() == 'B'
					|| HKEYPAD_vGetPressedKey() == 'C' || HKEYPAD_vGetPressedKey() == 'D' || HKEYPAD_vGetPressedKey() == 0);
			G_u16timer = HKEYPAD_vGetPressedKey();
			G_u16timer = G_u16timer - '0';
			HLCD_vCursor_Position(2,1);
			HLCD_vWriteChar(G_u16timer + '0');
			MSTK_vSetBusyWait(250000000);
			HLCD_vSendCmd(LCD_CLEAR);
			G_u16timer*=12;
			while(G_u16timer > 0)
			{
				while(G_u8Pause_Flag_1st_time && !G_u8Pause_Flag_2nd_time);		// wait at first time pause;
				if(G_u8Pause_Flag_2nd_time == 1)
				{
					HLCD_vSendCmd(LCD_CLEAR);
					break;									// exit at second time pause
				}
				counter_display();
				G_u16timer--;
				if(G_u16timer == 0)
				{
					MSTK_vSetBusyWait(250000000);
					flash_display();
				}
			};
			HLCD_vSendCmd(LCD_CLEAR);
		}
		else if(HKEYPAD_vGetPressedKey() == 'D')
		{
			HLCD_vWriteString("Cooking Time?", sizeof("Cooking Time?"));
			G_u16timer = 0;
			G_u8Start_flag = 0;
			G_u8Displayed_Time[0] = 0;
			G_u8Displayed_Time[1] = 0;
			G_u8Displayed_Time[2] = 0;
			G_u8Displayed_Time[3] = 0;
			while(!G_u8Start_flag)
			{
				while(G_u8Pause_Flag_1st_time && !G_u8Pause_Flag_2nd_time);		// wait at first time pause;
				if(G_u8Pause_Flag_2nd_time == 1)
				{
					HLCD_vSendCmd(LCD_CLEAR);
					break;									// exit at second time pause
				}
				HLCD_vCursor_Position(2,1);
				MSTK_vSetBusyWait(100000);
				HLCD_vCursor_Position(2,1);
				HLCD_vWriteChar(G_u8Displayed_Time[3] + '0');
				HLCD_vCursor_Position(2,2);
				HLCD_vWriteChar(G_u8Displayed_Time[2] + '0');			// Minutes
				HLCD_vCursor_Position(2,3);
				HLCD_vWriteChar(':');
				HLCD_vCursor_Position(2,4);
				HLCD_vWriteChar(G_u8Displayed_Time[1] + '0');		// Seconds (tens)
				HLCD_vCursor_Position(2,5);
				HLCD_vWriteChar(G_u8Displayed_Time[0] + '0');		// seconds (units)
				G_u16timer = G_u8Displayed_Time[0] + G_u8Displayed_Time[1]*10
						+ G_u8Displayed_Time[2]*60 + G_u8Displayed_Time[3]*10*60;	// total seconds
				while((HKEYPAD_vGetPressedKey() == 'A' || HKEYPAD_vGetPressedKey() == 'B'
						|| HKEYPAD_vGetPressedKey() == 'C' || HKEYPAD_vGetPressedKey() == 'D' || HKEYPAD_vGetPressedKey() == 0)
						&& (!G_u8Start_flag));
				if(G_u8Start_flag) break;
				G_u16timer = HKEYPAD_vGetPressedKey();
				MSTK_vSetBusyWait(5000000);
				G_u16timer = G_u16timer - '0';
				G_u8Displayed_Time[3] = G_u8Displayed_Time[2];
				G_u8Displayed_Time[2] = G_u8Displayed_Time[1];
				G_u8Displayed_Time[1] = G_u8Displayed_Time[0];
				G_u8Displayed_Time[0] = G_u16timer;
			}
			while(G_u16timer != 0)
			{
				while(G_u8Pause_Flag_1st_time && !G_u8Pause_Flag_2nd_time);		// wait at first time pause;
				if(G_u8Pause_Flag_2nd_time == 1)
				{
					HLCD_vSendCmd(LCD_CLEAR);
					break;									// exit at second time pause
				}
				counter_display();
				G_u16timer--;
				if(G_u16timer == 0)
				{
					MSTK_vSetBusyWait(250000000);
					flash_display();
				}
			}
		}
	}
}

void flash_display()
{
	u8 i;
	for(i = 0; i < 6; i++)
	{
		if(i%2)
		{
			MSTK_vSetBusyWait(250000000);
			HLCD_vCursor_Position(2,1);
			HLCD_vWriteString("00:00", 5);
		}
		else
		{
			MSTK_vSetBusyWait(250000000);
			HLCD_vCursor_Position(2,1);
			HLCD_vWriteString("     ", 5);
		}
	}
}

void counter_display()
{
	HLCD_vCursor_Position(2,1);
	MSTK_vSetBusyWait(250000000);
	HLCD_vCursor_Position(2,1);
	HLCD_vWriteChar(G_u16timer/600 + '0');
	HLCD_vCursor_Position(2,2);
	HLCD_vWriteChar((G_u16timer/60)%10 + '0');
	HLCD_vCursor_Position(2,3);
	HLCD_vWriteChar(':');
	HLCD_vCursor_Position(2,4);
	HLCD_vWriteChar((G_u16timer%60)/10 + '0');
	HLCD_vCursor_Position(2,5);
	HLCD_vWriteChar((G_u16timer%60)%10 + '0');
}
