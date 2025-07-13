input.onButtonPressed(Button.A, function () {
    bleMidiTx.noteOn(60, 100, 1) // C4, Velocity 100, Channel 1
    basic.showString("C")
})
input.onButtonPressed(Button.B, function () {
    bleMidiTx.noteOff(60, 1) // C4, Channel 1
    basic.showString("_")
})
bleMidiTx.startBluetoothMidi()
