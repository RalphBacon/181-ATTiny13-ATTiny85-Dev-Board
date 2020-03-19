/* tiny touch sense example for AVR MCUs
 * touch sense code takes only 40 bytes of flash
 * Ralph Doncaster 2020 open source MIT license
 */

#include <avr/io.h>
#include <util/delay.h>

const unsigned TOUCH_GPIO = 3;
const unsigned LED_GPIO = 4;

uint8_t rise_time()
{
	uint8_t time = 0;
	DDRB &= ~(1 << TOUCH_GPIO);           // input mode
	do {
		PINB = (1 << TOUCH_GPIO);           // pullup on
		PINB = (1 << TOUCH_GPIO);           // pullup off
		time++;
	} while (bit_is_clear(PINB, TOUCH_GPIO));
	DDRB |= (1 << TOUCH_GPIO);            // discharge
	return time;
}

bool tsense(uint8_t threshold) {
	return (rise_time() > (uint8_t) (threshold + 1));
}

__attribute((noinline))
void wait_ms(uint16_t count) {
	while (--count)
		_delay_us(999.5);
}

int main()
{
	uint8_t no_touch;
	DDRB |= (1 << LED_GPIO);              // output mode
	PINB = (1 << LED_GPIO);				// LED off on dev board (sink)

	// calibrate sensitivity
	rise_time();                        // discard 1st sample
	no_touch = rise_time();

	bool LED_is_on = false;
//	while (1) {
//		if (tsense(no_touch)) {
//			PINB |= (1 << LED_GPIO);          // toggle LED
//			//wait_ms(1000);
//			while (tsense(no_touch));
//			LED_is_on = true;
//		}
//		wait_ms(100);
//	}

	while (1) {
		if (!LED_is_on && tsense(no_touch)) {
			PINB |= (1 << LED_GPIO);
			//while (tsense(no_touch));
			LED_is_on = true;
		}

		if (!tsense(no_touch) && LED_is_on) {
			PINB |= (1 << LED_GPIO);
			LED_is_on = false;
		}

		wait_ms(100);
	}

}
