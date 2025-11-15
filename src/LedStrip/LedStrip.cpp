#include "LedStrip.h"
#include "../SystemTypes.h"
#include "../DeviceModel.h"
#include "../commands/feqCmdProcessor.h"

CRGB leds[RGB_LED_STRIP_NUMBER_OF_LEDS];

/**
 * @note Assuming Led Strip task interval is 100 milliseconds. 
 * 
 */
#ifndef THREAD_TIME_MULTIPLIER_SECONDS
#define THREAD_TIME_MULTIPLIER_SECONDS 10
#endif

CRGB colorsAvailable[] = {

    CRGB::Red,    
    CRGB::Green,  
    CRGB::Blue,   
    CRGB::Yellow,  
    CRGB::Purple,
    CRGB::DarkBlue, 
    CRGB::Magenta,
    CRGB::Tomato,

};

volatile LedPatternAttributes ledPatternStrip;

volatile LedNodeUpdateAttributes ledUpdateNode;

static void displayLedStripStartUp(void){

  uint8_t colorsCount = sizeof(colorsAvailable)/sizeof(CRGB);
  for(uint8_t l = 0; l < RGB_LED_STRIP_NUMBER_OF_LEDS; l ++){
      leds[l] = colorsAvailable[0];
      FastLED.show();
      vTaskDelay(pdMS_TO_TICKS(10));
  }
  for(uint8_t l = 0; l < RGB_LED_STRIP_NUMBER_OF_LEDS; l ++){
      leds[l] = CRGB::Black;
      FastLED.show();
  }

}

static void fillStrip(const CRGB colour){

  for(uint8_t k = 0; k < RGB_LED_STRIP_NUMBER_OF_LEDS; k ++){
    leds[k] = colour;
    FastLED.show();
  }

}


