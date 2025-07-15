#ifndef MICROBIT_BLE_MIDI_SERVICE_H
#define MICROBIT_BLE_MIDI_SERVICE_H

#include "MicroBit.h"
#include "MicroBitBLEService.h"

#include "ble/BLE.h"
#include "ble/Gap.h"
#include "ble/GattServer.h"
/*
#include "MicroBitBLEManager.h"
#include "MicroBitBLEService.h"
*/
// Standard Bluetooth MIDI Service and Characteristic UUIDs (128-bit)
// MIDI Service UUID: 03B80E5A-EDE8-4B33-A751-6CE34EC4C700
const static uint8_t MIDIServiceUUID_base[] = {
    0x00, 0xC7, 0xC4, 0x4E, 0xE3, 0x6C, 0x51, 0xA7,
    0x33, 0x4B, 0xE8, 0xED, 0x5A, 0x0E, 0xB8, 0x03
};
const static Uuid UUID_MIDI_SERVICE(MIDIServiceUUID_base);

// MIDI Data Characteristic UUID: 7772E5DB-3868-410F-A2A7-A8BB2C782FBC
const static uint8_t MIDIDataCharacteristicUUID_base[] = {
    0xBC, 0x2F, 0x78, 0x2C, 0xBB, 0xA8, 0xA7, 0xA2,
    0x0F, 0x41, 0x68, 0x38, 0xDB, 0xE5, 0x72, 0x77
};
const static Uuid UUID_MIDI_DATA_CHAR(MIDIDataCharacteristicUUID_base);

// カスタムイベントのソースIDとイベントコードを定義
#define MICROBIT_ID_BLE_MIDI_TX_SERVICE_EVT (MICROBIT_ID_END + 1) // 新しいID

namespace bleMidiTx { // ★この行を追加！
    enum MicroBitBleMidiTxServiceEvent {
        MICROBIT_BLE_MIDI_TX_CONNECTED = 1,
        MICROBIT_BLE_MIDI_TX_DISCONNECTED = 2,
    };
    // クラス定義も namespace 内に置くか、または extern class宣言をする
    // 今回はクラス定義そのものは namespace 外に置いて、shim経由でアクセスするのが一般的
    // なので、enumだけnamespace内に置くのが良いでしょう。
} // ★この行を追加！

class MicroBitBLEMIDIService : public MicroBitBLEService
{
public:
    GattCharacteristic midiDataCharacteristic;  // キャラクターリスティックは必要なので残す

    // ★コンストラクタだけを残します
    MicroBitBLEMIDIService(MicroBit &microbit);

    // ★以下の行は全て削除またはコメントアウトしてください
    // 既存の接続/切断イベントハンドラ
    // void onConnection(const Gap::ConnectionCallbackParams *params);
    // void onDisconnection(const Gap::DisconnectionCallbackParams *params);
    // void onUpdatesEnabled(const GattUpdatesEnabledCallbackParams *params);
    // void onUpdatesDisabled(const GattUpdatesDisabledCallbackParams *params);
    // bool sendMIDIData(const uint8_t *midi_data, uint16_t midi_len);

private:
    BLE &ble;
    MicroBit &microbit;
    bool notificationsEnabled; // この変数も必要なので残す
    static const int MIDI_MAX_PACKET_SIZE = 20; // この変数も必要なので残す
};

#endif // MICROBIT_BLE_MIDI_SERVICE_H
