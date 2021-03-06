#include "HAL_device.h"
#include "HAL_conf.h"
#include "rcc.h"
#include "sys.h"

/********************************************************************************************************
**function: SetSysClock_HSI
**@brief    This function sets the HSI clock
**                   
**@param    PLL �� setting PLL value 2~16
**
**@return   None.
********************************************************************************************************/
#if 1//using internal RC as clock ref
void SetSysClock_HSI(u8 PLL)
{
  unsigned char temp = 0;
  RCC->CR |= RCC_CR_HSION;
  while (!(RCC->CR & RCC_CR_HSIRDY));
  RCC->CFGR = RCC_CFGR_PPRE1_2;         //APB1=DIV2;APB2=DIV1;AHB=DIV1;
  RCC->CFGR &= ~RCC_CFGR_PLLSRC;        //PLLSRC ON
  RCC->CR &= ~(RCC_CR_HSI_72M_EN);      //clear PLL// RCC->CR &=~(7<<20);   //clear PLL
  RCC->CR &= ~(0x1f << 26);
  RCC->CR |= (PLL - 1) << 26;           //setting PLL value 2~16
  FLASH->ACR = FLASH_ACR_LATENCY_1 | FLASH_ACR_PRFTBE; //FLASH 2 delay clk cycles
  RCC->CR |= RCC_CR_PLLON;              //PLLON
  while (!(RCC->CR & RCC_CR_PLLRDY));   //waiting for PLL locked
  RCC->CFGR &= ~RCC_CFGR_SW;
  RCC->CFGR |= RCC_CFGR_SW_HSI;         //PLL to be the sys clock
  while (temp != 0x02)                  //waiting PLL become the sys clock
  {
    temp = RCC->CFGR >> 2;
    temp &= 0x03;
  }
}
/********************************************************************************************************
**function: SysClk8to48
**@brief    This function sets the HSI clock from 8MHZ to 48MHZ.Turn off low power mode.
**                   
**@param    None.
**
**@return   None.
********************************************************************************************************/
void SysClk8to48(void)//////DO NOT REMOVE, used in ble lib//////
{
  unsigned char temp = 0;
  RCC->CR |= RCC_CR_HSION;
  while (!(RCC->CR & RCC_CR_HSIRDY));
  RCC->CR &= ~(1 << 20);
  RCC->CR &= ~(1 << 2);
  RCC->CFGR = RCC_CFGR_PPRE1_2;
  FLASH->ACR = FLASH_ACR_LATENCY_1 | FLASH_ACR_PRFTBE;
  RCC->CFGR &= ~RCC_CFGR_SW;
  RCC->CFGR |= 0x02;
  while (temp != 0x02)
  {
    temp = RCC->CFGR >> 2;
    temp &= 0x03;
  }
}
/********************************************************************************************************
**function: SysClk48to8
**@brief    This function sets the HSI clock from 48MHZ to 8MHZ.Turn on low power mode.
**                   
**@param    None.
**
**@return   None.
********************************************************************************************************/
void SysClk48to8(void)
{
  RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);//selecting PLL clock as sys clock
  while (RCC_GetSYSCLKSource() != 0x0)
  {}
  RCC->CR &= ~(RCC_CR_PLLON);   //clear PLL
  SysTick_Config(8000);
}
#endif

