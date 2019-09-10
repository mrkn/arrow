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

/// \class Column
class ARROW_DF_EXPORT Column {
 public:
  /// \brief Construct a column from a vector of arrays
  ///
  /// The chunks' data type must equal to field's one.
  Column(const std::shared_ptr<Field>& field, const ArrayVector& chunks);

  /// \brief Construct a column from a chunked array
  ///
  /// The chunked array's data type must equal to field's one.
  Column(const std::shared_ptr<Field>& field, const std::shared_ptr<ChunkedArray>& chunk);

  /// \brief Construct a column from a single array
  ///
  /// The chunked array's data type must equal to field's one.
  Column(const std::shared_ptr<Field>& field, const std::shared_ptr<Array>& array);

  /// \brief Construct a column from a name and a vector of arrays
  Column(const std::string& name, const ArrayVector& chunks);

  /// \brief Construct a column from a name and a chunked array
  Column(const std::string& name, const std::shared_ptr<ChunkedArray>& chunk);

  /// \brief Construct a column from a name and a single array
  Column(const std::string& name, const std::shared_ptr<Array>& array);

  /// \brief Return the column field
  std::shared_ptr<Field> field() const { return field_; }

  /// \brief Return the column name
  const std::string& name() const { return field()->name(); }

  /// \brief Return the column data type
  std::shared_ptr<DataType> type() const { return field()->type(); }

  /// \brief Return whether the column is nullable
  bool nullable() const { return field()->nullable(); }

  /// \brief Return the column chunked array
  std::shared_ptr<ChunkedArray> chunked_array() const { return data_; }

  /// \return the total length of the chunked array; computed on construction
  int64_t length() const { return chunked_array()->length(); }

  /// \return the total number of nulls among all chunks
  int64_t null_count() const { return chunked_array()->null_count(); }

  /// \return the total number of chunks in the column's chunked array
  int num_chunks() const { return chunked_array()->num_chunks(); }

  /// \return chunk a particular chunk from the column's chunked array
  std::shared_ptr<Array> chunk(int i) const { return chunked_array()->chunk(i); }

  /// \brief Construct a zero-copy slice of the column with the indicated
  /// offset and length
  ///
  /// \param[in] offset the position of the first element in the constructed
  /// slice
  /// \param[in] length the length of the slice. If there are not enough
  /// elements in the chunked array, the length will be adjusted accordingly
  ///
  /// \return a new object wrapped in std::shared_ptr<ChunkedArray>
  std::shared_ptr<Column> Slice(int64_t offset, int64_t length) const {
    return std::make_shared<Column>(field(), chunked_array()->Slice(offset, length));
  }

  /// \brief Slice from offset until end of the chunked array
  std::shared_ptr<Column> Slice(int64_t offset) const {
    return std::make_shared<Column>(field(), chunked_array()->Slice(offset));
  }

  /// \brief Determine if two columns are equal.
  ///
  /// Two columns can be equal only if they have equal datatypes.
  /// However, they may be equal even if they have different chunkings.
  bool Equals(const Column& other) const;

  /// \brief Determine if two columns are equal.
  bool Equals(const std::shared_ptr<Column>& other) const;

  /// \brief Verify that the column's array data is consistent with the passed
  /// field's metadata
  Status ValidateData() const;

 protected:
  std::shared_ptr<Field> field_;
  std::shared_ptr<ChunkedArray> data_;
};

/// \class DataFrame
class ARROW_DF_EXPORT DataFrame {
 public:
  virtual ~DataFrame() = default;

  /// \brief Construct a DataFrame from a schema and chunked arrays
  /// If chunked arrays is zero-length, the data frame's number of rows is zero
  ///
  /// \param[in] schema The data frame's schema (column types and names)
  /// \param[in] columns The data frame's columns as a vector of ChunkedArrays
  /// \param[out] out The resulting DataFrame
  static Status FromChunkedArray(
      const std::shared_ptr<Schema>& schema,
      const std::vector<std::shared_ptr<ChunkedArray>>& chunked_arrays,
      std::shared_ptr<DataFrame>* out);

  /// \brief Construct a DataFrame from a schema and chunked arrays
  /// If chunked arrays is zero-length, the data frame's number of rows is zero
  ///
  /// \param[in] schema The data frame's schema (column types and names)
  /// \param[in] columns The data frame's columns as a vector of ChunkedArrays
  /// \param[in] num_rows The number of rows in the data frame, -1 (default) to infer from columns
  /// \param[out] out The resulting DataFrame
  static Status FromChunkedArray(
      const std::shared_ptr<Schema>& schema,
      const std::vector<std::shared_ptr<ChunkedArray>>& chunked_arrays,
      int64_t num_rows, std::shared_ptr<DataFrame>* out);

  /// \brief Construct a DataFrame from a schema and arrays
  ///
  /// \param[in] schema The data frame's schema (column types and names)
  /// \param[in] arrays The data frame's columns as a vector of Arrays
  /// \param[out] out The resulting DataFrame
  static Status FromArrays(
      const std::shared_ptr<Schema>& schema,
      const std::vector<std::shared_ptr<Array>>& arrays,
      std::shared_ptr<DataFrame>* out);

  /// \brief Construct a DataFrame from a schema and arrays
  ///
  /// \param[in] schema The data frame's schema (column types and names)
  /// \param[in] arrays The data frame's columns as a vector of Arrays
  /// \param[in] num_rows The number of rows in the data frame, -1 (default) to infer from arrays
  /// \param[out] out The resulting DataFrame
  static Status FromArrays(
      const std::shared_ptr<Schema>& schema,
      const std::vector<std::shared_ptr<Array>>& arrays,
      int64_t num_rows, std::shared_ptr<DataFrame>* out);

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
  virtual Status ToTable(std::shared_ptr<Table>* out) const = 0;

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
  virtual Status AddColumn(int i, const std::shared_ptr<Column>& column,
                           std::shared_ptr<DataFrame>* out) const = 0;

  /// \brief Add column to the data frame, producing a new DataFrame
  virtual Status AddColumn(int i, const std::shared_ptr<Field>& field,
                           const std::shared_ptr<ChunkedArray>& chunk,
                           std::shared_ptr<DataFrame>* out) const {
    return AddColumn(i, std::make_shared<Column>(field, column), out);
  }

  /// \brief Add column to the data frame by the inplace mutation
  virtual Status AddColumn(int i, const std::shared_ptr<Column>& column) = 0;

  /// \brief Add column to the data frame by the inplace mutation
  virtual Status AddColumn(int i, const std::shared_ptr<Field>& field,
                           const std::shared_ptr<ChunkedArray>& chunk) {
    return AddColumn(i, std::make_shared<Column>(field, chunk));
  }

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
