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

/* arrow::dataset::IpcFileFormat */

#define GAD_TYPE_IPC_FILE_FORMAT (gad_parquet_ipc_format_get_type())
G_DECLARE_DERIVABLE_TYPE(GADIpcFileFormat,
                         gad_ipc_file_format,
                         GAD,
                         IPC_FILE_FORMAT,
                         GADFileFormat)
struct _GADIpcFileFormatClass
{
  GADFileFormatClass parent_class;
};

GADIpcFileFormat *gad_ipc_file_format_new(void);

/* arrow::dataset::IpcFragment */

#define GAD_TYPE_IPC_FRAGMENT (gad_ipc_fragment_get_type())
G_DECLARE_DERIVABLE_TYPE(GADIpcFragment,
                         gad_ipc_fragment,
                         GAD,
                         IPC_FRAGMENT,
                         GADFileFragment)
struct _GADIpcFragmentClass
{
  GADFileFragmentClass parent_class;
};

GADIpcFragment *gad_ipc_fragment_new(GADFileSource *source,
                                     GADScanOptions *options);


G_END_DECLS
