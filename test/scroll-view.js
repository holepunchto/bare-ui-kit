const { test } = require('bare-tap')
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
