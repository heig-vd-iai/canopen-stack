#pragma once

namespace CANopen
{
    class TIME
    {
    private:
        bool enabled = false;
        class Node &node;

    public:
        TIME(class Node &node);
        void enable();
        void disable();
    };
}
