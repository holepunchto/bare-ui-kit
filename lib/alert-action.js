const EventEmitter = require('bare-events')
const binding = require('../binding')
const registry = require('bare-foundation-registry')

// No mask: the handler is set when the action is made, so it always reports.
module.exports = exports = class UIKitAlertAction extends EventEmitter {
  constructor(opts = {}) {
    super()

    const { tag = null, title = '', style = exports.STYLE.DEFAULT } = opts

    this._tag = tag === null ? binding.alertActionInit(title, style, this) : tag

    this._token = binding.claim(this._tag, this)
  }

  get [registry.tag]() {
    return this._tag
  }

  get [registry.handle]() {
    return binding.handle(this._tag)
  }

  get title() {
    return binding.alertActionTitle(this._tag)
  }

  get style() {
    return binding.alertActionStyle(this._tag)
  }

  _onselected() {
    this.emit('selected')
  }

  [Symbol.for('bare.inspect')]() {
    return {
      __proto__: { constructor: UIKitAlertAction }
    }
  }
}

exports.STYLE = {
  DEFAULT: binding.ALERT_ACTION_STYLE_DEFAULT,
  CANCEL: binding.ALERT_ACTION_STYLE_CANCEL,
  DESTRUCTIVE: binding.ALERT_ACTION_STYLE_DESTRUCTIVE
}
