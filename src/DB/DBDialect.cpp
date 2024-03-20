// Licensed to the Apache Software Foundation (ASF) under one
// or more contributor license agreements.  See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  The ASF licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.

#include "mlir/IR/Builders.h"
#include "mlir/IR/OpImplementation.h"

#include "DB/DBDialect.h"
#include "DB/DBOps.h"

using namespace mlir;
using namespace basic;

//===----------------------------------------------------------------------===//
// DB dialect.
//===----------------------------------------------------------------------===//

#include "DB/DBOpsDialect.cpp.inc"

void DBDialect::initialize() {
  addOperations<
#define GET_OP_LIST
#include "DB/DBOps.cpp.inc"
      >();
}

// void basic::ConstantOp::build(::mlir::OpBuilder &odsBuilder, ::mlir::OperationState &., ::mlir::Type i64_result, uint64_t i64_val)
// {
//  auto dataType = builder.getIntegerType(64);
//  basic::ConstantOp
// }
// void basic::ConstantOp::build(mlir::OpBuilder &builder,
//                              mlir::OperationState &state, int value) {
//  // convert int value to mlir::Value

//   auto dataType = builder.getIntegerType(64);
//  basic::ConstantOp::build(builder, state, dataType, value);
// }

// void basic::ConstantOp::build(::mlir::OpBuilder &builder, ::mlir::OperationState &state, int value = 0) {
// //       state.addAttribute("i64_val", builder.getI64IntegerAttr(value));
// // }

mlir::Operation *DBDialect::materializeConstant(mlir::OpBuilder &builder,
                                                   mlir::Attribute value,
                                                   mlir::Type type,
                                                   mlir::Location loc) {
  return builder.create<db::ConstantOp>(
      loc, type, value.cast<mlir::IntegerAttr>());
}