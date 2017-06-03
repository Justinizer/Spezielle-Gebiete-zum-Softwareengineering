#ifndef _COMMUNICATION_H_
#define _COMMUNICATION_H_


#define TRANSMIT_BUFFER_SIZE	20

void transmit_data_to_pc(int pm2_5, int pm10, int temperature, int humidity);
void send_string(const char *string);

#endif
