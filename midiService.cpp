#include "midiService.h"

// MicroBitインスタンスへのポインタ（グローバルまたはシングルトンで管理することが多い）
// PXTのC++拡張機能では、通常 shim 関数内で MicroBit& を受け取る
extern MicroBit uBit;
static MicroBitBLEMIDIService *instance = nullptr;

MicroBitBLEMIDIService::MicroBitBLEMIDIService(MicroBit &microbit_ref) :
    MicroBitBLEService(microbit_ref),
    ble(microbit_ref.ble),
    microbit(microbit_ref),
    midiDataCharacteristic(UUID_MIDI_DATA_CHAR,
                           nullptr, 0,
                           MIDI_MAX_PACKET_SIZE,
                           GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY)// 送信のみ
{
    // サービスの初期化
    GattCharacteristic *midiChars[] = {&midiDataCharacteristic};
    GattService midiService(UUID_MIDI_SERVICE, midiChars, sizeof(midiChars) / sizeof(GattCharacteristic *));

    // BLE Manager経由でサービスを登録
    if (ble.gattServer().addService(midiService) == BLE_STATUS_SUCCESS) {
        // microbit.display.printAsync("MIDI SVC OK");
    } else {
        // microbit.display.printAsync("MIDI SVC Fail");
    }

    // イベントハンドラを登録
    ble.gap().onConnection(this, &MicroBitBLEMIDIService::onConnection);
    ble.gap().onDisconnection(this, &MicroBitBLEMIDIService::onDisconnection);
    ble.gattServer().onUpdatesEnabled(this, &MicroBitBLEMIDIService::onUpdatesEnabled);
    ble.gattServer().onUpdatesDisabled(this, &MicroBitBLEMIDIService::onUpdatesDisabled);

    notificationsEnabled = false; // 初期状態では通知は無効
    instance = this; // シングルトンインスタンスを保存
}



void MicroBitBLEMIDIService::onConnection(const Gap::ConnectionCallbackParams *params)
{
    // microbit.display.printAsync("MIDI Conn");
    notificationsEnabled = false;
    // 接続イベントをディスパッチ
    microbit.messageBus.queue(MICROBIT_ID_BLE_MIDI_TX_SERVICE_EVT, MICROBIT_BLE_MIDI_TX_CONNECTED);
}

void MicroBitBLEMIDIService::onDisconnection(const Gap::DisconnectionCallbackParams *params)
{
    // microbit.display.printAsync("MIDI Disc");
    notificationsEnabled = false;
    // 切断イベントをディスパッチ
    microbit.messageBus.queue(MICROBIT_ID_BLE_MIDI_TX_SERVICE_EVT, MICROBIT_BLE_MIDI_TX_DISCONNECTED);
}

void MicroBitBLEMIDIService::onUpdatesEnabled(const GattUpdatesEnabledCallbackParams *params)
{
    if (params->charHandle == midiDataCharacteristic.getValueHandle()) {
        notificationsEnabled = true;
        // microbit.display.printAsync("MIDI Notif On");
    }
}

void MicroBitBLEMIDIService::onUpdatesDisabled(const GattUpdatesDisabledCallbackParams *params)
{
    if (params->charHandle == midiDataCharacteristic.getValueHandle()) {
        notificationsEnabled = false;
        // microbit.display.printAsync("MIDI Notif Off");
    }
}

bool MicroBitBLEMIDIService::sendMIDIData(const uint8_t *midi_data, uint16_t midi_len)
{
    if (!notificationsEnabled) {
        // 通知が有効になっていない場合は送信しない
        return false;
    }

    // MIDIパケットバッファ (MTS 2バイト + MIDIデータ)
    // MIDI_MAX_PACKET_SIZE は BLE の MTU (デフォルト23バイト) から L2CAPヘッダ(3バイト) を引いた値など
    // 通常は20バイト程度が安全
    if (midi_len + 2 > MIDI_MAX_PACKET_SIZE) {
        // パケットサイズが大きすぎる場合はエラー、または分割送信ロジックを実装する
        return false;
    }

    uint8_t packet[MIDI_MAX_PACKET_SIZE];
    uint16_t packet_len = 0;

    // 1. タイムスタンプの取得と計算
    // microbit.systemTime() は micro:bit の電源投入時からのミリ秒数
    uint32_t current_time_ms = microbit.systemTime();
    uint16_t timestamp_value = current_time_ms % 8192; // 13ビットに収める (0-8191)

    // 2. MIDI Time Stamp (MTS) ヘッダの構築
    // ヘッダバイト: 10xxxxxxb (xxxxxx = タイムスタンプ上位6ビット)
    packet[packet_len++] = 0x80 | ((timestamp_value >> 7) & 0x3F);
    // タイムスタンプバイト: 1xxxxxxxb (xxxxxxb = タイムスタンプ下位7ビット)
    packet[packet_len++] = 0x80 | (timestamp_value & 0x7F);

    // 3. MIDIデータのコピー
    memcpy(&packet[packet_len], midi_data, midi_len);
    packet_len += midi_len;

    // 4. BLE通知で送信
    // microbit.display.printAsync("TX"); // デバッグ用
    return ble.gattServer().notify(midiDataCharacteristic.getValueHandle(), packet, packet_len) == BLE_STATUS_SUCCESS;
}


// --- PXT MakeCode Shims ---
// TypeScript (main.ts) から呼び出されるC++関数

namespace bleMidiTx { // ★この行を追加！
    /**
     * Bluetooth MIDIサービスを初期化します。
     */
    //% shim=bleMidiTx::_initMidiService
    void _initMidiService() {
        if (instance == nullptr) {
            instance = new MicroBitBLEMIDIService(uBit);
        }
    }

    /**
     * MIDIデータをBLE経由で送信します。
     * @param data 送信するMIDI生データ (MTSはC++側で付加)
     */
    //% shim=bleMidiTx::_sendMidiData
    void _sendMidiData(Buffer data) {
        if (instance) {
            instance->sendMIDIData((const uint8_t*)data->data, data->length);
        }
    }
} // ★この行を追加！
