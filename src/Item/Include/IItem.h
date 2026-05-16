#pragma once

#include <string>

namespace Items {
class IItem {
public:
  virtual ~IItem() = default;

  virtual void PreProcess() = 0;
  virtual void Process() = 0;
  virtual void PostProcess() = 0;
};
}; // namespace Items
