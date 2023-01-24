#ifndef SPLASH_SCREEN_H_
#define SPLASH_SCREEN_H_

#define VERSION_TOP 1
#define VERSION_BOTTOM 2


// Starts a Splash Screen on an independant thread.
// Note: Only One splash screen should be active at a time.
// Note: No raylib window should be initialized while splash screen is active
void beginSplashScreen(const char * filename, const char * version, int versionPos);
// Ends an active splash screen.
// Program waits until splash screen has been fully terminated.
void endSplashScreen(void);

#endif