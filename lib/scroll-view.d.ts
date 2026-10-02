import UIKitView = require('./view')

/** A view that scrolls its content, as a `UIScrollView`. It does not report touches. */
interface UIKitScrollView<
  M extends Record<keyof M, unknown[]> = UIKitScrollView.Events
> extends UIKitView<M> {
  /** The size of what there is to scroll. A scroll view does not work it out from its subviews. */
  get contentSize(): { width: number; height: number }
  set contentSize(size: { width?: number; height?: number })

  /** How far the content is scrolled. */
  get contentOffset(): { x: number; y: number }
  set contentOffset(offset: { x?: number; y?: number })

  /** How the safe area insets the content, as a `CONTENT_INSET_ADJUSTMENT_BEHAVIOR` constant. */
  contentInsetAdjustmentBehavior: number

  /** The insets actually applied, safe area included. */
  readonly adjustedContentInset: UIKitView.Insets
}

declare class UIKitScrollView<M extends Record<keyof M, unknown[]> = UIKitScrollView.Events> {
  constructor(frame?: Partial<UIKitView.Rect>)

  static readonly CONTENT_INSET_ADJUSTMENT_BEHAVIOR: {
    readonly AUTOMATIC: number
    readonly SCROLLABLE_AXES: number
    readonly NEVER: number
    readonly ALWAYS: number
  }
}

declare namespace UIKitScrollView {
  export interface Events {
    didScroll: [offset: { x: number; y: number }]
  }
}

export = UIKitScrollView
