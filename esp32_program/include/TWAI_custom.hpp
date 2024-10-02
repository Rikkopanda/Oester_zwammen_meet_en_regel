
#ifndef TWAI_CUSTOM_HPP
# define TWAI_CUSTOM_HPP

typedef enum e_code_type
{
    TWAI_STATUS,
    TRANSMIT_RECEIVE_START_STOP
} t_code_type;

void print_twai(int code, t_code_type type);
void send_can_frame(uint8_t can_frame_id, void *_data);
void configure_twai_can();

#endif