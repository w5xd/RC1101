/*
Modified March, 2018 by Wayne E. Wright, W5XD
Compile-time linkage to the slave mode callbacks and various other changes.

twi.h - TWI/I2C library for Wiring & Arduino
Copyright (c) 2006 Nicholas Zambetti.  All right reserved.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef twi_slave_h
#define twi_slave_h

#define TWI_BUFFER_LENGTH 34 // For RC-1101. Two byte check summ 

#include <inttypes.h>

//#define ATMEGA8

#ifndef TWI_FREQ
#define TWI_FREQ 100000L
#endif

#ifndef TWI_BUFFER_LENGTH
#define TWI_BUFFER_LENGTH 32
#endif



void twi_init();
void twi_disable(void);
void twi_setAddress(uint8_t);
uint8_t twi_transmit(const uint8_t*, uint8_t);
void twi_setFrequency(uint32_t);
uint8_t twi_readFrom(uint8_t, uint8_t*, uint8_t, uint8_t);
uint8_t twi_writeTo(uint8_t, uint8_t*, uint8_t, uint8_t, uint8_t);
#if 0

void twi_attachSlaveRxEvent( void (*)(uint8_t*, int) );
void twi_attachSlaveTxEvent( void (*)(void) );
#else
void twi_onSlaveTransmit(void);
void twi_onSlaveReceive(uint8_t*, int);
void twi_onSlaveAddressed();
#endif

#endif

