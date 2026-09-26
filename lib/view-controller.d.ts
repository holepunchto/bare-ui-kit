import EventEmitter from 'bare-events'
import { tag, handle, Handle, Wrapper } from 'bare-foundation-registry'
import UIKitView = require('./view')

/** Manages a view, as a `UIViewController`. */
interface UIKitViewController<
  M extends Record<keyof M, unknown[]> = UIKitViewController.Events
> extends EventEmitter<M> {
  get view(): UIKitView | null
  set view(view: Wrapper | null)

  /** Show `controller` over this one. */
  present(controller: Wrapper, animated?: boolean): this

  /** Close the controller this one presented, or this one if it was presented. */
  dismiss(animated?: boolean): this

  readonly [tag]: number

  readonly [handle]: Handle
}

declare class UIKitViewController<
  M extends Record<keyof M, unknown[]> = UIKitViewController.Events
> {
  constructor()
}

declare namespace UIKitViewController {
  export interface Events {
    /** The view was laid out, for example because the window changed size. */
    didLayoutSubviews: []
    safeAreaInsetsDidChange: []
  }
}

export = UIKitViewController
