#pragma once
#include <iostream>
#include <vector>
#include <string>

class Helpers {
  private:
    int size;
    std::vector<std::string> args;
    Helpers() = default;

  public:
    static Helpers& get_instance() {
      static Helpers instance;
      return instance;
    }

    void set_args(const int& new_size, const std::vector<std::string>& new_args) {
      this->size = new_size;
      this->args = new_args;
    }

    std::vector<std::string> get_args(){
      return this->args;
    }

    Helpers(const Helpers&) = delete;
    void operator=(const Helpers&) = delete;
};

