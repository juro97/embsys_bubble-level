#include "app.h"
extern atomic sfr sbit WKUP;

/** Wake_signal
* @note        assert wake signal on RE9, wait 1 ms, deassert
* @param
* @return
*/
void Wake_signal()
{
    WKUP = 0;                                             //assert wake signal
    Delay_ms(2);                                          //spec says 3µs assertion, let's use ms delay and wait ~2 ms
    WKUP = 1;                                             //de-assert wake signal
}