const char* colorToName(const CRGB color){

  if(CRGB::Red == color) return "Red";
  if(CRGB::Green == color) return "Green";
  if(CRGB::Blue == color) return "Blue";
  if(CRGB::Yellow == color) return "Yellow";
  if(CRGB::Purple == color) return "Purple";
  if(CRGB::DarkBlue == color) return "DarkBlue";
  if(CRGB::Magenta == color) return "Magenta";
  if(CRGB::Tomato == color) return "Tomato";
  /**
   * @brief Extra colours added for completion
   * 
   */
  if (color == CRGB::AliceBlue) return "AliceBlue";
    if (color == CRGB::Amethyst) return "Amethyst";
    if (color == CRGB::AntiqueWhite) return "AntiqueWhite";
    if (color == CRGB::Aqua) return "Aqua";
    if (color == CRGB::Aquamarine) return "Aquamarine";
    if (color == CRGB::Azure) return "Azure";
    if (color == CRGB::Beige) return "Beige";
    if (color == CRGB::Bisque) return "Bisque";
    if (color == CRGB::Black) return "Black";
    if (color == CRGB::BlanchedAlmond) return "BlanchedAlmond";
    if (color == CRGB::Blue) return "Blue";
    if (color == CRGB::BlueViolet) return "BlueViolet";
    if (color == CRGB::Brown) return "Brown";
    if (color == CRGB::BurlyWood) return "BurlyWood";
    if (color == CRGB::CadetBlue) return "CadetBlue";
    if (color == CRGB::Chartreuse) return "Chartreuse";
    if (color == CRGB::Chocolate) return "Chocolate";
    if (color == CRGB::Coral) return "Coral";
    if (color == CRGB::CornflowerBlue) return "CornflowerBlue";
    if (color == CRGB::Cornsilk) return "Cornsilk";
    if (color == CRGB::Crimson) return "Crimson";
    if (color == CRGB::Cyan) return "Cyan";
    if (color == CRGB::DarkBlue) return "DarkBlue";
    if (color == CRGB::DarkCyan) return "DarkCyan";
    if (color == CRGB::DarkGoldenrod) return "DarkGoldenrod";
    if (color == CRGB::DarkGray) return "DarkGray";
    if (color == CRGB::DarkGrey) return "DarkGrey";
    if (color == CRGB::DarkGreen) return "DarkGreen";
    if (color == CRGB::DarkKhaki) return "DarkKhaki";
    if (color == CRGB::DarkMagenta) return "DarkMagenta";
    if (color == CRGB::DarkOliveGreen) return "DarkOliveGreen";
    if (color == CRGB::DarkOrange) return "DarkOrange";
    if (color == CRGB::DarkOrchid) return "DarkOrchid";
    if (color == CRGB::DarkRed) return "DarkRed";
    if (color == CRGB::DarkSalmon) return "DarkSalmon";
    if (color == CRGB::DarkSeaGreen) return "DarkSeaGreen";
    if (color == CRGB::DarkSlateBlue) return "DarkSlateBlue";
    if (color == CRGB::DarkSlateGray) return "DarkSlateGray";
    if (color == CRGB::DarkSlateGrey) return "DarkSlateGrey";
    if (color == CRGB::DarkTurquoise) return "DarkTurquoise";
    if (color == CRGB::DarkViolet) return "DarkViolet";
    if (color == CRGB::DeepPink) return "DeepPink";
    if (color == CRGB::DeepSkyBlue) return "DeepSkyBlue";
    if (color == CRGB::DimGray) return "DimGray";
    if (color == CRGB::DimGrey) return "DimGrey";
    if (color == CRGB::DodgerBlue) return "DodgerBlue";
    if (color == CRGB::FireBrick) return "FireBrick";
    if (color == CRGB::FloralWhite) return "FloralWhite";
    if (color == CRGB::ForestGreen) return "ForestGreen";
    if (color == CRGB::Fuchsia) return "Fuchsia";
    if (color == CRGB::Gainsboro) return "Gainsboro";
    if (color == CRGB::GhostWhite) return "GhostWhite";
    if (color == CRGB::Gold) return "Gold";
    if (color == CRGB::Goldenrod) return "Goldenrod";
    if (color == CRGB::Gray) return "Gray";
    if (color == CRGB::Grey) return "Grey";
    if (color == CRGB::Green) return "Green";
    if (color == CRGB::GreenYellow) return "GreenYellow";
    if (color == CRGB::Honeydew) return "Honeydew";
    if (color == CRGB::HotPink) return "HotPink";
    if (color == CRGB::IndianRed) return "IndianRed";
    if (color == CRGB::Indigo) return "Indigo";
    if (color == CRGB::Ivory) return "Ivory";
    if (color == CRGB::Khaki) return "Khaki";
    if (color == CRGB::Lavender) return "Lavender";
    if (color == CRGB::LavenderBlush) return "LavenderBlush";
    if (color == CRGB::LawnGreen) return "LawnGreen";
    if (color == CRGB::LemonChiffon) return "LemonChiffon";
    if (color == CRGB::LightBlue) return "LightBlue";
    if (color == CRGB::LightCoral) return "LightCoral";
    if (color == CRGB::LightCyan) return "LightCyan";
    if (color == CRGB::LightGoldenrodYellow) return "LightGoldenrodYellow";
    if (color == CRGB::LightGreen) return "LightGreen";
    if (color == CRGB::LightGrey) return "LightGrey";
    if (color == CRGB::LightPink) return "LightPink";
    if (color == CRGB::LightSalmon) return "LightSalmon";
    if (color == CRGB::LightSeaGreen) return "LightSeaGreen";
    if (color == CRGB::LightSkyBlue) return "LightSkyBlue";
    if (color == CRGB::LightSlateGray) return "LightSlateGray";
    if (color == CRGB::LightSlateGrey) return "LightSlateGrey";
    if (color == CRGB::LightSteelBlue) return "LightSteelBlue";
    if (color == CRGB::LightYellow) return "LightYellow";
    if (color == CRGB::Lime) return "Lime";
    if (color == CRGB::LimeGreen) return "LimeGreen";
    if (color == CRGB::Linen) return "Linen";
    if (color == CRGB::Magenta) return "Magenta";
    if (color == CRGB::Maroon) return "Maroon";
    if (color == CRGB::MediumAquamarine) return "MediumAquamarine";
    if (color == CRGB::MediumBlue) return "MediumBlue";
    if (color == CRGB::MediumOrchid) return "MediumOrchid";
    if (color == CRGB::MediumPurple) return "MediumPurple";
    if (color == CRGB::MediumSeaGreen) return "MediumSeaGreen";
    if (color == CRGB::MediumSlateBlue) return "MediumSlateBlue";
    if (color == CRGB::MediumSpringGreen) return "MediumSpringGreen";
    if (color == CRGB::MediumTurquoise) return "MediumTurquoise";
    if (color == CRGB::MediumVioletRed) return "MediumVioletRed";
    if (color == CRGB::MidnightBlue) return "MidnightBlue";
    if (color == CRGB::MintCream) return "MintCream";
    if (color == CRGB::MistyRose) return "MistyRose";
    if (color == CRGB::Moccasin) return "Moccasin";
    if (color == CRGB::NavajoWhite) return "NavajoWhite";
    if (color == CRGB::Navy) return "Navy";
    if (color == CRGB::OldLace) return "OldLace";
    if (color == CRGB::Olive) return "Olive";
    if (color == CRGB::OliveDrab) return "OliveDrab";
    if (color == CRGB::Orange) return "Orange";
    if (color == CRGB::OrangeRed) return "OrangeRed";
    if (color == CRGB::Orchid) return "Orchid";
    if (color == CRGB::PaleGoldenrod) return "PaleGoldenrod";
    if (color == CRGB::PaleGreen) return "PaleGreen";
    if (color == CRGB::PaleTurquoise) return "PaleTurquoise";
    if (color == CRGB::PaleVioletRed) return "PaleVioletRed";
    if (color == CRGB::PapayaWhip) return "PapayaWhip";
    if (color == CRGB::PeachPuff) return "PeachPuff";
    if (color == CRGB::Peru) return "Peru";
    if (color == CRGB::Pink) return "Pink";
    if (color == CRGB::Plaid) return "Plaid";
    if (color == CRGB::Plum) return "Plum";
    if (color == CRGB::PowderBlue) return "PowderBlue";
    if (color == CRGB::Purple) return "Purple";
    if (color == CRGB::Red) return "Red";
    if (color == CRGB::RosyBrown) return "RosyBrown";
    if (color == CRGB::RoyalBlue) return "RoyalBlue";
    if (color == CRGB::SaddleBrown) return "SaddleBrown";
    if (color == CRGB::Salmon) return "Salmon";
    if (color == CRGB::SandyBrown) return "SandyBrown";
    if (color == CRGB::SeaGreen) return "SeaGreen";
    if (color == CRGB::Seashell) return "Seashell";
    if (color == CRGB::Sienna) return "Sienna";
    if (color == CRGB::Silver) return "Silver";
    if (color == CRGB::SkyBlue) return "SkyBlue";
    if (color == CRGB::SlateBlue) return "SlateBlue";
    if (color == CRGB::SlateGray) return "SlateGray";
    if (color == CRGB::SlateGrey) return "SlateGrey";
    if (color == CRGB::Snow) return "Snow";
    if (color == CRGB::SpringGreen) return "SpringGreen";
    if (color == CRGB::SteelBlue) return "SteelBlue";
    if (color == CRGB::Tan) return "Tan";
    if (color == CRGB::Teal) return "Teal";
    if (color == CRGB::Thistle) return "Thistle";
    if (color == CRGB::Tomato) return "Tomato";
    if (color == CRGB::Turquoise) return "Turquoise";
    if (color == CRGB::Violet) return "Violet";
    if (color == CRGB::Wheat) return "Wheat";
    if (color == CRGB::White) return "White";
    if (color == CRGB::WhiteSmoke) return "WhiteSmoke";
    if (color == CRGB::Yellow) return "Yellow";
    if (color == CRGB::YellowGreen) return "YellowGreen";
    if (color == CRGB::FairyLight) return "FairyLight";
    if (color == CRGB::FairyLightNCC) return "FairyLightNCC";

    return "Unknown";

}

