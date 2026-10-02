import EventEmitter from 'bare-events'
import { tag, handle, Handle, Wrapper } from 'bare-foundation-registry'
import UIKitColor = require('./color')

/**
 * The base of every view, as a `UIView`. Views that are added to a view are kept alive by it, so
 * their listeners keep working.
 */
interface UIKitView<
  M extends Record<keyof M, unknown[]> = UIKitView.Events
> extends EventEmitter<M> {
  /** The position and size of the view in its superview. Missing fields are 0. */
  get frame(): UIKitView.Rect
  set frame(frame: Partial<UIKitView.Rect>)

  /** The position and size of the view in its own coordinates. Missing fields are 0. */
  get bounds(): UIKitView.Rect
  set bounds(bounds: Partial<UIKitView.Rect>)

  hidden: boolean

  /** How opaque the view is, from 0 to 1. */
  alpha: number

  /** How the content is fitted to the view, as a `CONTENT_MODE` constant. */
  contentMode: number

  clipsToBounds: boolean

  userInteractionEnabled: boolean

  get backgroundColor(): UIKitColor | null
  set backgroundColor(color: Wrapper | null)

  get tintColor(): UIKitColor | null
  set tintColor(color: Wrapper | null)

  /** How far the safe area is inset from each edge of the view. */
  readonly safeAreaInsets: UIKitView.Insets

  readonly superview: UIKitView | null

  readonly subviews: UIKitView[]

  /** Add `view` on top of the other subviews. It can come from another addon. */
  addSubview(view: Wrapper): this

  insertSubviewAtIndex(view: Wrapper, index: number): this

  insertSubviewBelowSubview(view: Wrapper, sibling: Wrapper): this

  insertSubviewAboveSubview(view: Wrapper, sibling: Wrapper): this

  removeFromSuperview(): this

  setNeedsLayout(): this

  setNeedsDisplay(): this

  /**
   * Stop editing anywhere inside the view, which hides the keyboard. With `force`, the default,
   * editing stops even if a field asks to keep it. Returns whether editing stopped.
   */
  endEditing(force?: boolean): boolean

  /** Convert a point in the view to the coordinates of `view`, or of the window when `null`. */
  convertPointToView(x: number, y: number, view?: UIKitView | null): { x: number; y: number }

  /** Convert a point in `view`, or in the window when `null`, to the coordinates of the view. */
  convertPointFromView(x: number, y: number, view?: UIKitView | null): { x: number; y: number }

  /** The size the view would like to be, within `width` by `height`. */
  sizeThatFits(width: number, height: number): { width: number; height: number }

  /** Resize the view to the size it would like to be. */
  sizeToFit(): this

  /** Take the keyboard focus. Returns whether the view took it. */
  becomeFirstResponder(): boolean

  /** Give up the keyboard focus. Returns whether the view gave it up. */
  resignFirstResponder(): boolean

  /** Whether the view has the keyboard focus. */
  readonly firstResponder: boolean

  /** Force light or dark mode for the view, as a `USER_INTERFACE_STYLE` constant. */
  overrideUserInterfaceStyle: number

  /** Whether the view is drawn in light or dark mode, as a `USER_INTERFACE_STYLE` constant. */
  readonly userInterfaceStyle: number

  readonly [tag]: number

  readonly [handle]: Handle
}

declare class UIKitView<M extends Record<keyof M, unknown[]> = UIKitView.Events> {
  /** Create a view with `frame`. Missing fields are 0. */
  constructor(frame?: Partial<UIKitView.Rect>)

  static readonly USER_INTERFACE_STYLE: {
    readonly UNSPECIFIED: number
    readonly LIGHT: number
    readonly DARK: number
  }

  static readonly CONTENT_MODE: {
    readonly SCALE_TO_FILL: number
    readonly SCALE_ASPECT_FIT: number
    readonly SCALE_ASPECT_FILL: number
    readonly REDRAW: number
    readonly CENTER: number
    readonly TOP: number
    readonly BOTTOM: number
    readonly LEFT: number
    readonly RIGHT: number
    readonly TOP_LEFT: number
    readonly TOP_RIGHT: number
    readonly BOTTOM_LEFT: number
    readonly BOTTOM_RIGHT: number
  }
}

declare namespace UIKitView {
  export interface Rect {
    x: number
    y: number
    width: number
    height: number
  }

  export interface Insets {
    top: number
    left: number
    bottom: number
    right: number
  }

  /** A touch. `pointerId` tells fingers apart. */
  export interface Touch {
    x: number
    y: number
    pointerId: number
  }

  export interface Events {
    touchesBegan: [touch: Touch]
    touchesMoved: [touch: Touch]
    touchesEnded: [touch: Touch]
    touchesCancelled: [touch: Touch]

    /** A trait changed, such as light or dark mode. */
    traitChange: []
  }
}

export = UIKitView
