#include "gpio.h"

#include <stm32f0xx_conf.h>

typedef struct GPIOPortMap {
  volatile uint32_t *const reg_mode;
  volatile uint32_t *const reg_type;
  volatile uint32_t *const reg_speed;
  volatile uint32_t *const reg_resistor;
  volatile uint32_t *const reg_input;
  volatile uint32_t *const reg_output;
  volatile uint32_t *const reg_alt_function;
}

static const GPIOPortMap port_map[] = {
  {GPIOA_MODER, GPIOA_OTYPER, GPIOA_OSPEEDR, GPIOA_PUPDR, GPIOA_IDR, GPIOA_ODR, GPIOA_AFRL},
  {GPIOB_MODER, GPIOB_OTYPER, GPIOB_OSPEEDR, GPIOB_PUPDR, GPIOB_IDR, GPIOB_ODR, GPIOB_AFRL},
  {GPIOC_MODER, GPIOC_OTYPER, GPIOC_OSPEEDR, GPIOC_PUPDR, GPIOC_IDR, GPIOC_ODR, GPIOC_AFRL},
  {GPIOD_MODER, GPIOD_OTYPER, GPIOD_OSPEEDR, GPIOD_PUPDR, GPIOD_IDR, GPIOD_ODR, GPIOD_AFRL},
  {GPIOE_MODER, GPIOE_OTYPER, GPIOE_OSPEEDR, GPIOE_PUPDR, GPIOE_IDR, GPIOE_ODR, GPIOE_AFRL},
  {GPIOF_MODER, GPIOF_OTYPER, GPIOF_OSPEEDR, GPIOF_PUPDR, GPIOF_IDR, GPIOF_ODR, GPIOF_AFRL}
}

void gpio_init() {
  for (int i = 0; i < 6; ++i) {
    port_map[i].reg_mode |= 0xFFFFFFFF;
    port_map[i].reg_speed &= 0;
    port_map[i].reg_resistor &= 0;
  }
}

void gpio_init_pin(GPIOAddress *address, GPIOSettings *settings) {
  if (*settings->alt_function == GPIO_AF_ANALOG) {
    port_map[*address->port].reg_mode |= 0x03 << (*address->pin * 2);
  } else if (*settings->alt_function != GPIO_AF_0) {
    port_map[*address->port].alt_function |=
        ~(0x0F << (*address->pin * 4)) | (*settings->alt_function << (*address->pin * 4));
    port_map[*address->port].reg_mode |=
        ~(0x03 << (*address->pin * 2)) | (0x02 << (*address->pin * 2));
  } else {
    port_map[*address->port].reg_mode |=
        ~(0x03 << (*address->pin * 2)) | (*settings->direction << (*address->pin * 2));
  }
  gpio_set_pin_state(address, settings->state);
  port_map[*address->port].reg_resistor |=
      ~(0x03 << (*address->pin * 2)) | (*settings->resistor << (*address->pin * 2));
}

void gpio_set_pin_state(GPIOAddress *address, GPIOState *state) {
  if (*state == GPIO_STATE_HIGH) {
    port_map[*address->port].reg_output |= (*state << *address->pin);
  } else {
    port_map[*address->port].reg_output &= ~(*state << *address->pin);
  }
}

void gpio_toggle_state(GPIOAddress *address) {
  port_map[*address->port] &= ~(port_map[*address->port] & 0x01 << *address->pin);
}

GPIOState gpio_get_value(GPIOAddress *address) {
  return (port_map[*address->port] & 0x01 << *address->pin) >> *address->pin;
}
