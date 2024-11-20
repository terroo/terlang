#pragma once

#include <any>
#include <vector>

#include "Expr.hpp"

class ArrayType {
  private:
    void insertAtIndex(int index, std::any value);

  public:
    std::vector<std::any> values;
    void append(std::any value);
    bool setAtIndex(int index, std::any value);
    std::any getEleAt(int index);
    int length();
};
