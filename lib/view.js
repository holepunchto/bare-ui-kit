const EventEmitter = require('bare-events')
const binding = require('../binding')

module.exports = class UIKitView extends EventEmitter {
  constructor(handle = null) {
    super()

    this._handle = handle
  }

  get hidden() {
    return binding.viewHidden(this._handle)
  }

  set hidden(value) {
    binding.viewHidden(this._handle, value)
  }

  addSubview(view) {
    binding.viewAddSubview(this._handle, view._handle)
    return this
  }

  [Symbol.for('bare.inspect')]() {
    return {
      __proto__: { constructor: UIKitView }
    }
  }
}
