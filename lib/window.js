const binding = require('../binding')
const UIKitView = require('./view')
const UIKitViewController = require('./view-controller')

module.exports = exports = class UIKitWindow extends UIKitView {
  constructor() {
    super()

    this._handle = binding.windowInit(this)
  }

  get rootViewController() {
    const handle = binding.windowRootViewController(this._handle)

    if (handle === undefined) return null

    return new UIKitViewController(handle)
  }

  set rootViewController(viewController) {
    binding.windowRootViewController(this._handle, viewController._handle)
  }

  makeKeyWindow() {
    binding.windowMakeKeyWindow(this._handle)
    return this
  }

  [Symbol.for('bare.inspect')]() {
    return {
      __proto__: { constructor: UIKitWindow }
    }
  }
}
