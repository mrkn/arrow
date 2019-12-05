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

/* arrow::dataset::Partitioning */

#define GAD_TYPE_PARTITIONING (gad_partitioning_get_type())
G_DECLARE_DERIVABLE_TYPE(GADPartitioning,
                         gad_partitioning,
                         GAD,
                         PARTITIONING,
                         GObject)
struct _GADPartitioningClass
{
  GObjectClass parent_class;
};

GADPartitioning *
gad_partitioning_new(void);

gchar *
gad_partitioning_get_type_name(GADPartitioning *partitioning);

GADExpression *
gad_partitioning_parse_segment(GADPartitioning *partitioning,
                               const gchar *segment,
                               gint i,
                               GError **error);

GADExpression *
gad_partitioning_parse_path(GADPartitioning *partitioning,
                            const gchar *path,
                            GError **error);

GArrowSchema *
gad_partitioning_get_schema(GADPartitioning *partitioning);

/* arrow::dataset::PartitioningFactory */

#define GAD_TYPE_PARTITIONING_FACTORY (gad_partitioning_factory_get_type())
G_DECLARE_DERIVABLE_TYPE(GADPartitioningFactory,
                         gad_partitioning_factory,
                         GAD,
                         PARTITIONING_FACTORY,
                         GObject)
struct _GADPartitioningFactoryClass
{
  GObjectClass parent_class;
};

GArrowSchema *
gad_partitioning_factory_inspect(GADPartitioningFactory *factory,
                                 const gchar **paths,
                                 gsize n_paths,
                                 GError **error);
GADPartitioning *
gad_partitioning_factory_finish(GADPartitioningFactory *factory,
                                GArrowSchema *schema);

/* arrow::dataset::DefaultPartitioning */

#define GAD_TYPE_DEFAULT_PARTITIONING (gad_default_partitioning_get_type())
G_DECLARE_DERIVABLE_TYPE(GADDefaultPartitioning,
                         gad_default_partitioning,
                         GAD,
                         DEFAULT_PARTITIONING,
                         GADPartitioning)
struct _GADDefaultPartitioningClass
{
  GADPartitioningClass parent_class;
};

GADDefaultPartitioning *
gad_default_partitioning_new(void);

/* arrow::dataset::SegmentDictionaryPartitioning */

#define GAD_TYPE_SEGMENT_DICTIONARY_PARTITIONING (gad_segment_dictionary_partitioning_get_type())
G_DECLARE_DERIVABLE_TYPE(GADSegmentDictionaryPartitioning,
                         gad_segment_dictionary_partitioning,
                         GAD,
                         SEGMENT_DICTIONARY_PARTITIONING,
                         GADPartitioning)
struct _GADSegmentDictionaryPartitioningClass
{
  GADPartitioningClass parent_class;
};

GADSegmentDictionaryPartitioning *
gad_segment_dictionary_partitioning_new(GArrowSchema *schema,
                                        GHashTable **dictionaries,
                                        gsize n_dictionaries);

/* arrow::dataset::KeyValuePartitioning */

#define GAD_TYPE_KEY_VALUE_PARTITIONING (gad_key_value_partitioning_get_type())
G_DECLARE_DERIVABLE_TYPE(GADKeyValuePartitioning,
                         gad_key_value_partitioning,
                         GAD,
                         KEY_VALUE_PARTITIONING,
                         GADPartitioning)
struct _GADKeyValuePartitioningClass
{
  GADPartitioningClass parent_class;
};

GADExpression *
gad_key_value_partitioning_convert_key(GVariant *key,
                                       GArrowSchema *schema);

GVariant *
gad_key_value_partitioning_parse_key(GADKeyValuePartitioning *partitioning,
                                     const gchar *segment,
                                     gint i);

/* arrow::dataset::DirectoryPartitioning */

#define GAD_TYPE_DIRECTORY_PARTITIONING (gad_directory_partitioning_get_type())
G_DECLARE_DERIVABLE_TYPE(GADDirectoryPartitioning,
                         gad_directory_partitioning,
                         GAD,
                         DIRECTORY_PARTITIONING,
                         GADKeyValuePartitioning)
struct _GADDirectoryPartitioningClass
{
  GADKeyValuePartitioningClass parent_class;
};

GADDirectoryPartitioning *
gad_directory_partitioning_new(GArrowSchema *schema);

GADPartitioningFactory *
gad_directory_partitioning_make_factory(const gchar *field_names,
                                        gsize n_field_names);

/* arrow::dataset::HivePartitioning */

#define GAD_TYPE_HIVE_PARTITIONING (gad_hive_partitioning_get_type())
G_DECLARE_DERIVABLE_TYPE(GADHivePartitioning,
                         gad_hive_partitioning,
                         GAD,
                         HIVE_PARTITIONING,
                         GADKeyValuePartitioning)
struct _GADHivePartitioningClass
{
  GADKeyValuePartitioningClass parent_class;
};

GADHivePartitioning *
gad_hive_partitioning_new(GArrowSchema *schema);

GADPartitioningFactory *
gad_hive_partitioning_make_factory(void);

/* arrow::dataset::FunctionPartitioning */

#define GAD_TYPE_FUNCTION_PARTITIONING (gad_function_partitioning_get_type())
G_DECLARE_DERIVABLE_TYPE(GADFunctionPartitioning,
                         gad_function_partitioning,
                         GAD,
                         FUNCTION_PARTITIONING,
                         GADPartitioning)
struct _GADFunctionPartitioningClass
{
  GADPartitioningClass parent_class;
};

typedef GADExpression *(* GADFunctionPartitioningFunction)(const gchar *, gint, GError **);

GADFunctionPartitioning *
gad_function_partitioning_new(GArrowSchema *schema,
                              GADFunctionPartitioningFunction *function,
                              const gchar *name);

G_END_DECLS
