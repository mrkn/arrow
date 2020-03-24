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

G_BEGIN_DECLS

/* arrow::dataset::FileSource */

#define GAD_TYPE_FILE_SOURCE (gad_file_source_get_type())
G_DECLARE_DERIVABLE_TYPE(GADFileSource,
                         gad_file_source,
                         GAD,
                         FILE_SOURCE,
                         GObject)
struct _GADFileSourceClass
{
  GObjectClass parent_class;
};

GADFileSource *
gad_file_source_new_path(const gchar *path,
                         GArrowFileSystem *filesystem,
                         GArrowCompressionType compression);

GADFileSource *
gad_file_source_new_buffer(GArrowBuffer *buffer,
                           GArrowCompressionType compression);

gboolean
gad_file_source_equal(GADFileSource *file_source,
                      GADFileSource *other_file_source);

GADFileSourceType
gad_file_source_get_type(GADFileSource *file_source);

GArrowCompressionType
gad_file_source_get_compression_type(GADFileSource *file_source);

const gchar *
gad_file_source_get_path(GADFileSource *file_source);

GArrowFileSystem *
gad_file_source_get_filesystem(GADFileSource *file_source);

GArrowBuffer *
gad_file_source_get_buffer(GADFileSource *file_source);

GArrowSeekableInputStream *
gad_file_source_open(GADFileSource *file_source,
                     GError **error);

/* arrow::dataset::FileFormat */

#define GAD_TYPE_FILE_FORMAT (gad_file_format_get_type())
G_DECLARE_DERIVABLE_TYPE(GADFileFormat,
                         gad_file_format,
                         GAD,
                         FILE_FORMAT,
                         GObject)
struct _GADFileFormatClass
{
  GObjectClass parent_class;
};

gchar *gad_file_format_get_type_name(GADFileFormat *format);

gboolean gad_file_format_is_supported(GADFileFormat *format,
                                      GADFileSource *source,
                                      GError **error);

GArrowSchema *gad_file_format_inspect(GADFileFormat *format,
                                      GADFileSource *source,
                                      GError **error);

GList *gad_file_format_scan_file(GADFileFormat *format,
                                 GADFileSource *source,
                                 GADScanOptions *options,
                                 GADScanContext *context,
                                 GError **error);

GADFragment *gad_file_format_make_fragment(GADFileFormat *format,
                                           GADFileSource *location,
                                           GADScanOptions *options,
                                           GError **error);

/* arrow::dataset::FileFragment */

#define GAD_TYPE_FILE_FRAGMENT (gad_file_fragment_get_type())
G_DECLARE_DERIVABLE_TYPE(GADFileFragment,
                         gad_file_fragment,
                         GAD,
                         FILE_FRAGMENT,
                         GADFragment)
struct _GADFileFragmentClass
{
  GADFragmentClass parent_class;
};

GADFileFragment *gad_file_fragment_new(GADFileSource *source,
                                       GADFileFormat *format,
                                       GADScanOptions *options);
const GADFileSource *gad_file_fragment_get_source(GADFileFragment *fragment);
const GADFileFormat *gad_file_fragment_get_format(GADFileFragment *fragment);

/* arrow::dataset::FileSystemDataset */

#define GAD_TYPE_FILE_SYSTEM_DATASET (gad_file_system_dataset_get_type())
G_DECLARE_DERIVABLE_TYPE(GADFileSystemDataset,
                         gad_file_system_dataset,
                         GAD,
                         FILE_SYSTEM_DATASET,
                         GADDataset)
struct _GADFileSystemDatasetClass
{
  GADDatasetClass parent_class;
};

GADFileSystemDataset *
gad_file_system_dataset_new(GArrowSchema *schema,
                            GADExpression *root_partition,
                            GADFileFormat *format,
                            GArrowFileSystem *file_system,
                            GArrowFileInfo *infos);

GADFileSystemDataset *
gad_file_system_dataset_new_partitions(GArrowSchema *schema,
                                       GADExpression *root_partition,
                                       GADFileFormat *format,
                                       GArrowFileSystem *file_system,
                                       GArrowFileInfo *infos,
                                       GADExpression **partitions,
                                       gsize n_partitions);

GADFileFormat *
gad_file_system_dataset_get_format(GADFileSystemDataset *dataset);

gchar **
gad_file_system_dataset_get_files(GADFileSystemDataset *dataset,
                                  gsize *n_files);
gchar *
gad_file_system_dataset_to_string(GADFileSystemDataset *dataset);

G_END_DECLS
