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

GADFileSource *gad_file_source_new  (const gchar *path,
                                     GArrowFileSystem *filesystem,
                                     GArrowCompressionType compression);
GADFileSource *gad_file_source_new  (GArrowBuffer *buffer,
                                     GArrowCompressionType compression);

gboolean       gad_file_source_equal(GADFileSource *file_source,
                                     GADFileSource *other_file_source);

GADFileSourceType       gad_file_source_get_type            (GADFileSource *file_source);
GArrowCompressionType   gad_file_source_get_compression_type(GADFileSource *file_source);
const gchar            *gad_file_source_get_path            (GADFileSource *file_source);
GArrowFileSystem       *gad_file_source_get_filesystem      (GADFileSource *file_source);
GArrowBuffer           *gad_file_source_get_buffer          (GADFileSource *file_source);
GArrowRandomAccessFile *gad_file_source_open                (GADFileSource *file_source,
                                                             GError **error);

G_END_DECLS
