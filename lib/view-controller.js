const EventEmitter = require('bare-events')
const binding = require('../binding')
const registry = require('bare-foundation-registry')
const { adopt } = require('./handle')
const observe = require('./events')
const retain = require('./retain')
const UIKitView = require('./view')

module.exports = exports = class UIKitViewController extends EventEmitter {
  constructor(opts = {}) {
    super()

    const { tag = null } = opts

    this._tag = tag === null ? binding.viewControllerInit(this) : tag
    this._view = null

    this._token = binding.claim(this._tag, this)

    if (tag === null) this.view = new UIKitView()

    observe(this)
  }

  get [registry.tag]() {
    return this._tag
  }

  get [registry.handle]() {
    return binding.handle(this._tag)
  }

  get view() {
    return retain(this, '_view', UIKitView, binding.viewControllerView(this._tag))
  }

  set view(view) {
    binding.viewControllerView(this._tag, adopt(view))
    this._view = view
  }

  present(controller, animated = true) {
    binding.viewControllerPresent(this._tag, adopt(controller), animated)

    return this
  }

  dismiss(animated = true) {
    binding.viewControllerDismiss(this._tag, animated)

    return this
  }

  _ondidlayoutsubviews() {
    this.emit('didLayoutSubviews')
  }

  _onsafeareainsetsdidchange() {
    this.emit('safeAreaInsetsDidChange')
  }

  [Symbol.for('bare.inspect')]() {
    return {
      __proto__: { constructor: UIKitViewController }
    }
  }
}

exports._events = {
  didLayoutSubviews: binding.VIEW_CONTROLLER_EVENT_DID_LAYOUT_SUBVIEWS,
  safeAreaInsetsDidChange: binding.VIEW_CONTROLLER_EVENT_SAFE_AREA_INSETS_DID_CHANGE
}
