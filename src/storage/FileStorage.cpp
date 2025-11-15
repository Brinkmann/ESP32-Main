#include "FileStorage.h"

#ifndef FILE_STORAGE_CREDENTIALS_PATH 
#define FILE_STORAGE_CREDENTIALS_PATH  "/credentials/creds.json"
#endif 

#ifndef FILES_PATTERNS_PATH
#define FILES_PATTERNS_PATH "/patterns"
#endif


#if DEVICE_MODE_OPERATION == DEVICE_MODE_CONTROLLER

extern FeqPatterns feqPatterns;

#endif


FileStorage::FileStorage(){
    jsonDoc = new DynamicJsonDocument(12800);
    memset(&feqPatterns,0,sizeof(feqPatterns));
    credentials.pwrdLength = 0;
    credentials.ssidLength = 0;
}

void FileStorage::init(void){
    if (!SPIFFS.begin(true)){
        while(true){
            delay(2000);
            log_e("Error mounting SPIFFS.");
        }
    }
    log_i("SPI Flash File Storage initialized successfully !");
    readFileCredentials();
}

String FileStorage::readFileTxt(fs::FS &fs, const char * path){
  log_d("Reading file: %s\r\n", path);
  File file = fs.open(path);
  if(!file || file.isDirectory()){
    log_e("Failed to open text file for reading");
    return String();
  }
  String fileContent;
  while(file.available()){
    fileContent = file.readString();
    break;     
  }
  return fileContent;
}

bool FileStorage::isPatternFileNameValid(const char* name){

    const char* patternPrefix = "pattern_";
    const char* jsonSuffix = ".json";
    const uint8_t maxFileNameLenght = 16;

    return ( strstr(name, patternPrefix)       && 
             strlen(name) <= maxFileNameLenght &&
             strstr(name, jsonSuffix));

}

uint8_t FileStorage::validatePatternFile(const char* fileName){
    const uint8_t maxPatternsSupport = 16;
    if(isPatternFileNameValid(fileName)){
        uint8_t patternIndex = atoi(&fileName[8]);
        if(patternIndex <= maxPatternsSupport){
            return patternIndex;
        }
    }
    log_e("Invalid pattern file name.");
    return 0xFF;
}

