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

#pragma once

#include "arrow/data_frame/visibility.h"
#include "arrow/dataset/dataset.h"
#include "arrow/record_batch.h"
#include "arrow/table.h"

namespace arrow {
namespace data_frame {

class ARROW_DF_EXPORT DataFrame {
 public:
  static Status FromRecordBatches(
      const std::vector<std::shared_ptr<RecordBatch>>& batches,
      std::shared_ptr<DataFrame>* data_frame);

  static Status FromRecordBatches(
      const std::shared_ptr<Schema>& schema,
      const std::vector<std::shared_ptr<RecordBatch>>& batches,
      std::shared_ptr<DataFrame>* data_frame);

  static Status FromTable(
      const std::shared_ptr<Table>& table,
      std::shared_ptr<DataFrame>* data_frame);

  static Status FromTable(
      const std::shared_ptr<Schema>& schema,
      const std::shared_ptr<Table>& table,
      std::shared_ptr<DataFrame>* data_frame);

  // TODO
  // static Status FromDataset(
  //     const std::shared_ptr<dataset::Dataset>& dataset,
  //     std::shared_ptr<DataFrame>* data_frame);

  // TODO
  // static Status FromDataset(
  //     const std::shared_ptr<Schema>& schema,
  //     const std::shared_ptr<dataset::Dataset>& dataset,
  //     std::shared_ptr<DataFrame>* data_frame);

 protected:
  Table();

  std::shared_ptr<Schema> schema_;
  int64_t num_rows_;

 private:
  ARROW_DISALLOW_COPY_AND_ASSIGN(Table);
};

}
}
