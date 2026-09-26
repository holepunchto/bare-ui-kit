const binding = require('../binding')
const { adopt, expose } = require('./handle')
const retain = require('./retain')
const UIKitView = require('./view')
const UIKitScene = require('./scene')
const UIKitViewController = require('./view-controller')

// UIKit, not JS, would otherwise be the only owner of a window that has been
// made key, and the registry holds the single retain.
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
    const { scene = null, x = 0, y = 0, width = 0, height = 0 } = opts

    return binding.windowInit(scene === null ? null : adopt(scene), x, y, width, height, this)
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

expose(exports)
