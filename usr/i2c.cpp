#include "stm32_base.h"
#include "i2c.h"

static SemaphoreHandle_t mutex;
static SemaphoreHandle_t semaphore;

i2c_class::i2c_class(const i2c_cfg_t *const cfg) 
            : cfg(cfg), SCL(cfg->scl_cfg), SDA(cfg->sda_cfg)
{
}

int i2c_class::Init(void)
{   
    int rv;
    if(rv = this->ClockEnable() != 0) return rv;
    
    if( rv = SCL.Init() != 0) return rv;
    if( rv = SDA.Init() != 0) return rv;

    NVIC_SetPriorityGrouping(0);
    if(cfg->I2C == I2C1) {
        NVIC_EnableIRQ(I2C1_EV_IRQn);
        NVIC_EnableIRQ(I2C1_ER_IRQn);
        NVIC_SetPriority(I2C1_EV_IRQn, 11);
        NVIC_SetPriority(I2C1_ER_IRQn, 11);
    } else if(cfg->I2C == I2C2) {
        NVIC_EnableIRQ(I2C2_EV_IRQn);
        NVIC_EnableIRQ(I2C2_ER_IRQn);
        NVIC_SetPriority(I2C2_EV_IRQn, 11);
        NVIC_SetPriority(I2C2_ER_IRQn, 11);
    } else {
        return EINVAL;
    }

    I2C_InitStructure.Instance = cfg->I2C;
    I2C_InitStructure.Init.ClockSpeed = cfg->speed;
    I2C_InitStructure.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    I2C_InitStructure.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    I2C_InitStructure.Init.DutyCycle = I2C_DUTYCYCLE_2;
    I2C_InitStructure.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    I2C_InitStructure.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

    if(HAL_I2C_Init(&I2C_InitStructure) != HAL_OK) return EACCES;
    if(mutex == NULL) mutex = xSemaphoreCreateMutex();
    if(semaphore == NULL) semaphore = xSemaphoreCreateBinary();
    return 0;
}

int i2c_class::ClockEnable(void)
{
    switch(reinterpret_cast<uint32_t>(cfg->I2C)) {

#ifdef I2C1
        case I2C1_BASE:
            __HAL_RCC_I2C1_CLK_ENABLE();
            break;
#endif

#ifdef I2C2
        case I2C2_BASE:
            __HAL_RCC_I2C2_CLK_ENABLE();
            break;
#endif
        default:
            return EINVAL;
    }
    return 0;
}

int i2c_class::Transmit(uint8_t *pdata, uint16_t length, uint16_t address) 
{   
    state = state_t::Busy;
    if(xSemaphoreTake(mutex, cfg->timeout) == pdTRUE) {

        HAL_I2C_Master_Transmit_IT(&I2C_InitStructure, address, pdata, length);

        if(xSemaphoreTake(semaphore, pdMS_TO_TICKS(cfg->timeout)) == pdFALSE) {
            xSemaphoreGive(mutex);
            return ETIME;
        } else {
            xSemaphoreGive(mutex);
            state = state_t::Done;
            return 0;
        }
    } else {
        return ETIME;
    }
}

int i2c_class::Receive(uint8_t *pdata, uint16_t length, uint16_t address) 
{   
    state = state_t::Busy;
    if(xSemaphoreTake(mutex, cfg->timeout) == pdTRUE) {

        HAL_I2C_Master_Receive_IT(&I2C_InitStructure, address, pdata, length);

        if(xSemaphoreTake(semaphore, pdMS_TO_TICKS(cfg->timeout)) == pdFALSE) {
            xSemaphoreGive(mutex);
            return ETIME;
        } else {
            xSemaphoreGive(mutex);
            state = state_t::Done;
            return 0;
        }
    } else {
        return ETIME;
    }
}

int i2c_class::EV_Handler(void) 
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    HAL_I2C_EV_IRQHandler(&I2C_InitStructure);

    if(I2C_InitStructure.State == HAL_I2C_STATE_READY) {
        xSemaphoreGiveFromISR(semaphore, &xHigherPriorityTaskWoken);
        if(xHigherPriorityTaskWoken == pdTRUE) {
            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        }
    }
    return 0;
}

int i2c_class::ER_Handler(void) 
{
    while(1);
}