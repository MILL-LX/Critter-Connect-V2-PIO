#pragma once

class Action {
public:
    virtual ~Action() = default;

    // Pure virtual function, making Action an abstract class
    virtual void performAction() = 0;
};
