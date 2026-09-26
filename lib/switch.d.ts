import UIKitView = require('./view')

/** An on and off switch, as a `UISwitch`. */
interface UIKitSwitch extends UIKitView<UIKitSwitch.Events> {
  /** Whether the switch is on. Named so it does not hide the `on()` method of the emitter. */
  isOn: boolean

  enabled: boolean
}

declare class UIKitSwitch {
  constructor(frame?: Partial<UIKitView.Rect>)
}

declare namespace UIKitSwitch {
  export interface Events {
    /** The user turned the switch on or off. Setting `isOn` does not emit this. */
    valueChanged: []
  }
}

export = UIKitSwitch