CRGB FileStorage::mapColourNameToColour(const char* colorName){

    if (strcmp(colorName, "Red") == 0) return CRGB::Red;
    if (strcmp(colorName, "Green") == 0) return CRGB::Green;
    if (strcmp(colorName, "Blue") == 0) return CRGB::Blue;
    if (strcmp(colorName, "Yellow") == 0) return CRGB::Yellow;
    if (strcmp(colorName, "Purple") == 0) return CRGB::Purple;
    if (strcmp(colorName, "DarkBlue") == 0) return CRGB::DarkBlue;
    if (strcmp(colorName, "Magenta") == 0) return CRGB::Magenta;
    if (strcmp(colorName, "Tomato") == 0) return CRGB::Tomato;
    /**
     * @brief Extra colours added for completion.
     * 
     */
    if (strcmp(colorName, "AliceBlue") == 0) return CRGB::AliceBlue;
    if (strcmp(colorName, "Amethyst") == 0) return CRGB::Amethyst;
    if (strcmp(colorName, "AntiqueWhite") == 0) return CRGB::AntiqueWhite;
    if (strcmp(colorName, "Aqua") == 0) return CRGB::Aqua;
    if (strcmp(colorName, "Aquamarine") == 0) return CRGB::Aquamarine;
    if (strcmp(colorName, "Azure") == 0) return CRGB::Azure;
    if (strcmp(colorName, "Beige") == 0) return CRGB::Beige;
    if (strcmp(colorName, "Bisque") == 0) return CRGB::Bisque;
    if (strcmp(colorName, "Black") == 0) return CRGB::Black;
    if (strcmp(colorName, "BlanchedAlmond") == 0) return CRGB::BlanchedAlmond;
    if (strcmp(colorName, "Blue") == 0) return CRGB::Blue;
    if (strcmp(colorName, "BlueViolet") == 0) return CRGB::BlueViolet;
    if (strcmp(colorName, "Brown") == 0) return CRGB::Brown;
    if (strcmp(colorName, "BurlyWood") == 0) return CRGB::BurlyWood;
    if (strcmp(colorName, "CadetBlue") == 0) return CRGB::CadetBlue;
    if (strcmp(colorName, "Chartreuse") == 0) return CRGB::Chartreuse;
    if (strcmp(colorName, "Chocolate") == 0) return CRGB::Chocolate;
    if (strcmp(colorName, "Coral") == 0) return CRGB::Coral;
    if (strcmp(colorName, "CornflowerBlue") == 0) return CRGB::CornflowerBlue;
    if (strcmp(colorName, "Cornsilk") == 0) return CRGB::Cornsilk;
    if (strcmp(colorName, "Crimson") == 0) return CRGB::Crimson;
    if (strcmp(colorName, "Cyan") == 0) return CRGB::Cyan;
    if (strcmp(colorName, "DarkBlue") == 0) return CRGB::DarkBlue;
    if (strcmp(colorName, "DarkCyan") == 0) return CRGB::DarkCyan;
    if (strcmp(colorName, "DarkGoldenrod") == 0) return CRGB::DarkGoldenrod;
    if (strcmp(colorName, "DarkGray") == 0) return CRGB::DarkGray;
    if (strcmp(colorName, "DarkGrey") == 0) return CRGB::DarkGrey;
    if (strcmp(colorName, "DarkGreen") == 0) return CRGB::DarkGreen;
    if (strcmp(colorName, "DarkKhaki") == 0) return CRGB::DarkKhaki;
    if (strcmp(colorName, "DarkMagenta") == 0) return CRGB::DarkMagenta;
    if (strcmp(colorName, "DarkOliveGreen") == 0) return CRGB::DarkOliveGreen;
    if (strcmp(colorName, "DarkOrange") == 0) return CRGB::DarkOrange;
    if (strcmp(colorName, "DarkOrchid") == 0) return CRGB::DarkOrchid;
    if (strcmp(colorName, "DarkRed") == 0) return CRGB::DarkRed;
    if (strcmp(colorName, "DarkSalmon") == 0) return CRGB::DarkSalmon;
    if (strcmp(colorName, "DarkSeaGreen") == 0) return CRGB::DarkSeaGreen;
    if (strcmp(colorName, "DarkSlateBlue") == 0) return CRGB::DarkSlateBlue;
    if (strcmp(colorName, "DarkSlateGray") == 0) return CRGB::DarkSlateGray;
    if (strcmp(colorName, "DarkSlateGrey") == 0) return CRGB::DarkSlateGrey;
    if (strcmp(colorName, "DarkTurquoise") == 0) return CRGB::DarkTurquoise;
    if (strcmp(colorName, "DarkViolet") == 0) return CRGB::DarkViolet;
    if (strcmp(colorName, "DeepPink") == 0) return CRGB::DeepPink;
    if (strcmp(colorName, "DeepSkyBlue") == 0) return CRGB::DeepSkyBlue;
    if (strcmp(colorName, "DimGray") == 0) return CRGB::DimGray;
    if (strcmp(colorName, "DimGrey") == 0) return CRGB::DimGrey;
    if (strcmp(colorName, "DodgerBlue") == 0) return CRGB::DodgerBlue;
    if (strcmp(colorName, "FireBrick") == 0) return CRGB::FireBrick;
    if (strcmp(colorName, "FloralWhite") == 0) return CRGB::FloralWhite;
    if (strcmp(colorName, "ForestGreen") == 0) return CRGB::ForestGreen;
    if (strcmp(colorName, "Fuchsia") == 0) return CRGB::Fuchsia;
    if (strcmp(colorName, "Gainsboro") == 0) return CRGB::Gainsboro;
    if (strcmp(colorName, "GhostWhite") == 0) return CRGB::GhostWhite;
    if (strcmp(colorName, "Gold") == 0) return CRGB::Gold;
    if (strcmp(colorName, "Goldenrod") == 0) return CRGB::Goldenrod;
    if (strcmp(colorName, "Gray") == 0) return CRGB::Gray;
    if (strcmp(colorName, "Grey") == 0) return CRGB::Grey;
    if (strcmp(colorName, "Green") == 0) return CRGB::Green;
    if (strcmp(colorName, "GreenYellow") == 0) return CRGB::GreenYellow;
    if (strcmp(colorName, "Honeydew") == 0) return CRGB::Honeydew;
    if (strcmp(colorName, "HotPink") == 0) return CRGB::HotPink;
    if (strcmp(colorName, "IndianRed") == 0) return CRGB::IndianRed;
    if (strcmp(colorName, "Indigo") == 0) return CRGB::Indigo;
    if (strcmp(colorName, "Ivory") == 0) return CRGB::Ivory;
    if (strcmp(colorName, "Khaki") == 0) return CRGB::Khaki;
    if (strcmp(colorName, "Lavender") == 0) return CRGB::Lavender;
    if (strcmp(colorName, "LavenderBlush") == 0) return CRGB::LavenderBlush;
    if (strcmp(colorName, "LawnGreen") == 0) return CRGB::LawnGreen;
    if (strcmp(colorName, "LemonChiffon") == 0) return CRGB::LemonChiffon;
    if (strcmp(colorName, "LightBlue") == 0) return CRGB::LightBlue;
    if (strcmp(colorName, "LightCoral") == 0) return CRGB::LightCoral;
    if (strcmp(colorName, "LightCyan") == 0) return CRGB::LightCyan;
    if (strcmp(colorName, "LightGoldenrodYellow") == 0) return CRGB::LightGoldenrodYellow;
    if (strcmp(colorName, "LightGreen") == 0) return CRGB::LightGreen;
    if (strcmp(colorName, "LightGrey") == 0) return CRGB::LightGrey;
    if (strcmp(colorName, "LightPink") == 0) return CRGB::LightPink;
    if (strcmp(colorName, "LightSalmon") == 0) return CRGB::LightSalmon;
    if (strcmp(colorName, "LightSeaGreen") == 0) return CRGB::LightSeaGreen;
    if (strcmp(colorName, "LightSkyBlue") == 0) return CRGB::LightSkyBlue;
    if (strcmp(colorName, "LightSlateGray") == 0) return CRGB::LightSlateGray;
    if (strcmp(colorName, "LightSlateGrey") == 0) return CRGB::LightSlateGrey;
    if (strcmp(colorName, "LightSteelBlue") == 0) return CRGB::LightSteelBlue;
    if (strcmp(colorName, "LightYellow") == 0) return CRGB::LightYellow;
    if (strcmp(colorName, "Lime") == 0) return CRGB::Lime;
    if (strcmp(colorName, "LimeGreen") == 0) return CRGB::LimeGreen;
    if (strcmp(colorName, "Linen") == 0) return CRGB::Linen;
    if (strcmp(colorName, "Magenta") == 0) return CRGB::Magenta;
    if (strcmp(colorName, "Maroon") == 0) return CRGB::Maroon;
    if (strcmp(colorName, "MediumAquamarine") == 0) return CRGB::MediumAquamarine;
    if (strcmp(colorName, "MediumBlue") == 0) return CRGB::MediumBlue;
    if (strcmp(colorName, "MediumOrchid") == 0) return CRGB::MediumOrchid;
    if (strcmp(colorName, "MediumPurple") == 0) return CRGB::MediumPurple;
    if (strcmp(colorName, "MediumSeaGreen") == 0) return CRGB::MediumSeaGreen;
    if (strcmp(colorName, "MediumSlateBlue") == 0) return CRGB::MediumSlateBlue;
    if (strcmp(colorName, "MediumSpringGreen") == 0) return CRGB::MediumSpringGreen;
    if (strcmp(colorName, "MediumTurquoise") == 0) return CRGB::MediumTurquoise;
    if (strcmp(colorName, "MediumVioletRed") == 0) return CRGB::MediumVioletRed;
    if (strcmp(colorName, "MidnightBlue") == 0) return CRGB::MidnightBlue;
    if (strcmp(colorName, "MintCream") == 0) return CRGB::MintCream;
    if (strcmp(colorName, "MistyRose") == 0) return CRGB::MistyRose;
    if (strcmp(colorName, "Moccasin") == 0) return CRGB::Moccasin;
    if (strcmp(colorName, "NavajoWhite") == 0) return CRGB::NavajoWhite;
    if (strcmp(colorName, "Navy") == 0) return CRGB::Navy;
    if (strcmp(colorName, "OldLace") == 0) return CRGB::OldLace;
    if (strcmp(colorName, "Olive") == 0) return CRGB::Olive;
    if (strcmp(colorName, "OliveDrab") == 0) return CRGB::OliveDrab;
    if (strcmp(colorName, "Orange") == 0) return CRGB::Orange;
    if (strcmp(colorName, "OrangeRed") == 0) return CRGB::OrangeRed;
    if (strcmp(colorName, "Orchid") == 0) return CRGB::Orchid;
    if (strcmp(colorName, "PaleGoldenrod") == 0) return CRGB::PaleGoldenrod;
    if (strcmp(colorName, "PaleGreen") == 0) return CRGB::PaleGreen;
    if (strcmp(colorName, "PaleTurquoise") == 0) return CRGB::PaleTurquoise;
    if (strcmp(colorName, "PaleVioletRed") == 0) return CRGB::PaleVioletRed;
    if (strcmp(colorName, "PapayaWhip") == 0) return CRGB::PapayaWhip;
    if (strcmp(colorName, "PeachPuff") == 0) return CRGB::PeachPuff;
    if (strcmp(colorName, "Peru") == 0) return CRGB::Peru;
    if (strcmp(colorName, "Pink") == 0) return CRGB::Pink;
    if (strcmp(colorName, "Plaid") == 0) return CRGB::Plaid;
    if (strcmp(colorName, "Plum") == 0) return CRGB::Plum;
    if (strcmp(colorName, "PowderBlue") == 0) return CRGB::PowderBlue;
    if (strcmp(colorName, "Purple") == 0) return CRGB::Purple;
    if (strcmp(colorName, "Red") == 0) return CRGB::Red;
    if (strcmp(colorName, "RosyBrown") == 0) return CRGB::RosyBrown;
    if (strcmp(colorName, "RoyalBlue") == 0) return CRGB::RoyalBlue;
    if (strcmp(colorName, "SaddleBrown") == 0) return CRGB::SaddleBrown;
    if (strcmp(colorName, "Salmon") == 0) return CRGB::Salmon;
    if (strcmp(colorName, "SandyBrown") == 0) return CRGB::SandyBrown;
    if (strcmp(colorName, "SeaGreen") == 0) return CRGB::SeaGreen;
    if (strcmp(colorName, "Seashell") == 0) return CRGB::Seashell;
    if (strcmp(colorName, "Sienna") == 0) return CRGB::Sienna;
    if (strcmp(colorName, "Silver") == 0) return CRGB::Silver;
    if (strcmp(colorName, "SkyBlue") == 0) return CRGB::SkyBlue;
    if (strcmp(colorName, "SlateBlue") == 0) return CRGB::SlateBlue;
    if (strcmp(colorName, "SlateGray") == 0) return CRGB::SlateGray;
    if (strcmp(colorName, "SlateGrey") == 0) return CRGB::SlateGrey;
    if (strcmp(colorName, "Snow") == 0) return CRGB::Snow;
    if (strcmp(colorName, "SpringGreen") == 0) return CRGB::SpringGreen;
    if (strcmp(colorName, "SteelBlue") == 0) return CRGB::SteelBlue;
    if (strcmp(colorName, "Tan") == 0) return CRGB::Tan;
    if (strcmp(colorName, "Teal") == 0) return CRGB::Teal;
    if (strcmp(colorName, "Thistle") == 0) return CRGB::Thistle;
    if (strcmp(colorName, "Tomato") == 0) return CRGB::Tomato;
    if (strcmp(colorName, "Turquoise") == 0) return CRGB::Turquoise;
    if (strcmp(colorName, "Violet") == 0) return CRGB::Violet;
    if (strcmp(colorName, "Wheat") == 0) return CRGB::Wheat;
    if (strcmp(colorName, "White") == 0) return CRGB::White;
    if (strcmp(colorName, "WhiteSmoke") == 0) return CRGB::WhiteSmoke;
    if (strcmp(colorName, "Yellow") == 0) return CRGB::Yellow;
    if (strcmp(colorName, "YellowGreen") == 0) return CRGB::YellowGreen;
    if (strcmp(colorName, "FairyLight") == 0) return CRGB::FairyLight;
    if (strcmp(colorName,"FairyLightNCC") == 0) return CRGB::FairyLightNCC;

    return CRGB::Black;

}

