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

/* arrow::dataset::SimpleScanTask */

#define GAD_TYPE_SIMPLE_SCAN_TASK (gad_simple_scan_task_get_type())
G_DECLARE_DERIVABLE_TYPE(GADSimpleScanTask,
                         gad_simple_scan_task,
                         GAD,
                         SIMPLE_SCAN_TASK,
                         GADScanTask)
struct _GADSimpleScanTaskClass
{
  GADScanTaskClass parent_class;
};

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

G_END_DECLS
