#include <M5Unified.h>
#include <HijelHID_BLEMouse.h>

// -----------------------------------------------------------------------------
// 変数
// -----------------------------------------------------------------------------
HijelBLEMouse mouse;

// -----------------------------------------------------------------------------
// 関数
// -----------------------------------------------------------------------------

static void setLed(int x, int y, uint8_t r, uint8_t g, uint8_t b) {
    if (x >= 0 && x < 5 && y >= 0 && y < 5) {
        M5.Led.setColor(y * 5 + x, r, g, b);
    }
}

static void clearLeds(void) {
    M5.Led.setAllColor(0, 0, 0);
}

// Bluetooth未接続: 赤い× (lit=false のときは消灯)
static void showDisconnected(bool lit) {
    clearLeds();
    if (lit) {
        for (int i = 0; i < 5; ++i) {
            setLed(i, i, 255, 0, 0);
            setLed(4 - i, i, 255, 0, 0);
        }
    }
    M5.Led.display();
}

// Bluetooth接続済: 緑のチェックマーク
static void showConnected(void) {
    clearLeds();
    setLed(0, 2, 0, 255, 0);
    setLed(1, 3, 0, 255, 0);
    setLed(2, 2, 0, 255, 0);
    setLed(3, 1, 0, 255, 0);
    setLed(4, 0, 0, 255, 0);
    M5.Led.display();
}

// moveTo() 実行中: 青の左右矢印
static void showMoving(void) {
    clearLeds();
    for (int x = 1; x <= 3; ++x) {
        setLed(x, 2, 0, 0, 255);
    }
    setLed(0, 2, 0, 0, 255);
    setLed(1, 1, 0, 0, 255);
    setLed(1, 3, 0, 0, 255);
    setLed(4, 2, 0, 0, 255);
    setLed(3, 1, 0, 0, 255);
    setLed(3, 3, 0, 0, 255);
    M5.Led.display();
}

void setup() {
    M5.begin();
    M5.Led.setBrightness(32);
    M5.Led.setAutoDisplay(false);
    clearLeds();
    M5.Led.display();
    mouse.begin();
}

void loop() {
    M5.update();

    // Bluetooth未接続: 1秒間隔で赤い×を点滅
    if (!mouse.isPaired()) {
        static bool lit = true;
        showDisconnected(lit);
        lit = !lit;
        delay(1000);
        return;
    }

    // moveTo() 実行中: 青の左右矢印
    showMoving();
    mouse.moveTo(1, 0);
    delay(100);
    mouse.moveTo(-1, 0);

    // Bluetooth接続済: 緑のチェックマーク
    // 1秒ごとに接続を再確認し、途中で切断されたら未接続表示へ切り替える
    for (uint32_t elapsed = 0; elapsed < (60 * 1000) && mouse.isPaired(); elapsed += 1000) {
        showConnected();
        delay(1000);
    }
}
