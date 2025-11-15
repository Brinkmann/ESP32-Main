#ifndef MESH_NETWORK_H
#define MESH_NETWORK_H

#ifndef ESP_NOW_CHANNEL_TO_USE
#define ESP_NOW_CHANNEL_TO_USE 6
#endif

class MeshNetwork{

    public: 
        MeshNetwork(void);
        void init(void);
    private:
        void initEspNow(void);
        void addPeersToList(void);
        void setDeviceId(void);

};

#endif