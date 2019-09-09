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

#include <memory>
#include <string>
#include <vector>

namespace arrow {
namespace data_frame {

/// \class DataFrame
class ARROW_DF_EXPORT DataFrame {
 public:
   virtual ~DataFrame() = default;

   /// \brief Construct a DataFrame from a schema and columns
   /// If columns is zero-length, the data frame's number of rows is zero
   ///
   /// \param[in] schema The data frame's schema (column types and names)
   /// \param[in] columns The data frame's columns as a vector of ChunkedArrays
   /// \param[in] num_rows The number of rows in the data frame, -1 (default) to infer from columns
   ///
   /// \return the resulting DataFrame
   static std::shared_ptr<DataFrame> Make(
       const std::shared_ptr<Schema>& schema,
       const std::vector<std::shared_ptr<ChunkedArray>>& columns,
       int64_t num_rows = -1);

   /// \brief Construct a DataFrame from a schema and arrays
   ///
   /// \param[in] schema The data frame's schema (column types and names)
   /// \param[in] arrays The data frame's columns as a vector of Arrays
   /// \param[in] num_rows The number of rows in the data frame, -1 (default) to infer from arrays
   ///
   /// \return the resulting DataFrame
   static std::shared_ptr<DataFrame> Make(
       const std::shared_ptr<Schema>& schema,
       const std::vector<std::shared_ptr<Array>>& arrays,
       int64_t num_rows = -1);

  /// \brief Construct a DataFrame from a RecordBatches, using the schema supplied by the first record batch
  static Status FromRecordBatches(
      const std::vector<std::shared_ptr<RecordBatch>>& batches,
      std::shared_ptr<DataFrame>* data_frame);

  /// \brief Construct a DataFrame from a RecordBatches, using the supplied schema.  There may be zero RecordBatches
  static Status FromRecordBatches(
      const std::shared_ptr<Schema>& schema,
      const std::vector<std::shared_ptr<RecordBatch>>& batches,
      std::shared_ptr<DataFrame>* data_frame);

  /// \brief Construct a DataFrame from a Table, using the schema supplied by the Table
  static Status FromTable(
      const std::shared_ptr<Table>& table,
      std::shared_ptr<DataFrame>* data_frame);

  /// \brief Construct a DataFrame from a Table, using the supplied schema
  static Status FromTable(
      const std::shared_ptr<Schema>& schema,
      const std::shared_ptr<Table>& table,
      std::shared_ptr<DataFrame>* data_frame);

  // TODO: Support creating from dataset
  // static Status FromDataset(
  //     const std::shared_ptr<dataset::Dataset>& dataset,
  //     std::shared_ptr<DataFrame>* data_frame);
  //
  // static Status FromDataset(
  //     const std::shared_ptr<Schema>& schema,
  //     const std::shared_ptr<dataset::Dataset>& dataset,
  //     std::shared_ptr<DataFrame>* data_frame);

  /// \brief Dump the content of the data frame into a new Table
  virtual Status ToTable(std::shared_ptr<Table>* table) const;

  /// Return the schema of the data frame
  std::shared_ptr<Schema> schema() const { return schema_; }

  /// \brief Return the number of columns in the data frame
  int num_columns() const { return schema()->num_fields(); }

  /// \brief Return the number of columns in the data frame
  int num_rows() const { return num_rows_; }

  /// Return a column's field by index
  std::shared_ptr<Field> field(int i) const { return schema_->field(i); }

  /// \brief Construct a zero-copy slice of the data frame with the indicated offset and length
  ///
  /// \param[in] offset the index of the first row in the constructed slice
  /// \param[in] length the number of rows of the slice. If there are not enough rows in the data frame, the length will be adjusted accordingly
  ///
  /// \return a new object wrapped in std::shared_ptr<DataFrame>
  virtual std::shared_ptr<DataFrame> Slice(int64_t offset, int64_t length) const = 0;

  /// \brief Slice from first row at offset until end of the data frame
  std::shared_ptr<DataFrame> Slice(int64_t offset) const { return Slice(offset, num_rows_); }

  /// \brief Return a column by name
  /// \param[in] name the field name
  /// \return an Array or null if no field was found
  std::shared_ptr<ChunkedArray> GetColumnByName(const std::string& name) const {
    auto i = schema()->GetFieldIndex(name);
    return i == -1 ? NULLPTR : column(i);
  }

  /// \brief Return names of all columns
  std::vector<std::string> ColumnNames() const;

  /// \brief Remove column from the data frame, producing a new DataFrame
  virtual Status RemoveColumn(int i, std::shared_ptr<DataFrame>* out) const = 0;

  /// \brief Remove column from the data frame by the inplace mutation
  virtual Status RemoveColumn(int i) const = 0;

  /// \brief Add column to the data frame, producing a new DataFrame
  virtual Status AddColumn(int i, const std::shared_ptr<Field>& field, const std::shared_ptr<ChunkedArray>& column, std::shared_ptr<DataFrame>* out) const = 0;

  /// \brief Add column to the data frame by the inplace mutation
  virtual Status AddColumn(int i, const std::shared_ptr<Field>& field, const std::shared_ptr<ChunkedArray>& column) = 0;

  /// \brief Set column to the data frame, producing a new DataFrame
  virtual Status SetColumn(int i, const std::shared_ptr<Field>& field, const std::shared_ptr<ChunkedArray>& column, std::shared_ptr<DataFrame>* out) const = 0;

  /// \brief Set column to the data frame by the inplace mutation
  virtual Status SetColumn(int i, const std::shared_ptr<Field>& field, const std::shared_ptr<ChunkedArray>& column) = 0;

  /// \brief Rename columns with the provided names, producing a new DataFrame
  virtual Status RenameColumns(const std::vector<std::string>& names, std::shared_ptr<DataFrame>* out) const;

  /// \brief Rename columns with the provided names by the inplace mutation
  virtual Status RenameColumns(const std::vector<std::string>& names);

  /// \brief Rename a column with the provided name, producing a new DataFrame
  virtual Status RenameColumn(int i, const std::string& name, std::shared_ptr<DataFrame>* out) const;

  /// \brief Rename a column with the provided name by the inplace mutation
  virtual Status RenameColumn(int i, const std::string& name);

  /// \brief Determine if two DataFrames are equal
  ///
  /// Two data frames can be equal only if they have equal schemas.
  /// However, they may be equal even if they have different chunkings.
  bool Equals(const DataFrame& other) const;

 protected:
  DataFrame();

  std::shared_ptr<Schema> schema_;
  int64_t num_rows_;

 private:
  ARROW_DISALLOW_COPY_AND_ASSIGN(DataFrame);
};

}
}
