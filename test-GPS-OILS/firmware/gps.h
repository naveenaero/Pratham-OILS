/*
 * gps.h
 *
 * Created: 31-03-2012 15:22:32
 *  Author: Hussain
 */ 

/**
 * @file gps.h
 * @brief Functions to read the GPS device
 */

#ifndef GPS_H_
#define GPS_H_

/** @brief Initialise the UART interface for GPS
 */
void init_UART_GPS(void );

void process_GPS(void);

uint8_t receive_GPS(void);


#endif /* GPS_H_ */