#if DEVICE_MODE_OPERATION == DEVICE_MODE_CONTROLLER

/**
 * @brief Device controller mode execution, 
 *        for LED strup specific tasks.
 * 
 */

#ifndef LED_STRIP_TASK_SLEEP_MS_CONTROLLER
#define LED_STRIP_TASK_SLEEP_MS_CONTROLLER 100
#endif

typedef struct NodeActionExecution{

  uint32_t durationMs;
  CRGB colour;
  uint8_t phase;
  uint8_t node; 

} NodeActionExecution;

typedef struct PatternExecution{

  uint8_t nodesInPattern;
  uint8_t phasesInPattern;
  uint32_t cycleDuration;
  uint8_t patternActive;
  bool running;
  NodeActionExecution runners[PATTERN_NODE_ACTIONS_MAX_N];

} PatternExecution;

typedef struct NodeScanningProcess{

  uint32_t timeOut;
  uint8_t nodesFound;

} NodeScanningProcess;

static PatternExecution pEx = {0};
const SystemPattern* pt;
const PatternPhase* ph;
FeqPatterns feqPatterns;
NodeScanningProcess nodeScanning; 

extern void meshNetworkSendPacket(BinaryPacket*, uint8_t);

static void displayPatternInformation(uint8_t patternIndex){

  if(patternIndex > feqPatterns.patternsCount){
    log_e("Invalid pattern index = %d",patternIndex);
    return;
  }
  const SystemPattern* const pattern = &feqPatterns.patterns[patternIndex];
  log_i("Pattern duration : %d , name = %s , phases =%d",
        pattern->duration, pattern->name,pattern->phasesCount);
  for(uint8_t k = 0; k < pattern->phasesCount; k++){
    const PatternPhase* const phase = &pattern->phases[k];
    log_i("Feautres for phase : %d -> node count -> %d , index -> %d",
          k,phase->nodeCount,phase->index);
    for(uint8_t node = 0; node < phase->nodeCount; ++node){
      const NodeAction* const action = &phase->actions[node];
      log_i("node %d , Duration = %d , Colour = 0x%04X , %s",
              action->index,action->duration,
              action->colour,colorToName(action->colour));
    }
  }

}

