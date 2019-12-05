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

/* arrow::dataset::RecordBatchProjector */

#define GAD_TYPE_RECORD_BATCH_PROJECTOR (gad_record_batch_projector_get_type())
G_DECLARE_DERIVABLE_TYPE(GADRecordBatchProjector,
                         gad_record_batch_projector,
                         GAD,
                         RECORD_BATCH_PROJECTOR,
                         GObject)
struct _GADRecordBatchProjectorClass
{
  GObjectClass parent_class;
};

#define GAD_RECORD_BATCH_PROJECTOR_NO_MATCH (-1)

GADRecordBatchProjector *
gad_record_batch_projector_new(GArrowSchema *schema);

gboolean
gad_record_batch_projector_set_default_value(GADRecordBatchProjector *projector,
                                             gint index,
                                             GArrowScalar *scalar);

/* TODO: GArrowMemoryPool */
GArrowRecordBatch *
gad_record_batch_projector_project(GADRecordBatchProjector *projector,
                                   GArrowRecordBatch *record_batch,
                                   GArrowMemoryPool *pool);

GArrowSchema *
gad_record_batch_projector_get_schema(GADRecordBatchProjector *projector);

gboolean
gad_record_batch_projector_set_input_schema(GADRecordBatchProjector *projector,
                                            GArrowSchema *input_schema,
                                            GArrowMemoryPool *pool);

G_END_DECLS
