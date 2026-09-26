const binding = require('../binding')
const retain = require('./retain')
const wrap = require('./wrap')
const UIKitScreen = require('./screen')

module.exports = exports = class UIKitScene {
  constructor(opts = {}) {
    const { tag = null } = opts

    this._tag = tag

    this._token = binding.claim(this._tag, this)
  }

  static get connected() {
    return binding.sceneConnected().map((tag) => wrap(UIKitScene, tag))
  }

  get screen() {
    return retain(this, '_screen', UIKitScreen, binding.sceneScreen(this._tag))
  }

  get activationState() {
    return binding.sceneActivationState(this._tag)
  }

  [Symbol.for('bare.inspect')]() {
    return {
      __proto__: { constructor: UIKitScene }
    }
  }
}

exports.ACTIVATION_STATE = {
  UNATTACHED: binding.SCENE_ACTIVATION_STATE_UNATTACHED,
  FOREGROUND_ACTIVE: binding.SCENE_ACTIVATION_STATE_FOREGROUND_ACTIVE,
  FOREGROUND_INACTIVE: binding.SCENE_ACTIVATION_STATE_FOREGROUND_INACTIVE,
  BACKGROUND: binding.SCENE_ACTIVATION_STATE_BACKGROUND
}
