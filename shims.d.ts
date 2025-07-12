// Auto-generated. Do not edit.
declare namespace bleMidiTx {
    /**
     * Bluetooth MIDIサービスを初期化します。
     * @internal
     */
    //% shim=bleMidiTx::_initMidiService
    function _initMidiService(): void;

    /**
     * MIDIデータをBLE経由で送信します。
     * @param data 送信するMIDI生データ (MTSはC++側で付加)
     * @internal
     */
    //% shim=bleMidiTx::_sendMidiData
    function _sendMidiData(data: Buffer): void;
}
