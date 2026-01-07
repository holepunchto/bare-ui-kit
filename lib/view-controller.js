const EventEmitter = require('bare-events')
const binding = require('../binding')
const UIKitView = require('./view')

module.exports = class UIKitViewController extends EventEmitter {
  constructor(handle = null) {
    super()

    this._handle = handle || binding.viewControllerInit(this)
  }

  get view() {
    return new UIKitView(binding.viewControllerView(this._handle))
  }

  set view(view) {
    binding.viewControllerView(this._handle, view._handle)
  }

  [Symbol.for('bare.inspect')]() {
    return {
      __proto__: { constructor: UIKitViewController }
    }
  }
}
