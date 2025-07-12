#ifndef MICROBIT_BLE_MIDI_SERVICE_H
#define MICROBIT_BLE_MIDI_SERVICE_H

#include "MicroBit.h"
#include "MicroBitBLEService.h"
#include "ble/BLE.h"
#include "ble/Gap.h"
#include "ble/GattServer.h"

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

class MicroBitBLEMIDIService : public MicroBitBLEService
{
public:
    GattCharacteristic midiDataCharacteristic;

    // コンストラクタ
    MicroBitBLEMIDIService(MicroBit &microbit);

    // 接続イベントハンドラ
    void onConnection(const Gap::ConnectionCallbackParams *params);

    // 切断イベントハンドラ
    void onDisconnection(const Gap::DisconnectionCallbackParams *params);

    // 通知有効化イベントハンドラ
    void onUpdatesEnabled(const GattUpdatesEnabledCallbackParams *params);

    // 通知無効化イベントハンドラ
    void onUpdatesDisabled(const GattUpdatesDisabledCallbackParams *params);

    // MIDIデータを送信する関数
    bool sendMIDIData(const uint8_t *midi_data, uint16_t midi_len);

private:
    BLE &ble;
    MicroBit &microbit;
    bool notificationsEnabled; // 通知が有効化されているかのフラグ

    static const int MIDI_MAX_PACKET_SIZE = 20; // BLEの最大MTUに合わせて調整可能 (23バイトがデフォルト、ヘッダ含め20が安全)
};

#endif // MICROBIT_BLE_MIDI_SERVICE_H
