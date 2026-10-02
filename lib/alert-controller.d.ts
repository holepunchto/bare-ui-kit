import UIKitViewController = require('./view-controller')
import UIKitAlertAction = require('./alert-action')

/** An alert or action sheet, as a `UIAlertController`. Show it with `present()`. */
interface UIKitAlertController extends UIKitViewController {
  /** Add a button. Listen for its `selected` event to know when it is pressed. */
  addAction(action: UIKitAlertAction): this
}

declare class UIKitAlertController {
  /** `preferredStyle` is a `STYLE` constant and defaults to `STYLE.ALERT`. */
  constructor(opts?: { title?: string; message?: string | null; preferredStyle?: number })

  static readonly STYLE: {
    readonly ACTION_SHEET: number
    readonly ALERT: number
  }
}

export = UIKitAlertController
