#include <M5Atom.h>
#include <BleMouse.h>

// -----------------------------------------------------------------------------
// 変数
// -----------------------------------------------------------------------------
// ESP32-BLE-Mouse
BleMouse bleMouse;

// -----------------------------------------------------------------------------
// 関数
// -----------------------------------------------------------------------------
void setup() {
    // Serial, I2C, Display
    M5.begin(true, false, false);

    // Bluetooth接続
    bleMouse.begin();
}

void loop() {
    // Bluetooth接続済み
    static bool isConnected = false;

    M5.update();

    if (bleMouse.isConnected()) {
        // Bluetooth接続済みの場合

        if (!isConnected) {
            // Bluetooth接続されたらメッセージを表示
            Serial.println("Connected!");
            isConnected = true;
        }

        // マウスを動かす
        bleMouse.move(1, 0);
        delay(100);
        bleMouse.move(-1, 0);

        // 60秒待つ
        delay(60 * 1000);
    }
    else {
        // Bluetooth未接続の場合

        if (isConnected) {
            // Bluetooth切断されたらメッセージを表示
            Serial.println("Not connected!");
            isConnected = false;
        }

        // Bluetooth未接続の場合は1秒待つ
        delay(1000);
    }
}
