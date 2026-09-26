const binding = require('../binding')
const { adopt } = require('./handle')
const retain = require('./retain')
const UIKitView = require('./view')
const UIKitScene = require('./scene')
const UIKitViewController = require('./view-controller')

// Windows made key are kept here, because UIKit would otherwise be their only
// owner.
const key = new Set()

module.exports = exports = class UIKitWindow extends UIKitView {
  constructor(opts = {}) {
    super(opts)

    this._rootViewController = null
    this._windowScene = null
  }

  _onkeyboardwillchangeframe(x, y, width, height) {
    this.emit('keyboardWillChangeFrame', { x, y, width, height })
  }

  _init(opts) {
    const { scene = null, x, y, width, height } = opts

    const framed = x !== undefined || y !== undefined || width !== undefined || height !== undefined

    return binding.windowInit(
      scene === null ? null : adopt(scene),
      framed ? (x ?? 0) : null,
      framed ? (y ?? 0) : null,
      framed ? (width ?? 0) : null,
      framed ? (height ?? 0) : null,
      this
    )
  }

  get rootViewController() {
    return retain(
      this,
      '_rootViewController',
      UIKitViewController,
      binding.windowRootViewController(this._tag)
    )
  }

  set rootViewController(viewController) {
    binding.windowRootViewController(this._tag, adopt(viewController))
    this._rootViewController = viewController
  }

  get windowScene() {
    return retain(this, '_windowScene', UIKitScene, binding.windowWindowScene(this._tag))
  }

  set windowScene(windowScene) {
    binding.windowWindowScene(this._tag, adopt(windowScene))
    this._windowScene = windowScene
  }

  get keyWindow() {
    return binding.windowKeyWindow(this._tag)
  }

  makeKeyWindow() {
    binding.windowMakeKeyWindow(this._tag)
    key.add(this)
    return this
  }

  makeKeyAndVisible() {
    binding.windowMakeKeyAndVisible(this._tag)
    key.add(this)
    return this
  }

  [Symbol.for('bare.inspect')]() {
    return {
      __proto__: { constructor: UIKitWindow }
    }
  }
}

exports._events = {
  keyboardWillChangeFrame: binding.WINDOW_EVENT_KEYBOARD_WILL_CHANGE_FRAME
}
