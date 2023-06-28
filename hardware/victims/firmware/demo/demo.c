/*
    This file is part of the ChipWhisperer Example Targets
    Copyright (C) 2012-2017 NewAE Technology Inc.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "aes-independant.h"
#include "hal.h"
#include "simpleserial.h"
#include <stdint.h>
#include <stdlib.h>

uint8_t get_mask(uint8_t* m, uint8_t len)
{
  aes_indep_mask(m, len);
  return 0x00;
}

uint8_t get_key(uint8_t* k, uint8_t len)
{
	aes_indep_key(k);
	return 0x00;
}


uint8_t sleep(uint8_t* pt, uint8_t len)
{
    int n = 1950; //number of rounds to do nothing
    int i; 

    trigger_high();
        for (i = 0; i < n; ++i) 
        { 
            // 10 nops so it will have more weight than the loop intructions
            __asm__ ("NOP");
            __asm__ ("NOP");
            __asm__ ("NOP");
            __asm__ ("NOP");
            __asm__ ("NOP");

            __asm__ ("NOP");
            __asm__ ("NOP");
            __asm__ ("NOP");
            __asm__ ("NOP");
            __asm__ ("NOP");
        } 
    trigger_low();
    return 0x00;

}

uint8_t encrypt(uint8_t* pt, uint8_t len)
{
    aes_indep_enc_pretrigger(pt);

	trigger_high();
	    aes_indep_enc(pt); /* encrypting the data block */
	trigger_low();

    aes_indep_enc_posttrigger(pt);

	simpleserial_put('r', 16, pt);
	return 0x00;
}

uint8_t matrix_multiplication(uint8_t* pt, uint8_t len)
{
    int reps = 5;
    trigger_high();
        // ----------------- variables for matrix multiplication  -----------------------
        // num of rows and columns in each matrix
        int r1 = 4;  int r2 = 4; int c2 = 4;
            int mat1[4][4] = {
                {1, 1, 1, 1},
                {2, 2, 2, 2},
                {3, 3, 3, 3},
                {4, 4, 4, 4}
        };
    
        int mat2[4][4] = {
                {1, 1, 1, 1},
                {2, 2, 2, 2},
                {3, 3, 3, 3},
                {4, 4, 4, 4}
        };
        int rslt[r1][c2];
        // ------------- end of variables for matrix multiplication  ---------------------

        for (int repeats = 0 ; repeats < reps; repeats++)
        {
            for (int i = 0; i < r1; i++) 
            {
                for (int j = 0; j < c2; j++) 
                {
                    rslt[i][j] = 0;

                    for (int k = 0; k < r2; k++) {
                        rslt[i][j] += mat1[i][k] * mat2[k][j];
                    }

                }
            }
        }
    trigger_low();
    return 0x00;
}

// executing 50 nops
uint8_t nops(uint8_t* pt, uint8_t len)
{
    trigger_high();
            __asm__ ("NOP");
            __asm__ ("NOP");
            __asm__ ("NOP");
            __asm__ ("NOP");
            __asm__ ("NOP");

            __asm__ ("NOP");
            __asm__ ("NOP");
            __asm__ ("NOP");
            __asm__ ("NOP");
            __asm__ ("NOP");

            __asm__ ("NOP");
            __asm__ ("NOP");
            __asm__ ("NOP");
            __asm__ ("NOP");
            __asm__ ("NOP");

            __asm__ ("NOP");
            __asm__ ("NOP");
            __asm__ ("NOP");
            __asm__ ("NOP");
            __asm__ ("NOP");

            __asm__ ("NOP");
            __asm__ ("NOP");
            __asm__ ("NOP");
            __asm__ ("NOP");
            __asm__ ("NOP");

            __asm__ ("NOP");
            __asm__ ("NOP");
            __asm__ ("NOP");
            __asm__ ("NOP");
            __asm__ ("NOP");

            __asm__ ("NOP");
            __asm__ ("NOP");
            __asm__ ("NOP");
            __asm__ ("NOP");
            __asm__ ("NOP");

            __asm__ ("NOP");
            __asm__ ("NOP");
            __asm__ ("NOP");
            __asm__ ("NOP");
            __asm__ ("NOP");

            __asm__ ("NOP");
            __asm__ ("NOP");
            __asm__ ("NOP");
            __asm__ ("NOP");
            __asm__ ("NOP");

            __asm__ ("NOP");
            __asm__ ("NOP");
            __asm__ ("NOP");
            __asm__ ("NOP");
            __asm__ ("NOP");
    trigger_low();
    return 0x00;
}

