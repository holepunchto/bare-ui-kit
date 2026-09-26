const EventEmitter = require('bare-events')
const binding = require('../binding')
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

    observe(this)
  }

  get view() {
    return retain(this, '_view', UIKitView, binding.viewControllerView(this._tag))
  }

  set view(view) {
    binding.viewControllerView(this._tag, adopt(view))
    this._view = view
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
  didLayoutSubviews: 1,
  safeAreaInsetsDidChange: 2
}