void FileStorage::populateSystemPatterns(uint8_t patternIndex, File& file){

    //StaticJsonDocument<6000> jsonDoc;
    DeserializationError error = deserializeJson(*jsonDoc, file);
    if (error) {
        log_e("Failed to read patterns JSON file error = %d",error);
        file.close();
        return;
    }
    SystemPattern* systemPattern = &feqPatterns.patterns[patternIndex];
    feqPatterns.patternsCount ++;
    JsonObject root = (*jsonDoc).as<JsonObject>();
    for (JsonObject::iterator it = root.begin(); it != root.end(); ++it) {
        const char* rootKey = it->key().c_str();
        systemPattern->nameLength = strlen(rootKey) + 1;
        memcpy(systemPattern->name,rootKey,systemPattern->nameLength);
        log_d("Iterate through: %s , length %d",rootKey,systemPattern->nameLength);
        JsonObject patternObj = it->value().as<JsonObject>();
        if(patternObj.containsKey("duration")){
            systemPattern->duration =  patternObj["duration"] | 0;
        }
        if (patternObj.containsKey("phases")) {
            JsonArray phasesArray = patternObj["phases"].as<JsonArray>();
            systemPattern->phasesCount = 0;
            for (JsonVariant phaseVar : phasesArray) {
                PatternPhase* phase = &systemPattern->phases[systemPattern->phasesCount++];
                JsonObject phaseObj = phaseVar.as<JsonObject>();
                phase->index = phaseObj["phase"] | 0;
                log_d("Extracting phase %d :",phase->index);
                JsonArray actionsArray = phaseObj["nodes"];
                phase->nodeCount = 0;
                for (JsonVariant actionVar : actionsArray) {
                    JsonObject actionObj = actionVar.as<JsonObject>();
                    NodeAction* action = &phase->actions[phase->nodeCount++];
                    const char* colour = actionObj["color"];
                    action->index = actionObj["node"] | 0;
                    action->colour = mapColourNameToColour(colour);
                    action->duration = actionObj["secs"] | 0;;
                    log_d("Node index = %d, color String: %s , HEX 0x%04X , secs = %d",
                        action->index,
                        colour,
                        action->colour,
                        action->duration);
                }
            }
            break;
        }
    }
}

