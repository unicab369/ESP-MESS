class Serv_Behaviors {
    Sto_Behavior* stoBehavior;

    public:
        void setup(Sto_Behavior* input) {
            stoBehavior = input;
        }

        void test() {
            // stoBehavior->deleteData();
            
            // ControlOutput action1;
            // action1.load(11, 22);
            // BehaviorItem behav1;
            // behav1.configure<ControlOutput>(TRIGGER_SINGLECLICK, action1);
            // stoBehavior->load(0, behav1);

            // ControlWS2812 action2;
            // action2.load(33, 223344);
            // BehaviorItem behav2;
            // behav2.configure<ControlWS2812>(TRIGGER_DOUBLECLICK, action2);
            // stoBehavior->load(1, behav2);

            // ControlSend action3;
            // BehaviorItem behav3;
            // behav3.configure<ControlSend> (TRIGGER_SINGLECLICK, action3);
            // stoBehavior->load(2, behav3);

            // stoBehavior->storeData();
        }
};