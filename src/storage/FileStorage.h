#ifndef FILE_STORAGE_H
#define FILE_STORAGE_H
#include "SPIFFS.h"
#include "ArduinoJson.h"
#include "../SystemTypes.h"

class FileStorage{

    public: 
        FileStorage(void);
        void init(void);
        CredentialsType* getCredentials(void);
        bool writeCredentials(fs::FS &fs,CredentialsType* cred);
        bool resetCredentials(fs::FS &fs);
        void readFilesPatterns(void);
    private:
        void readFileCredentials(void);
        void extractPatternsData(File&);
        void populateSystemPatterns(uint8_t,File&);
        CRGB mapColourNameToColour(const char*);
        uint8_t validatePatternFile(const char*);
        bool isPatternFileNameValid(const char*);
        String readFileTxt(fs::FS &fs, const char * path);
        CredentialsType credentials;
        DynamicJsonDocument* jsonDoc;

};

#endif


