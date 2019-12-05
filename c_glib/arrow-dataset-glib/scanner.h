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

/* arrow::dataset::ScanContext */

#define GAD_TYPE_SCAN_CONTEXT (gad_scan_context_get_type())
G_DECLARE_DERIVABLE_TYPE(GADScanContext,
                         gad_scan_context,
                         GAD,
                         SCAN_CONTEXT,
                         GObject)
struct _GADScanContextClass
{
  GObjectClass parent_class;
};

GADScanContext *
gad_scan_context_new(void);

/* arrow::dataset::ScanOptions */

#define GAD_TYPE_SCAN_OPTIONS (gad_scan_options_get_type())
G_DECLARE_DERIVABLE_TYPE(GADScanOptions,
                         gad_scan_options,
                         GAD,
                         SCAN_OPTIONS,
                         GObject)
struct _GADScanOptionsClass
{
  GObjectClass parent_class;
};

GADScanOptions *
gad_scan_options_new(GArrowSchema *schema);

GArrowSchema *
gad_scan_options_get_schema(GArrowSchema *schema);

gchar **
gad_scan_options_materialized_fields(GArrowSchema *schema,
                                     gsize *n_strings);

/* arrow::dataset::ScanTask */

#define GAD_TYPE_SCAN_TASK (gad_scan_task_get_type())
G_DECLARE_DERIVABLE_TYPE(GADScanTask,
                         gad_scan_task,
                         GAD,
                         SCAN_TASK,
                         GObject)
struct _GADScanTaskClass
{
  GObjectClass parent_class;
};

GList *
gad_scan_task_execute(GADScanTask *task,
                      GError **error);

const GADScanOptions *
gad_scan_task_get_options(GADScanTask *task);

const GADScanContext *
gad_scan_task_get_context(GADScanTask *task);

/* arrow::dataset::InMemoryScanTask */

#define GAD_TYPE_IN_MEMORY_SCAN_TASK (gad_in_memory_scan_task_get_type())
G_DECLARE_DERIVABLE_TYPE(GADInMemoryScanTask,
                         gad_in_memory_scan_task,
                         GAD,
                         IN_MEMORY_SCAN_TASK,
                         GADScanTask)
struct _GADInMemoryScanTaskClass
{
  GADScanTaskClass parent_class;
};

GADInMemoryScanTask *
gad_in_memory_scan_task_new(GArrowRecordBatch **record_batches,
                            gsize n_record_batches,
                            GADScanOptions *options,
                            GADScanContext *context);

/* arrow::dataset::ScanTaskIteratorFromRecordBatch() */

GList *
gad_scan_tasks_from_record_batch(GArrowRecordBatch **record_batches,
                                 gsize n_record_batches,
                                 GADScanOptions *options,
                                 GADScanContext *context,
                                 GError **error);

/* arrow::dataset::Scanner */

#define GAD_TYPE_SCANNER (gad_scanner_get_type())
G_DECLARE_DERIVABLE_TYPE(GADScanner,
                         gad_scanner,
                         GAD,
                         SCANNER,
                         GObject)
struct _GADScannerClass
{
  GObject parent_class;
};

GADScanner *
gad_scanner_new(GADDataset *dataset,
                GADScanOptions *options,
                GADScanContext *context);

GList *
gad_scanner_scan(GADScanner *scanner,
                 GError **error);

GArrowTable *
gad_scanner_to_table(GADScanner *scanner,
                     GError **error);

GArrowSchema *
gad_scanner_get_schema(GADScanner *scanner);

/* arrow::dataset::ScannerBuilder */

#define GAD_TYPE_SCANNER_BUILDER (gad_scanner_builder_get_type())
G_DECLARE_DERIVABLE_TYPE(GADScannerBuilder,
                         gad_scanner_builder,
                         GAD,
                         SCANNER_BUILDER,
                         GObject)
struct _GADScannerBuilderClass
{
  GObject parent_class;
};

GADScannerBuilder *
gad_scanner_builder_new(GADDataset *dataset,
                        GADScanContext *context);

gboolean
gad_scanner_builer_project(GADScannerBuilder *builder,
                           gchar **columns,
                           gsize n_columns,
                           GError **error);

gboolean
gad_scanner_builder_filter(GADScannerBuilder *builder,
                           GADExpression *filter,
                           GError **error);

gboolean
gad_scanner_builder_use_threads(GADScannerBuilder *builder,
                                gboolean use_threads,
                                GError **error);
gboolean
gad_scanner_builder_batch_size(GADScannerBuilder *builder,
                               gint64 batch_size,
                               GError **error);

GADScanner *
gad_scanner_builder_finish(GADScannerBuilder *builder,
                           GError **error);

GArrowSchema *
gad_scanner_builder_get_schema(GADScannerBuilder *builder);

G_END_DECLS
