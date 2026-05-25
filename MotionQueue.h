#ifndef MOTION_QUEUE_H
#define MOTION_QUEUE_H

#include <stdint.h>

#include "Structures.h"

const uint8_t MOTION_QUEUE_CAPACITY = 4;

class MotionQueue
{
private:
  JointPositions mItems[MOTION_QUEUE_CAPACITY];
  uint8_t mHead = 0;
  uint8_t mTail = 0;
  uint8_t mCount = 0;

public:
  bool enqueue(const JointPositions& positions);
  bool dequeue(JointPositions& positions);
  bool isEmpty() const;
  bool isFull() const;
  uint8_t size() const;
  void clear();
};

#endif