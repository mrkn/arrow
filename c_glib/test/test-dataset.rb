# Licensed to the Apache Software Foundation (ASF) under one
# or more contributor license agreements.  See the NOTICE file
# distributed with this work for additional information
# regarding copyright ownership.  The ASF licenses this file
# to you under the Apache License, Version 2.0 (the
# "License"); you may not use this file except in compliance
# with the License.  You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing,
# software distributed under the License is distributed on an
# "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
# KIND, either express or implied.  See the License for the
# specific language governing permissions and limitations
# under the License.

class TestDataset < Test::Unit::TestCase
  def setup
    omit("Arrow Datasets is required") unless defined?(::GAD)
  end

  sub_test_case("end-to-end test") do
    def setup
      nullable = false
      @schema = Arrow::Schema.new([
        Arrow::Field.new("country", Arrow::StringDataType.new, nullable),
        Arrow::Field.new("region", Arrow::StringDataType.new, nullable),
        Arrow::Field.new("model", Arrow::StringDataType.new, nullable),
        Arrow::Field.new("year", Arrow::Int32DataType.new, nullable),
        Arrow::Field.new("sales", Arrow::Int32DataType.new, nullable),
      ])
    end

    files = {
      "/2018/01/US.json" => <<~END_JSON,
        [
          {"country": "US", "region": "NY", "year": 2018, "model": "3", "sales": 742.0},
          {"country": "US", "region": "NY", "year": 2018, "model": "S", "sales": 304.125},
          {"country": "US", "region": "NY", "year": 2018, "model": "X", "sales": 136.25},
          {"country": "US", "region": "NY", "year": 2018, "model": "Y", "sales": 27.5}
        ]
      END_JSON
      "/dataset/2018/01/CA.json" => <<~END_JSON,
        [
          {"country": "US", "region": "CA", "year": 2018, "model": "3", "sales": 512},
          {"country": "US", "region": "CA", "year": 2018, "model": "S", "sales": 978},
          {"country": "US", "region": "CA", "year": 2018, "model": "X", "sales": 1.0},
          {"country": "US", "region": "CA", "year": 2018, "model": "Y", "sales": 69}
        ]
      END_JSON
      "/dataset/2019/01/US.json" => <<~END_JSON,
        [
          {"country": "CA", "region": "QC", "year": 2019, "model": "3", "sales": 273.5},
          {"country": "CA", "region": "QC", "year": 2019, "model": "S", "sales": 13},
          {"country": "CA", "region": "QC", "year": 2019, "model": "X", "sales": 54},
          {"country": "CA", "region": "QC", "year": 2019, "model": "Y", "sales": 21}
        ]
      END_JSON
      "/dataset/2019/01/CA.json" => <<~END_JSON,
        [
          {"country": "CA", "region": "QC", "year": 2019, "model": "3", "sales": 152.25},
          {"country": "CA", "region": "QC", "year": 2019, "model": "S", "sales": 10},
          {"country": "CA", "region": "QC", "year": 2019, "model": "X", "sales": 42},
          {"country": "CA", "region": "QC", "year": 2019, "model": "Y", "sales": 37}
        ]
      END_JSON
      "/dataset/.pesky" => "garbage content"
    }

    @tmpdir = Dir.mktmpdir
    files.each do |path, json_content|
      full_path = File.join(@tmpdir, path)
      FileUtils.mkpath(File.dirname(full_path))
      File.write(full_path, json_content)
    end

    test do
      options = GAD::FileSystemDiscoveryOptions.new
      options.ignore_prefixes = ["."]

      discovery = GAD::FileSystemDataSourceDiscovery.new(
        GAD::LocalFileSystem.new
        selector,
        # FIXME: JSONRecordBatchFileFormat is test-specific API in Arrow C++ Datasets
        GAD::JSONRecordBatchFileFormat.new,
        options
      )

      inspected_schema = discovery.inspect_schema
      assert_equal(@schema, inspected_schema)

      partition_schema = Arrow::Schema.new([
        Arrow::Field.new("year", Arrow::Int32DataType.new),
        Arrow::Field.new("month", Arrow::Int32DataType.new),
        Arrow::Field.new("country", Arrow::StringDataType.new)
      ])

      partition_scheme = GAD::SchemaPartitionScheme.new(partition_schema)
      discovery.partition_scheme = partition_scheme

      data_source = discovery.finish
      dataset = GAD::Dataset.new([data_source], inspected_schema)

      scanner_builder = dataset.new_scan

      columns = ["sales", "model"]
      scanner_builder.project(columns)

      # build the expression ("year"_ == 2019 && "sales"_ > 100.0)
      year_eq_2019 = GAD::ComparisonExpression.new(
        Arrow::Compare::EQUAL,
        GAD::FieldExpression.new("year"),
        GAD::ScalarExpression.new(Arrow::ScalarInt32.new(2019))
      )
      sales_gt_100 = GAD::ComparisonExpression.new(
        Arrow::Compare::GREATER,
        GAD::FieldExpression.new("sales"),
        GAD::ScalarExpression.new(Arrow::ScalarDouble.new(100.0))
      )
      filter = GAD::AndExpression.new(year_eq_2019, sales_gt_100)
      scanner_builder.filter(filter)

      scanner = scanner_builder.finish
      table = scanner.to_table

      expected_schema = Arrow::Schema.new([
        Arrow::Field("sales", Arrow::DoubleDataType.new),
        Arrow::Field("model", Arrow::StringDataType.new)
      ])
      expected_arrays = [
        build_double_array([152.25, 273.5]),
        build_string_array(["3", "3"])
      ]
      expected_table = Arrow::Table.new(expected_schema, expected_arrays)

      assert_equal(expected_table,
                   table.combine_chunks)
    end
  end
end
