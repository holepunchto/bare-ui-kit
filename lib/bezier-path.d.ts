import { tag, handle, Handle, Wrapper } from 'bare-foundation-registry'

/** A path made of lines and curves, as a `UIBezierPath`. */
interface UIKitBezierPath {
  moveTo(x: number, y: number): this

  lineTo(x: number, y: number): this

  /** Add a curve to `x`, `y`, with the control points `x1`, `y1` and `x2`, `y2`. */
  curveTo(x: number, y: number, x1: number, y1: number, x2: number, y2: number): this

  /** Add an arc around `x`, `y`. The angles are in radians. */
  addArc(x: number, y: number, radius: number, startAngle: number, endAngle: number): this

  closePath(): this

  readonly [tag]: number

  readonly [handle]: Handle
}

declare class UIKitBezierPath {
  constructor()
}

export = UIKitBezierPath
