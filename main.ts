/**
 * micro:bit Bluetooth MIDI Transmit Extension
 */
//% weight=100 color=#0078D4 icon="\uf001"
namespace bleMidiTx {
    let _initialized = false;

    /**
     * Bluetooth MIDIサービスを初期化します。
     * このブロックは、プログラムの開始時に一度だけ呼び出してください。
     */
    //% blockId="ble_midi_tx_init" block="Bluetooth MIDIを開始"
    //% subcategory="設定"
    export function startBluetoothMidi(): void {
        if (_initialized) {
            return;
        }
        _initialized = true;
        // C++側の初期化関数を呼び出す
        _initMidiService();
    }

    /**
     * MIDIノートオンメッセージを送信します。
     * @param note ノート番号 (0-127), 例: 60 (C4)
     * @param velocity ベロシティ (0-127), 例: 100
     * @param channel MIDIチャンネル (1-16), 例: 1
     */
    //% blockId="ble_midi_tx_note_on" block="MIDI ノートオン|ノート %note|ベロシティ %velocity|チャンネル %channel"
    //% note.min=0 note.max=127 velocity.min=0 velocity.max=127 channel.min=1 channel.max=16
    //% inlineInputMode=inline
    //% subcategory="送信"
    export function noteOn(note: number, velocity: number, channel: number): void {
        if (!_initialized) {
            startBluetoothMidi(); // 自動初期化
        }
        // pxt-midi を使ってMIDIノートオンメッセージの生データを生成
        const midiData = pins.createBuffer(3);
        midiData[0] = 0x90 | ((channel - 1) & 0x0F); // Note On status byte + channel
        midiData[1] = note & 0x7F;                 // Note number
        midiData[2] = velocity & 0x7F;             // Velocity

        // C++側の送信関数を呼び出す
        _sendMidiData(midiData);
    }

    /**
     * MIDIノートオフメッセージを送信します。
     * @param note ノート番号 (0-127), 例: 60 (C4)
     * @param channel MIDIチャンネル (1-16), 例: 1
     */
    //% blockId="ble_midi_tx_note_off" block="MIDI ノートオフ|ノート %note|チャンネル %channel"
    //% note.min=0 note.max=127 channel.min=1 channel.max=16
    //% inlineInputMode=inline
    //% subcategory="送信"
    export function noteOff(note: number, channel: number): void {
        if (!_initialized) {
            startBluetoothMidi(); // 自動初期化
        }
        // pxt-midi を使ってMIDIノートオフメッセージの生データを生成
        const midiData = pins.createBuffer(3);
        midiData[0] = 0x80 | ((channel - 1) & 0x0F); // Note Off status byte + channel
        midiData[1] = note & 0x7F;                 // Note number
        midiData[2] = 0x00;                        // Velocity (0 for note off)

        // C++側の送信関数を呼び出す
        _sendMidiData(midiData);
    }

    // 他のMIDIメッセージ（CC, PBなど）も同様に追加可能

    //% shim=bleMidiTx::_initMidiService
    function _initMidiService(): void {
        // C++実装
        return
    }

    //% shim=bleMidiTx::_sendMidiData
    function _sendMidiData(data: Buffer): void {
        // C++実装
        return
    }
}