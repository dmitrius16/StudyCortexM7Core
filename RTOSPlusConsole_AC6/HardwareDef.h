//
//HardwareDef.h - defines hardware perephirial used in project
// 
#pragma once
#include "stm32f7xx_hal.h"

//########################### UART description #########################
#define USE_UART4_CONSOLE
#undef USE_UART4_CONSOLE
#define USE_USART1_CONSOLE

//#undef USE_USART1_CONSOLE
//#ifdef USE_UART4_CONSOLE

//#define USARTx                           UART4

#define USART4_CLK_ENABLE()              __UART4_CLK_ENABLE()
#define USART4_RX_GPIO_CLK_ENABLE()      __GPIOC_CLK_ENABLE()
#define USART4_TX_GPIO_CLK_ENABLE()      __GPIOC_CLK_ENABLE()

#define USART4_FORCE_RESET()             __UART4_FORCE_RESET()
#define USART4_RELEASE_RESET()           __UART4_RELEASE_RESET()

/* Definition for USARTx Pins */
#define USART4_TX_PIN                    GPIO_PIN_10
#define USART4_TX_GPIO_PORT              GPIOC  
#define USART4_TX_AF                     GPIO_AF8_UART4
#define USART4_RX_PIN                    GPIO_PIN_11
#define USART4_RX_GPIO_PORT              GPIOC 
#define USART4_RX_AF                     GPIO_AF8_UART4

/* Definition for USARTx's NVIC */
//#define USART4_IRQn                      UART4_IRQn
//#define USART4_IRQHandler                UART4_IRQHandler
//#elif defined (USE_USART1_CONSOLE)

//#define USARTx                           USART1
#define USART1_CLK_ENABLE()              __USART1_CLK_ENABLE()
#define USART1_RX_GPIO_CLK_ENABLE()      __GPIOA_CLK_ENABLE()
#define USART1_TX_GPIO_CLK_ENABLE()      __GPIOA_CLK_ENABLE() 

#define USART1_FORCE_RESET()             __USART1_FORCE_RESET()
#define USART1_RELEASE_RESET()           __USART1_RELEASE_RESET()

/* Definition for USARTx Pins */
#define USART1_TX_PIN                    GPIO_PIN_9
#define USART1_TX_GPIO_PORT              GPIOA  
#define USART1_TX_AF                     GPIO_AF7_USART1
#define USART1_RX_PIN                    GPIO_PIN_10
#define USART1_RX_GPIO_PORT              GPIOA 
#define USART1_RX_AF                     GPIO_AF7_USART1

/* Definition for USARTx's NVIC */
//#define USARTx_IRQn                      USART1_IRQn
//#define USARTx_IRQHandler                USART1_IRQHandler

///#endif

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

