#include "Web_IOConfig.h"
#include "Web_IRConfig.h"
#include "Web_BehavConfig.h"

class Web_ControlConfig: public Web_Base {
    void makeContent() override {
        startPage();
        startTable("Device Configs");

        makeButton("I/O CONFIG", ioConfig.getPath());
        makeButton("IR CONFIG", iRConfig.getPath());
        makeButton("BEHAVIORS CONFIG", behavConfig.getPath());

        makeReturnButton();
        stopTable();
        stopPage();
    }

    Web_IOConfig ioConfig;
    Web_IRConfig iRConfig;
    Web_BehavConfig behavConfig;
    
    public:
        Web_ControlConfig(): Web_Base("Web_Contr") {}

        void begin(Serv_Network *network, WebServer *server) {
            load("/ctrlConfig", network, server);
            
            ioConfig.begin(network, server);
            iRConfig.begin(network, server);
            behavConfig.begin(network, server);
        }
};