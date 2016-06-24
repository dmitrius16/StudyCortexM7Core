//
//HardwareDef.h - defines hardware perephirial used in project
// 
#pragma once
#include "stm32f7xx_hal.h"

//########################### UART description #########################

#define USART1_CLK_ENABLE()              __HAL_RCC_USART1_CLK_ENABLE()
#define USART1_RX_GPIO_CLK_ENABLE()      __GPIOB_CLK_ENABLE()
#define USART1_TX_GPIO_CLK_ENABLE()      __GPIOA_CLK_ENABLE()

#define USART1_FORCE_RESET()             __HAL_RCC_USART1_FORCE_RESET()
#define USART1_RELEASE_RESET()           __HAL_RCC_USART1_RELEASE_RESET()

/* Definition for USARTx Pins */
#define USART1_TX_PIN                    GPIO_PIN_9
#define USART1_TX_GPIO_PORT              GPIOA
#define USART1_TX_AF                     GPIO_AF7_USART1
#define USART1_RX_PIN                    GPIO_PIN_7
#define USART1_RX_GPIO_PORT              GPIOB
#define USART1_RX_AF                     GPIO_AF7_USART1


/* Definition for USARTx's NVIC */
#define UART1_IRQn                      USART1_IRQn
#define UART1_IRQHandler                USART1_IRQHandler
//#elif defined (USE_USART1_CONSOLE)

#define USARTx_CLK_ENABLE(UART_HANDLE)  UART_HANDLE##_CLK_ENABLE()
#define USARTx_RX_GPIO_CLK_ENABLE(UART_HANDLE)  UART_HANDLE##_RX_GPIO_CLK_ENABLE()
#define USARTx_TX_GPIO_CLK_ENABLE(UART_HANDLE) UART_HANDLE##_TX_GPIO_CLK_ENABLE()

#define USARTx_FORCE_RESET(UART_HANDLE) UART_HANDLE##_FORCE_RESET()
#define USARTx_RELEASE_RESET(UART_HANDLE) UART_HANDLE##_RELEASE_RESET()


#define USARTx_TX_PIN(UART_HANDLE) UART_HANDLE##_TX_PIN
#define USARTx_TX_GPIO_PORT(UART_HANDLE) UART_HANDLE##_TX_GPIO_PORT
#define USARTx_TX_AF(UART_HANDLE) UART_HANDLE##_TX_AF

#define USARTx_RX_PIN(UART_HANDLE) UART_HANDLE##_RX_PIN
#define USARTx_RX_GPIO_PORT(UART_HANDLE) UART_HANDLE##_RX_GPIO_PORT
#define USARTx_RX_AF(UART_HANDLE) UART_HANDLE##_RX_AF

#define USARTx_IRQ(UART_HANDLE) UART_HANDLE##_IRQn
#define USARTx_IRQHandler(UART_HANDLE) UART_HANDLE##_IRQHandler


// ################# End UART peripherial description ####################

//####################### Timer for timeBase definition #####################
#define TIMx                        TIM2

#define TIMx_CLK_ENABLE()           __TIM2_CLK_ENABLE()

#define TIMx_FORCE_RESET()			__TIM2_FORCE_RESET()
#define TIMx_RELEASE_RESET()		__TIM2_RELEASE_RESET() 

/*Definition for TIM NVIC*/
#define TIMx_IRQn                  TIM2_IRQn
#define TIMx_IRQHandler            TIM2_IRQHandler
                                   
//########################## End Timer definition ###########################

//########################## Timer for accept WDT TU ########################
#define TIM_WDT_TU                  TIM3
#define TIM_WDT_TU_CLK_ENABLE()      __TIM3_CLK_ENABLE()
#define TIM_WDT_TU_FORCE_RESET()     __TIM3_FORCE_RESET()
#define TIM_WDT_TU_RELEASE_RESET()   __TIM3_RELEASE_RESET()

/* definition for TIMx_IRQn */
#define TIM_WDT_TU_IRQn             TIM3_IRQn
#define TIM_WDT_TU_IRQHandler       TIM3_IRQHandler

//########################## End Timer description ##########################

