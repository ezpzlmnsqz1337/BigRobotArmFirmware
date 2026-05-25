#include "MotionQueue.h"

bool MotionQueue::enqueue(const JointPositions& positions)
{
  if (isFull())
  {
    return false;
  }

  mItems[mTail] = positions;
  mTail = (mTail + 1) % MOTION_QUEUE_CAPACITY;
  mCount++;
  return true;
}

bool MotionQueue::dequeue(JointPositions& positions)
{
  if (isEmpty())
  {
    return false;
  }

  positions = mItems[mHead];
  mHead = (mHead + 1) % MOTION_QUEUE_CAPACITY;
  mCount--;
  return true;
}

bool MotionQueue::isEmpty() const
{
  return mCount == 0;
}

bool MotionQueue::isFull() const
{
  return mCount >= MOTION_QUEUE_CAPACITY;
}

uint8_t MotionQueue::size() const
{
  return mCount;
}

void MotionQueue::clear()
{
  mHead = 0;
  mTail = 0;
  mCount = 0;
}