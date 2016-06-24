#include "StdAfx.h"
#include "DriverUART.h"

const int IndUARTDesc_1 = 0;
const int IndUARTDesc_4 = 1;

const int BUFFER_SIZE = 20;

class CFifoQueue
{
    BYTE *pBuffer;
    const short m_SizeQueue;
    short m_CntFreeElem;
    short m_HeadElemInd;
    short m_TailElemInd;
private:
    void IncElemInd(short &ind)
    {
        ind++;
        ind %= m_SizeQueue;
    }
public:
    CFifoQueue(BYTE *pBuf,short size):pBuffer(pBuf),m_SizeQueue(size),m_CntFreeElem(size),m_HeadElemInd(0),m_TailElemInd(0){}
    bool PopElem(BYTE &val)
    {
        if(m_CntFreeElem == m_SizeQueue)
            return false;
        val = pBuffer[m_HeadElemInd];
        IncElemInd(m_HeadElemInd);
        m_CntFreeElem++;
        return true;
    }
    bool PushElem(BYTE val)
    {
        if(GetCntFreeElems() != 0) 
        {
            m_CntFreeElem--;
            pBuffer[m_TailElemInd] = val;
            IncElemInd(m_TailElemInd);
            return true;
        }
        return false;
    }
    short GetCntFreeElems(){return m_CntFreeElem;}
	short GetCntElem(){return  m_SizeQueue-m_CntFreeElem;}
	bool IsQueueEmpty()
	{
		return m_CntFreeElem == m_SizeQueue;
	}
	void FlushQueue()
	{
		m_CntFreeElem = m_SizeQueue;
		m_HeadElemInd = 0;
		m_TailElemInd = 0;
	}
};

typedef struct
{
	UART_HandleTypeDef uartHandle;
	CFifoQueue *pFifoBuf;
	CSemaphore m_SmphrRx;
	CSemaphore m_SmphrTx;
}UART_DRIVER_DESCRIPTOR;

UART_DRIVER_DESCRIPTOR uartDesc[NUM_USED_UARTS];

int FindUartDescFromHandle(UART_HandleTypeDef *UartHandle);
static void UART_IRQHandler(UART_HandleTypeDef *huart);
static void UART_RX_ISR(UART_HandleTypeDef *huart);
static void UART_TX_ISR(UART_HandleTypeDef *huart);


extern "C"{

void USART1_IRQHandler(void)
{
    //UART_IRQHandler(&uartDesc[IndUARTDesc_1].uartHandle);
	UART_IRQHandler(&uartDesc[IndUARTDesc_1].uartHandle);
}
}

void UART_IRQHandler(UART_HandleTypeDef *huart)
{
	  /* UART parity error interrupt occurred -------------------------------------*/
	  if((__HAL_UART_GET_IT(huart, UART_IT_PE) != RESET) && (__HAL_UART_GET_IT_SOURCE(huart, UART_IT_PE) != RESET))
	  {
			__HAL_UART_CLEAR_PEFLAG(huart);

			huart->ErrorCode |= HAL_UART_ERROR_PE;
			huart->State = HAL_UART_STATE_READY;
	  }

	  /* UART frame error interrupt occurred --------------------------------------*/
	  if((__HAL_UART_GET_IT(huart, UART_IT_FE) != RESET) && (__HAL_UART_GET_IT_SOURCE(huart, UART_IT_ERR) != RESET))
	  {
		  __HAL_UART_CLEAR_FEFLAG(huart);

		  huart->ErrorCode |= HAL_UART_ERROR_FE;
		  huart->State = HAL_UART_STATE_READY;
	  }

	  /* UART noise error interrupt occurred --------------------------------------*/
	  if((__HAL_UART_GET_IT(huart, UART_IT_NE) != RESET) && (__HAL_UART_GET_IT_SOURCE(huart, UART_IT_ERR) != RESET))
	  {
		  __HAL_UART_CLEAR_NEFLAG(huart);

		  huart->ErrorCode |= HAL_UART_ERROR_NE;
		  huart->State = HAL_UART_STATE_READY;
	  }

	  /* UART Over-Run interrupt occurred -----------------------------------------*/
	  if((__HAL_UART_GET_IT(huart, UART_IT_ORE) != RESET) && (__HAL_UART_GET_IT_SOURCE(huart, UART_IT_ERR) != RESET))
	  {
		  __HAL_UART_CLEAR_OREFLAG(huart);

		  huart->ErrorCode |= HAL_UART_ERROR_ORE;
		  huart->State = HAL_UART_STATE_READY;
	  }

	   /* Call UART Error Call back function if need be --------------------------*/
	  if(huart->ErrorCode != HAL_UART_ERROR_NONE)
	  {
		  HAL_UART_ErrorCallback(huart);
	  }

	  /* UART in mode Receiver ---------------------------------------------------*/
	  if((__HAL_UART_GET_IT(huart, UART_IT_RXNE) != RESET) && (__HAL_UART_GET_IT_SOURCE(huart, UART_IT_RXNE) != RESET))
	  {
		  UART_RX_ISR(huart);
		// Clear RXNE interrupt flag
		//__HAL_UART_SEND_REQ(huart, UART_RXDATA_FLUSH_REQUEST);	///~~~ clear it ????
	  }


	  // UART in mode Transmitter
	 if((__HAL_UART_GET_IT(huart, UART_IT_TXE) != RESET) &&(__HAL_UART_GET_IT_SOURCE(huart, UART_IT_TXE) != RESET))
	 {
		 UART_TX_ISR(huart);
	 }

	  // UART in mode Transmitter (transmission end) end transmission interrupt
	 if((__HAL_UART_GET_IT(huart, UART_IT_TC) != RESET) &&(__HAL_UART_GET_IT_SOURCE(huart, UART_IT_TC) != RESET))
	 {
		 __HAL_UART_CLEAR_IT(huart,USART_ICR_TCCF);
	 }
}


