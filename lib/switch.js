const binding = require('../binding')
const UIKitControl = require('./view')

module.exports = exports = class UIKitSwitch extends UIKitControl {
  _init(opts) {
    const { x = 0, y = 0, width = 0, height = 0 } = opts

    return binding.switchInit(x, y, width, height, this)
  }

  // Named `isOn`, because a property called `on` would hide the `on()` method
  // of the emitter.
  get isOn() {
    return binding.switchOn(this._tag)
  }

  set isOn(value) {
    binding.switchOn(this._tag, value)
  }

  get enabled() {
    return binding.switchEnabled(this._tag)
  }

  set enabled(enabled) {
    binding.switchEnabled(this._tag, enabled)
  }

  [Symbol.for('bare.inspect')]() {
    return {
      __proto__: { constructor: UIKitSwitch }
    }
  }

  _onvaluechanged() {
    this.emit('valueChanged')
  }
}

exports._events = {
  valueChanged: binding.SWITCH_EVENT_VALUE_CHANGED
}
