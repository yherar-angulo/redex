/**
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <gtest/gtest.h>

#include "DexClass.h"
#include "DexLoader.h"
#include "IRCode.h"
#include "RedexTest.h"

using ResourceFiles = std::unordered_map<std::string, std::string>;
ResourceFiles decode_resource_paths(const char* location, const char* suffix);

struct PreVerify : public RedexTest {
  DexClasses classes;
  ResourceFiles resources;
  PreVerify()
      : classes(load_classes_from_dex(std::getenv("dex_pre"),
                                      /* balloon */ false)),
        resources(decode_resource_paths(std::getenv("extracted_resources"),
                                        "pre")) {}
};

struct PostVerify : public RedexTest {
  DexClasses classes;
  ResourceFiles resources;
  PostVerify()
      : classes(load_classes_from_dex(std::getenv("dex_post"),
                                      /* balloon */ false)),
        resources(decode_resource_paths(std::getenv("extracted_resources"),
                                        "post")) {}
};

DexClass* find_class_named(const DexClasses& classes, const char* name);
DexMethod* find_vmethod_named(const DexClass& cls, const char* name);
DexMethod* find_dmethod_named(const DexClass& cls, const char* name);
DexMethod* find_method_named(const DexClass& cls, const char* name);
/* Find the first invoke instruction that calls a particular method name */
DexOpcodeMethod* find_invoke(const DexMethod* m,
                             DexOpcode opcode,
                             const char* mname,
                             DexType* receiver = nullptr);
DexOpcodeMethod* find_invoke(std::vector<DexInstruction*>::iterator begin,
                             std::vector<DexInstruction*>::iterator end,
                             DexOpcode opcode,
                             const char* target_mname,
                             DexType* receiver = nullptr);
DexInstruction* find_instruction(DexMethod* m, DexOpcode opcode);

void verify_type_erased(const DexClass* cls, size_t num_dmethods = 0);
