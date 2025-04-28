#include "ArrayType.hpp"

void ArrayType::append(std::any value) {
  values.push_back(value);
}

std::any ArrayType::getEleAt(int index) {
  return values.at(static_cast<size_t>(index));
}

int ArrayType::length() {
  return static_cast<int>(values.size());
}

bool ArrayType::setAtIndex(int index, std::any value) {
  if(index == length()){
    values.insert(values.begin() + index, value);
  }else if(index < length() && index >= 0) {
    values[static_cast<size_t>(index)] = value;
  }else{
    return false;
  }
  return true;
}
