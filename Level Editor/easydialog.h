#ifndef EASYDIALOG_H_
#define EASYDIALOG_H_

// Enumerations for YesNoCancelDialog() [YES, NO, and CANCEL]
enum YesNo{
    YES = 6,
    NO = 7,
    CANCEL = 2
};

// Brings window to foreground, returns true if successful
bool bringToFront(void * windowHandle);

// Opens dialog message with "OK" button with WIN32API 
void OKDialog(const char * title, const char * message, void * windowHandle);

// Opens dialog message with "Yes", "No", and "Cancel" buttons with WIN32API, returns a YesNo enumeration
short YesNoCancelDialog(const char * title, const char * message, void * windowHandle);

// Opens dialog message with "Yes" and "No" buttons with WIN32API, returns true or false
bool YesNoDialog(const char * title, const char * message, void * windowHandle);

// Initializes save file dialog, sets filename string to selected filename, returns true or false to confirm file is selected
bool save_file(const char * extensionTypes, int filterStartIndex, char * filename, void * windowHandle);

// Initializes open file dialog, sets filename string to selected filename, returns true or false to confirm file is selected
bool open_file(const char * extensionTypes, int filterStartIndex, char * filename, void * windowHandle);

#endif
