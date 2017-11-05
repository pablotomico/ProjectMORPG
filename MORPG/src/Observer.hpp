#pragma once

#include <functional>

#include "Message.hpp"

using Callback = std::function<void(Message)>;

class Observer {

};