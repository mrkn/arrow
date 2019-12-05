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

/* arrow::dataset::DataSourceDiscovery */

#define GAD_TYPE_DATA_SOURCE_DISCOVERY (gad_data_source_discovery_get_type())
G_DECLARE_DERIVABLE_TYPE(GADDataSourceDiscovery,
                         gad_data_source_discovery,
                         GAD,
                         DATA_SOURCE_DISCOVERY,
                         GObject)
struct _GADDataSourceDiscoveryClass
{
  GObjectClass parent_class;
};

GArrowSchema  *gad_data_source_discovery_inspect_schema(GADDataSourceDiscovery *discovery,
                                                        GError **error);
GADDataSource *gad_data_source_discovery_finish        (GADDataSourceDiscovery *discovery,
                                                        GError **error);
GArrowSchema  *gad_data_source_discovery_get_schema    (GADDataSourceDiscovery *discovery);
void           gad_data_source_discovery_set_schema    (GADDataSourceDiscovery *discovery,
                                                        GArrowSchema *schema,
                                                        GError **error);

GADPartitionScheme *gad_data_source_discovery_get_partition_scheme(GADDataSourceDiscovery *discovery);
void                gad_data_source_discovery_set_partition_scheme(GADDataSourceDiscovery *discovery,
                                                                   GADPartitionScheme *partition_scheme,
                                                                   GError **error);

GADExpression *gad_data_source_discovery_get_root_partition(GADDataSourceDiscovery *discovery);
void           gad_data_source_discovery_set_root_partition(GADDataSourceDiscovery *discovery,
                                                            GADExpression *root_partition,
                                                            GError **error);

/* arrow::dataset::FileSystemDiscoveryOptions */

#define GAD_TYPE_FILE_SYSTEM_DISCOVERY_OPTIONS (gad_file_system_discovery_options_get_type())
G_DECLARE_DERIVABLE_TYPE(GADFileSystemDiscoveryOptions,
                         gad_file_system_discovery_options,
                         GAD,
                         FILE_SYSTEM_DISCOVERY_OPTIONS,
                         GObject)
struct _GADFileSystemDiscoveryOptions
{
  GObjectClass parent_class;
};

GADFileSystemDiscoveryOptions *
gad_file_system_discovery_options_new                      (void);
gchar *
gad_file_system_discovery_options_get_partition_base_dir   (GADFileSystemDiscoveryOptions *options);
void
gad_file_system_discovery_options_set_partition_base_dir   (GADFileSystemDiscoveryOptions *options,
                                                            const gchar *partition_base_dir);

gboolean
gad_file_system_discovery_options_get_exclude_invalid_files(GADFileSystemDiscoveryOptions *options);
void
gad_file_system_discovery_options_set_exclude_invalid_files(GADFileSystemDiscoveryOptions *options,
                                                            gboolean exclude_invalid_files);

gchar **
gad_file_system_discovery_options_get_ignore_prefixes      (GADFileSystemDiscoveryOptions *options);
void
gad_file_system_discovery_options_set_ignore_prefixes      (GADFileSystemDiscoveryOptions *options,
                                                            const gchar **ignore_prefixes,
                                                            gsize n_ignore_prefixes);

/* arrow::dataset::FileSystemDataSourceDiscovery */

#define GAD_TYPE_FILE_SYSTEM_DATA_SOURCE_DISCOVERY (gad_file_system_data_source_discovery_get_type())
G_DECLARE_DERIVABLE_TYPE(GADFileSystemDataSourceDiscovery,
                         gad_file_system_data_source_discovery,
                         GAD,
                         FILE_SYSTEM_DATA_SOURCE_DISCOVERY,
                         GADDataSourceDiscovery)
struct _GADFileSystemDataSourceDiscoveryClass
{
  GADDataSourceDiscoveryClass parent_class;
};

GADFileSystemDataSourceDiscovery *
gad_file_system_data_source_discovery_new_paths   (GArrowFileSystem *filesystem,
                                                   GList *paths,
                                                   GADFileFormat *format,
                                                   GADFileSystemDiscoveryOptions *options);

GADFileSystemDataSourceDiscovery *
gad_file_system_data_source_discovery_new_selector(GArrowFileSystem *filesystem,
                                                   GArrowFileSystemSelector *selector,
                                                   GADFileFormat *format,
                                                   GADFileSystemDiscoveryOptions *options);

G_END_DECLS
