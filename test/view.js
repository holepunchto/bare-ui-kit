const { test } = require('bare-tap')
const { afterAnimationFrame } = require('bare-animation-frame')
const { Color, View } = require('..')
const { open } = require('./helpers')

test('sets the frame and bounds', (t) => {
  const view = new View({ x: 10, y: 20, width: 30, height: 40 })

  t.deepStrictEqual(view.frame, { x: 10, y: 20, width: 30, height: 40 }, 'initial frame')
  t.deepStrictEqual(view.bounds, { x: 0, y: 0, width: 30, height: 40 }, 'initial bounds')

  view.frame = { x: 1, y: 2, width: 3, height: 4 }

  t.deepStrictEqual(view.frame, { x: 1, y: 2, width: 3, height: 4 }, 'frame')

  view.bounds = { x: 5, y: 6, width: 3, height: 4 }

  t.deepStrictEqual(view.bounds, { x: 5, y: 6, width: 3, height: 4 }, 'bounds')
  t.deepStrictEqual(view.frame, { x: 1, y: 2, width: 3, height: 4 }, 'frame unchanged')
})

test('builds a hierarchy', (t) => {
  const parent = new View()
  const a = new View()
  const b = new View()

  t.equal(a.superview, null, 'no superview')
  t.deepStrictEqual(parent.subviews, [], 'no subviews')

  parent.addSubview(a)
  parent.addSubview(b)

  t.ok(a.superview === parent, 'same superview wrapper')
  t.equal(parent.subviews.length, 2, 'two subviews')
  t.ok(parent.subviews[0] === a, 'first subview')
  t.ok(parent.subviews[1] === b, 'second subview')

  a.removeFromSuperview()

  t.equal(a.superview, null, 'removed')
  t.equal(parent.subviews.length, 1, 'one subview')
  t.ok(parent.subviews[0] === b, 'remaining subview')
})

test('orders subviews', (t) => {
  const parent = new View()
  const a = new View()
  const b = new View()
  const c = new View()
  const d = new View()

  parent.addSubview(a)
  parent.insertSubviewAtIndex(b, 0)
  parent.insertSubviewAboveSubview(c, b)
  parent.insertSubviewBelowSubview(d, b)

  const subviews = parent.subviews

  t.ok(subviews[0] === d, 'below')
  t.ok(subviews[1] === b, 'at index')
  t.ok(subviews[2] === c, 'above')
  t.ok(subviews[3] === a, 'added')
})

test('moves a view to another superview', (t) => {
  const first = new View()
  const second = new View()
  const view = new View()

  first.addSubview(view)
  second.addSubview(view)

  t.ok(view.superview === second, 'new superview')
  t.deepStrictEqual(first.subviews, [], 'left the old superview')
  t.equal(second.subviews.length, 1, 'joined the new superview')
})

test('converts points between views', (t) => {
  const parent = new View({ width: 200, height: 200 })
  const child = new View({ x: 30, y: 40, width: 50, height: 50 })

  parent.addSubview(child)

  t.deepStrictEqual(child.convertPointToView(5, 5, parent), { x: 35, y: 45 }, 'to the parent')
  t.deepStrictEqual(child.convertPointFromView(35, 45, parent), { x: 5, y: 5 }, 'from the parent')
})

test('sizes a view to fit', (t) => {
  const view = new View({ width: 30, height: 40 })

  t.deepStrictEqual(view.sizeThatFits(100, 100), { width: 30, height: 40 })
})

test('sets the appearance', (t) => {
  const view = new View()

  view.hidden = true
  view.alpha = 0.5
  view.clipsToBounds = true

  t.equal(view.hidden, true, 'hidden')
  t.equal(view.alpha, 0.5, 'alpha')
  t.equal(view.clipsToBounds, true, 'clips')

  const color = Color.systemRedColor

  view.backgroundColor = color

  t.ok(view.backgroundColor === color, 'same color wrapper')

  view.backgroundColor = null

  t.equal(view.backgroundColor, null, 'no color')
})

test('overrides the interface style', async (t) => {
  const { controller } = open(t)
  const view = controller.view

  view.overrideUserInterfaceStyle = View.USER_INTERFACE_STYLE.DARK

  t.equal(view.overrideUserInterfaceStyle, View.USER_INTERFACE_STYLE.DARK, 'override')

  await afterAnimationFrame()

  t.equal(view.userInterfaceStyle, View.USER_INTERFACE_STYLE.DARK, 'effective')
})