void UART_RX_ISR(UART_HandleTypeDef *huart)
{
    uint32_t tmp1 = 0;   
    tmp1 = huart->State; 
    if((tmp1 == HAL_UART_STATE_BUSY_RX) || (tmp1 == HAL_UART_STATE_BUSY_TX_RX))
    {
    	uint8_t rxValue =  (uint8_t)(huart->Instance->RDR & (uint8_t)0x00FF);
    	BYTE indDesc = FindUartDescFromHandle(huart);
        uartDesc[indDesc].pFifoBuf->PushElem(rxValue);
    }
}


void UART_TX_ISR(UART_HandleTypeDef *huart)
{
	uint16_t* tmp;

	if ((huart->State == HAL_UART_STATE_BUSY_TX) || (huart->State == HAL_UART_STATE_BUSY_TX_RX))
	{
		huart->Instance->TDR = (uint8_t)(*huart->pTxBuffPtr++ & (uint8_t)0xFF);
		huart->TxXferCount--;

		if(huart->TxXferCount == 0)
		{
			  // Disable the UART Transmit Data Register Empty Interrupt
			  __HAL_UART_DISABLE_IT(huart, UART_IT_TXE);

			  /* Check if a receive Process is ongoing or not */
			  if(huart->State == HAL_UART_STATE_BUSY_TX_RX)
				  huart->State = HAL_UART_STATE_BUSY_RX;
			  else
				  huart->State = HAL_UART_STATE_READY;

			  // Wait on TC flag to be able to start a second transfer
			  if(UART_WaitOnFlagUntilTimeout(huart, UART_FLAG_TC, RESET, HAL_UART_TIMEOUT_VALUE) != HAL_OK)
				  return;

			  HAL_UART_TxCpltCallback(huart);
			  return;
		}
	}
}

int FindUartDescFromHandle(UART_HandleTypeDef *UartHandle)
{
    bool res = false;
	int i;
	for(i=0; i < NUM_USED_UARTS; i++)
	{
		if(UartHandle == &uartDesc[i].uartHandle)
		{	
			res = true;
			break;
		}
	}
	if(!res) i = -1;
	return i;
}

/**
  * @brief  Tx Transfer completed callback
  * @param  UartHandle: UART handle. 
  * @note   This example shows a simple way to report end of IT Tx transfer, and 
  *         you can add your own implementation. 
  * @retval None
  */

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *UartHandle)
{
	int i = FindUartDescFromHandle(UartHandle);
	if(i >=0 )
		uartDesc[i].m_SmphrTx.PostSemaphore();
}

/**
  * @brief  Rx Transfer completed callback
  * @param  UartHandle: UART handle
  * @note   This example shows a simple way to report end of IT Rx transfer, and 
  *         you can add your own implementation.
  * @retval None
  */

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
	int i = FindUartDescFromHandle(UartHandle);
	if(i >=0 )
		uartDesc[i].m_SmphrRx.PostSemaphore();
}
/**
  * @brief  UART error callbacks
  * @param  UartHandle: UART handle
  * @note   This example shows a simple way to report transfer error, and you can
  *         add your own implementation.
  * @retval None
  */

void HAL_UART_ErrorCallback(UART_HandleTypeDef *UartHandle)
{
  /* Turn LED3 on: Transfer error in reception/transmission process */
}

//think about initialization
bool CDriverUART::Init(USART_TypeDef *pUsart)
{
	return Init(pUsart, 115200, UART_WORDLENGTH_8B, UART_STOPBITS_1, UART_PARITY_NONE);
}

