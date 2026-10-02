const binding = require('../binding')
const { adopt } = require('./handle')
const UIKitViewController = require('./view-controller')

module.exports = exports = class UIKitAlertController extends UIKitViewController {
  constructor(opts = {}) {
    const { title = '', message = null, preferredStyle = exports.STYLE.ALERT } = opts

    super({ tag: binding.alertControllerInit(title, message, preferredStyle) })

    this._actions = new Set()
  }

  addAction(action) {
    binding.alertControllerAddAction(this._tag, adopt(action))

    // Keep the wrapper, so its listeners live as long as the controller.
    this._actions.add(action)

    return this
  }

  [Symbol.for('bare.inspect')]() {
    return {
      __proto__: { constructor: UIKitAlertController }
    }
  }
}

exports.STYLE = {
  ACTION_SHEET: binding.ALERT_CONTROLLER_STYLE_ACTION_SHEET,
  ALERT: binding.ALERT_CONTROLLER_STYLE_ALERT
}
