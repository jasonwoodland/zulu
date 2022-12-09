/* #define DEBUG 1 */

#ifdef DEBUG
#define micros() (clock() / 10) // slow debug clock rate
#else
#define micros() (clock() * 1000000 / CLOCKS_PER_SEC)
#endif

