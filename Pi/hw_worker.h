//
// Created by mmilord on 2/24/16.
//

#ifndef HW_WORKER_H
#define HW_WORKER_H

class hw_worker
{
public:
    void step_motor(int duration);
    bool ir_detect();
    bool motion_detect();
};

#endif //HW_WORKER_H
