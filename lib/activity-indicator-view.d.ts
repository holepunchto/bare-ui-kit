import { tag, handle, Handle, Wrapper } from 'bare-foundation-registry'
import UIKitView = require('./view')
import UIKitColor = require('./color')

/** A spinning busy indicator, as a `UIActivityIndicatorView`. */
interface UIKitActivityIndicatorView extends UIKitView {
  /** The size of the indicator, as a `STYLE` constant. */
  style: number

  get color(): UIKitColor | null
  set color(color: Wrapper | null)

  /** Whether the indicator is hidden while it is not spinning. */
  hidesWhenStopped: boolean

  /** Whether the indicator is spinning. */
  animating: boolean
}

declare class UIKitActivityIndicatorView {
  constructor(frame?: Partial<UIKitView.Rect>)

  static readonly STYLE: {
    readonly MEDIUM: number
    readonly LARGE: number
  }
}

export = UIKitActivityIndicatorView
