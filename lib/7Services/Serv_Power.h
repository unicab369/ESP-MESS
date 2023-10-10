
#define THRESHOLD_SENSITIVITY   40
#define MS_TO_SLEEP  10000    
// RTC_DATA_ATTR int bootCount2 = 0;
#define BUTTON_PIN_BITMASK 0x8000000

// ESP8266 Sleep modes https://blog.creations.de/?p=149

class Mng_Power {
    // Only RTC IO can be used as a source for external wake
    // source. They are pins: 0,2,4,12-15,25-27.

    public:
        #ifdef ESP32
            uint8_t wakeup_pinCode(){
                touch_pad_t  touchPin = esp_sleep_get_touchpad_wakeup_status();

                #ifdef ESP32
                    switch(touchPin) {
                        case 0: return 4;   case 1: return 0;
                        case 2: return 2;   case 3: return 15;
                        case 4: return 13;  case 5: return 12;
                        case 6: return 14;  case 7: return 27;
                        case 8: return 33;  case 9: return 32;
                        default: return 255;
                    }
                #else
                    return pinCode;
                #endif
            }

            esp_sleep_wakeup_cause_t printWakeupCause() {
                AppPrint("\n[Power]", __func__);
                esp_sleep_wakeup_cause_t wakeup_reason = esp_sleep_get_wakeup_cause();

                switch(wakeup_reason) {
                    case ESP_SLEEP_WAKEUP_EXT0: 
                        Serial.println(": by RTC_IO"); break;
                    case ESP_SLEEP_WAKEUP_EXT1:
                        Serial.println(": by EXT1"); break;
                    case ESP_SLEEP_WAKEUP_TIMER: 
                        Serial.println(": by timer"); break;
                    case ESP_SLEEP_WAKEUP_TOUCHPAD: 
                        Serial.println("by touchpad: " + String(wakeup_pinCode())); break;
                    case ESP_SLEEP_WAKEUP_GPIO:
                        char output[22];
                        sprintf(output, "by gpio: %u", esp_sleep_get_ext1_wakeup_status());
                        //  = "by gpio: " + esp_sleep_get_ext1_wakeup_status();
                        Serial.println(output); break;
                    case ESP_SLEEP_WAKEUP_ULP: 
                        Serial.println(": by ULP program"); break;
                    default : 
                        Serial.printf(" reason: %d\n", wakeup_reason); break;
                }

                return wakeup_reason;
            }

            bool wakeUpCauseResume() {
                Serial.print("\n[Power] "); Serial.println(__func__);
                esp_sleep_wakeup_cause_t cause = printWakeupCause();
                return cause == ESP_SLEEP_WAKEUP_TIMER || cause == ESP_SLEEP_WAKEUP_GPIO || 
                        cause == ESP_SLEEP_WAKEUP_EXT0 || cause == ESP_SLEEP_WAKEUP_EXT1 ||
                        cause == ESP_SLEEP_WAKEUP_TOUCHPAD;
            }
        #else
            bool wakeUpCauseResume() {
                AppPrint("\n[Power]", __func__);
                rst_info *resetInfo = ESP.getResetInfoPtr();

                switch(resetInfo->reason) {
                    case REASON_DEFAULT_RST:      Serial.println(": Power On"); break;
                    case REASON_WDT_RST:          Serial.println(": Hardware Watchdog"); break;
                    case REASON_EXCEPTION_RST:    Serial.println(": Exception"); break;
                    case REASON_SOFT_WDT_RST:     Serial.println(": Software Watchdog"); break;
                    case REASON_SOFT_RESTART:     Serial.println(": Software/System restart"); break;
                    case REASON_DEEP_SLEEP_AWAKE: Serial.println(": Deep-Sleep Wake"); break;
                    case REASON_EXT_SYS_RST:      Serial.println(": External System"); break;
                    default:                      Serial.println(": Unknown"); break;
                }

                return true;
            }
        #endif

        //! LightSleep
        void goToSleep(uint8_t pin, bool deep1light0 = false, uint16_t msDuration = 10000) {
            AppPrint("\n[Power]", __func__);

            #ifdef ESP32
                // int wake_thresh = 3;    // 0x3ff;
                // uart_set_wakeup_threshold(UART_NUM_0, wake_thresh);
                // esp_sleep_enable_uart_wakeup(UART_NUM_0);
                esp_wifi_stop();

                // Ex: GPIO0 and GPIO15 -> Calculate 2^2 + 2^15. You should get 32772
                // Convert that number to hex. You should get: 8004
                // Replace that number in the BUTTON_PIN_BITMASK
                uint64_t value = pow(2, pin);

                //! workable pins Only RTC IO 0,2,4,12-15,25-27.
                esp_sleep_enable_ext1_wakeup(value, ESP_EXT1_WAKEUP_ALL_LOW);       
                // esp_sleep_enable_ext0_wakeup(GPIO_NUM_23, GPIO_INTR_LOW_LEVEL);
                esp_sleep_enable_timer_wakeup(msDuration*1000);
                esp_sleep_enable_gpio_wakeup();

                if (deep1light0) {
                    esp_deep_sleep_start();
                } else {
                    esp_light_sleep_start();
                }
            #else
                if (deep1light0) {
                    ESP.deepSleep(msDuration*1000, RF_DEFAULT);
                } else {
                    wifi_station_disconnect();
                    wifi_set_opmode(NULL_MODE);
                    wifi_fpm_set_sleep_type(LIGHT_SLEEP_T);
                    wifi_fpm_open();
                    gpio_pin_wakeup_enable(GPIO_ID_PIN(2), GPIO_PIN_INTR_LOLEVEL);
                    wifi_fpm_do_sleep(0xFFFFFFF);
                }

                delay(100);
            #endif  

            Serial.println("[Power] sleep Done");
        }

        //! ModemSleep
        void modemSleepStart() {
            AppPrint("\n[Power]", __func__);

            #ifdef ESP32
                WiFi.setSleep(true);
                // if (!setCpuFrequencyMhz(40)) {
                //     Serial.println("[Power] failed to set freq 40Mhz");
                // }
            #else
                // WiFi.disconnect();
                WiFi.forceSleepBegin();
                delay(1);
                // wifi_fpm_set_sleep_type(NONE_SLEEP_T);	
                // wifi_fpm_auto_sleep_set_in_null_mode(NULL_MODE);
                // then call modemSleepEnd()

                wifi_set_opmode(NULL_MODE);
                wifi_fpm_set_sleep_type(MODEM_SLEEP_T);
                wifi_fpm_open();
                wifi_fpm_do_sleep(10000);
                delay(10);
            #endif
        }

        void modemSleepEnd() {
            AppPrint("\n[Power]", __func__);

            #ifdef ESP32
                if (!setCpuFrequencyMhz(240)) {
                    Serial.println("[Power] failed to set freq 240Mhz");
                }
            #else
                WiFi.forceSleepWake();
                delay(1);
            #endif
        }
};