static void displayPatternsAvailable(void){

  vTaskDelay(pdMS_TO_TICKS(500));
  log_i("System patterns found = %d",feqPatterns.patternsCount);
  for(uint8_t k = 0; k < feqPatterns.patternsCount; k ++){
    const SystemPattern* const pattern = &feqPatterns.patterns[k];
    log_d("name: %s , duration: %d Secs , phasesCount %d",
        pattern->name,
        pattern->duration,
        pattern->phasesCount);
  }

}

static const SystemPattern* getCurrentPattern(uint8_t index){
  return &feqPatterns.patterns[index];
}

static bool checkForPatternUpdates(void){

  if(ledPatternStrip.receivedUpdate){
    ledPatternStrip.receivedUpdate = false;
    return true;
  }
  return false;

}

static void clearAllNodesOnStartUp(void){

  FeqCmd cmd;
  cmd.id = FEQ_CMD_CLEAR_STRIP;
  FeqCmdClearStrip* clearCmd = &cmd.context.clear;
  uint8_t data[255] = {0};
  BinaryPacket bin = {
    .data = data,
    .length = 0,
  };
  clearCmd->clearType = 0x00;
  clearCmd->delayToClear = 0x00;
  FeqCmdProcessor::encodeCmd(&cmd,&bin);
  meshNetworkSendPacket(&bin,FEQ_CMD_TARGET_BROADCAST);

}


