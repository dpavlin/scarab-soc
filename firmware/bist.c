#include <generated/csr.h>
#ifdef CSR_GENERATOR_BASE
#include "bist.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <console.h>
#include "ci.h"


#define test_size 512*1024

unsigned int ticks;
unsigned int speed;

static void busy_wait(unsigned int ds)
{
	timer0_en_write(0);
	timer0_reload_write(0);
	timer0_load_write(SYSTEM_CLOCK_FREQUENCY/10*ds);
	timer0_en_write(1);
	timer0_update_value_write(1);
	while(timer0_value_read()) timer0_update_value_write(1);
}

void bist(void) {
	while(readchar_nonblock() == 0) {
			// write
			printf("writing %d Kbytes...\n", test_size/1024);
			generator_reset_write(1);
			generator_reset_write(0);
			generator_base_write(0x10000);
			generator_length_write((test_size*8)/16);

			generator_shoot_write(1);
			while(generator_done_read() == 0);

			// read

			printf("reading %d Kbytes...\n", test_size/1024);
			checker_reset_write(1);
			checker_reset_write(0);
			checker_base_write(0x10000);
			checker_length_write((test_size*8)/16);

			checker_shoot_write(1);
			while(checker_done_read() == 0);

			// errors
			printf("errors: %d\n", checker_error_count_read());

			// delay
			busy_wait(10);
	}

}

#endif