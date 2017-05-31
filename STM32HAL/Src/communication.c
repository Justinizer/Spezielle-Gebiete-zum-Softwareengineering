#include "communication.h"

#include "usbd_cdc_if.h"


void transmit_data_to_pc(int pm2_5, int pm10) {
  char buffer[TRANSMIT_BUFFER_SIZE];

  snprintf(buffer, TRANSMIT_BUFFER_SIZE, "%d;%d\n", pm2_5, pm10);
  CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer));
}