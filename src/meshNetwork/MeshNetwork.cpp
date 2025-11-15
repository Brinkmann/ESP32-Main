#include "MeshNetwork.h"
#include <WiFi.h>
#include "esp32-hal-log.h"
#include "../DeviceModel.h"
#include <esp_now.h>
#include <esp_wifi.h>
#include "../ledStrip/LedStrip.h"
#include "../commands/feqCommands.h"
#include "../commands/feqCmdProcessor.h"


#ifndef MESH_NETWORK_SIZE_DEVICES_N
#define MESH_NETWORK_SIZE_DEVICES_N 18
#endif


static const uint8_t devicesMacAddress[MESH_NETWORK_SIZE_DEVICES_N][FEQ_DEVICE_MAC_LENGTH] = {

    {0x34, 0x85, 0x18, 0x6d, 0x37, 0xb0}, // -> Master Device, Also strip one , Board physical label 1.
    {0x34, 0x85, 0x18, 0x99, 0xfd, 0x88}, // -> Node Device, strip two,         Board physical label 2. 
    {0x34, 0x85, 0x18, 0x97, 0x57, 0x9c}, // -> Node Device, strip three,       Board physical label 3.
    {0x34, 0x85, 0x18, 0x6c, 0xa5, 0x74}, // -> Node Device, strip four,        Board physical label 4.
    {0x34, 0x85, 0x18, 0x6c, 0x0c, 0xf8}, // -> Node Device, strip five,        Board physical label 5.
    {0x34, 0x85, 0x18, 0x75, 0xd6, 0xa4}, // -> Node Device, strip six,         Board physical label 6.
    {0x34, 0x85, 0x18, 0x99, 0xfc, 0x8c}, // -> Node Device, strip seven,       Board physical label 7.
    {0x34, 0x85, 0x18, 0x74, 0x77, 0xd4}, // -> Node Device, strip eight,       Board physical label 8.
    {0x34, 0x85, 0x18, 0x41, 0x8e, 0x18}, // -> Node Device, strip nine,        Board physical label 9.
    {0x34, 0x85, 0x18, 0x75, 0xe4, 0xa8}, // -> Node Device, strip ten,         Board physical label 10.
    {0x34, 0x85, 0x18, 0x9b, 0x53, 0xb0}, // -> Node Device, strip eleven,      Board physical label 11.  
    {0x34, 0x85, 0x18, 0x9b, 0x4a, 0xf8}, // -> Node Device, strip twelve,      Board physical label 12.  
    {0x34, 0x85, 0x18, 0x8d, 0x72, 0x7c}, // -> Node Device, strip thriteen,    Board physical label 13.
    {0x34, 0x85, 0x18, 0x99, 0xb6, 0x54}, // -> Node Device, strip Fourteen,    Board physical label 14.
    {0x34, 0x85, 0x18, 0x97, 0x61, 0xa8}, // -> Node Device, strip fifteen,     Board physical label 15.
    {0x34, 0x85, 0x18, 0x96, 0xd6, 0x9c}, // -> Node Device, strip sixteen,     Board physical label 16.
    {0x34, 0x85, 0x18, 0x9b, 0x4a, 0xcc}, // -> Node Device, stirp seventeen,   Board physical label 17.
    {0x34, 0x85, 0x18, 0x97, 0x4a, 0x0c}, // -> Node Device, strip eightenn,    Board physical label 18.

};

static const uint8_t meshAddressBroadcast[FEQ_DEVICE_MAC_LENGTH] = {
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};
 
extern LedPatternAttributes ledPatternStrip;
extern LedNodeUpdateAttributes ledUpdateNode;
volatile bool pendingPingResponse = false;
static esp_now_peer_info_t peerInfo;
static FeqCmd receivedCommand;

MeshNetwork::MeshNetwork(void){

#if DEVICE_MODE_OPERATION == DEVICE_MODE_CONTROLLER
    log_i("Mesh network instantiated as Controller.");
#else
    log_i("Mesh network instantiated as Node.");
#endif

}

void meshNetworkSendPacket(BinaryPacket* bin, uint8_t target){

  log_d("Send packet to node %d , contents: ",target);
  log_buf_d(bin->data,bin->length);
#if DEVICE_MODE_OPERATION == DEVICE_MODE_CONTROLLER
  if(target){
        esp_err_t result = esp_now_send((target == 0xFF) ? meshAddressBroadcast   : 
                                                           devicesMacAddress[target], 
                                        bin->data, 
                                        bin->length);
        if (result == ESP_OK) {
            log_i("Sending confirmed");
        }
        else {
            log_e("Sending error %d",result);
        }
  }

 #endif 

}

static void meshNetworkTask(void *pvParameters) {

    while(true){

 #if DEVICE_MODE_OPERATION != DEVICE_MODE_CONTROLLER

    if(pendingPingResponse){

        uint8_t data[5] = {0};
        data[0] = 0x4E;
        data[1] = 0x4F;
        data[2] = device->meshId;
        data[3] = 0x52;
        data[4] = 0x0A;
        esp_err_t result = esp_now_send(devicesMacAddress[0], 
                                        data, 
                                        5);
        if (result == ESP_OK) {
            log_i("Sending confirmed");
        }
        else {
            log_e("Sending error %d",result);
        }


        pendingPingResponse = false;
    }

#endif

        vTaskDelay(pdMS_TO_TICKS(1000));

    }
}

