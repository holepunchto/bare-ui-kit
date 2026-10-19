const { test } = require('bare-tap')
const Pointer = require('bare-pointer')
const { ScrollView } = require('..')
const { open } = require('./helpers')

function scroll(t) {
  const { controller } = open(t)

  const scrollView = new ScrollView({ width: 200, height: 100 })
  scrollView.contentInsetAdjustmentBehavior = ScrollView.CONTENT_INSET_ADJUSTMENT_BEHAVIOR.NEVER
  scrollView.contentSize = { width: 200, height: 1000 }

  controller.view.addSubview(scrollView)

  return scrollView
}

test('sets the content size', (t) => {
  const scrollView = scroll(t)

  t.deepStrictEqual(scrollView.contentSize, { width: 200, height: 1000 })
})

test('emits when scrolled', (t) => {
  const scrollView = scroll(t)

  const offsets = []

  scrollView.on('didScroll', (offset) => offsets.push(offset))
  scrollView.contentOffset = { x: 0, y: 300 }

  t.deepStrictEqual(offsets, [{ x: 0, y: 300 }], 'offset')
  t.deepStrictEqual(scrollView.contentOffset, { x: 0, y: 300 }, 'content offset')
  t.equal(scrollView.bounds.y, 300, 'bounds')
})

test('scrolls a scroll view with the pointer', async (t) => {
  const { controller } = open(t)
  const root = controller.view
  const pointer = new Pointer()

  t.teardown(() => pointer.destroy())

  const scrollView = new ScrollView({ x: 40, y: 100, width: 200, height: 300 })

  scrollView.contentInsetAdjustmentBehavior = ScrollView.CONTENT_INSET_ADJUSTMENT_BEHAVIOR.NEVER
  scrollView.contentSize = { width: 200, height: 1000 }

  root.addSubview(scrollView)

  let scrolls = 0

  scrollView.on('didScroll', () => scrolls++)

  // The content moves with the touch, so the touch is placed relative to the root.
  await pointer.down(root, 140, 350)

  for (let y = 330; y >= 150; y -= 20) await pointer.move(root, 140, y)

  await pointer.up()

  t.ok(scrolls > 0, 'scrolled')
  t.ok(scrollView.contentOffset.y > 100, 'down the content')
})