bool CDriverUART::Init(USART_TypeDef *pUsart, UINT BaudRate, UINT WordLength, UINT StopBits, UINT Parity)
{
	if((pUsart == UART4 || pUsart == USART1) == false)
		return false;
		
	m_HandlInd = pUsart == UART4 ? IndUARTDesc_4 : IndUARTDesc_1;
	uartDesc[m_HandlInd].uartHandle.Instance = pUsart;
	
	if(!uartDesc[m_HandlInd].m_SmphrRx.Create(0,1))
		return false;
	if(!uartDesc[m_HandlInd].m_SmphrTx.Create(0,1))
		return false;
	
	uartDesc[m_HandlInd].uartHandle.Init.BaudRate   = BaudRate;
	uartDesc[m_HandlInd].uartHandle.Init.WordLength = WordLength;
    uartDesc[m_HandlInd].uartHandle.Init.StopBits   = StopBits;
    uartDesc[m_HandlInd].uartHandle.Init.Parity     = Parity;
    uartDesc[m_HandlInd].uartHandle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
    uartDesc[m_HandlInd].uartHandle.Init.Mode       = UART_MODE_TX_RX;

	bool res =  HAL_UART_Init(&uartDesc[m_HandlInd].uartHandle) == HAL_OK;

    pRxBuffer = new BYTE[BUFFER_SIZE];
    
    if(!pRxBuffer)
        res = false;

    uartDesc[m_HandlInd].pFifoBuf = new CFifoQueue(pRxBuffer,BUFFER_SIZE);
    
	return res;
}

bool CDriverUART::Open()
{
    //create buffer 
	m_bOpen = true;
	uartDesc[m_HandlInd].uartHandle.ErrorCode = HAL_UART_ERROR_NONE;
    
	// Check if a transmit process is ongoing or not

	if(uartDesc[m_HandlInd].uartHandle.State == HAL_UART_STATE_BUSY_TX)
    {
		uartDesc[m_HandlInd].uartHandle.State = HAL_UART_STATE_BUSY_TX_RX;
    }
    else
    {
		uartDesc[m_HandlInd].uartHandle.State = HAL_UART_STATE_BUSY_RX;
    }
    
    // Enable the UART Parity Error Interrupt
    __HAL_UART_ENABLE_IT(&uartDesc[m_HandlInd].uartHandle, UART_IT_PE);
    // Enable the UART Error Interrupt: (Frame error, noise error, overrun error)
    __HAL_UART_ENABLE_IT(&uartDesc[m_HandlInd].uartHandle, UART_IT_ERR);
	__HAL_UART_ENABLE_IT(&uartDesc[m_HandlInd].uartHandle, UART_IT_RXNE);

    return true;
}

bool CDriverUART::Close()
{
	m_bOpen = false;
	return true;
}

void CDriverUART::Turn485Tx()
{
    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_7,GPIO_PIN_SET);
}
void CDriverUART::Turn485Rx()
{
    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_7,GPIO_PIN_RESET);
}

void CDriverUART::Flush()
{
	//disable uart interrupt
	 __disable_irq();
		uartDesc[m_HandlInd].pFifoBuf->FlushQueue();
	 __enable_irq();
	//enable uart interrupt
}

//******************************************************************************************************
//* ReadCh - read character from UART
//******************************************************************************************************
//* Output:
//*		char if character exists in fifo return charactern else 0
//******************************************************************************************************
char CDriverUART::ReadCh()
{
	char val;
	__disable_irq();
	if(!uartDesc[m_HandlInd].pFifoBuf->PopElem((BYTE&)val))
		val = 0;
	__enable_irq();
	return val;
}

BYTE CDriverUART::Available()
{
	if(!m_bOpen)
		return 0;
	BYTE cntElem;
	__disable_irq();
	cntElem = uartDesc[m_HandlInd].pFifoBuf->GetCntElem();
	 __enable_irq();
	return cntElem;
}


BYTE CDriverUART::ReadBytes(BYTE *rxBuffer,int len)
{
	if(!m_bOpen)
		return 0;
	__disable_irq();
	BYTE cnt = 0;
	for(int i=0; i < len; i++)
	{
		if(!uartDesc[m_HandlInd].pFifoBuf->PopElem(rxBuffer[i]))
			break;
		cnt++;
	}
	__enable_irq();
	return cnt;
}



BYTE CDriverUART::Write(BYTE val)
{
	return WriteBytes(&val,1);
}

BYTE CDriverUART::WriteBytes(BYTE *txBuffer,int len)
{
	if(!m_bOpen)
		return 0;
#if defined(TU8_APP)	
	//if(m_HandlInd == IndUARTDesc_1)
        Turn485Tx();
#endif
    while(HAL_UART_Transmit_IT(&uartDesc[m_HandlInd].uartHandle, (uint8_t*)txBuffer, len) != HAL_OK)
    {    
        osDelay(1);
    }
	
    if(!uartDesc[m_HandlInd].m_SmphrTx.WaitSemaphoreMs(1000))
		__NOP();

	return len;
}