static void evaluateStripState(void){

  if(checkForPatternUpdates()){
    clearAllNodesOnStartUp();
    pt = getCurrentPattern(ledPatternStrip.patternActive);
    pEx.patternActive = ledPatternStrip.patternActive;
    pEx.running = ledPatternStrip.state;
    pEx.nodesInPattern = pt->phases[0].nodeCount;
    pEx.phasesInPattern = pt->phasesCount;
    log_d("Rx to start with pattern %d , state %s ,phases %d , nodes: %d",
          pEx.patternActive,
          (pEx.running)?"ON":"OFF" ,
          pEx.phasesInPattern, 
          pEx.nodesInPattern);
    if(pEx.running){
      static const uint8_t startingPhase = 0;
      ph = &pt->phases[startingPhase];
      for(uint8_t k = 0; k < pEx.nodesInPattern; ++k){
        pEx.runners[k].colour = ph->actions[k].colour;
        pEx.runners[k].durationMs = ph->actions[k].duration * THREAD_TIME_MULTIPLIER_SECONDS;
        pEx.runners[k].node   = ph->actions[k].index - 1;
        pEx.runners[k].phase  = startingPhase+1;
        log_d("Start Node %d , %s for %d seconds",
              pEx.runners[k].node,
              colorToName(pEx.runners[k].colour),
              pEx.runners[k].durationMs/THREAD_TIME_MULTIPLIER_SECONDS);
        if(pEx.runners[k].node == 0){
          fillStrip(pEx.runners[k].colour);
        }
        else{
            FeqCmd cmd;
            cmd.id = FEQ_CMD_FILL_STRIP;
            FeqCmdFillStrip* fillCmd = &cmd.context.fill;
            uint8_t data[255] = {0};
            BinaryPacket bin = {
              .data = data,
              .length = 0,
            };
            fillCmd->colour = pEx.runners[k].colour;
            fillCmd->delayToSart = CMD_DELAY_TO_START_FILL_DEFAULT;
            fillCmd->fillType    = CMD_FILL_TYPE_DEFAULT;
            fillCmd->pattern     = CMD_PATTERN_TYPE_DEFAULT;
            FeqCmdProcessor::encodeCmd(&cmd,&bin);
            meshNetworkSendPacket(&bin,pEx.runners[k].node);
        }
      }
    }
    else{
      log_d("Clear all nodes, received commands to turn pattern off.");
      fillStrip(CRGB::Black);
      clearAllNodesOnStartUp();
    }
  }
  if(pEx.running){

    for(uint8_t node = 0; node < pEx.nodesInPattern; ++node){

      if(--pEx.runners[node].durationMs == 0){

        ph = &pt->phases[pEx.runners[node].phase++];
        pEx.runners[node].durationMs = 
            ( ph->actions[node].duration * THREAD_TIME_MULTIPLIER_SECONDS);
        pEx.runners[node].colour = ph->actions[node].colour;

        if(node == 0){
          fillStrip(pEx.runners[node].colour);
        }
        else{
            FeqCmd cmd;
            cmd.id = FEQ_CMD_FILL_STRIP;
            FeqCmdFillStrip* fillCmd = &cmd.context.fill;
            uint8_t data[255] = {0};
            BinaryPacket bin = {
              .data = data,
              .length = 0,
            };
            fillCmd->colour = pEx.runners[node].colour;
            fillCmd->delayToSart = CMD_DELAY_TO_START_FILL_DEFAULT;
            fillCmd->fillType    = CMD_FILL_TYPE_DEFAULT;
            fillCmd->pattern     = CMD_PATTERN_TYPE_DEFAULT;
            FeqCmdProcessor::encodeCmd(&cmd,&bin);
            meshNetworkSendPacket(&bin,pEx.runners[node].node);
        }
        if(pEx.runners[node].phase >= pEx.phasesInPattern){
          pEx.runners[node].phase = 0;
        }

      }

    }
  }
}

static bool isNodeScanningDue(void){

  return((nodeScanning.timeOut++ % 202) == 0);

}

static void sendNodesPingRequest(void){

  FeqCmd cmd;
  cmd.id = FEQ_CMD_PING_REQUEST;
  FeqCmdPingRequest* pingReq = &cmd.context.pingReq;
  uint8_t data[255] = {0};
  BinaryPacket bin = {
    .data = data,
    .length = 0,
  };
  pingReq->reqType = 0x00;
  FeqCmdProcessor::encodeCmd(&cmd,&bin);
  meshNetworkSendPacket(&bin,FEQ_CMD_TARGET_BROADCAST);

}

