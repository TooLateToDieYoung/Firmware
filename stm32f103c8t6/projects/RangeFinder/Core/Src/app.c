#include "main.h"

#ifndef STM32F103xx_UNREADY

typedef enum
{
  Nothing = 0x00,
  HC05_WAIT = 0x01,
  HC05_BUSY = 0x10,
  LSM6DS3_WAIT = 0x02,
  LSM6DS3_BUSY = 0x20,
  VL53L1X_WAIT = 0x03,
  VL53L1X_BUSY = 0x30
} APP_FLAG_Enum;

/** Class Private Variables Begin ---------------------------------------------------------------
 * @brief
 *
 */

static struct
{

  struct
  {
    HC05_DS *restrict device;
    Buffer_DS *restrict rx;
    Buffer_DS *restrict tx;
  } hc05;

  struct
  {
    LSM6DS3_DS *restrict device;
    Buffer_DS *restrict buffer;
  } lsm6ds3;

  struct
  {
    VL53L1X_DS *restrict device;
    uint16_t distance;
  } vl53l1x;

  struct
  {
    SevenSegment_DS *restrict device[3];
    uint8_t number[3];
  } display;

  struct
  {
    Button_DS *restrict device;
    bool_t isToggled;
  } button;

  volatile flag8_t schedule;

} app;

/* ---------------------------------------------------------------- Class Private Variables End */

/** Class Functions Forward Declare Begin -------------------------------------------------------
 * @brief
 *
 */

// ! Private
// ? HC05 ---------------------------------------------------------------------------------------
static task_t HC05_Init(void);
static task_t HC05_Printf(const uint8_t str[], size_t len);

// ? LSM6DS3 ------------------------------------------------------------------------------------
static task_t LSM6DS3_Init(void);
static task_t LSM6DS3_Task(void);

// ? VL53L1X ------------------------------------------------------------------------------------
static task_t VL53L1X_Init(void);
static task_t VL53L1X_Task(void);

// ? Seven Segment ------------------------------------------------------------------------------
static task_t SevenSegment_Init(void);

// ? Button -------------------------------------------------------------------------------------
static task_t Button_Init(void);

// ! Public
// ? Init --------------------------------------------------------------------------------------
task_t APP_Init(void);

// ? Loop --------------------------------------------------------------------------------------
task_t APP_Task(void);

// ? SysTick IT ----------------------------------------------------------------------------------
void APP_SysTick_Handler(void);

// ? TIM4 IT ------------------------------------------------------------------------------------
void APP_TIM4_IRQHandler(void);

// ? USART1 IT ----------------------------------------------------------------------------------
void APP_USART1_IRQHandler(void);

/* ------------------------------------------------------- Class Functions Forward Declare End */

/** Class Private Functions Begin ---------------------------------------------------------------
 * @brief
 *
 */

// ? HC05 ---------------------------------------------------------------------------------------
static task_t HC05_Init(void)
{
  app.hc05.device = HC05_Constructor(USART1);
  app.hc05.tx = Buffer_Constructor(20);
  app.hc05.rx = Buffer_Constructor(10);

  if (!app.hc05.device)
    return Fail;
  if (!app.hc05.tx)
    return Fail;
  if (!app.hc05.rx)
    return Fail;

  return Success;
}

static task_t HC05_Printf(const uint8_t str[], size_t len)
{
  app.schedule |= HC05_BUSY;

  Buffer_Flush(app.hc05.tx);
  Buffer_Flush(app.hc05.rx);

  for (size_t i = 0; i < len; ++i)
    Buffer_Push(app.hc05.tx, str[i]);

  // ? clear residual data
  uint8_t temp = app.hc05.device->USARTx->DR;
  (void)temp;

  // app.hc05.device->USARTx->CR1 |= _BIT(5); // enable RXNEIE
  app.hc05.device->USARTx->CR1 |= _BIT(7); // enable TXEIE

  return Success;
}

// ? LSM6DS3 ------------------------------------------------------------------------------------
static task_t LSM6DS3_Init(void)
{
  const port_t CS = {.GPIOx = SCS_GPIO_Port, .order = 4};
  app.lsm6ds3.device = LSM6DS3_Constructor(SPI1, &CS); // pin order is 4 -> GPIOA pin 4
  app.lsm6ds3.buffer = Buffer_Constructor(6);

  if (!app.lsm6ds3.device)
    return Fail;
  if (!app.lsm6ds3.buffer)
    return Fail;

  LL_SPI_Enable(SPI1);
  LL_mDelay(200);

  return LSM6DS3_DefaultInit(app.lsm6ds3.device);
}

