#include "time.hpp"
#include "node.hpp"
using namespace CANopen;

TIME::TIME(Node &node) : node(node) {}

void TIME::enable() { enabled = true; }

void TIME::disable() { enabled = false; }