void ledStripTask(void *pvParameters){

  displayPatternsAvailable();
  displayLedStripStartUp();
  clearAllNodesOnStartUp();
  ledPatternStrip.receivedUpdate = false;
  nodeScanning.timeOut = 200;
  nodeScanning.nodesFound = 0;
  log_i("FEQ Led strip controller mode , is ready to receive commands. ");
  while(true){

    evaluateStripState();
    if(isNodeScanningDue()){
      sendNodesPingRequest();
    }
    vTaskDelay(pdMS_TO_TICKS(LED_STRIP_TASK_SLEEP_MS_CONTROLLER));

  }
}

LedStrip::LedStrip(){}

void LedStrip::init(void){

    FastLED.addLeds<WS2813, 
                    RGB_LED_STRIP_DATA_PIN_PRIMARY, 
                    GRB>(leds, RGB_LED_STRIP_NUMBER_OF_LEDS);
    FastLED.setBrightness(SYSTEM_LED_BRIGHTNESS_DEFAULT);
    xTaskCreate(ledStripTask, "ledStripTask",20000, NULL, 1, NULL);

}

#else
/**
 * @brief Device node mode execution, 
 *        for LED strup specific tasks.
 * 
 */
#ifndef LED_STRIP_TASK_SLEEP_MS_NODE
#define LED_STRIP_TASK_SLEEP_MS_NODE 50
#endif

#ifndef NODE_ONE_MINUTE_INACTIVITY
#define NODE_ONE_MINUTE_INACTIVITY ((60U*1000U)/LED_STRIP_TASK_SLEEP_MS_NODE)
#endif

uint32_t nodeInactivityTimeut = 0; 

static bool checkForNodeUpdates(void){

  if(ledUpdateNode.receivedUpdate){
    ledUpdateNode.receivedUpdate = false;
    return true;
  }
  return false;

}

static void checkForNodeInactivityTimeout(void){

  if(nodeInactivityTimeut ++ >= NODE_ONE_MINUTE_INACTIVITY){

    log_w("Device has not received a command in 1 minute, turn strip off");
    fillStrip(CRGB::Black);
    nodeInactivityTimeut = 0;
  }

}

void ledStripTask(void *pvParameters){
  displayLedStripStartUp();
  ledUpdateNode.receivedUpdate = false;
  log_i("FEQ Led strip node mode is ready to receive commands. ");
  while(true){

    if(checkForNodeUpdates()){

      switch(ledUpdateNode.cmd->id){

        case FEQ_CMD_CLEAR_STRIP:{
            const FeqCmdClearStrip* clear = &ledUpdateNode.cmd->context.clear;
            log_i("Rx fill clear led strip , delay to clear %d , type %d",
                 clear->delayToClear,clear->clearType);
            fillStrip(CRGB::Black);
        }break;
        case FEQ_CMD_RESTART_NODE:{

        }break;
        case FEQ_CMD_PING_REQUEST:{

        }break;
        case FEQ_CMD_FILL_STRIP:{
          const FeqCmdFillStrip* fill = &ledUpdateNode.cmd->context.fill;
          log_i("Rx fill colour %s , fT = 0x%02X , dS = 0x%02X , pT = 0x%02X",
               colorToName(fill->colour),fill->fillType,
                                         fill->delayToSart,
                                         fill->pattern);
          fillStrip(fill->colour);
        }break;
        case FEQ_CMD_PERFORM_PATTERN:{

        }break;
        case FEQ_CMD_PERFORM_SELF_COMPLETE:{

        }
        break;
        default:
            log_e("Node Received unkown command , ID = %d",ledUpdateNode.cmd->id);
        break;

      }
      nodeInactivityTimeut = 0;

    }

    vTaskDelay(pdMS_TO_TICKS(LED_STRIP_TASK_SLEEP_MS_NODE));
    checkForNodeInactivityTimeout();
  }
}

LedStrip::LedStrip(){}

void LedStrip::init(void){

    FastLED.addLeds<WS2813, 
                    RGB_LED_STRIP_DATA_PIN_PRIMARY, 
                    GRB>(leds, RGB_LED_STRIP_NUMBER_OF_LEDS);
    FastLED.setBrightness(SYSTEM_LED_BRIGHTNESS_DEFAULT);
    xTaskCreate(ledStripTask, "ledStripTask",20000, NULL, 1, NULL);

}

#endif
