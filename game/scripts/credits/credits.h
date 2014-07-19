#ifndef _CREDITS_H_
#define _CREDITS_H_

typedef struct {
	/**
	 * Gets called on stopping or ending credits.
	 */
	void onCreditsEnd();
} Credits;

Credits credits;

/**
 * Initializes the credits.
 */
void credits_init();

/**
 * Starts the credits.
 */
void credits_start();

/**
 * Stops the credits.
 */
void credits_stop();

#endif // #ifndef _CREDITS_H_