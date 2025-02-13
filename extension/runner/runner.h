/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <executorch/runtime/executor/program.h>

namespace torch::executor {

class Runner {
 public:
  explicit Runner(
      std::unique_ptr<DataLoader> dataLoader,
      std::unique_ptr<MemoryAllocator> memoryAllocator);
  Runner(const Runner&) = delete;
  Runner& operator=(const Runner&) = delete;

  Error run(
      const std::string& methodName,
      const std::vector<EValue>& inputs,
      std::vector<EValue>& outputs);

 private:
  Error loadMethod(const std::string& methodName);

 private:
  struct MethodHolder {
    std::vector<std::vector<uint8_t>> plannedBuffers;
    std::vector<Span<uint8_t>> plannedSpans;
    std::unique_ptr<HierarchicalAllocator> plannedMemory;
    std::unique_ptr<MemoryManager> memoryManager;
    std::unique_ptr<Method> method;
  };

 private:
  std::unique_ptr<DataLoader> dataLoader_;
  std::unique_ptr<Program> program_;
  std::unique_ptr<MemoryAllocator> memoryAllocator_;
  std::unordered_map<std::string, MethodHolder> methods_;
};

} // namespace torch::executor
