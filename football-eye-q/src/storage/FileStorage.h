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

        // REMOVED: This function loaded all patterns.
        // void readFilesPatterns(void); 

        // ADDED: These are our new on-demand functions.
        SystemPattern* loadPatternFromFile(uint8_t patternId);
        void freePattern(SystemPattern* pattern);

    private:
        void readFileCredentials(void);
        
        // MODIFIED: This will be our helper for the new load function.
        void populateSystemPattern(SystemPattern* pattern, File& file);

        CRGB mapColourNameToColour(const char*);
        
        // REMOVED: These functions were part of the old "load all" logic
        // void extractPatternsData(File&);
        // uint8_t validatePatternFile(const char*);
        // bool isPatternFileNameValid(const char*);
        
        String readFileTxt(fs::FS &fs, const char * path);
        CredentialsType credentials;
        DynamicJsonDocument* jsonDoc;

};

#endif