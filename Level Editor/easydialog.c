#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <windows.h>

enum YesNo{
    YES = 6,
    NO = 7,
    CANCEL = 2
};

bool bringToFront(void * windowHandle){
    return SetForegroundWindow(windowHandle);
}

void OKDialog(const char * title, const char * message, void * windowHandle){
    BringWindowToTop(GetWindow(windowHandle, 0));
    MessageBox(windowHandle, message, title, MB_OK);
    return;
}

short YesNoCancelDialog(const char * title, const char * message, void * windowHandle){
    BringWindowToTop(GetWindow(windowHandle, 0));
    short answer = MessageBox(windowHandle, message, title, MB_YESNOCANCEL);
    return answer;
}

bool YesNoDialog(const char * title, const char * message, void * windowHandle){
    BringWindowToTop(GetWindow(windowHandle, 0));
    int answer = MessageBox(windowHandle, message, title, MB_YESNO);
    printf("%d", answer);
    if (answer == 6){
        return true;
    }
    else{
        return false;
    }
}

bool save_file(LPCSTR extensionTypes, int filterStartIndex, char * filename, void * windowHandle){
    // HWND saveDialogue = CreateWindowA("STATIC", "Save File", WS_VISIBLE, 0, 0,100, 100,NULL,NULL,NULL,NULL);
    // ShowWindow(saveDialogue, SW_HIDE);
    // SetWindowTextA(saveDialogue, "Save File");
    
    OPENFILENAME saveDialogueBox;
    filterStartIndex++;
    ZeroMemory(&saveDialogueBox, sizeof(OPENFILENAME));
    saveDialogueBox.lStructSize = sizeof(OPENFILENAME);
    saveDialogueBox.hwndOwner = windowHandle;
    saveDialogueBox.lpstrFile = filename;
    saveDialogueBox.lpstrFile[0] = 0;
    saveDialogueBox.nMaxFile = FILENAME_MAX;
    saveDialogueBox.lpstrFilter = extensionTypes;
    saveDialogueBox.nFilterIndex = filterStartIndex;
    BringWindowToTop(GetWindow(windowHandle, 0));
    GetSaveFileName(&saveDialogueBox);

    if (saveDialogueBox.lpstrFile[0] == 0){
        return false;
    }
    else{
        filename = saveDialogueBox.lpstrFile;
        return  true;
    }
}

bool open_file(LPCSTR extensionTypes, int filterStartIndex, char * filename, void * windowHandle){
    // HWND openDialogue = CreateWindowA("STATIC", "Open File", WS_VISIBLE,0,0,100,100,NULL,NULL,NULL,NULL);
    // ShowWindow(openDialogue, SW_HIDE);
    // SetWindowTextA(openDialogue, "Open File");
    OPENFILENAME openDialogueBox;
    filterStartIndex++;
    ZeroMemory(&openDialogueBox, sizeof(OPENFILENAME));
    openDialogueBox.lStructSize = sizeof(OPENFILENAME);
    openDialogueBox.hwndOwner = windowHandle;
    openDialogueBox.lpstrFile = filename;
    openDialogueBox.lpstrFile[0] = 0;
    openDialogueBox.nMaxFile = FILENAME_MAX;
    openDialogueBox.lpstrFilter = extensionTypes;
    openDialogueBox.nFilterIndex = filterStartIndex;
    BringWindowToTop(GetWindow(windowHandle, 0));
    GetOpenFileName(&openDialogueBox);
    if (openDialogueBox.lpstrFile[0] == 0){
        return false;
    }
    else{
        filename = openDialogueBox.lpstrFile;
        return  true;
    }
}
