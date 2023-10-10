class Web_DevFile: public Web_Base {
    void makeContent() override {
        startPage();
        startTable("Device Files");

        // storage->fs.createDir("/dirA");
        // storage->fs.createDir("/dirB");
        // storage->fs.listDir("/", 3);

        // storage->fs.listDir("/", [](File *file) {
        //     webServer.sendContent("<tr><td><a href=\"" + String(file->name()) + "\">" 
        //                             + file->name() + "</a> (" + file->size() + " bytes)</td></tr>");
        // });

        makeReturnButton();
        stopTable();
        stopPage();
    }

    public:
         Web_DevFile(): Web_Base("Web_File") {}

        void begin(Serv_Network *network, WebServer *server) {
            load("/devFiles", network, server);
        }
};