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

#include <arrow-dataset-glib/filter.h>
#include <arrow-dataset-glib/scanner.h>

G_BEGIN_DECLS

/* arrow::dataset::DataFragment */

#define GAD_TYPE_DATA_FRAGMENT (gad_data_fragment_get_type())
G_DECLARE_DERIVABLE_TYPE(GADDataFragment,
                         gad_data_fragment,
                         GAD,
                         DATA_FRAGMENT,
                         GObject)
struct _GADDataFragmentClass
{
  GObjectClass parent_class;
};

gboolean  gad_data_fragment_splittable      (GADDataFragment *data_fragment);
GList     gad_data_fragment_get_scan_options(GADDataFragment *data_fragment);
GList    *gad_data_fragment_scan            (GADDataFragment *data_fragment,
                                             GADScanContext *scan_context);

/* arrow::dataset::SimpleDataFragment */

#define GAD_TYPE_SIMPLE_DATA_FRAGMENT (gad_simple_data_fragment_get_type())
G_DECLARE_DERIVABLE_TYPE(GADSimpleDataFragment,
                         gad_simple_data_fragment,
                         GAD,
                         SIMPLE_DATA_FRAGMENT,
                         GADDataFragment)
struct _GADSimpleDataFragmentClass
{
  GADDataFragmentClass parent_class;
};

GADSimpleDataFragment *gad_simple_data_fragment_new(GArrowRecordBatch **record_batches,
                                                    gsize n_record_batches);

/* arrow::dataset::DataSource */

#define GAD_TYPE_DATA_SOURCE (gad_data_source_get_type())
G_DECLARE_DERIVABLE_TYPE(GADDataSource,
                         gad_data_source,
                         GAD,
                         DATA_SOURCE,
                         GObject)
struct _GADDataSourceClass
{
  GObjectClass parent_class;
};

GList               *gad_data_source_get_fragments           (GADDataSource *data_source,
                                                              GADScanOptions *scan_options);
const GADExpression *gad_data_source_get_partition_expression(GADDataSource *data_source);
const gchar         *gad_data_source_get_type                (GADDataSource *data_source);

/* arrow::dataset::SimpleDataSource */

#define GAD_TYPE_SIMPLE_DATA_SOURCE (gad_simple_data_source_get_type())
G_DECLARE_DERIVABLE_TYPE(GADSimpleDataSource,
                         gad_simple_data_source,
                         GAD,
                         SIMPLE_DATA_SOURCE,
                         GADDataSource)
struct _GADSimpleDataSourceClass
{
  GADDataSourceClass parent_class;
};

GADSimpleDataSource *gad_simple_data_source_new(GADDataFragment **fragments,
                                                gsize n_fragments);

/* arrow::dataset::TreeDataSource */

#define GAD_TYPE_TREE_DATA_SOURCE (gad_tree_data_source_get_type())
G_DECLARE_DERIVABLE_TYPE(GADTreeDataSource,
                         gad_tree_data_source,
                         GAD,
                         TREE_DATA_SOURCE,
                         GADDataSource)
struct _GADTreeDataSourceClass
{
  GADDataSourceClass parent_class;
};

GADTreeDataSource *gad_tree_data_source_new(GADDataSource **children,
                                            gsize n_children);

/* arrow::dataset::Dataset */

#define GAD_TYPE_DATASET (gad_dataset_get_type())
G_DECLARE_DERIVABLE_TYPE(GADDataset,
                         gad_dataset,
                         GAD,
                         DATASET,
                         GObject)
struct _GADDatasetClass
{
  GObjectClass parent_class;
};

GADDataset           *gad_dataset_new        (GADDataSource **sources,
                                              gsize n_sources,
                                              GArrowSchema *schema);
GADScannerBuilder    *gad_dataset_new_scan   (GADDataset *dataset,
                                              GADScanContext *context);
const GADDataSource **gad_dataset_get_sources(GADDataset *dataset,
                                              gsize *n_sources);
GArrowSchema         *gad_dataset_get_schema (GADDataset *dataset);

G_END_DECLS
