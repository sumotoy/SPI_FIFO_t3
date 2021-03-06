/* 
This is a test to create a Teensy 3.x SPI FIFO library that can be integrated in other libraries
to have fast SPI without deal directly with register or complicated stuff.
I have tried to keep simple but also powerful.
I'm currently try to use a standard method to do this so I can use the same approach
for other CPU's.
Use is simple:

	1) startTransaction
		2) writeByte_cont or writeWord_cont
		...
		...
		n) writeByte_last or writeWord_last
	x) endTransaction
	
One nice feature is the ability to use DC for many display, in that case writeByte or writeWord have
a flag called dataMode, if true the DC will set automatically.
Library use SPI fifo for reading as well.
In addition, this library support K64/K66 CPU and alternative SPI1 and SPI2 (when enabled),
and all alternative SPI pins on all Teensy 3.x CPU's
 * This library works with: Teensy 3.0 / 3.1 / 3.2 / 3.5 / 3.6
---------------------------------------------------------------------------------------------------------------------
Version history:
1.0: First beta.
---------------------------------------------------------------------------------------------------------------------
		Copyright (c) 2016, s.u.m.o.t.o.y [sumotoy(at)gmail.com]
		Coded by: Max MC Costa
---------------------------------------------------------------------------------------------------------------------
Licence:
	Licensed as GNU General Public License.
    SPI_FIFO_t3 Library is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    SPI_FIFO_t3 Library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
 */
 
#ifndef _SPI_FIFOT3LIB_H_INCLUDED
#define _SPI_FIFOT3LIB_H_INCLUDED

#ifdef __cplusplus
#include <Arduino.h>
#include "pins_arduino.h"
#include <SPI.h>


#if (defined(__MK20DX128__) || defined(__MK20DX256__) || defined(__MK64FX512__) || defined(__MK66FX1M0__))
	//check for the correct version of Teensyduino
	#if (TEENSYDUINO < 129)
		#error "Please upgrade your Teensyduino at the latest version!"
	#endif
#else
#error "SPI FIFO library works only with Teensy 3.0,3.1,3.2,3.5,3.6 or never!"
#endif


static SPISettings 	_spiSettings;

class SPI_FIFO_t3 {
	public:
		SPI_FIFO_t3();//used for include in other libraries
		SPI_FIFO_t3(const uint8_t csPin,const uint8_t dcPin=255,const uint8_t mosiPin=11,const uint8_t sclkPin=13,const uint8_t misoPin=255);
		//this must be called before begin inside other libraries
		void		postInstance(const uint8_t csPin,const uint8_t dcPin,const uint8_t mosiPin,const uint8_t sclkPin,const uint8_t misoPin);
		bool		begin(SPISettings settings=SPISettings(30000000, MSBFIRST, SPI_MODE0),bool avoidInit=false);
		void		setSpiSettings(SPISettings settings);
		uint8_t 	getSPIbus(void);
		void		startTransaction(void);
		void 		endTransaction(void);
		void		writeByte_cont(uint8_t val,bool dataMode=false);
		void		writeByte_last(uint8_t val,bool dataMode=false);
		void		writeWord_cont(uint16_t val,bool dataMode=false);
		void		writeWord_last(uint16_t val,bool dataMode=false);
		uint8_t		readByte_cont(bool dataMode=false);
		uint16_t	readWord_cont(bool dataMode=false);
		int 		getInterruptNumber(uint8_t pin);
		void 		waitTransmitComplete(void);
		void 		usingInterrupt(uint8_t n);
		void 		usingInterrupt(IRQ_NUMBER_t interruptName); 
		void 		notUsingInterrupt(IRQ_NUMBER_t interruptName);
		
	protected:
		volatile uint8_t _pcs_data;
		volatile uint8_t _pcs_command;
	private:
		uint8_t		_cs;
		uint8_t		_dc;
		uint8_t		_mosi;
		uint8_t		_miso;
		uint8_t		_sclk;
		uint8_t		_initError;
		#if defined(__MK64FX512__) || defined(__MK66FX1M0__)
		uint8_t		_spiBus;
		#endif
		void 		waitFifoNotFull(void);
		void 		waitFifoEmpty(void);
		void 		waitTransmitComplete(uint32_t mcr);
		void 		clearFifoBuffer(bool dataMode);
};

#endif
#endif