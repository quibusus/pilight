#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../pilight.h"
#include "common.h"
#include "log.h"
#include "protocol.h"
#include "hardware.h"
#include "binary.h"
#include "gc.h"
#include "fukang.h"


void fukangParseCode(void) {
	int x = 0;

	/* Convert the one's and zero's into binary */
	for(x=0; x<fukang->rawlen; x+=4) {
		if(fukang->code[x+3] == 1 || fukang->code[x+0] == 1) {
			fukang->binary[x/4]=1;
		} else {
			fukang->binary[x/4]=0;
		}
	}

	int systemcode = binToDec(fukang->binary, 0, 4);
	int programcode = binToDec(fukang->binary, 5, 9);

	fukang->message = json_mkobject();
	json_append_member(fukang->message, "systemcode", json_mknumber(systemcode));
	json_append_member(fukang->message, "programcode", json_mknumber(programcode));

}

void fukangInit(void) {
    protocol_register(&fukang);
    protocol_set_id(fukang, "fukang");
    protocol_device_add(fukang, "fukang", "fukang door sensor protocol");

	protocol_plslen_add(fukang, 430); // The pulse length of this protocol is 8500 / 34 = 250
	fukang->devtype = SENSOR;
	fukang->hwtype = RF433;           // The codes are sent through the 433.92Mhz freq.
	fukang->pulse = 3;                // The high pulses are around 2 * 250 = 500us long
	fukang->rawlen = 50;               // The raw length of the pulse train is 11 pulses (0 till 10)
	fukang->binlen = 12;

	fukang->parseCode=&fukangParseCode;
}