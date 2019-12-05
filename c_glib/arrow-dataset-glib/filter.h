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

struct GADInExpression;
struct GADCastExpression;

/* arrow::dataset::ExpressionType */

/**
 * GADExpressionType
 * @GAD_EXPRESSION_TYPE_FIELD:
 *   A reference to a column within a record batch,
 *   will evaluate to an array
 * @GAD_EXPRESSION_TYPE_SCALAR:
 *   A literal singular value encapsulated in a Scalar
 * @GAD_EXPRESSION_TYPE_NOT:
 *   An inversion of another expression
 * @GAD_EXPRESSION_TYPE_CAST:
 *   Cast an expression to a given DataType
 * @GAD_EXPRESSION_TYPE_AND:
 *   A conjunction of multiple expressions (true if all operands are true)
 * @GAD_EXPRESSION_TYPE_OR:
 *   A disjunction of multiple expressions (true if any operand is true)
 * @GAD_EXPRESSION_TYPE_COMPARISON:
 *   A comparison of two other expressions
 * @GAD_EXPRESSION_TYPE_IS_VALID:
 *   Extract validity as a boolean expression
 * @GAD_EXPRESSION_TYPE_IN:
 *   Check each element for membership in a set
 * @GAD_EXPRESSION_TYPE_CUSTOM:
 *   Custom user defined expression
 */
typedef enum {
  GAD_EXPRESSION_TYPE_FIELD,
  GAD_EXPRESSION_TYPE_SCALAR,
  GAD_EXPRESSION_TYPE_NOT,
  GAD_EXPRESSION_TYPE_CAST,
  GAD_EXPRESSION_TYPE_AND,
  GAD_EXPRESSION_TYPE_OR,
  GAD_EXPRESSION_TYPE_COMPARISON,
  GAD_EXPRESSION_TYPE_IS_VALID,
  GAD_EXPRESSION_TYPE_IN,
  GAD_EXPRESSION_TYPE_CUSTOM
} GADExpressionType;

/* arrow::dataset::Expression */

#define GAD_TYPE_EXPRESSION (gad_expression_get_type())
G_DECLARE_DERIVABLE_TYPE(GADExpression,
                         gad_expression,
                         GAD,
                         EXPRESSION,
                         GObject)
struct _GADExpressionClass
{
  GObjectClass parent_class;
};

gboolean
gad_expression_equal(GADExpression *expression,
                     GADExpression *other_expression);

/* TODO: GArrowScalar */
gboolean
gad_expression_equal_scalar(GADExpression *expression,
                            GArrowScalar *other_scalar);

gboolean
gad_expression_is_null(GADExpression *expression);

GArrowDataType *
gad_expression_validate(GADExpression *expression,
                        GArrowSchema *schema,
                        GError **error);

GADExpression *
gad_expression_assume(GADExpression *expression);

gchar *
gad_expression_to_string(GADExpression *expression);

GADExpressionType
gad_expression_get_type(GADExpression *expression);

GADExpression *
gad_expression_copy(GADExpression *expression);

GADInExpression *
gad_expression_in(GADExpression *expression,
                  GArrowArray *set);

GADCastExpression *
gad_expression_cast_to(GADExpression *expression,
                       GArrowDataType *type,
                       GArrowCastOptions *options);

GADCastExpression *
gad_expression_cast_like(GADExpression *expression,
                         GArrowCastOptions *options);

/* arrow::dataset::UnaryExpression */

#define GAD_TYPE_UNARY_EXPRESSION (gad_unary_expression_get_type())
G_DECLARE_DERIVABLE_TYPE(GADUnaryExpression,
                         gad_unary_expression,
                         GAD,
                         UNARY_EXPRESSION,
                         GADExpression)
struct _GADUnaryExpressionClass
{
  GADExpressionClass parent_class;
};

GADExpression *
gad_unary_expression_get_operand(GADUnaryExpression *expression);

/* arrow::dataset::BinaryExpression */