static task_t LSM6DS3_Task(void)
{
  app.schedule |= LSM6DS3_BUSY;
  app.schedule &= LSM6DS3_WAIT;

  uint8_t value = 0, raw[2] = {0}, str[5] = {0};
  sint16_t sdata = 0;
  uint16_t udata = 0;

  if (LSM6DS3_getRegister(app.lsm6ds3.device, STATUS_R, &value, 1000) != Success)
    return Fail;

  if (!_MASK(value, _BIT(0)))
    return Fail;

  if (LSM6DS3_getRegister(app.lsm6ds3.device, ACCE_Z_L, &raw[0], 1000) != Success)
    return Fail;

  if (LSM6DS3_getRegister(app.lsm6ds3.device, ACCE_Z_H, &raw[1], 1000) != Success)
    return Fail;

  sdata = (sint16_t)((raw[1] << 8) | raw[0]);
  udata = (sdata < 0) ? (-sdata) : (+sdata);

  str[0] = (sdata < 0) ? '-' : '+';

  udata /= 163;
  str[4] = '0' + (udata % 10);

  udata /= 10;
  str[3] = '0' + (udata % 10);

  str[2] = '.';

  udata /= 10;
  str[1] = '0' + (udata % 10);

  while (_MASK(app.schedule, HC05_BUSY))
  {
  }
  app.schedule &= ~LSM6DS3_BUSY;
  return HC05_Printf(str, 5);
}

// ? VL53L1X ------------------------------------------------------------------------------------
static task_t VL53L1X_Init(void)
{
  app.vl53l1x.device = VL53L1X_Constructor(I2C1, 0x52);

  if (!app.vl53l1x.device)
    return Fail;

  LL_I2C_Enable(I2C1);
  LL_mDelay(200);

  return VL53L1X_DefaultInit(app.vl53l1x.device);
}

static task_t VL53L1X_Task(void)
{
  task_t status = Success;

  app.schedule |= VL53L1X_BUSY;
  app.schedule &= VL53L1X_WAIT;

  while (!VL53L1X_isDataReady(app.vl53l1x.device))
  {
  }

  status = VL53L1X_GetDistance(app.vl53l1x.device, &app.vl53l1x.distance);
  if (status != Success)
    goto __END;

  app.vl53l1x.distance /= 10;
  app.display.number[0] = app.vl53l1x.distance % 10;

  app.vl53l1x.distance /= 10;
  app.display.number[1] = app.vl53l1x.distance % 10;

  app.vl53l1x.distance /= 10;
  app.display.number[2] = app.vl53l1x.distance % 10;

__END:
  app.schedule &= ~VL53L1X_BUSY;
  return status;
}

// ? Seven Segment ------------------------------------------------------------------------------
static task_t SevenSegment_Init(void)
{
  const port_t commonTable[3] = {
      [0] = {.GPIOx = SSG1_GPIO_Port, .order = 14},
      [1] = {.GPIOx = SSG2_GPIO_Port, .order = 13},
      [2] = {.GPIOx = SSG3_GPIO_Port, .order = 12}};

  const port_t ioTable[8] = {
      [0] = {.GPIOx = SSA_GPIO_Port, .order = 8},
      [1] = {.GPIOx = SSB_GPIO_Port, .order = 3},
      [2] = {.GPIOx = SSC_GPIO_Port, .order = 11},
      [3] = {.GPIOx = SSD_GPIO_Port, .order = 1},
      [4] = {.GPIOx = SSE_GPIO_Port, .order = 0},
      [5] = {.GPIOx = SSF_GPIO_Port, .order = 4},
      [6] = {.GPIOx = SSG_GPIO_Port, .order = 9},
      [7] = {.GPIOx = SSP_GPIO_Port, .order = 10}};

  for (size_t i = 0; i < 3; ++i)
  {
    app.display.device[i] = SevenSegment_Constructor(&commonTable[i], ioTable);

    if (!app.display.device[i])
      return Fail;
  }

  TIM4->DIER |= _BIT(0); // enable UIE
  TIM4->CR1 |= _BIT(0);  // enable CEN

  return Success;
}

// ? Button -------------------------------------------------------------------------------------
static task_t Button_Init(void)
{
  const port_t pin = {.GPIOx = SWFD_GPIO_Port, .order = 15};

  app.button.device = Button_Constructor(&pin);

  return app.button.device ? Success : Fail;
}

/* ---------------------------------------------------------------- Class Private Functions End */

/** Class Public Functions Begin ---------------------------------------------------------------
 * @brief
 *
 */

