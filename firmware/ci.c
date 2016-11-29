#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <generated/csr.h>
#include <generated/mem.h>
#include <time.h>
#include <console.h>
#include <hw/flags.h>

#include "ci.h"
#include "bist.h"

static void help_debug(void)
{
	puts("debug dna   - show Board's DNA");
}

static void ci_help(void)
{
	puts("help        - this command");
	puts("reboot      - reboot CPU");
#ifdef CSR_ETHPHY_MDIO_W_ADDR
	puts("mdio_dump   - dump mdio registers");
	puts("mdio_status - show mdio status");
#endif
#ifdef CSR_GENERATOR_BASE
	puts("bist        - DDR3 bist");
#endif
	help_debug();
}

static char *readstr(void)
{
	char c[2];
	static char s[64];
	static int ptr = 0;

	if(readchar_nonblock()) {
		c[0] = readchar();
		c[1] = 0;
		switch(c[0]) {
			case 0x7f:
			case 0x08:
				if(ptr > 0) {
					ptr--;
					putsnonl("\x08 \x08");
				}
				break;
			case 0x07:
				break;
			case '\r':
			case '\n':
				s[ptr] = 0x00;
				putsnonl("\n");
				ptr = 0;
				return s;
			default:
				if(ptr >= (sizeof(s) - 1))
					break;
				putsnonl(c);
				s[ptr] = c[0];
				ptr++;
				break;
		}
	}

	return NULL;
}

static char *get_token_generic(char **str, char delimiter)
{
	char *c, *d;

	c = (char *)strchr(*str, delimiter);
	if(c == NULL) {
		d = *str;
		*str = *str+strlen(*str);
		return d;
	}
	*c = 0;
	d = *str;
	*str = c+1;
	return d;
}

static char *get_token(char **str)
{
	return get_token_generic(str, ' ');
}

static void reboot(void)
{
	asm("call r0");
}

static void print_board_dna(void) {
	int i;
	printf("Board's DNA: ");
	for(i=0; i<CSR_DNA_ID_SIZE; i++) {
		printf("%02x", MMPTR(CSR_DNA_ID_ADDR+4*i));
	}
	printf("\n");
}

void ci_prompt(void)
{
	printf("RUNTIME>");
}

void ci_service(void)
{
	char *str;
	char *token;

	str = readstr();
	if(str == NULL) return;

	token = get_token(&str);

    if(strcmp(token, "help") == 0) {
		puts("Available commands:");
		token = get_token(&str);
		if(strcmp(token, "debug") == 0)
			help_debug();
		else
			ci_help();
		puts("");
	}
	else if(strcmp(token, "reboot") == 0) reboot();
#ifdef CSR_ETHPHY_MDIO_W_ADDR
	else if(strcmp(token, "mdio_status") == 0) mdio_status();
	else if(strcmp(token, "mdio_dump") == 0) mdio_dump();
#endif
#ifdef CSR_GENERATOR_BASE
	else if(strcmp(token, "bist") == 0) bist();
#endif
	else if((strcmp(token, "debug") == 0)) {
		token = get_token(&str);
		if(strcmp(token, "dna") == 0)
			print_board_dna();
		else
			help_debug();
	}
	ci_prompt();
}