static void receiveCallback(const uint8_t* macAddr, const uint8_t* data, int dataLen){

#if DEVICE_MODE_OPERATION != DEVICE_MODE_CONTROLLER
    log_d("Data received , Length: %d",dataLen);
    log_buf_d(data,dataLen);
    BinaryPacket raw;
    raw.data   = (uint8_t*)data;
    raw.length = (uint8_t)dataLen;
    if(!FeqCmdProcessor::decodeCmd(&raw,&receivedCommand)){
        log_e("Unable to decode command.");
        return;
    }
    switch(receivedCommand.id){

        case FEQ_CMD_CLEAR_STRIP:{
            ledUpdateNode.cmd = &receivedCommand;
            ledUpdateNode.receivedUpdate = true;
        }break;
        case FEQ_CMD_RESTART_NODE:{

        }break;
        case FEQ_CMD_PING_REQUEST:{
            const FeqCmdPingRequest* ping = &receivedCommand.context.pingReq;
            log_i("Received ping request , request type = %d",ping->reqType);
            pendingPingResponse = true;
        }break;
        case FEQ_CMD_FILL_STRIP:{
            ledUpdateNode.cmd = &receivedCommand;
            ledUpdateNode.receivedUpdate = true;
        }break;
        case FEQ_CMD_PERFORM_PATTERN:{

        }break;
        case FEQ_CMD_PERFORM_SELF_COMPLETE:{

        }
        break;
        default:
            log_e("Received unkown command , ID = %d",receivedCommand.id);
        break;

    }
#else

    log_d("Data received , Length: %d",dataLen);
    log_buf_d(data,dataLen);
    if(0x05 == dataLen && 
       0x4E == data[0] &&
       0x4F == data[1] && 
       0x52 == data[3] && 
       0x0A == data[4]){
        log_i("Ping response from node -> %d",data[2]);
    }

#endif

}

static void sentCallback(const uint8_t *macAddr, esp_now_send_status_t status){

    log_i("Packet: %s",status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");

}

void MeshNetwork::addPeersToList(void){

#if DEVICE_MODE_OPERATION == DEVICE_MODE_CONTROLLER

    WiFi.mode(WIFI_MODE_APSTA);
    peerInfo.channel = ESP_NOW_CHANNEL_TO_USE;
    peerInfo.encrypt = false;
    peerInfo.ifidx = WIFI_IF_AP;

    for(uint8_t node = 1; node < MESH_NETWORK_SIZE_DEVICES_N; node ++){
        memcpy(peerInfo.peer_addr,
               &devicesMacAddress[node][0], 
               FEQ_DEVICE_MAC_LENGTH);
        if (esp_now_add_peer(&peerInfo) != ESP_OK){
            log_e("Error adding ESP-NOW peer %d.",node);
            return;
        }
    }

    log_i("ESP-NOW Added %d Peers succesfully.",MESH_NETWORK_SIZE_DEVICES_N-1);

    memcpy(peerInfo.peer_addr,
           meshAddressBroadcast, 
           FEQ_DEVICE_MAC_LENGTH);
    if (esp_now_add_peer(&peerInfo) != ESP_OK){
        log_e("Error adding ESP-NOW broadcast peer");
        return;
    }
    log_i("Adding broadcast address as a peer as well.");
#else

    peerInfo.channel = ESP_NOW_CHANNEL_TO_USE;
    peerInfo.encrypt = false;
    peerInfo.ifidx = WIFI_IF_STA;

    memcpy(peerInfo.peer_addr,
           devicesMacAddress[0], 
           FEQ_DEVICE_MAC_LENGTH);

    if (esp_now_add_peer(&peerInfo) != ESP_OK){
        log_e("Error adding ESP-NOW peer.");
        return;
    }

    log_i("ESP-NOW Added Master to peer list. ");

#endif

}

void MeshNetwork::initEspNow(void){

#if DEVICE_MODE_OPERATION == DEVICE_MODE_NODE
       WiFi.mode(WIFI_MODE_STA);
       esp_wifi_set_promiscuous(true);
       esp_wifi_set_channel(ESP_NOW_CHANNEL_TO_USE, WIFI_SECOND_CHAN_NONE);
       esp_wifi_set_promiscuous(false);
       esp_err_t errCode = ESP_FAIL;
       errCode = esp_wifi_set_protocol(WIFI_IF_STA,WIFI_PROTOCOL_LR);
       if(errCode){
        log_e("Failed setting Wi-FI long range ");
      }
#endif

    if(ESP_OK == esp_now_init()){
        log_i("ESP-NOW Initialized succesfully.");
        esp_now_register_recv_cb(receiveCallback);
        esp_now_register_send_cb(sentCallback);
    }
    else{
        log_e("ESP-NOW Failed initialization.");
        return;
    }
    WiFi.setTxPower(WIFI_POWER_19_5dBm);
    addPeersToList();
}

void MeshNetwork::setDeviceId(void){
    bool foundDeviceInList = false;
    for(uint8_t ids = 0; ids < MESH_NETWORK_SIZE_DEVICES_N; ++ ids){
        uint8_t matches = 0;
        for(uint8_t k = 0; k < FEQ_DEVICE_MAC_LENGTH; ++k){
            if(devicesMacAddress[ids][k] == device->mac[k]){
                matches ++;
            }
        }
        if(FEQ_DEVICE_MAC_LENGTH == matches){
            device->meshId = ids;
            foundDeviceInList = true;
            break;
        }
    }
    if(foundDeviceInList){
        log_i("Device ID in the mesh network: %d",device->meshId);
    }
    else{
        log_e("Device ID is not on our list, resetting !");
        esp_restart();
    }

}

void MeshNetwork::init(void){

    initEspNow();
    WiFi.macAddress(device->mac);
    log_w("FEQ Device MAC: ");
    log_buf_w(device->mac,FEQ_DEVICE_MAC_LENGTH);
    setDeviceId();
    xTaskCreate(meshNetworkTask, "meshNetworsTask",10000, NULL, 1, NULL);

}
