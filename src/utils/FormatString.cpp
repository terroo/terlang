#include <any>
#include <iomanip>
#include <cmath>
#include <sstream>

#include "FormatString.hpp"

#include "../interpreter/Function.hpp"
#include "../interpreter/Class.hpp"
#include "../interpreter/Instance.hpp"
#include "../interpreter/ArrayType.hpp"

std::string FormatString::formatDouble(double value) {
  std::ostringstream oss;
  std::string text;

  if (value == 0.0) {
    return "0";
  }

  if (std::abs(value) >= sciLoweround && std::abs(value) <= sciUpperBound) {
    oss << std::fixed << std::setprecision(6) << value; // same precision as before
    text = oss.str();

    // Remove trailing zeros after the decimal point
    size_t decimal_pos = text.find('.');
    if (decimal_pos != std::string::npos) {
      // Trim trailing zeros
      text.erase(text.find_last_not_of('0') + 1, std::string::npos);
      // Remove trailing decimal point
      if (text.back() == '.') {
        text.pop_back();
      }
    }

    return text;
  }

  // Use scientific notation for very small/large numbers
  oss << std::scientific << std::setprecision(15) << value;
  text = oss.str();
  // Remove trailing zeros in the exponent part (e.g., "1.230000e+05" to "1.23e+05")
  size_t e_pos = text.find('e');
  if (e_pos != std::string::npos) {
      std::string mantissa = text.substr(0, e_pos);
      mantissa.erase(mantissa.find_last_not_of('0') + 1, std::string::npos);
      if (mantissa.back() == '.') {
          mantissa.pop_back();
      }
      text = mantissa + text.substr(e_pos);
  }

  return text;
};

std::string FormatString::stringify(std::any object) {
  if(object.type() == typeid(nullptr)) return "nil";

  if(object.type() == typeid(double)){
    return formatDouble(std::any_cast<double>(object));
  }

  if (object.type() == typeid(std::string)) {
    return std::any_cast<std::string>(object);
  }

  if(object.type() == typeid(bool)){
    if(std::any_cast<bool>(object)){
      std::string result{"true"};
      return result;
    }else{
      std::string result{"false"};
      return result;
    }
  }

  if(object.type() == typeid(std::shared_ptr<Function>)){
    std::shared_ptr<Callable> function;
    function = std::any_cast<std::shared_ptr<Function>>(object);
    return function->toString();
  }

  if(object.type() == typeid(std::shared_ptr<Instance>)){
    auto instance = std::any_cast<std::shared_ptr<Instance>>(object);
    return instance->toString();
  }

  if(object.type() == typeid(std::shared_ptr<Callable>)){
    auto callable = std::any_cast<std::shared_ptr<Callable>>(object);
    return callable->toString();
  }

  if(object.type() == typeid(std::shared_ptr<Class>)){
    auto klass = std::any_cast<std::shared_ptr<Class>>(object);
    return klass->toString();
  }

  if(object.type() == typeid(std::shared_ptr<ArrayType>)){
    std::string result = "[";
    std::shared_ptr<ArrayType> list;
    list = std::any_cast<std::shared_ptr<ArrayType>>(object);
    auto values = list->values;
    for (auto i = values.begin(); i != values.end(); ++i) {
      auto next = i + 1;
      result.append(stringify(*i));
      if (next != values.end()) {
        result.append(", ");
      }
    }
    result.append("]");
    return result;
  }

  return "stringify: cannot recognize type";
};
