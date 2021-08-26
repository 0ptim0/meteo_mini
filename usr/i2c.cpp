#include "stm32_base.h"
#include "i2c.h"

static SemaphoreHandle_t mutex;
static SemaphoreHandle_t semaphore;

void i2c::Init(void)
{   
    HAL_I2C_Init(&I2C_InitStructure);
    if(mutex == NULL) mutex = xSemaphoreCreateMutex();
    if(semaphore == NULL) semaphore = xSemaphoreCreateBinary();
}

int i2c::Transmit(uint8_t *pdata, uint16_t length, uint16_t address) 
{
    if(xSemaphoreTake(mutex, timeout) == pdTRUE) {

        HAL_I2C_Master_Transmit_IT(&I2C_InitStructure, address, pdata, length);

        if(xSemaphoreTake(semaphore, pdMS_TO_TICKS(timeout)) == pdFALSE) {
            xSemaphoreGive(mutex);
            return pdFALSE;
        } else {
            xSemaphoreGive(mutex);
            return pdTRUE;
        }
    } else {
        return pdFALSE;
    }
}

int i2c::EV_Handler(void) 
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    HAL_I2C_EV_IRQHandler(&I2C_InitStructure);

    if(I2C_InitStructure.State == HAL_I2C_STATE_READY) {
        xSemaphoreGiveFromISR(semaphore, &xHigherPriorityTaskWoken);
        if(xHigherPriorityTaskWoken == pdTRUE) {
            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        }
    }
}

int i2c::ER_Handler(void) 
{
    while(1);
}