#define GAD_TYPE_BINARY_EXPRESSION (gad_binary_expression_get_type())
G_DECLARE_DERIVABLE_TYPE(GADBinaryExpression,
                         gad_binary_expression,
                         GAD,
                         BINARY_EXPRESSION,
                         GADExpression)
struct _GADBinaryExpressionClass
{
  GADExpressionClass parent_class;
};

GADExpression *
gad_binary_expression_get_left_operand(GADBinaryExpression *expression);

GADExpression *
gad_binary_expression_get_right_operand(GADBinaryExpression *expression);

/* arrow::dataset::ComparisonExpression */

#define GAD_TYPE_COMPARISON_EXPRESSION (gad_comparison_expression_get_type())
G_DECLARE_DERIVABLE_TYPE(GADComparisonExpression,
                         gad_comparison_expression,
                         GAD,
                         COMPARISON_EXPRESSION,
                         GADBinaryExpression)
struct _GADComparisonExpressionClass
{
  GADBinaryExpressionClass parent_class;
};

GArrowCompareOperator
gad_comparison_expression_get_operator(GADComparisonExpression *expression);

/* arrow::dataset::AndExpression */

#define GAD_TYPE_AND_EXPRESSION (gad_and_expression_get_type())
G_DECLARE_DERIVABLE_TYPE(GADAndExpression,
                         gad_and_expression,
                         GAD,
                         AND_EXPRESSION,
                         GADBinaryExpression)
struct _GADAndExpressionClass
{
  GADBinaryExpressionClass parent_class;
};

GADAndExpression *
gad_and_expression_new(GADExpression *left_operand,
                       GADExpression *right_operand);

/* arrow::dataset::OrExpression */

#define GAD_TYPE_OR_EXPRESSION (gad_or_expression_get_type())
G_DECLARE_DERIVABLE_TYPE(GADOrExpression,
                         gad_or_expression,
                         GAD,
                         OR_EXPRESSION,
                         GADBinaryExpression)
struct _GADOrExpressionClass
{
  GADBinaryExpressionClass parent_class;
};

GADOrExpression *
gad_or_expression_new(GADExpression *left_operand,
                      GADExpression *right_operand);

/* arrow::dataset::NotExpression */

#define GAD_TYPE_NOT_EXPRESSION (gad_not_expression_get_type())
G_DECLARE_DERIVABLE_TYPE(GADNotExpression,
                         gad_not_expression,
                         GAD,
                         NOT_EXPRESSION,
                         GADUnaryExpression)
struct _GADNotExpressionClass
{
  GADUnaryExpressionClass parent_class;
};

GADNotExpression *
gad_not_expression_new(GADExpression *operand);

/* arrow::dataset::IsValidExpression */

#define GAD_TYPE_IS_VALID_EXPRESSION (gad_is_valid_expression_get_type())
G_DECLARE_DERIVABLE_TYPE(GADIsValidExpression,
                         gad_is_valid_expression,
                         GAD,
                         IS_VALID_EXPRESSION,
                         GADUnaryExpression)
struct _GADIsValidExpressionClass
{
  GADUnaryExpressionClass parent_class;
};

GADIsValidExpression *
gad_is_valid_expression_new(GADExpression *operand);

/* arrow::dataset::InExpression */

#define GAD_TYPE_IN_EXPRESSION (gad_in_expression_get_type())
G_DECLARE_DERIVABLE_TYPE(GADInExpression,
                         gad_in_expression,
                         GAD,
                         IN_EXPRESSION,
                         GADUnaryExpression)
struct _GADInExpressionClass
{
  GADUnaryExpressionClass parent_class;
};

GADInExpression *
gad_in_expression_new(GADExpression *operand,
                      GArrowArray *set);

GArrowArray *
gad_in_expression_get_set(GADInExpression *expression);

/* arrow::dataset::CastExpression */

#define GAD_TYPE_CAST_EXPRESSION (gad_cast_expression_get_type())
G_DECLARE_DERIVABLE_TYPE(GADCastExpression,
                         gad_cast_expression,
                         GAD,
                         CAST_EXPRESSION,
                         GADUnaryExpression)
