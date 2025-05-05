#include <any>
#include <cmath>
#include <string>

class FormatString {
  // Boundaries to start showing numbers in scientific notation
  static constexpr double sciLoweround = 0.001;
  static constexpr double sciUpperBound = 18446744073709551616.0;

  private:
    static std::string formatDouble(double value);

  public:
    static std::string stringify(std::any object);
};
