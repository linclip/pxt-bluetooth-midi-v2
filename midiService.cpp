#include "midiService.h" // ヘッダーは含める

// MicroBitインスタンスへのポインタ（グローバルまたはシングルトンで管理することが多い）
// PXTのC++拡張機能では、通常 shim 関数内で MicroBit& を受け取る
extern MicroBit uBit;
static MicroBitBLEMIDIService *instance = nullptr; // インスタンスポインタは残す

// MicroBitBLEMIDIService クラスのコンストラクタのみ残します
MicroBitBLEMIDIService::MicroBitBLEMIDIService(MicroBit &microbit_ref) :
    MicroBitBLEService(microbit_ref),
    ble(microbit_ref.ble),
    microbit(microbit_ref),
    midiDataCharacteristic(UUID_MIDI_DATA_CHAR,
                           nullptr, 0,
                           MIDI_MAX_PACKET_SIZE,
                           GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY)
{
    GattCharacteristic *midiChars[] = {&midiDataCharacteristic};
    GattService midiService(UUID_MIDI_SERVICE, midiChars, sizeof(midiChars) / sizeof(GattCharacteristic *));

    // サービス追加の成功/失敗をチェックするデバッグコードもここに残します
    // ★ サービス追加の成功/失敗をチェック
    if (ble.gattServer().addService(midiService) == BLE_STATUS_SUCCESS) {
         microbit.display.printAsync("SVC OK"); // デバッグ用
    } else {
        microbit.display.printAsync("SVC FAIL"); // デバッグ用
        // ここで何かエラーを示す処理（LED表示など）
        microbit.panic(100); // 強制的にパニックさせてエラーコードで確認
    }

    // ★以下のイベントハンドラ登録の行は全て削除またはコメントアウトしてください
    // ble.gap().onConnection(this, &MicroBitBLEMIDIService::onConnection);
    // ble.gap().onDisconnection(this, &MicroBitBLEMIDIService::onDisconnection);
    // ble.gattServer().onUpdatesEnabled(this, &MicroBitBLEMIDIService::onUpdatesEnabled);
    // ble.gattServer().onUpdatesDisabled(this, &MicroBitBLEMIDIService::onUpdatesDisabled);

    notificationsEnabled = false; // 必要なので残す
    instance = this; // 必要なので残す
}

/*
void MicroBitBLEMIDIService::onConnection(const Gap::ConnectionCallbackParams *params)
{
    // ここをコメントアウト
    // notificationsEnabled = false;
    // microbit.messageBus.queue(MICROBIT_ID_BLE_MIDI_TX_SERVICE_EVT, bleMidiTx::MICROBIT_BLE_MIDI_TX_CONNECTED);
}

void MicroBitBLEMIDIService::onDisconnection(const Gap::DisconnectionCallbackParams *params)
{
    // ここをコメントアウト
    // notificationsEnabled = false;
    // microbit.messageBus.queue(MICROBIT_ID_BLE_MIDI_TX_SERVICE_EVT, bleMidiTx::MICROBIT_BLE_MIDI_TX_DISCONNECTED);
}

void MicroBitBLEMIDIService::onUpdatesEnabled(const GattUpdatesEnabledCallbackParams *params)
{
    // ここをコメントアウト
    // if (params->charHandle == midiDataCharacteristic.getValueHandle()) {
    //     notificationsEnabled = true;
    // }
}

void MicroBitBLEMIDIService::onUpdatesDisabled(const GattUpdatesDisabledCallbackParams *params)
{
    // ここをコメントアウト
    // if (params->charHandle == midiDataCharacteristic.getValueHandle()) {
    //     notificationsEnabled = false;
    // }
}

// sendMIDIData はそのまま残す
bool MicroBitBLEMIDIService::sendMIDIData(const uint8_t *midi_data, uint16_t midi_len)
{
    if (!notificationsEnabled) {
        return false;
    }

    if (midi_len + 2 > MIDI_MAX_PACKET_SIZE) {
        return false;
    }

    uint8_t packet[MIDI_MAX_PACKET_SIZE];
    uint16_t packet_len = 0;

    uint32_t current_time_ms = microbit.systemTime();
    uint16_t timestamp_value = current_time_ms % 8192;

    packet[packet_len++] = 0x80 | ((timestamp_value >> 7) & 0x3F);
    packet[packet_len++] = 0x80 | (timestamp_value & 0x7F);

    memcpy(&packet[packet_len], midi_data, midi_len);
    packet_len += midi_len;

    return ble.gattServer().notify(midiDataCharacteristic.getValueHandle(), packet, packet_len) == BLE_STATUS_SUCCESS;
}
*/

// --- PXT MakeCode Shims ---
// TypeScript (main.ts) から呼び出されるC++関数

namespace bleMidiTx { // ★この行を追加！
    // _initMidiService の Shim 関数のみ残します
    /**
     * Bluetooth MIDIサービスを初期化します。
     */
    //% shim=bleMidiTx::_initMidiService
    void _initMidiService() {
        if (instance == nullptr) {
            instance = new MicroBitBLEMIDIService(uBit);
        }
    }

    // ★ _sendMidiData の Shim 関数は削除またはコメントアウトしてください
    /**
     * MIDIデータをBLE経由で送信します。
     * @param data 送信するMIDI生データ (MTSはC++側で付加)
     */
    /*
    //% shim=bleMidiTx::_sendMidiData
    void _sendMidiData(Buffer data) {
        if (instance) {
            instance->sendMIDIData((const uint8_t*)data->data, data->length);
        }
    }
    */
} // ★この行を追加！
