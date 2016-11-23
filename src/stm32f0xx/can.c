#include "can.h"
#include "gpio.h"
#include "stm32f0xx.h"

#define CAN_PRESCALER 12

// CAN ports
static volatile CANConfig *s_can[1];

static bool prv_transmit(const CANConfig *can, const CANMessage *msg) {
  CanTxMsg tx_msg = {
    .StdId = msg->id,
    .IDE = CAN_ID_STD,
    .RTR = msg->rtr,
    .DLC = msg->dlc
  };

  if (msg->dlc == 0 && msg->data != 0) {
    // Probably forgot to set the data length, so set it to 8 bytes (max)
    tx_msg.DLC = 8;
  }

  memcpy(tx_msg.Data, msg->data_u8, sizeof(*tx_msg.Data) * msg->dlc);

  return (CAN_Transmit(CAN, &tx_msg) != CAN_TxStatus_NoMailBox);
}

static bool prv_receive(CANConfig *can, uint8_t fifo) {
  if (CAN_GetITStatus(CAN, (fifo) ? CAN_IT_FMP1 : CAN_IT_FMP0) == RESET) {
    return false;
  }

  CanRxMsg rx_msg;
  CAN_Receive(CAN, fifo, &rx_msg);

  CANMessage msg = {
    .id = rx_msg.StdId,
    .rtr = rx_msg.RTR,
    .dlc = rx_msg.DLC
  };

  memcpy(msg.data_u8, rx_msg.Data, sizeof(*msg.data_u8) * msg.dlc);

  // No error handling
  can_queue_push(&can->rx_queue, &msg);

  return true;
}

void can_init(CANConfig *can) {
  can_queue_init(&can->tx_queue);
  can_queue_init(&can->rx_queue);

  // TODO: Do we need to support multiple CAN ports?
  s_can[0] = can;

  GPIOSettings gpio_settings = {
    .resistor = GPIO_RES_PULLUP,
    .alt_function = GPIO_ALTFN_4
  };

  // gpio_init_pin(&(GPIOAddress){ .port = 0, .pin = 11 }, &gpio_settings);
  // gpio_init_pin(&(GPIOAddress){ .port = 0, .pin = 12 }, &gpio_settings);

  // Enable CAN IRQ channel
  NVIC_InitTypeDef nvic_cfg = {
    .NVIC_IRQChannel = CEC_CAN_IRQn,
    .NVIC_IRQChannelPriority = 0,
    .NVIC_IRQChannelCmd = ENABLE
  };
  NVIC_Init(&nvic_cfg);

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN, ENABLE);

  RCC_ClocksTypeDef clocks;
  RCC_GetClocksFreq(&clocks);
  // time quanta = (APB1 / Prescaler) / baudrate, -1 for start bit
  uint16_t tq = clocks.PCLK_Frequency / 1000 / CAN_PRESCALER / can->speed - 1;
  uint16_t bs1 = tq * 7 / 8; // 87.5% sample point

  CAN_DeInit(CAN);

  CAN_InitTypeDef can_cfg;
  CAN_StructInit(&can_cfg);

  can_cfg.CAN_Mode = CAN_Mode_Silent_LoopBack;
  can_cfg.CAN_SJW = CAN_SJW_1tq;
  can_cfg.CAN_BS1 = bs1;
  can_cfg.CAN_BS2 = tq - bs1;
  can_cfg.CAN_Prescaler = CAN_PRESCALER;
  CAN_Init(CAN, &can_cfg);

  CAN_ITConfig(CAN, CAN_IT_TME, ENABLE);
  CAN_ITConfig(CAN, CAN_IT_FMP0, ENABLE);
  CAN_ITConfig(CAN, CAN_IT_FMP1, ENABLE);

  // Accept all messages
  can_add_filter(can, 0, 0);
  can->num_filters = 0;
}

bool can_add_filter(CANConfig *can, uint16_t filter, uint16_t mask) {
  // Distribute filters evenly between the two RX FIFOs
  volatile CAN_FilterInitTypeDef filter_cfg = {
    .CAN_FilterNumber = can->num_filters,
    .CAN_FilterMode = CAN_FilterMode_IdMask,
    .CAN_FilterScale = CAN_FilterScale_32bit, // TODO: Figure out good way to use 16bit filters
    .CAN_FilterIdHigh = filter << 5,
    .CAN_FilterIdLow = 0x0000,
    .CAN_FilterMaskIdHigh = mask << 5,
    .CAN_FilterMaskIdLow = 0x0000,
    .CAN_FilterFIFOAssignment = (can->num_filters % 2),
    .CAN_FilterActivation = ENABLE
  };
  CAN_FilterInit(&filter_cfg);

  can->num_filters++;
  return true;
}

bool can_transmit(CANConfig *can, const CANMessage *msg) {
  // If all of our TX mailboxes were full, try to put it into the TX queue
  bool success = prv_transmit(can, msg);

  return success || can_queue_push(&can->tx_queue, msg);
}

bool can_receive(CANConfig *can, CANMessage *msg) {
  return can_queue_pop(&can->rx_queue, msg);
}

void CEC_CAN_IRQHandler(void) {
  CANMessage temp;
  if (CAN_GetITStatus(CAN, CAN_IT_TME) == SET) {
    while (can_queue_pop(&s_can[0]->tx_queue, &temp)) {
      prv_transmit(s_can[0], &temp);
    }

    CAN_ClearITPendingBit(CAN, CAN_IT_TME);
  }

  while (prv_receive(s_can[0], CAN_Filter_FIFO0)) { }
  while (prv_receive(s_can[0], CAN_Filter_FIFO1)) { }
}
