
/* 
* This file is part of VL53L1 Platform 
* 
* Copyright (c) 2016, STMicroelectronics - All Rights Reserved 
* 
* License terms: BSD 3-clause "New" or "Revised" License. 
* 
* Redistribution and use in source and binary forms, with or without 
* modification, are permitted provided that the following conditions are met: 
* 
* 1. Redistributions of source code must retain the above copyright notice, this 
* list of conditions and the following disclaimer. 
* 
* 2. Redistributions in binary form must reproduce the above copyright notice, 
* this list of conditions and the following disclaimer in the documentation 
* and/or other materials provided with the distribution. 
* 
* 3. Neither the name of the copyright holder nor the names of its contributors 
* may be used to endorse or promote products derived from this software 
* without specific prior written permission. 
* 
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE 
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR 
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, 
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
* 
*/

#include "vl53l1_platform.h"
#include <string.h>
#include <time.h>
#include <math.h>

int8_t VL53L1_WriteMulti( uint16_t dev, uint16_t index, uint8_t *pdata, uint32_t count) {
	return __W_Series(dev, index, pdata, count, 0); // to be implemented
}

int8_t VL53L1_ReadMulti(uint16_t dev, uint16_t index, uint8_t *pdata, uint32_t count){
	return __R_Series(dev, index, pdata, count, 0); // to be implemented
}

int8_t VL53L1_WrByte(uint16_t dev, uint16_t index, uint8_t data) {
	return __W_Series(dev, index, &data, 1, 0); // to be implemented
}

int8_t VL53L1_WrWord(uint16_t dev, uint16_t index, uint16_t data) {
  uint8_t array[2] = {0};

  array[0] = (uint8_t)( ( data & 0xFF00U ) >> 8 );
  array[1] = (uint8_t)( data & 0x00FFU );

	return __W_Series(dev, index, array, 2, 0); // to be implemented
}

int8_t VL53L1_WrDWord(uint16_t dev, uint16_t index, uint32_t data) {
  uint8_t array[4] = {0};

  array[0] = (uint8_t)( ( data & 0xFF000000U ) >> 24 );
  array[1] = (uint8_t)( ( data & 0x00FF0000U ) >> 16 );
  array[2] = (uint8_t)( ( data & 0x0000FF00U ) >>  8 );
  array[3] = (uint8_t)( ( data & 0x000000FFU ) >>  0 );

	return __W_Series(dev, index, array, 4, 0); // to be implemented
}

int8_t VL53L1_RdByte(uint16_t dev, uint16_t index, uint8_t *data) {
	return __R_Series(dev, index, data, 1, 0); // to be implemented
}

int8_t VL53L1_RdWord(uint16_t dev, uint16_t index, uint16_t *data) {
	
	uint8_t array[2] = {0};
	
	__R_Series(dev, index, array, 2, 0);
	
	*data = (uint16_t)( 
		(uint16_t)( array[0] << 8 ) |
		(uint16_t)( array[1] ) 
	) ;
	
	return 0; // to be implemented
}

int8_t VL53L1_RdDWord(uint16_t dev, uint16_t index, uint32_t *data) {
	
	uint8_t array[4] = {0};
	
	__R_Series(dev, index, array, 4, 0);
	
	*data = (uint32_t)( 
		(uint32_t)( array[0] << 24 ) |
		(uint32_t)( array[1] << 16 ) |
		(uint32_t)( array[2] <<  8 ) |
		(uint32_t)( array[3] <<  0 )
	) ;
	
	return 0; // to be implemented
}

int8_t VL53L1_WaitMs(uint16_t dev, int32_t wait_ms) {

	LL_mDelay(wait_ms);

	return 0; // to be implemented
}
