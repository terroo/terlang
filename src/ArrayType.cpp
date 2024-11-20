#include "ArrayType.hpp"

void ArrayType::append(std::any value) {
  values.push_back(value);
}

std::any ArrayType::getEleAt(int index) {
  return values.at(index);
}

int ArrayType::length() {
  return values.size();
}

bool ArrayType::setAtIndex(int index, std::any value) {
  if(index == length()){
    values.insert(values.begin() + index, value);
  }else if(index < length() && index >= 0) {
    values[index] = value;
  }else{
    return false;
  }
  return true;
}
