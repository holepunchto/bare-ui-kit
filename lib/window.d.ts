import { tag, handle, Handle, Wrapper } from 'bare-foundation-registry'
import UIKitView = require('./view')
import UIKitScene = require('./scene')
import UIKitViewController = require('./view-controller')

/** A window, as a `UIWindow`. */
interface UIKitWindow extends UIKitView<UIKitWindow.Events> {
  get rootViewController(): UIKitViewController | null
  set rootViewController(viewController: Wrapper | null)

  get windowScene(): UIKitScene | null
  set windowScene(scene: Wrapper | null)

  /** Whether the window gets keyboard input. */
  readonly keyWindow: boolean

  makeKeyWindow(): this

  /** Show the window and make it get keyboard input. The window is kept alive from then on. */
  makeKeyAndVisible(): this
}

declare class UIKitWindow {
  /**
   * Create a window in `scene`. Without a frame, the window fills its scene. Give any frame field
   * to set the frame, and the missing ones are 0. A window without a scene is never shown.
   */
  constructor(opts?: {
    scene?: Wrapper | null
    x?: number
    y?: number
    width?: number
    height?: number
  })
}

declare namespace UIKitWindow {
  export interface Events {
    /** The keyboard is about to move to this frame, in screen coordinates. */
    keyboardWillChangeFrame: [frame: { x: number; y: number; width: number; height: number }]
  }
}

export = UIKitWindow
