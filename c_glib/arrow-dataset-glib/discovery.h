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

#include <arrow-glib/arrow-glib.h>

#include <arrow-dataset-glib/partition.h>

G_BEGIN_DECLS

/* arrow::dataset::DatasetFactory */

#define GAD_TYPE_DATASET_FACTORY (gad_dataset_factory_get_type())
G_DECLARE_DERIVABLE_TYPE(GADDatasetFactory,
                         gad_dataset_factory,
                         GAD,
                         DATASET_FACTORY,
                         GObject)
struct _GADDatasetFactoryClass
{
  GObjectClass parent_class;
};

GArrowSchema  *gad_dataset_factory_inspect_schema    (GADDatasetFactory *factory,
                                                      GError **error);
GArrowSchema  *gad_dataset_factory_inspect           (GADDatasetFactory *factory,
                                                      GError **error);
GADDataSource *gad_dataset_factory_finish            (GADDatasetFactory *factory,
                                                      GArrowSchema *schema,
                                                      GError **error);
GADExpression *gad_dataset_factory_get_root_partition(GADDatasetFactory *factory);
void           gad_dataset_factory_set_root_partition(GADDatasetFactory *factory,
                                                      GADExpression *root_partition,
                                                      GError **error);

/* arrow::dataset::UnionDatasetFactory */

#define GAD_TYPE_UNION_DATASET_FACTORY (gad_union_dataset_factory_get_type())
G_DECLARE_DERIVABLE_TYPE(GADUnionDatasetFactory,
                         gad_union_dataset_factory,
                         GAD,
                         UNION_DATASET_FACTORY,
                         GObject)
struct _GADUnionDatasetFactoryClass
{
  GADDatasetFactoryClass parent_class;
};

GADUnionDatasetFactory *gad_union_dataset_factory_new(GADDatasetFactory **factories,
                                                      gsize n_factories,
                                                      GError **error);
GADDatasetFactory     **gad_union_dataset_factory_get_factories(GADUnionDatasetFactory *factory,
                                                                gsize *n_factories);

/* arrow::dataset::FileSystemFactoryOptions */

#define GAD_TYPE_FILE_SYSTEM_FACTORY_OPTIONS (gad_file_system_factory_options_get_type())
G_DECLARE_DERIVABLE_TYPE(GADFileSystemFactoryOptions,
                         gad_file_system_factory_options,
                         GAD,
                         FILE_SYSTEM_FACTORY_OPTIONS,
                         GObject)
struct _GADFileSystemFactoryOptions
{
  GObjectClass parent_class;
};

GADFileSystemFactoryOptions *
gad_file_system_factory_options_new                      (void);
gchar *
gad_file_system_factory_options_get_partition_base_dir   (GADFileSystemFactoryOptions *options);
void
gad_file_system_factory_options_set_partition_base_dir   (GADFileSystemFactoryOptions *options,
                                                          const gchar *partition_base_dir);

gboolean
gad_file_system_factory_options_get_exclude_invalid_files(GADFileSystemFactoryOptions *options);
void
gad_file_system_factory_options_set_exclude_invalid_files(GADFileSystemFactoryOptions *options,
                                                          gboolean exclude_invalid_files);

gchar **
gad_file_system_factory_options_get_ignore_prefixes      (GADFileSystemFactoryOptions *options,
                                                          gsize *n_ignore_prefixes);
void
gad_file_system_factory_options_set_ignore_prefixes      (GADFileSystemFactoryOptions *options,
                                                          const gchar **ignore_prefixes,
                                                          gsize n_ignore_prefixes);

/* arrow::dataset::FileSystemDatasetFactory */

#define GAD_TYPE_FILE_SYSTEM_DATASET_DISCOVERY (gad_file_system_dataset_factory_get_type())
G_DECLARE_DERIVABLE_TYPE(GADFileSystemDatasetFactory,
                         gad_file_system_dataset_factory,
                         GAD,
                         FILE_SYSTEM_DATASET_DISCOVERY,
                         GADDatasetFactory)
struct _GADFileSystemDatasetFactoryClass
{
  GADDatasetFactoryClass parent_class;
};

GADFileSystemDatasetFactory *
gad_file_system_dataset_factory_new_paths   (GArrowFileSystem *filesystem,
                                             gchar **paths,
                                             gsize n_paths,
                                             GADFileFormat *format,
                                             GADFileSystemFactoryOptions *options);

GADFileSystemDatasetFactory *
gad_file_system_dataset_factory_new_selector(GArrowFileSystem *filesystem,
                                             GArrowFileSelector *selector,
                                             GADFileFormat *format,
                                             GADFileSystemFactoryOptions *options);

G_END_DECLS
