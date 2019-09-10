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

#include "arrow/dataset/dataset.h"

namespace arrow {
namespace data_frame {

class SimpleDataFrame : public DataFrame {
 public:
  SimpleDataFrame(const std::shared_ptr<Schema>& schema,
                  const std::vector<std::shared_ptr<Column>>& columns,
                  int64_t num_rows = -1)
    : columns_(columns)
  {
    schema_ = schema;
    if (num_rows < 0) {
      if (columns.size() == 0) {
        num_rows_ = 0;
      } else {
        num_rows_ = columns[0]->length();
      }
    } else {
      num_rows_ = num_rows;
    }
  }

 private:
  std::vector<std::shared_ptr<Column>> columns_;

  std::vector<std::shared_ptr<>> MakeColumns
};

std::shared_ptr<DataFrame> DataFrame::Make(
    const std::shared_ptr<Schema>& schema,
    const std::vector<std::shared_ptr<ChunkedArray>>& chunked_arrays,
    int64_t num_rows = -1) {
  std::vector<std::shared_ptr<Column>> columns;
  return std::make_shared<SimpleDataFrame>(schema, columns, num_rows);
}

}
}
