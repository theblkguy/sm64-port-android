#ifndef CONFIGFILE_H
#define CONFIGFILE_H

extern bool         configFullscreen;
extern unsigned int configKeyA;
extern unsigned int configKeyB;
extern unsigned int configKeyStart;
extern unsigned int configKeyR;
extern unsigned int configKeyZ;
extern unsigned int configKeyCUp;
extern unsigned int configKeyCDown;
extern unsigned int configKeyCLeft;
extern unsigned int configKeyCRight;
extern unsigned int configKeyStickUp;
extern unsigned int configKeyStickDown;
extern unsigned int configKeyStickLeft;
extern unsigned int configKeyStickRight;
extern char configGamepadA[32];
extern char configGamepadB[32];
extern char configGamepadZ[32];
extern char configGamepadR[32];
extern char configGamepadL[32];
extern char configGamepadStart[32];
extern char configGamepadCUp[32];
extern char configGamepadCDown[32];
extern char configGamepadCLeft[32];
extern char configGamepadCRight[32];
extern char configGamepadDUp[32];
extern char configGamepadDDown[32];
extern char configGamepadDLeft[32];
extern char configGamepadDRight[32];
extern char configGamepadTouchToggle[32];
extern bool configTouchControls;
extern bool configTouchAutoHide;

void configfile_load(const char *filename);
void configfile_save(const char *filename);

#endif
