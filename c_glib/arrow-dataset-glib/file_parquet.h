/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#pragma once

#include <arrow-dataset-glib/file-base.h>

G_BEGIN_DECLS

/* arrow::dataset::ParquetFileFormat */

#define GAD_TYPE_PARQUET_FILE_FORMAT (gad_parquet_file_format_get_type())
G_DECLARE_DERIVABLE_TYPE(GADParquetFileFormat,
                         gad_parquet_file_format,
                         GAD,
                         PARQUET_FILE_FORMAT,
                         GADFileFormat)
struct _GADParquetFileFormatClass
{
  GADFileFormatClass parent_class;
};

GADParquetFileFormat *
gad_parquet_file_format_new(void);

/* TODO: GParquetReaderProperties */
GADParquetFileFormat *
gad_parquet_file_format_new_properties(GParquetReaderProperties *properties);

/* arrow::dataset::ParquetFragment */

#define GAD_TYPE_PARQUET_FRAGMENT (gad_parquet_fragment_get_type())
G_DECLARE_DERIVABLE_TYPE(GADParquetFragment,
                         gad_parquet_fragment,
                         GAD,
                         PARQUET_FRAGMENT,
                         GADFileFragment)
struct _GADParquetFragmentClass
{
  GADFileFragmentClass parent_class;
};

GADParquetFragment *
gad_parquet_fragment_new(GADFileSource *source,
                         GADParquetFileFormat *format,
                         GADScanOptions *options);

G_END_DECLS
