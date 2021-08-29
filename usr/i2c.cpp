#include "stm32_base.h"
#include "i2c.h"

static SemaphoreHandle_t mutex;
static SemaphoreHandle_t semaphore;

int i2c_class::Init(void)
{   
    int rv;
    if(rv = this->ClockEnable() != 0) return rv;
    if(HAL_I2C_Init(&i2c_cfg->I2C_InitStructure) != HAL_OK) return EACCES;
    if(mutex == NULL) mutex = xSemaphoreCreateMutex();
    if(semaphore == NULL) semaphore = xSemaphoreCreateBinary();
}

int i2c_class::Transmit(uint8_t *pdata, uint16_t length, uint16_t address) 
{
    if(xSemaphoreTake(mutex, i2c_cfg->timeout) == pdTRUE) {

        HAL_I2C_Master_Transmit_IT(&i2c_cfg->I2C_InitStructure, address, pdata, length);

        if(xSemaphoreTake(semaphore, pdMS_TO_TICKS(i2c_cfg->timeout)) == pdFALSE) {
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

int i2c_class::EV_Handler(void) 
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    HAL_I2C_EV_IRQHandler(&i2c_cfg->I2C_InitStructure);

    if(i2c_cfg->I2C_InitStructure.State == HAL_I2C_STATE_READY) {
        xSemaphoreGiveFromISR(semaphore, &xHigherPriorityTaskWoken);
        if(xHigherPriorityTaskWoken == pdTRUE) {
            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        }
    }
}

int i2c_class::ER_Handler(void) 
{
    while(1);
}