// ? Init --------------------------------------------------------------------------------------
task_t APP_Init(void)
{
  // reset flag
  app.schedule = Nothing;

  // hc05
  if (HC05_Init() == Success)
    HC05_Printf((uint8_t *)"hc05 init done.\r\n", 17);
  // else return _ERROR();

  // lsm6ds3
  if (LSM6DS3_Init() == Success)
    HC05_Printf((uint8_t *)"lsm6ds3 init done.\r\n", 20);
  else
    HC05_Printf((uint8_t *)"lsm6ds3 init fail.\r\n", 20);

  // vl53l1x
  if (VL53L1X_Init() == Success)
    HC05_Printf((uint8_t *)"vl53v1x init done.\r\n", 20);
  else
    HC05_Printf((uint8_t *)"vl53v1x init fail.\r\n", 20);

  // button
  if (Button_Init() == Success)
    HC05_Printf((uint8_t *)"button init done.\r\n", 19);
  else
    HC05_Printf((uint8_t *)"button init fail.\r\n", 19);

  // display
  if (SevenSegment_Init() == Success)
    HC05_Printf((uint8_t *)"display init done.\r\n", 20);
  else
    HC05_Printf((uint8_t *)"display init fail.\r\n", 20);

  // enable systick interrupt
  SysTick->CTRL =
      SysTick_CTRL_ENABLE_Msk | // Enable SysTick Timer
      SysTick_CTRL_TICKINT_Msk; // Enable SysTick Timer Interrupt

  return Success;
}

// ? Loop --------------------------------------------------------------------------------------
task_t APP_Task(void)
{
  while (!Button_isFree(app.button.device))
  {
  }

  // check if any module is busy
  if (_MASK(app.schedule, 0xF0))
    return Success;

  if (app.button.isToggled && Button_isFree(app.button.device))
  {

    if (_MASK(app.schedule, VL53L1X_WAIT))
      VL53L1X_Task();

    if (_MASK(app.schedule, LSM6DS3_WAIT))
      LSM6DS3_Task();

    app.button.isToggled = False;
  }

  return Success;
}

// ? SysTick IT ----------------------------------------------------------------------------------
void APP_SysTick_Handler(void)
{
  if (_MASK(app.schedule, HC05_BUSY))
    return;

  if (!_MASK(app.schedule, LSM6DS3_BUSY))
    app.schedule |= LSM6DS3_WAIT;

  if (!_MASK(app.schedule, VL53L1X_BUSY))
    app.schedule |= VL53L1X_WAIT;

  if (!app.button.isToggled && !Button_isFree(app.button.device))
    app.button.isToggled = True;
}

// ? TIM4 IT ------------------------------------------------------------------------------------
void APP_TIM4_IRQHandler(void)
{
  static uint8_t index = 0;
  static const flag8_t segmentTable[] = {Num0, Num1, Num2, Num3, Num4, Num5, Num6, Num7, Num8, Num9};

  if (_MASK(TIM4->SR, _BIT(0)))
  {
    TIM4->SR &= ~_BIT(0);

    SevenSegment_Reset(app.display.device[index]);

    index = (index + 1) % 3;

    flag8_t segment = segmentTable[app.display.number[index]];

    if (index == 2)
      segment |= SegP;

    SevenSegment_Set(app.display.device[index], segment);
  }
}

// ? USART1 IT ----------------------------------------------------------------------------------
void APP_USART1_IRQHandler(void)
{
  const flag32_t flag = app.hc05.device->USARTx->SR;

  volatile uint8_t byte = 0;

  /*if (_MASK(flag, _BIT(5)))
  { // RXNE
    HC05_RxByte(app.hc05.device, &byte, 10);
    Buffer_Push(app.hc05.rx, byte);
    if (Buffer_isEndAs(app.hc05.rx, (uint8_t *)"OK\r\n", 4))
    {
      app.hc05.device->USARTx->CR1 &= ~_BIT(5);
      // app.schedule &= ~HC05_BUSY;
    }
  }
  else */
  if (_MASK(flag, _BIT(7)))
  { // TXE
    if (Buffer_Length(app.hc05.tx) == 1)
    {
      app.hc05.device->USARTx->CR1 &= ~_BIT(7);
      app.schedule &= ~HC05_BUSY;
    }
    Buffer_Take(app.hc05.tx, &byte);
    HC05_TxByte(app.hc05.device, byte, 10);
  }
}

/* ---------------------------------------------------------------- Class Public Functions End */

#endif // STM32F103xx_UNREADY