// executing 15 multiplications
uint8_t MUL(uint8_t* pt, uint8_t len)
{
    trigger_high();
        volatile long int A = 0x2BAA;
        A *= 2;
        A *= 2;
        A *= 2;
        A *= 2;
        A *= 2;
        
        A *= 2;
        A *= 2;
        A *= 2;
        A *= 2;
        A *= 2;

    trigger_low();
    return 0x00;
}

uint8_t reset(uint8_t* x, uint8_t len)
{
    // Reset key here if needed
	return 0x00;
}

static uint16_t num_encryption_rounds = 10;

uint8_t enc_multi_getpt(uint8_t* pt, uint8_t len)
{
    aes_indep_enc_pretrigger(pt);

    for(unsigned int i = 0; i < num_encryption_rounds; i++){
        trigger_high();
        aes_indep_enc(pt);
        trigger_low();
    }

    aes_indep_enc_posttrigger(pt);
	simpleserial_put('r', 16, pt);
    return 0;
}

uint8_t enc_multi_setnum(uint8_t* t, uint8_t len)
{
    //Assumes user entered a number like [0, 200] to mean "200"
    //which is most sane looking for humans I think
    num_encryption_rounds = t[1];
    num_encryption_rounds |= t[0] << 8;
    return 0;
}

#if SS_VER == SS_VER_2_1
uint8_t aes(uint8_t cmd, uint8_t scmd, uint8_t len, uint8_t *buf)
{
    uint8_t req_len = 0;
    uint8_t err = 0;
    uint8_t mask_len = 0;
    if (scmd & 0x04) {
        // Mask has variable length. First byte encodes the length
        mask_len = buf[req_len];
        req_len += 1 + mask_len;
        if (req_len > len) {
            return SS_ERR_LEN;
        }
        err = get_mask(buf + req_len - mask_len, mask_len);
        if (err)
            return err;
    }

    if (scmd & 0x02) {
        req_len += 16;
        if (req_len > len) {
            return SS_ERR_LEN;
        }
        err = get_key(buf + req_len - 16, 16);
        if (err)
            return err;
    }
    if (scmd & 0x01) {
        req_len += 16;
        if (req_len > len) {
            return SS_ERR_LEN;
        }
        err = encrypt(buf + req_len - 16, 16);
        if (err)
            return err;
    }

    if (len != req_len) {
        return SS_ERR_LEN;
    }

    return 0x00;

}
#endif

int main(void)
{
	uint8_t tmp[KEY_LENGTH] = {DEFAULT_KEY};

    platform_init();
    init_uart();
    trigger_setup();

	aes_indep_init();
	aes_indep_key(tmp);

    /* Uncomment this to get a HELLO message for debug */

    // putch('h');
    // putch('e');
    // putch('l');
    // putch('l');
    // putch('o');
    // putch('\n');

	simpleserial_init();
    #if SS_VER == SS_VER_2_1
    simpleserial_addcmd(0x01, 16, aes);
    #else
    simpleserial_addcmd('k', 16, get_key);
    
    // my additions
    simpleserial_addcmd('E', 16,  encrypt); //encrypt
    simpleserial_addcmd('S', 0,  sleep); //sleep
    simpleserial_addcmd('M', 0,  matrix_multiplication); //matrix multiplication
    simpleserial_addcmd('N', 0,  nops); // Executing NOPS instructions
    simpleserial_addcmd('U', 0,  MUL); // Executing MUL instructions


    
    simpleserial_addcmd('x',  0,   reset);
    simpleserial_addcmd_flags('m', 18, get_mask, CMD_FLAG_LEN);
    simpleserial_addcmd('s', 2, enc_multi_setnum);
    simpleserial_addcmd('f', 16, enc_multi_getpt);
    #endif
    while(1)
        simpleserial_get();
}
