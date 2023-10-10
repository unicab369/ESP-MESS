#include "4Mng_Network.h"
#include "3Mng_Runtime.h"

class Mng_Device: public BaseComponent {
    Serv_Device servDev;
    Mng_Network network;
    Mng_Power power;

    std::function<void()> onHandleSingleClick = [&]() {
        network.handleSingleClick();
    };

    std::function<void()> onHandleDoubleClick = [&]() {
        network.handleDoubleClick();
    };

    std::function<void()> onHandleAPRequest = [&]() {
        network.startAP(true);      //! Start access point
    };

    std::function<void()> onHandleResetWifi = [&]() {
        network.resetWifi();        //! Reset Wifi
    };

    public:
        Mng_Device(): BaseComponent("MngDev") {}

        Mng_Runtime runtime;
        // void showLadderId() override {
        //     servDev.i2c.addDisplayQueue("ladderId = " + String(network.tweet.attendant.ladderId), 0);
        // }

        void setup() {
            xLog(__func__);            
            //! setupNetwork first to save time connecting
            servDev.configure();
            servDev.onHandleSingleClick = &onHandleSingleClick;
            servDev.onHandleDoubleClick = &onHandleDoubleClick;
            servDev.onHandleAPRequest = &onHandleAPRequest;
            servDev.onHandleResetWifi = &onHandleResetWifi;

            network.setupNetwork(&servDev);
            runtime.setup(&servDev, &network);
            AppPrintHeap();
        }
};

// ESPNow, behavior
// PIR Multi
// Lora Bluetooth, mqtt
// FS update, sd storage
// device encryption
// device discovery

// multi channel
// - Storing channel
// - Scan channel
// - broadcast channel
