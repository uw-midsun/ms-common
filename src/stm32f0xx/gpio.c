#include "gpio.h"

#include "stm32f0xx.h"
#include "stm32f0xx_gpio.h"

static GPIO_TypeDef *gpio_port_map[] = {GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, GPIOF};

void gpio_init() {
  for (int i = 0; i < sizeof(gpio_port_map) / sizeof(GPIO_TypeDef); i++) {
    // Sets the pin to a default reset mode.
    // TODO(ckitagawa): determine if this is actually Lowest Power setting.
    GPIO_DeInit(gpio_port_map[i]);
  }
}

void gpio_init_pin(GPIOAddress *address, GPIOSettings *settings) {
  GPIO_InitTypeDef init_struct;
  uint16_t pin = 0x01 << address->pin;

  // Parse the GPIOAltF settings which are used to modify the mode and Alt Function.
  if (settings->alt_function == GPIO_ALTFN_ANALOG) {
    init_struct.GPIO_Mode = GPIO_Mode_AN;
  } else if (settings->alt_function == GPIO_ALTFN_NONE) {
    init_struct.GPIO_Mode = settings->direction;
  } else {
    // Subtract 1 due to the offset of the enum from the ALTFN_NONE entry
    init_struct.GPIO_Mode = GPIO_Mode_AF;
  }
  init_struct.GPIO_PuPd = settings->resistor;
  init_struct.GPIO_Pin = pin;

  // These are default values which are not intended to be changed.
  init_struct.GPIO_Speed = GPIO_Speed_Level_1;
  init_struct.GPIO_OType = GPIO_OType_PP;
  init_struct;
  if (init_struct.GPIO_Mode == GPIO_Mode_AF) {
    GPIO_PinAFConfig(gpio_port_map[address->port], pin, settings->alt_function - 1);
  }

  // Set the pin state.
  GPIO_WriteBit(gpio_port_map[address->port], pin, settings->state);

  // Use the init_struct to set the pin to its defaults.
  GPIO_Init(gpio_port_map[address->port], &init_struct);
}

void gpio_set_pin_state(GPIOAddress *address, GPIOState *state) {
  GPIO_WriteBit(gpio_port_map[address->port], 0x01 << address->pin, *state);
}

void gpio_toggle_state(GPIOAddress *address) {
  uint16_t pin = 0x01 << address->pin;
  uint8_t state = GPIO_ReadOutputDataBit(gpio_port_map[address->port], pin);
  if (state) {
    GPIO_ResetBits(gpio_port_map[address->port], pin);
  } else {
    GPIO_SetBits(gpio_port_map[address->port], pin);
  }
}

GPIOState gpio_get_value(GPIOAddress *address) {
  return GPIO_ReadInputDataBit(gpio_port_map[address->port], 0x01 << address->pin);
}
