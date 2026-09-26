import EventEmitter from 'bare-events'
import { tag, handle, Handle, Wrapper } from 'bare-foundation-registry'

/** A button in an alert, as a `UIAlertAction`. */
interface UIKitAlertAction extends EventEmitter<UIKitAlertAction.Events> {
  readonly title: string | null

  readonly style: number

  readonly [tag]: number

  readonly [handle]: Handle
}

declare class UIKitAlertAction {
  /** `style` is a `STYLE` constant and defaults to `STYLE.DEFAULT`. */
  constructor(opts?: { title?: string; style?: number })

  static readonly STYLE: {
    readonly DEFAULT: number
    readonly CANCEL: number
    readonly DESTRUCTIVE: number
  }
}

declare namespace UIKitAlertAction {
  export interface Events {
    /** The button was pressed. */
    selected: []
  }
}

export = UIKitAlertAction
