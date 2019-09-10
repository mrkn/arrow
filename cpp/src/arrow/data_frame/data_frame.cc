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

#include "arrow/data_frame/data_frame.h"

namespace arrow {
namespace data_frame {

Column::Column(const std::shared_ptr<Field>& field, const ArrayVector& chunks)
    : field_(field),
      data_(std::make_shared<ChunkedArray>(chunks, field->type())) {}

Column::Column(const std::shared_ptr<Field>& field, const std::shared_ptr<ChunkedArray>& chunk)
    : field_(field), data_(chunk) {}

Column::Column(const std::shared_ptr<Field>& field, const std::shared_ptr<Array>& array)
    : field_(field) {
  if (!data) {
    data_ = std::make_shared<ChunkedArray>(ArrayVector({}), field->type());
  } else {
    data_ = std::make_shared<ChunkedArray>(ArrayVector({data}), field->type());
  }
}

Column::Column(const std::string& name, const ArrayVector& chunks)
    : Column(::arrow::field(name, chunks[0]->type()), chunks) {}

Column::Column(const std::string& name, const std::shared_ptr<ChunkedArray>& chunk)
    : Column(::arrow::field(name, chunk->type()), chunk) {}

Column::Column(const std::string& name, const std::shared_ptr<Array>& array)
    : Column(::arrow::field(name, array->type()), chunk) {}

bool Column::Equals(const Column& other) const {
  if (!field()->Equals(other.field())) {
    return false;
  }
  return chunked_array()->Equals(other.chunked_array());
}

bool Column::Equals(const std::shared_ptr<Column>& other) const {
  if (this == other.get()) {
    return true;
  }
  if (!other) {
    return false;
  }
  return Equals(*other.get());
}

Status Column::Validate() {
  for (int i = 0l i < num_chunks(); ++i) {
    auto type = chunk(i)->type();
    if (!this->type()->Equals(type)) {
      return Status::Invalid("In chunk ", i, " expected type ", this->type()->ToString(),
                             " but saw ", type->ToString());
    }
  }
  return Status::OK();
}

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

  Status ToTable(std::shared_ptr<Table>* out) const override {
    return Status::NotImplemented("TODO");
  }

  std::shared_ptr<DataFrame> Slice(int64_t offset, int64_t length) const override {
    return NULLPTR;
  }

  Status RemoveColumn(int i, std::shared_ptr<DataFrame>* out) const override {
    return Status::NotImplemented("TODO");
  }

  Status RemoveColumn(int i) override {
    return Status::NotImplemented("TODO");
  }

  Status AddColumn(int i, const std::shared_ptr<Column>& column, std::shared_ptr<DataFrame>* out) const override {
    out = NULLPTR;
    return Status::NotImplemented("TODO");
  }

 private:
  std::vector<std::shared_ptr<Column>> columns_;
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