struct _GADCastExpressionClass
{
  GADUnaryExpressionClass parent_class;
};

GADCastExpression *
gad_cast_to_expression_new(GADExpression *operand,
                           GArrowDataType *type,
                           GArrowCastOptions *options);

GADCastExpression *
gad_cast_like_expression_new(GADExpression *operand,
                             GADExpression *like,
                             GArrowCastOptions *options);

GArrowCastOptions *
gad_cast_expression_get_options(GADCastExpression *expression);

/* arrow::dataset::ScalarExpression */

#define GAD_TYPE_EXPRESSION (gad_scalar_expression_get_type())
G_DECLARE_DERIVABLE_TYPE(GADScalarExpression,
                         gad_scalar_expression,
                         GAD,
                         SCALAR_EXPRESSION,
                         GADExpression)
struct _GADScalarExpressionClass
{
  GADExpressionClass parent_class;
};

/* TODO: GArrowScalar */
GADScalarExpression *
gad_scalar_expression_new(GArrowScalar *value);

GArrowScalar *
gad_scalar_expression_get_value(GADScalarExpression *expression);

/* arrow::dataset::FieldExpression */

#define GAD_TYPE_FIELD_EXPRESSION (gad_field_expression_get_type())
G_DECLARE_DERIVABLE_TYPE(GADFieldExpression,
                         gad_field_expression,
                         GAD,
                         FIELD_EXPRESSION,
                         GADExpression)
struct _GADFieldExpressionClass
{
  GADExpressionClass parent_class;
};

GADFieldExpression *
gad_field_expression_new(const gchar *name);

gchar *
gad_field_expression_get_name(GADFieldExpression *expression);

/* arrow::dataset::CustomExpression */

#define GAD_TYPE_CUSTOM_EXPRESSION (gad_custom_expression_get_type())
G_DECLARE_DERIVABLE_TYPE(GADCustomExpression,
                         gad_custom_expression,
                         GAD,
                         CUSTOM_EXPRESSION,
                         GADExpression)
struct _GADCustomExpressionClass
{
  GADExpressionClass parent_class;
};

/* arrow::dataset::ExpressionEvaluator */

#define GAD_TYPE_EXPRESSION_EVALUATOR (gad_expression_evaluator_get_type())
G_DECLARE_DERIVABLE_TYPE(GADExpressionEvaluator,
                         gad_expression_evaluator,
                         GAD,
                         EXPRESSION_EVALUATOR,
                         GObject)
struct _GAExpressionEvaluatorClass
{
  GObjectClass parent_class;
};

/* TODO: ArrowDatum */
GArrowDatum *
gad_expression_evaluator_evaluate(GADExpressionEvaluator *evaluator,
                                  GADExpression *expression,
                                  GArrowRecordBatch *record_batch,
                                  GArrowMemoryPool *pool);

GArrowRecordBatch *
gad_expression_evaluator_filter(GADExpressionEvaluator *evaluator,
                                GArrowDatum *selection,
                                GArrowRecordBatch *record_batch,
                                GArrowMemoryPool *pool);

GList *
gad_expression_evaluator_filter_batches(GADExpressionEvaluator *evaluator,
                                        GList *record_batches,
                                        GADExpression *filter,
                                        GArrowMemoryPool *pool);

GADExpressionEvaluator *
gad_expression_evaluator_new_null(void);

/* arrow::dataset::TreeEvaluator */

#define GAD_TYPE_TREE_EVALUATOR (gad_tree_evaulator_get_type())
G_DECLARE_DERIVABLE_TYPE(GADTreeEvaluator,
                         gad_tree_evaluator,
                         GAD,
                         TREE_EVALUATOR,
                         GADExpressionEvaluator)
struct _GATreeEvaluatorClass
{
  GExpressionEvaluatorClass parent_class;
};

GADTreeEvaluator *
gad_tree_evaluator_new(void);

G_END_DECLS