void FileStorage::extractPatternsData(File& file){
    const char* fileName = file.name();
    log_d("Found Pattern JSON file: %s",fileName);
    uint8_t patternIndex = validatePatternFile(fileName);
    if(patternIndex != 0xFF){
        populateSystemPatterns(patternIndex-1,file);
    }
}

void FileStorage::readFilesPatterns(void){
    log_d("Reading Patterns Files.");
    File pattersnDir = SPIFFS.open(FILES_PATTERNS_PATH);
    if(!pattersnDir){
        log_e("Unable to open patterns directory. ");
        return;
    }
    File file = pattersnDir.openNextFile();
    while(file){
        extractPatternsData(file);
        file = pattersnDir.openNextFile();
    }
    pattersnDir.close();
}

void FileStorage::readFileCredentials(void){

    if(!SPIFFS.exists(FILE_STORAGE_CREDENTIALS_PATH)){
        log_e("Credentials file does not exist.");
        return;
    }
    File file = SPIFFS.open(FILE_STORAGE_CREDENTIALS_PATH, "r");
    if (!file) {
        log_e("Failed to open credentials file");
        return;
    }
    //StaticJsonDocument<200> jsonDoc;
    DeserializationError error = deserializeJson(*jsonDoc, file);
    if (error) {
        log_e("Failed to read JSON file");
        file.close();
        return;
    }
    const char* SSID     = (*jsonDoc)["SSID"];
    const char* Password = (*jsonDoc)["Password"]; 
    uint8_t ssidLength     = measureJson((*jsonDoc)["SSID"]);
    uint8_t passwordLength = measureJson((*jsonDoc)["Password"]);
    file.close();
    credentials.pwrdLength = passwordLength - 1;
    for(uint8_t k = 0; k < credentials.pwrdLength; k++){
        credentials.pwrd[k] = Password[k];
    }
    credentials.ssidLength = ssidLength - 1;
    for(uint8_t k = 0; k < credentials.ssidLength; k++){
        credentials.ssid[k] = SSID[k];
    }
    log_i("Cred -> p=%s, s=%s",credentials.pwrd,credentials.ssid);
}

