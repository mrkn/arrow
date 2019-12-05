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

/* arrow::dataset::Fragment */

#define GAD_TYPE_FRAGMENT (gad_fragment_get_type())
G_DECLARE_DERIVABLE_TYPE(GADFragment,
                         gad_fragment,
                         GAD,
                         FRAGMENT,
                         GObject)
struct _GADFragmentClass
{
  GObjectClass parent_class;
};

gboolean  gad_fragment_splittable      (GADFragment *data_fragment);
GList     gad_fragment_get_scan_options(GADFragment *data_fragment);
GList    *gad_fragment_scan            (GADFragment *data_fragment,
                                        GADScanContext *scan_context);

/* arrow::dataset::InMemoryFragment */

#define GAD_TYPE_IN_MEMORY_FRAGMENT (gad_in_memory_fragment_get_type())
G_DECLARE_DERIVABLE_TYPE(GADInMemoryFragment,
                         gad_in_memory_fragment,
                         GAD,
                         IN_MEMORY_FRAGMENT,
                         GADFragment)
struct _GADInMemoryFragmentClass
{
  GADFragmentClass parent_class;
};

GADInMemoryFragment *gad_in_memory_fragment_new(GArrowRecordBatch **record_batches,
                                                gsize n_record_batches);

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

GADDataset           *gad_dataset_new          (GADDataSource **sources,
                                                gsize n_sources,
                                                GArrowSchema *schema);
GADScannerBuilder    *gad_dataset_new_scan     (GADDataset *dataset,
                                                GADScanContext *context);
GList                *gad_dataset_get_fragments(GADDataset *dataset,
                                                GADScanOptions *scan_options);
GArrowSchema         *gad_dataset_get_schema   (GADDataset *dataset);
const GADExpression  *gad_dataset_get_partition_expression(GADDataset *dataset);
const gchar          *gad_dataset_get_type_name(GADDataset *dataset);

/* arrow::dataset::InMemoryDataset */

#define GAD_TYPE_IN_MEMORY_DATASET (gad_in_memory_dataset_get_type())
G_DECLARE_DERIVABLE_TYPE(GADInMemoryDataset,
                         gad_in_memory_dataset,
                         GAD,
                         IN_MEMORY_DATASET,
                         GADDataset)
struct _GADInMemoryDatasetClass
{
  GADDatasetClass parent_class;
};

GADInMemoryDataset *gad_in_memory_dataset_new_batches(GArrowSchema *schema,
                                                      GArrowRecordBatch **record_batches,
                                                      gsize n_batches);
GADInMemoryDataset *gad_in_memory_dataset_new_table  (GArrowSchema *schema,
                                                      GArrowTable *table);

/* arrow::dataset::UnionDataset */

#define GAD_TYPE_UNION_DATASET (gad_union_dataset_get_type())
G_DECLARE_DERIVABLE_TYPE(GADUnionDataset,
                         gad_union_dataset,
                         GAD,
                         UNION_DATASET,
                         GADDataset)
struct _GADUnionDatasetClass
{
  GADDatasetClass parent_class;
};

GADUnionDataset *gad_union_dataset_new     (GArrowSchema *schema,
                                            GADDataset **children,
                                            gsize n_children);
GADDataset **gad_union_dataset_get_children(GADUnionDataset *union_dataset,
                                            gsize *n_children);

G_END_DECLS