bool FileStorage::resetCredentials(fs::FS &fs){
    File file = fs.open(FILE_STORAGE_CREDENTIALS_PATH, FILE_WRITE);

    if(!file){

        log_e("Failed to open file for writing");
        return false;

    }
    else{
        log_w("Proceeding to reset credentials in the device !");
        DynamicJsonDocument jsonDocument(1024);
        jsonDocument["SSID"] = "NS";
        jsonDocument["Password"] ="NP";
        serializeJson(jsonDocument, file);
        file.close();
        return true;
    }
}

bool FileStorage::writeCredentials(fs::FS &fs, CredentialsType* cred){

    if(cred != nullptr){

        File file = fs.open(FILE_STORAGE_CREDENTIALS_PATH, FILE_WRITE);
        if(!file){
            log_e("Failed to open file for writing");
            return false;
        }

        log_w("Proceeding to save the following credentials ");
        log_w("SSID:  %s , length = %d",cred->ssid,cred->ssidLength);
        log_w("Password: %s , length = %d",cred->pwrd,cred->pwrdLength);
        DynamicJsonDocument jsonDocument(1024);
        jsonDocument["SSID"] = cred->ssid;
        jsonDocument["Password"] = cred->pwrd;
        serializeJson(jsonDocument, file);
        file.close();
        return true;
    }
    else{
        log_e("Credentials do not exist, Failed updating SPIFFS !");
        return false;
    }

}

CredentialsType* FileStorage::getCredentials(void){    
    
    if( credentials.pwrdLength < SYSTEM_CREDENTIALS_MIN_LENGTH ||
        credentials.pwrdLength > SYSTEM_CREDENTIALS_MAX_LENGTH || 
        credentials.ssidLength < SYSTEM_CREDENTIALS_MIN_LENGTH ||
        credentials.ssidLength > SYSTEM_CREDENTIALS_MAX_LENGTH
        ){
        return nullptr;
    }
    return &credentials;
}