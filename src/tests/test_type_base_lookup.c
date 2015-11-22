/*
 * opencurry: tests/test_type_base_lookup.c
 *
 * Copyright (c) 2015, Byron James Johnson
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors
 * may be used to endorse or promote products derived from this software without
 * specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "../base.h"
#include "testing.h"
#include "test_type_base_lookup.h"

#include "../type_base_prim.h"
#include "../type_base_lookup.h"

#include "../type_base_memory_manager.h"
#include "../type_base_compare.h"
#include "../type_base_type.h"

#include "../util.h"

#ifdef TODO
#error "TODO: test_type_base_lookup: tests for bnode_t too."
#endif /* #ifdef TODO */

int test_type_base_lookup_cli(int argc, char **argv)
{
  return run_test_suite(type_base_lookup_test);
}

/* ---------------------------------------------------------------- */

static void init_memory_methods(void);

/* type_base_lookup tests. */
unit_test_t type_base_lookup_test =
  {  test_type_base_lookup_run
  , "test_type_base_lookup"
  , "type_base_lookup tests."
  };

/* Array of type_base_lookup tests. */
unit_test_t *type_base_lookup_tests[] =
  { &lookup_memory_management_test
  , &lookup_insert_test
  , &lookup_insert_delete_test
  , &lookup_duplicates_test

  , NULL
  };

unit_test_result_t test_type_base_lookup_run(unit_test_context_t *context)
{
  init_memory_methods();

  return run_tests(context, type_base_lookup_tests);
}

/* ---------------------------------------------------------------- */

static void *(*calloc)(void *context, size_t nmemb, size_t size);
static void   *calloc_context;

static void *(*realloc)(void *context, void *area, size_t size);
static void   *realloc_context;

static void  (*free)(void *context, void *area);
static void   *free_context;

static void init_memory_methods(void)
{
  calloc  = (void *(*)(void *context, size_t nmemb, size_t size))
    default_memory_manager->calloc;
  realloc = (void *(*)(void *context, void *area, size_t size))
    default_memory_manager->realloc;
  free    = (void  (*)(void *context, void *area))
    default_memory_manager->free;

  calloc_context  = (void *) default_memory_manager;
  realloc_context = (void *) default_memory_manager;
  free_context    = (void *) default_memory_manager;
}

#define LOOKUP_EXPAND(lookup, num) \
  lookup_expand(lookup, num, calloc, calloc_context, realloc, realloc_context)

#define LOOKUP_SHRINK(lookup, num) \
  lookup_shrink(lookup, num, realloc, realloc_context, free, free_context)

#define LOOKUP_RESIZE(lookup, num) \
  lookup_expand(lookup, num, calloc, calloc_context, realloc, realloc_context, free, free_context)

#define LOOKUP_DEINIT(lookup) \
  lookup_deinit(lookup, free, free_context)

size_t checked_lookup_num_used_values(unit_test_context_t *context, unit_test_result_t *out_result, const lookup_t *lookup)
{
  unit_test_result_t result = assert_success(context);

  size_t num = 0;

  ENCLOSE()
  {
    size_t i;

    for (i = 0; i < lookup->capacity; ++i)
    {
      const bnode_t *node = &lookup->order[i];

      if (bnode_get_value_in_use_bit(node))
        ++num;

      ASSERT2( inteq, bnode_get_value_in_use_bit(node), BNODE_GET_VALUE_IN_USE_BIT(node) );
    }; BREAKABLE(result);
  }

  if (out_result)
    *out_result |= result;

  return num;
}

static size_t lookup_num_nodes_from(const lookup_t *lookup, const bnode_t *node)
{
  size_t num;

  if (lookup_empty(lookup))
    return 0;

  if (!node)
    node = &lookup->order[0];

  num = 1;

  if (!BNODE_IS_LEAF(node->left))
    num += lookup_num_nodes_from(lookup, LOOKUP_INDEX_CORDER(lookup, BNODE_GET_REF(node->left)));
  if (!BNODE_IS_LEAF(node->right))
    num += lookup_num_nodes_from(lookup, LOOKUP_INDEX_CORDER(lookup, BNODE_GET_REF(node->right)));

  return num;
}

size_t checked_lookup_num_used_nodes(unit_test_context_t *context, unit_test_result_t *out_result, const lookup_t *lookup)
{
  unit_test_result_t result = assert_success(context);

  size_t num = 0;

  ENCLOSE()
  {
    size_t i;

    for (i = 0; i < lookup->capacity; ++i)
    {
      const bnode_t *node = &lookup->order[i];

      if (bnode_get_order_in_use_bit(node))
        ++num;

      ASSERT2( inteq, bnode_get_order_in_use_bit(node), BNODE_GET_ORDER_IN_USE_BIT(node) );
    }; BREAKABLE(result);

    ASSERT2( inteq, lookup_num_nodes_from(lookup, NULL), num );
  }

  if (out_result)
    *out_result |= result;

  return num;
}

unit_test_result_t assert_lookup_int_ordered_from(ASSERT_PARAMS, const lookup_t *lookup, const bnode_t *node)
{
  unit_test_result_t result = assert_success(context);

  typedef int value_type;

  ENCLOSE()
  {
    ASSERT1( true, IS_TRUE(lookup) );

    if (lookup_empty(lookup))
      break;

    if (!node)
      node = &lookup->order[0];

    ASSERT2( inteq,  bnode_get_order_in_use_bit(node), BNODE_GET_ORDER_IN_USE_BIT(node) );
    ASSERT2( inteq,  bnode_get_order_in_use_bit(node), 1 );

    ASSERT2( sizeeq, bnode_get_value(node->value),                         BNODE_GET_VALUE(node->value) );
    ASSERT2( inteq,  bnode_get_value_in_use_bit(LOOKUP_INDEX_ORDER(lookup, BNODE_GET_VALUE(node->value))), BNODE_GET_VALUE_IN_USE_BIT(LOOKUP_INDEX_ORDER(lookup, BNODE_GET_VALUE(node->value))) );
    ASSERT2( inteq,  bnode_get_value_in_use_bit(LOOKUP_INDEX_ORDER(lookup, BNODE_GET_VALUE(node->value))), 1 );

    ASSERT2( inteq,  lookup_get_value_in_use_bit(lookup, BNODE_GET_VALUE(node->value)), LOOKUP_GET_VALUE_IN_USE_BIT(lookup, BNODE_GET_VALUE(node->value)) );
    ASSERT2( inteq,  lookup_get_value_in_use_bit(lookup, BNODE_GET_VALUE(node->value)), 1 );

    if (!BNODE_IS_LEAF(node->left))
    {
      const bnode_t *child;

      const value_type *value;
      const value_type *child_value;

      ASSERT2( sizeeq, lookup->value_size, sizeof(value_type) );

      child       = LOOKUP_INDEX_ORDER(lookup, BNODE_GET_REF(node->left));

      value       = LOOKUP_INDEX_VALUE(lookup, BNODE_GET_VALUE(node->value));
      child_value = LOOKUP_INDEX_VALUE(lookup, BNODE_GET_VALUE(child->value));

      ASSERT1( true, *child_value <= *value );

      ASSERT2( lookup_int_ordered_from, lookup, child );
    }
    if (!BNODE_IS_LEAF(node->right))
    {
      const bnode_t *child;

      const value_type *value;
      const value_type *child_value;

      ASSERT2( sizeeq, lookup->value_size, sizeof(value_type) );

      child       = LOOKUP_INDEX_ORDER(lookup, BNODE_GET_REF(node->right));

      value       = LOOKUP_INDEX_VALUE(lookup, BNODE_GET_VALUE(node->value));
      child_value = LOOKUP_INDEX_VALUE(lookup, BNODE_GET_VALUE(child->value));

      ASSERT1( true, *value <= *child_value );

      ASSERT2( lookup_int_ordered_from, lookup, child );
    }
  }

  return result;
}

unit_test_result_t assert_lookup_int_ordered(ASSERT_PARAMS, const lookup_t *lookup)
{
  return assert_lookup_int_ordered_from(ASSERT_ARGS, lookup, NULL);
}

unit_test_result_t assert_lookup_sufficiently_balanced_from(ASSERT_PARAMS, const lookup_t *lookup, const bnode_t *node, int *out_height, size_t *out_num)
{
  unit_test_result_t result = assert_success(context);

  WRITE_OUTPUT(out_height, -2);
  WRITE_OUTPUT(out_num,    0);

  ENCLOSE()
  {
    size_t num;
    int    height;

    ASSERT1( true, IS_TRUE(lookup) );

    if (lookup_empty(lookup))
    {
      WRITE_OUTPUT(out_height, -1);
      WRITE_OUTPUT(out_num,    0);
      break;
    }

    if (!node)
      node = &lookup->order[0];

    if      ( BNODE_IS_LEAF(node->left) &&  BNODE_IS_LEAF(node->right))
    {
      height = 0;

      num    = 1;
    }
    else if (!BNODE_IS_LEAF(node->left) &&  BNODE_IS_LEAF(node->right))
    {
      size_t left_num;
      int    left_height;

      ASSERT4( lookup_sufficiently_balanced_from, lookup, LOOKUP_INDEX_ORDER(lookup, BNODE_GET_REF(node->left )), &left_height,  &left_num  );

      height =
        left_height + 1;

      num = left_num + 1;
    }
    else if ( BNODE_IS_LEAF(node->left) && !BNODE_IS_LEAF(node->right))
    {
      size_t right_num;
      int    right_height;

      ASSERT4( lookup_sufficiently_balanced_from, lookup, LOOKUP_INDEX_ORDER(lookup, BNODE_GET_REF(node->right)), &right_height, &right_num );

      height =
        right_height + 1;

      num = right_num + 1;
    }
    else
    {
      size_t left_num;
      size_t right_num;

      int    left_height;
      int    right_height;

      ASSERT4( lookup_sufficiently_balanced_from, lookup, LOOKUP_INDEX_ORDER(lookup, BNODE_GET_REF(node->left )), &left_height,  &left_num  );
      ASSERT4( lookup_sufficiently_balanced_from, lookup, LOOKUP_INDEX_ORDER(lookup, BNODE_GET_REF(node->right)), &right_height, &right_num );

      height =
        MAX
          ( left_height
          , right_height
          ) + 1;

      num = left_num + right_num + 1;
    }

    WRITE_OUTPUT(out_height, height);
    WRITE_OUTPUT(out_num,    num);

/* TODO: balance tree!! */
#ifdef TODO
#error "TODO: balance tree!!"
    {
      char details[DEFAULT_BUF_SIZE];

      snprintf
        ( (char *) details, size_less_null(sizeof(details))
        , "Details:\n"
          "  num - - - - - - - - - : %d\n"
          "  height  - - - - - - - : %d\n"
          "\n"
          "  height  - - - - - - - : %d\n"
          "  lookup_max_height(num): %d\n"

        , (int) num
        , (int) height

        , (int) height
        , (int) lookup_max_height(num)
        );

      test_set_details_msg(context, details);
    }
    ASSERT1( true, height <= lookup_max_height(num) );
    reset_err_msg_details(context);
#endif /* #ifdef TODO */
  }

  return result;
}

unit_test_result_t assert_lookup_sufficiently_balanced(ASSERT_PARAMS, const lookup_t *lookup, int *out_height, size_t *out_num)
{
  return assert_lookup_sufficiently_balanced_from(ASSERT_ARGS, lookup, NULL, out_height, out_num);
}

size_t checked_lookup_int_len(unit_test_context_t *context, unit_test_result_t *out_result, const lookup_t *lookup)
{
  unit_test_result_t result = assert_success(context);

  typedef int value_type;

  size_t len;

  ENCLOSE()
  {
    size_t num;
    int    height;

    len = lookup_len(lookup);

    ASSERT2( inteq, len, LOOKUP_LEN(lookup) );

    /* ---------------------------------------------------------------- */

    ASSERT2( inteq, CHECKED_LOOKUP_NUM_USED_VALUES(lookup), len );
    ASSERT2( inteq, CHECKED_LOOKUP_NUM_USED_NODES (lookup), len );

    /* ---------------------------------------------------------------- */

    ASSERT2( sizeeq, lookup->value_size, sizeof(value_type) );

    ASSERT1( lookup_int_ordered,           lookup );
    ASSERT3( lookup_sufficiently_balanced, lookup, &height, &num );

    ASSERT2( inteq, height, lookup_height(lookup) );
    ASSERT2( inteq, num,    len                   );

/* TODO: balance tree!! */
#ifdef TODO
#error "TODO: balance tree!!"
    {
      char details[DEFAULT_BUF_SIZE];

      snprintf
        ( (char *) details, size_less_null(sizeof(details))
        , "Details:\n"
          "  height  - - - - - - - : %d\n"
          "  lookup_height(lookup) : %d\n"
          "  num - - - - - - - - - : %d\n"
          "  len - - - - - - - - - : %d\n"
          "\n"
          "  lookup_height(lookup) : %d\n"
          "  lookup_max_height(len): %d\n"

        , (int) height
        , (int) lookup_height(lookup)
        , (int) num
        , (int) len

        , (int) lookup_height(lookup)
        , (int) lookup_max_height(len)
        );

      test_set_details_msg(context, details);
    }
    ASSERT1( true,  lookup_height(lookup) <= lookup_max_height(len) );
    reset_err_msg_details(context);
#endif /* #ifdef TODO */

    /* Traverse the tree, and make sure the number of nodes matches. */
  }

  if (out_result)
    *out_result |= result;

  return len;
}

/* ---------------------------------------------------------------- */

unit_test_t lookup_memory_management_test =
  {  lookup_memory_management_test_run
  , "lookup_memory_management_test"
  , "Testing memory management with lookup values."
  };

unit_test_result_t lookup_memory_management_test_run(unit_test_context_t *context)
{
  unit_test_result_t result = assert_success(context);

  typedef int value_type;

  lookup_t lookup_val;
  lookup_t *lookup = &lookup_val;

  init_memory_methods();

  lookup_init_empty(lookup, sizeof(value_type));

  ENCLOSE()
  {
    ASSERT2( inteq,  lookup_capacity(lookup), 0 );
    ASSERT1( true,   lookup_empty(lookup) );

    ASSERT2( objpeq, LOOKUP_EXPAND(lookup, 3), &lookup_val );
    ASSERT2( inteq,  lookup_capacity(lookup), 3 );
    ASSERT1( true,   lookup_empty(lookup) );

    ASSERT2( objpeq, LOOKUP_EXPAND(lookup, 3), &lookup_val );
    ASSERT2( inteq,  lookup_capacity(lookup), 3 );
    ASSERT1( true,   lookup_empty(lookup) );

    ASSERT2( objpeq, LOOKUP_EXPAND(lookup, 1), &lookup_val );
    ASSERT2( inteq,  lookup_capacity(lookup), 3 );
    ASSERT1( true,   lookup_empty(lookup) );

    ASSERT2( objpeq, LOOKUP_EXPAND(lookup, 3), &lookup_val );
    ASSERT2( inteq,  lookup_capacity(lookup), 3 );
    ASSERT1( true,   lookup_empty(lookup) );

    ASSERT2( objpeq, LOOKUP_EXPAND(lookup, 0), &lookup_val );
    ASSERT2( inteq,  lookup_capacity(lookup), 3 );
    ASSERT1( true,   lookup_empty(lookup) );

    ASSERT2( objpeq, LOOKUP_EXPAND(lookup, 3), &lookup_val );
    ASSERT2( inteq,  lookup_capacity(lookup), 3 );
    ASSERT1( true,   lookup_empty(lookup) );

    ASSERT2( objpeq, LOOKUP_EXPAND(lookup, 7), &lookup_val );
    ASSERT2( inteq,  lookup_capacity(lookup), 7 );
    ASSERT1( true,   lookup_empty(lookup) );


    LOOKUP_DEINIT(lookup);
    ASSERT2( inteq, lookup_capacity(lookup), 0 );
    ASSERT1( true,  lookup_empty(lookup));

    lookup_init_empty(lookup, sizeof(value_type));
    ASSERT2( inteq, lookup_capacity(lookup), 0 );
    ASSERT1( true,  lookup_empty(lookup));


    ASSERT2( objpeq,LOOKUP_EXPAND(lookup, 3), &lookup_val);
    ASSERT2( inteq, lookup_capacity(lookup), 3 );
    ASSERT1( true,  lookup_empty(lookup));


    lookup_init_empty(lookup, sizeof(value_type));
    ASSERT2( inteq, lookup_capacity(lookup), 0 );
    ASSERT1( true,  lookup_empty(lookup));


    ASSERT2( objpeq,LOOKUP_EXPAND(lookup, 3), &lookup_val);
    ASSERT2( inteq, lookup_capacity(lookup), 3 );
    ASSERT1( true,  lookup_empty(lookup));
  }

  LOOKUP_DEINIT(lookup);

  return result;
}

/* ---------------------------------------------------------------- */

static int val_or_m1(const int *val)
{
  if (!val)
    return -1;

  return *val;
}

#define LOOKUP_INSERT_TESTS_NUM_ADDITIONAL_VALUES 4096
static unit_test_result_t lookup_insert_tests(unit_test_context_t *context, lookup_t *lookup_val_ref, lookup_t **lookup_ref)
{
  unit_test_result_t result = assert_success(context);

  typedef int value_type;

  lookup_t *lookup = *lookup_ref;

  init_memory_methods();

  ENCLOSE()
  {
    static const size_t num_additional_values = LOOKUP_INSERT_TESTS_NUM_ADDITIONAL_VALUES;

    value_type value    = -1, *val  = &value;
    value_type value2   = -1, *val2 = &value2;
    value_type retrieve = -1, *ret  = &retrieve;

    int is_duplicate = -1, *dp = &is_duplicate;

    callback_compare_t cmp = callback_compare_int();

    ASSERT2( inteq, lookup_capacity(lookup), 0);
    ASSERT1( true,  lookup_empty(lookup));

    /* ---------------------------------------------------------------- */

    /* 0-size lookup insertion. */

    value = 42;
    ASSERT2( objpeq, lookup_insert(lookup, val, 0, cmp, dp), NULL );
    ASSERT2( inteq,  is_duplicate, 0 );

    /* ---------------------------------------------------------------- */

    /* 3-size lookup insertion. */

    ASSERT2( objpeq, LOOKUP_EXPAND(lookup, 3), lookup_val_ref);
    ASSERT2( inteq,  lookup_capacity(lookup), 3 );
    ASSERT1( true,   lookup_empty(lookup));

    ASSERT2( inteq,  CHECKED_LOOKUP_INT_LEN(lookup), 0);

    retrieve = 42;
    ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), -1 );

    /* Inserting same value. */

    value = 42;
    ASSERT2( objpeq, lookup_insert(lookup, val,  0, cmp, dp), lookup_val_ref );
    ASSERT2( inteq,  is_duplicate, 0 );
    ASSERT2( inteq,  CHECKED_LOOKUP_INT_LEN(lookup), 1);

    retrieve = 42;
    ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 42 );
    retrieve = 43;
    ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), -1 );


    value = 42;
    ASSERT2( objpeq, lookup_insert(lookup, val,  0, cmp, dp), lookup_val_ref );
    ASSERT2( inteq,  is_duplicate, 1 );
    ASSERT2( inteq,  CHECKED_LOOKUP_INT_LEN(lookup), 1);

    retrieve = 42;
    ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 42 );
    retrieve = 43;
    ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), -1 );


    value2 = 42;
    ASSERT2( objpeq, lookup_insert(lookup, val2, 0, cmp, dp), lookup_val_ref );
    ASSERT2( inteq,  is_duplicate, 1 );
    ASSERT2( inteq,  CHECKED_LOOKUP_INT_LEN(lookup), 1);

    retrieve = 42;
    ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 42 );
    retrieve = 43;
    ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), -1 );


    value2 = 42;
    ASSERT2( objpeq, lookup_insert(lookup, val2, 0, cmp, dp), lookup_val_ref );
    ASSERT2( inteq,  is_duplicate, 1 );
    ASSERT2( inteq,  CHECKED_LOOKUP_INT_LEN(lookup), 1);

    retrieve = 42;
    ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 42 );
    retrieve = 43;
    ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), -1 );


    /* Insert new value. */

    value = 43;
    ASSERT2( objpeq, lookup_insert(lookup, val,  0, cmp, dp), lookup_val_ref );
    ASSERT2( inteq,  is_duplicate, 0 );
    ASSERT2( inteq,  CHECKED_LOOKUP_INT_LEN(lookup), 2);

    retrieve = 42;
    ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 42 );
    retrieve = 43;
    ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 43 );


    value = 43;
    ASSERT2( objpeq, lookup_insert(lookup, val,  0, cmp, dp), lookup_val_ref );
    ASSERT2( inteq,  is_duplicate, 1 );
    ASSERT2( inteq,  CHECKED_LOOKUP_INT_LEN(lookup), 2);

    retrieve = 42;
    ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 42 );
    retrieve = 43;
    ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 43 );


    /* Inserting a multiple. */

    value = 42;
    ASSERT2( objpeq, lookup_insert(lookup, val,  0, cmp, dp), lookup_val_ref );
    ASSERT2( inteq,  is_duplicate, 1 );
    ASSERT2( inteq,  CHECKED_LOOKUP_INT_LEN(lookup), 2);
    ASSERT1( false,  lookup_max_capacity(lookup) );

    retrieve = 42;
    ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 42 );
    retrieve = 43;
    ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 43 );


    value = 42;
    ASSERT2( objpeq, lookup_insert(lookup, val,  1, cmp, dp), lookup_val_ref );
    ASSERT2( inteq,  is_duplicate, 1 );
    ASSERT2( inteq,  CHECKED_LOOKUP_INT_LEN(lookup), 3 );
    ASSERT1( true,   lookup_max_capacity(lookup) );

    retrieve = 42;
    ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 42 );
    retrieve = 43;
    ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 43 );


    value = 42;
    ASSERT2( objpeq, lookup_insert(lookup, val,  0, cmp, dp), lookup_val_ref );
    ASSERT2( inteq,  is_duplicate, 1 );
    ASSERT2( inteq,  CHECKED_LOOKUP_INT_LEN(lookup), 3 );
    ASSERT1( true,   lookup_max_capacity(lookup) );

    retrieve = 42;
    ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 42 );
    retrieve = 43;
    ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 43 );


    /* Exceeding capacity. */

    value = 7;
    ASSERT2( objpeq, lookup_insert(lookup, val,  0, cmp, dp), NULL );
    ASSERT2( inteq,  is_duplicate, 0 );
    ASSERT2( inteq,  CHECKED_LOOKUP_INT_LEN(lookup), 3 );
    ASSERT1( true,   lookup_max_capacity(lookup) );

    retrieve = 42;
    ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 42 );
    retrieve = 43;
    ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 43 );
    retrieve = 7;
    ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), -1 );


    value = 7;
    ASSERT2( objpeq, lookup_insert(lookup, val,  1, cmp, dp), NULL );
    ASSERT2( inteq,  is_duplicate, 0 );
    ASSERT2( inteq,  CHECKED_LOOKUP_INT_LEN(lookup), 3 );
    ASSERT1( true,   lookup_max_capacity(lookup) );

    retrieve = 42;
    ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 42 );
    retrieve = 43;
    ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 43 );
    retrieve = 7;
    ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), -1 );


    /* Same insertion after expansion. */

    ASSERT2( objpeq, LOOKUP_EXPAND(lookup, 7), lookup_val_ref);
    ASSERT2( inteq,  lookup_capacity(lookup), 7 );
    ASSERT1( false,  lookup_empty(lookup));
    ASSERT1( false,  lookup_max_capacity(lookup) );
    ASSERT2( inteq,  CHECKED_LOOKUP_INT_LEN(lookup), 3);


    value = 7;
    ASSERT2( objpeq, lookup_insert(lookup, val,  0, cmp, dp), lookup_val_ref );
    ASSERT2( inteq,  is_duplicate, 0 );
    ASSERT2( inteq,  CHECKED_LOOKUP_INT_LEN(lookup), 4 );
    ASSERT1( false,  lookup_max_capacity(lookup) );

    retrieve = 42;
    ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 42 );
    retrieve = 43;
    ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 43 );
    retrieve = 7;
    ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 7  );


    value = 7;
    ASSERT2( objpeq, lookup_insert(lookup, val,  1, cmp, dp), lookup_val_ref );
    ASSERT2( inteq,  is_duplicate, 1 );
    ASSERT2( inteq,  CHECKED_LOOKUP_INT_LEN(lookup), 5 );
    ASSERT1( false,  lookup_max_capacity(lookup) );

    retrieve = 42;
    ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 42 );
    retrieve = 43;
    ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 43 );
    retrieve = 7;
    ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 7  );

    /* More insertions. */

    retrieve = 42;
    ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 42 );
    retrieve = 43;
    ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 43 );
    retrieve = 7;
    ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 7  );
    retrieve = 8;
    ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), -1 );
    retrieve = 9;
    ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), -1 );
    retrieve = 10;
    ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), -1 );

    /* Insert 8. */
    value = 8;
    ASSERT2( objpeq, lookup_insert(lookup, val,  1, cmp, dp), lookup_val_ref );
    ASSERT2( inteq,  is_duplicate, 0 );
    ASSERT2( inteq,  CHECKED_LOOKUP_INT_LEN(lookup), 6 );
    ASSERT1( false,  lookup_max_capacity(lookup) );

    retrieve = 42;
    ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 42 );
    retrieve = 43;
    ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 43 );
    retrieve = 7;
    ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 7  );
    retrieve = 8;
    ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 8  );
    retrieve = 9;
    ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), -1 );
    retrieve = 10;
    ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), -1 );


    /* Insert 9. */
    value = 9;
    ASSERT2( objpeq, lookup_insert(lookup, val,  1, cmp, dp), lookup_val_ref );
    ASSERT2( inteq,  is_duplicate, 0 );
    ASSERT2( inteq,  CHECKED_LOOKUP_INT_LEN(lookup), 7 );
    ASSERT1( true,   lookup_max_capacity(lookup) );

    retrieve = 42;
    ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 42 );
    retrieve = 43;
    ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 43 );
    retrieve = 7;
    ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 7  );
    retrieve = 8;
    ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 8  );
    retrieve = 9;
    ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 9  );
    retrieve = 10;
    ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), -1 );


    /* Insert 10 beyond capacity. */
    value = 10;
    ASSERT2( objpeq, lookup_insert(lookup, val,  1, cmp, dp), NULL );
    ASSERT2( inteq,  is_duplicate, 0 );
    ASSERT2( inteq,  CHECKED_LOOKUP_INT_LEN(lookup), 7 );
    ASSERT1( true,   lookup_max_capacity(lookup) );

    retrieve = 42;
    ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 42 );
    retrieve = 43;
    ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 43 );
    retrieve = 7;
    ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 7  );
    retrieve = 8;
    ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 8  );
    retrieve = 9;
    ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 9  );
    retrieve = 10;
    ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), -1 );


    /* Beyond-capacity already-exists tests. */

    value = 1;
    ASSERT2( objpeq, lookup_insert(lookup, val,  1, cmp, dp), NULL );
    ASSERT2( inteq,  is_duplicate, 0 );
    ASSERT2( inteq,  CHECKED_LOOKUP_INT_LEN(lookup), 7 );
    ASSERT1( true,   lookup_max_capacity(lookup) );

    value = 1;
    ASSERT2( objpeq, lookup_insert(lookup, val,  0, cmp, dp), NULL );
    ASSERT2( inteq,  is_duplicate, 0 );
    ASSERT2( inteq,  CHECKED_LOOKUP_INT_LEN(lookup), 7 );
    ASSERT1( true,   lookup_max_capacity(lookup) );


    value = 7;
    ASSERT2( objpeq, lookup_insert(lookup, val,  1, cmp, dp), NULL );
    ASSERT2( inteq,  is_duplicate, 1 );
    ASSERT2( inteq,  CHECKED_LOOKUP_INT_LEN(lookup), 7 );
    ASSERT1( true,   lookup_max_capacity(lookup) );

    value = 7;
    ASSERT2( objpeq, lookup_insert(lookup, val,  0, cmp, dp), lookup_val_ref );
    ASSERT2( inteq,  is_duplicate, 1 );
    ASSERT2( inteq,  CHECKED_LOOKUP_INT_LEN(lookup), 7 );
    ASSERT1( true,   lookup_max_capacity(lookup) );


    value = 8;
    ASSERT2( objpeq, lookup_insert(lookup, val,  1, cmp, dp), NULL );
    ASSERT2( inteq,  is_duplicate, 1 );
    ASSERT2( inteq,  CHECKED_LOOKUP_INT_LEN(lookup), 7 );
    ASSERT1( true,   lookup_max_capacity(lookup) );

    value = 8;
    ASSERT2( objpeq, lookup_insert(lookup, val,  0, cmp, dp), lookup_val_ref );
    ASSERT2( inteq,  is_duplicate, 1 );
    ASSERT2( inteq,  CHECKED_LOOKUP_INT_LEN(lookup), 7 );
    ASSERT1( true,   lookup_max_capacity(lookup) );


    value = 10;
    ASSERT2( objpeq, lookup_insert(lookup, val,  1, cmp, dp), NULL );
    ASSERT2( inteq,  is_duplicate, 0 );
    ASSERT2( inteq,  CHECKED_LOOKUP_INT_LEN(lookup), 7 );
    ASSERT1( true,   lookup_max_capacity(lookup) );

    value = 10;
    ASSERT2( objpeq, lookup_insert(lookup, val,  0, cmp, dp), NULL );
    ASSERT2( inteq,  is_duplicate, 0 );
    ASSERT2( inteq,  CHECKED_LOOKUP_INT_LEN(lookup), 7 );
    ASSERT1( true,   lookup_max_capacity(lookup) );


    /* Expand and insert 10. */
    ASSERT2( objpeq, LOOKUP_EXPAND(lookup, 3), lookup_val_ref);
    ASSERT2( inteq,  lookup_capacity(lookup), 7 );
    ASSERT1( false,  lookup_empty(lookup));
    ASSERT1( true,   lookup_max_capacity(lookup) );
    ASSERT2( inteq,  CHECKED_LOOKUP_INT_LEN(lookup), 7 );

    ASSERT2( objpeq, LOOKUP_EXPAND(lookup, 0), lookup_val_ref);
    ASSERT2( inteq,  lookup_capacity(lookup), 7 );
    ASSERT1( false,  lookup_empty(lookup));
    ASSERT1( true,   lookup_max_capacity(lookup) );
    ASSERT2( inteq,  CHECKED_LOOKUP_INT_LEN(lookup), 7 );

    ASSERT2( objpeq, LOOKUP_EXPAND(lookup, 7), lookup_val_ref);
    ASSERT2( inteq,  lookup_capacity(lookup), 7 );
    ASSERT1( false,  lookup_empty(lookup));
    ASSERT1( true,   lookup_max_capacity(lookup) );
    ASSERT2( inteq,  CHECKED_LOOKUP_INT_LEN(lookup), 7 );

    ASSERT2( objpeq, LOOKUP_EXPAND(lookup, 8), lookup_val_ref);
    ASSERT2( inteq,  lookup_capacity(lookup), 8 );
    ASSERT1( false,  lookup_empty(lookup));
    ASSERT1( false,  lookup_max_capacity(lookup) );
    ASSERT2( inteq,  CHECKED_LOOKUP_INT_LEN(lookup), 7 );


    value = 10;
    ASSERT2( objpeq, lookup_insert(lookup, val,  1, cmp, dp), lookup_val_ref );
    ASSERT2( inteq,  is_duplicate, 0 );
    ASSERT2( inteq,  CHECKED_LOOKUP_INT_LEN(lookup), 8 );
    ASSERT1( true,   lookup_max_capacity(lookup) );

    retrieve = 42; ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 42 );
    retrieve = 43; ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 43 );
    retrieve = 7;  ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 7  );
    retrieve = 8;  ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 8  );
    retrieve = 9;  ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 9  );
    retrieve = 10; ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 10 );
    retrieve = 12; ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), -1 );
    retrieve = 0;  ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), -1 );
    retrieve = 1;  ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), -1 );
    retrieve = 2;  ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), -1 );
    retrieve = 3;  ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), -1 );
    retrieve = 4;  ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), -1 );


    /* Expand and insert 12. */
    ASSERT2( objpeq, LOOKUP_EXPAND(lookup, 3), lookup_val_ref);
    ASSERT2( inteq,  lookup_capacity(lookup), 8 );
    ASSERT1( false,  lookup_empty(lookup));
    ASSERT1( true,   lookup_max_capacity(lookup) );
    ASSERT2( inteq,  CHECKED_LOOKUP_INT_LEN(lookup), 8 );

    ASSERT2( objpeq, LOOKUP_EXPAND(lookup, 0), lookup_val_ref);
    ASSERT2( inteq,  lookup_capacity(lookup), 8 );
    ASSERT1( false,  lookup_empty(lookup));
    ASSERT1( true,   lookup_max_capacity(lookup) );
    ASSERT2( inteq,  CHECKED_LOOKUP_INT_LEN(lookup), 8 );

    ASSERT2( objpeq, LOOKUP_EXPAND(lookup, 7), lookup_val_ref);
    ASSERT2( inteq,  lookup_capacity(lookup), 8 );
    ASSERT1( false,  lookup_empty(lookup));
    ASSERT1( true,   lookup_max_capacity(lookup) );
    ASSERT2( inteq,  CHECKED_LOOKUP_INT_LEN(lookup), 8 );

    ASSERT2( objpeq, LOOKUP_EXPAND(lookup, 8), lookup_val_ref);
    ASSERT2( inteq,  lookup_capacity(lookup), 8 );
    ASSERT1( false,  lookup_empty(lookup));
    ASSERT1( true,   lookup_max_capacity(lookup) );
    ASSERT2( inteq,  CHECKED_LOOKUP_INT_LEN(lookup), 8 );

    ASSERT2( objpeq, LOOKUP_EXPAND(lookup, 9), lookup_val_ref);
    ASSERT2( inteq,  lookup_capacity(lookup), 9 );
    ASSERT1( false,  lookup_empty(lookup));
    ASSERT1( false,  lookup_max_capacity(lookup) );
    ASSERT2( inteq,  CHECKED_LOOKUP_INT_LEN(lookup), 8 );


    value = 12;
    ASSERT2( objpeq, lookup_insert(lookup, val,  0, cmp, dp), lookup_val_ref );
    ASSERT2( inteq,  is_duplicate, 0 );
    ASSERT2( inteq,  CHECKED_LOOKUP_INT_LEN(lookup), 9 );
    ASSERT1( true,   lookup_max_capacity(lookup) );

    retrieve = 42; ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 42 );
    retrieve = 43; ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 43 );
    retrieve = 7;  ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 7  );
    retrieve = 8;  ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 8  );
    retrieve = 9;  ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 9  );
    retrieve = 10; ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 10 );
    retrieve = 12; ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 12 );
    retrieve = 0;  ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), -1 );
    retrieve = 1;  ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), -1 );
    retrieve = 2;  ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), -1 );
    retrieve = 3;  ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), -1 );
    retrieve = 4;  ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), -1 );

    /* TODO: shrinking. */
#ifdef TODO
    #error "TODO: test with shrinking"
#endif /* #ifdef TODO */

    /* Check parts of current state match what we expect. */

    ASSERT2( objpeq, LOOKUP_EXPAND(lookup, 1), lookup_val_ref);
    ASSERT2( inteq,  lookup_capacity(lookup), 9 );
    ASSERT1( false,  lookup_empty(lookup));
    ASSERT1( true,   lookup_max_capacity(lookup) );
    ASSERT2( inteq,  CHECKED_LOOKUP_INT_LEN(lookup), 9);

    retrieve = 42; ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 42 );
    retrieve = 43; ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 43 );
    retrieve = 7;  ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 7  );
    retrieve = 8;  ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 8  );
    retrieve = 9;  ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 9  );
    retrieve = 10; ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 10 );
    retrieve = 12; ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 12 );
    retrieve = 0;  ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), -1 );
    retrieve = 1;  ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), -1 );
    retrieve = 2;  ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), -1 );
    retrieve = 3;  ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), -1 );
    retrieve = 4;  ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), -1 );

    /* Inserting many values. */
    {
      static int skip_checks       = 0;
      static int skip_length_check = 0;

      int i;
      const int mul[] =
        { 1, -1, 2 };
      const size_t mul_size =
        sizeof(mul)/sizeof(mul[0]);

      ASSERT2( objpeq, LOOKUP_EXPAND(lookup, 9 + num_additional_values), lookup_val_ref);
      ASSERT2( inteq,  lookup_capacity(lookup), 9 + num_additional_values );
      ASSERT1( false,  lookup_empty(lookup));
      if (num_additional_values >= 1)
      {
        ASSERT1( false,  lookup_max_capacity(lookup) );
      }
      ASSERT2( inteq,  CHECKED_LOOKUP_INT_LEN(lookup), 9 );

      for (i = 0; i < num_additional_values; ++i)
      {
        int should_already_exist = -1;

        /* Value to insert. */
        value = mul[i % mul_size] * (i + 44);

        if (!skip_checks)
        {
          /* Does this value already exists? */
          retrieve = value;
          should_already_exist = val_or_m1(lookup_retrieve(lookup, ret, cmp)) != -1;
        }

        /* Insert the value. */
        ASSERT2( objpeq, lookup_insert(lookup, val,  1, cmp, dp), lookup_val_ref );
        if (!skip_checks)
        {
          ASSERT2( inteq,  is_duplicate, should_already_exist );
        }

        /* Check length. */
        if (  ( !skip_length_check )
           && (  ( i <= 10                                 )
              || ( DISTANCE(i, num_additional_values) <= 3 )
              || ( i % 128 == 0                            )
              )
           )
        {
          /* More thorough testing for the first 10 and last 10 values, and for
           * every 128th value. */
          ASSERT2( inteq,  CHECKED_LOOKUP_INT_LEN(lookup), 9 + i + 1 );
        }
        else
        {
          ASSERT2( inteq,  lookup_len(lookup), 9 + i + 1 );
        }
        if (i + 1 < num_additional_values )
          ASSERT1( false, lookup_max_capacity(lookup) );
        else
          ASSERT1( true,  lookup_max_capacity(lookup) );

        if (!skip_checks)
        {
          /* Make sure we can retrieve it. */
          retrieve = value; ASSERT2( inteq, val_or_m1(lookup_retrieve(lookup, ret, cmp)), retrieve );
          retrieve = value; ASSERT2( inteq, val_or_m1(lookup_retrieve(lookup, ret, cmp)), value    );
        }
      }; BREAKABLE(result);

      skip_checks       = 1;
      skip_length_check = 1;
    }

    /* Check parts of final state match what we expect. */

    ASSERT2( objpeq, LOOKUP_EXPAND(lookup, 1), lookup_val_ref);
    ASSERT2( inteq,  lookup_capacity(lookup), 9 + LOOKUP_INSERT_TESTS_NUM_ADDITIONAL_VALUES );
    ASSERT1( false,  lookup_empty(lookup));
    ASSERT1( true,   lookup_max_capacity(lookup) );
    ASSERT2( inteq,  CHECKED_LOOKUP_INT_LEN(lookup), 9 + LOOKUP_INSERT_TESTS_NUM_ADDITIONAL_VALUES );

    retrieve = 42; ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 42 );
    retrieve = 43; ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 43 );
    retrieve = 7;  ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 7  );
    retrieve = 8;  ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 8  );
    retrieve = 9;  ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 9  );
    retrieve = 10; ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 10 );
    retrieve = 12; ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 12 );
    retrieve = 0;  ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), -1 );
    retrieve = 1;  ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), -1 );
    retrieve = 2;  ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), -1 );
    retrieve = 3;  ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), -1 );
    retrieve = 4;  ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), -1 );
  }

  return result;
}

/* ---------------------------------------------------------------- */

unit_test_t lookup_insert_test =
  {  lookup_insert_test_run
  , "lookup_insert_test"
  , "Testing lookup insertion."
  };

unit_test_result_t lookup_insert_test_run(unit_test_context_t *context)
{
  unit_test_result_t result = assert_success(context);

  typedef int value_type;

  lookup_t lookup_val;
  lookup_t *lookup = &lookup_val;

  init_memory_methods();

  lookup_init_empty(lookup, sizeof(value_type));

  ENCLOSE()
  {
    COMPOUND_TEST
      ( "lookup_insert_tests"
      , "lookup_insert_tests: run #1."
      , lookup_insert_tests(context, &lookup_val, &lookup)
      );

    LOOKUP_DEINIT(lookup);
    LOOKUP_DEINIT(lookup);
    LOOKUP_DEINIT(lookup);

    lookup_init_empty(lookup, sizeof(value_type));

    COMPOUND_TEST
      ( "lookup_insert_tests #2"
      , "lookup_insert_tests: run #2."
      , lookup_insert_tests(context, &lookup_val, &lookup)
      );

    {
      int run;

      for (run = 3; run <= 8; ++run)
      {
        char name       [DEFAULT_BUF_SIZE];
        char description[DEFAULT_BUF_SIZE];

        snprintf
          ( (char *) name, (size_t) terminator_size(sizeof(name))
          , "lookup_insert_tests #%d"
          , (int) run
          );

        snprintf
          ( (char *) description, (size_t) terminator_size(sizeof(description))
          , "lookup_insert_tests: run #%d."
          , (int) run
          );

        LOOKUP_DEINIT(lookup);
        lookup_init_empty(lookup, sizeof(value_type));

        COMPOUND_TEST
          ( name
          , description
          , lookup_insert_tests(context, &lookup_val, &lookup)
          );
      }; BREAKABLE(result);
    }
  }

  LOOKUP_DEINIT(lookup);

  return result;
}

/* ---------------------------------------------------------------- */

unit_test_t lookup_insert_delete_test =
  {  lookup_insert_delete_test_run
  , "lookup_insert_delete_test"
  , "Testing lookup insertion and deletion."
  };

unit_test_result_t lookup_insert_delete_test_run(unit_test_context_t *context)
{
  unit_test_result_t result = assert_success(context);

  typedef int value_type;

  lookup_t lookup_val;

  lookup_t *lookup         = &lookup_val;
  lookup_t *lookup_val_ref = &lookup_val;

  init_memory_methods();

  lookup_init_empty(lookup, sizeof(value_type));

  ENCLOSE()
  {
    size_t num_deletions = 0;

    value_type value    = -1, *val  = &value;
    value_type retrieve = -1, *ret  = &retrieve;

    int    is_duplicate = -1, *dp = &is_duplicate;
    size_t num_deleted  = -1, *nd = &num_deleted;

    callback_compare_t cmp = callback_compare_int();

    /* ---------------------------------------------------------------- */

    COMPOUND_TEST
      ( "lookup_insert_tests"
      , "lookup_insert_tests: deletion tests preparation."
      , lookup_insert_tests(context, &lookup_val, &lookup)
      );

    /* Make sure "lookup_insert_tests" ends in what we expect. */

    ASSERT2( objpeq, LOOKUP_EXPAND(lookup, 1), lookup_val_ref);
    ASSERT2( inteq,  lookup_capacity(lookup), 9 + LOOKUP_INSERT_TESTS_NUM_ADDITIONAL_VALUES );
    ASSERT1( false,  lookup_empty(lookup));
    ASSERT1( true,   lookup_max_capacity(lookup) );
    ASSERT2( inteq,  CHECKED_LOOKUP_INT_LEN(lookup), 9 + LOOKUP_INSERT_TESTS_NUM_ADDITIONAL_VALUES );

    retrieve = 42; ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 42 );
    retrieve = 43; ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 43 );
    retrieve = 7;  ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 7  );
    retrieve = 8;  ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 8  );
    retrieve = 9;  ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 9  );
    retrieve = 10; ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 10 );
    retrieve = 0;  ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), -1 );
    retrieve = 1;  ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), -1 );
    retrieve = 2;  ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), -1 );
    retrieve = 3;  ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), -1 );
    retrieve = 4;  ASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), -1 );

    /* Delete 7. */
    retrieve = 7; ASSERT2( inteq, val_or_m1(lookup_retrieve(lookup, ret, cmp)), 7 );

    value = 7;
    ++num_deletions;
    ++num_deletions;
    ASSERT2( objpeq, lookup_delete(lookup, val,  cmp, nd), lookup_val_ref );
    ASSERT2( inteq,  num_deleted, 2 );
    ASSERT2( inteq,  CHECKED_LOOKUP_INT_LEN(lookup), 9 + LOOKUP_INSERT_TESTS_NUM_ADDITIONAL_VALUES - num_deletions );
    ASSERT1( false,  lookup_max_capacity(lookup) );

    retrieve = 7; ASSERT2( inteq, val_or_m1(lookup_retrieve(lookup, ret, cmp)), -1 );

    /* Delete 7. */
    retrieve = 7; ASSERT2( inteq, val_or_m1(lookup_retrieve(lookup, ret, cmp)), -1 );

    value = 7;
    ASSERT2( objpeq, lookup_delete(lookup, val,  cmp, nd), lookup_val_ref );
    ASSERT2( inteq,  num_deleted, 0 );
    ASSERT2( inteq,  CHECKED_LOOKUP_INT_LEN(lookup), 9 + LOOKUP_INSERT_TESTS_NUM_ADDITIONAL_VALUES - num_deletions );
    ASSERT1( false,  lookup_max_capacity(lookup) );

    retrieve = 7; ASSERT2( inteq, val_or_m1(lookup_retrieve(lookup, ret, cmp)), -1 );


    /* Insert 7. */
    retrieve = 7; ASSERT2( inteq, val_or_m1(lookup_retrieve(lookup, ret, cmp)), -1 );

    value = 7;
    --num_deletions;
    ASSERT2( objpeq, lookup_insert(lookup, val,  1, cmp, dp), lookup_val_ref );
    ASSERT2( inteq,  is_duplicate, 0 );
    ASSERT2( inteq,  CHECKED_LOOKUP_INT_LEN(lookup), 9 + LOOKUP_INSERT_TESTS_NUM_ADDITIONAL_VALUES - num_deletions );
    ASSERT1( false,  lookup_max_capacity(lookup) );

    retrieve = 7; ASSERT2( inteq, val_or_m1(lookup_retrieve(lookup, ret, cmp)), 7 );


    /* Delete 7. */
    retrieve = 7; ASSERT2( inteq, val_or_m1(lookup_retrieve(lookup, ret, cmp)), 7 );

    value = 7;
    ++num_deletions;
    ASSERT2( objpeq, lookup_delete(lookup, val,  cmp, nd), lookup_val_ref );
    ASSERT2( inteq,  num_deleted, 1 );
    ASSERT2( inteq,  CHECKED_LOOKUP_INT_LEN(lookup), 9 + LOOKUP_INSERT_TESTS_NUM_ADDITIONAL_VALUES - num_deletions );
    ASSERT1( false,  lookup_max_capacity(lookup) );

    retrieve = 7; ASSERT2( inteq, val_or_m1(lookup_retrieve(lookup, ret, cmp)), -1 );

    /* Delete 7. */
    retrieve = 7; ASSERT2( inteq, val_or_m1(lookup_retrieve(lookup, ret, cmp)), -1 );

    value = 7;
    ASSERT2( objpeq, lookup_delete(lookup, val,  cmp, nd), lookup_val_ref );
    ASSERT2( inteq,  num_deleted, 0 );
    ASSERT2( inteq,  CHECKED_LOOKUP_INT_LEN(lookup), 9 + LOOKUP_INSERT_TESTS_NUM_ADDITIONAL_VALUES - num_deletions );
    ASSERT1( false,  lookup_max_capacity(lookup) );

    retrieve = 7; ASSERT2( inteq, val_or_m1(lookup_retrieve(lookup, ret, cmp)), -1 );


    /* Delete 42. */
    retrieve = 42; ASSERT2( inteq, val_or_m1(lookup_retrieve(lookup, ret, cmp)), 42 );

    value = 42;
    ++num_deletions;
    ++num_deletions;
    ASSERT2( objpeq, lookup_delete(lookup, val,  cmp, nd), lookup_val_ref );
    ASSERT2( inteq,  num_deleted, 2 );
    ASSERT2( inteq,  CHECKED_LOOKUP_INT_LEN(lookup), 9 + LOOKUP_INSERT_TESTS_NUM_ADDITIONAL_VALUES - num_deletions );
    ASSERT1( false,  lookup_max_capacity(lookup) );

    retrieve = 42; ASSERT2( inteq, val_or_m1(lookup_retrieve(lookup, ret, cmp)), -1 );

    value = 42;
    ASSERT2( objpeq, lookup_delete(lookup, val,  cmp, nd), lookup_val_ref );
    ASSERT2( inteq,  num_deleted, 0 );
    ASSERT2( inteq,  CHECKED_LOOKUP_INT_LEN(lookup), 9 + LOOKUP_INSERT_TESTS_NUM_ADDITIONAL_VALUES - num_deletions );
    ASSERT1( false,  lookup_max_capacity(lookup) );

    retrieve = 42; ASSERT2( inteq, val_or_m1(lookup_retrieve(lookup, ret, cmp)), -1 );


    /* Delete 9. */
    retrieve = 9; ASSERT2( inteq, val_or_m1(lookup_retrieve(lookup, ret, cmp)), 9 );

    value = 9;
    ++num_deletions;
    ASSERT2( objpeq, lookup_delete(lookup, val,  cmp, nd), lookup_val_ref );
    ASSERT2( inteq,  num_deleted, 1 );
    ASSERT2( inteq,  CHECKED_LOOKUP_INT_LEN(lookup), 9 + LOOKUP_INSERT_TESTS_NUM_ADDITIONAL_VALUES - num_deletions );
    ASSERT1( false,  lookup_max_capacity(lookup) );

    retrieve = 9; ASSERT2( inteq, val_or_m1(lookup_retrieve(lookup, ret, cmp)), -1 );
  }

  LOOKUP_DEINIT(lookup);

  return result;
}

/* ---------------------------------------------------------------- */

#define LOOKUP_DUPLICATES_TEST_CAPACITY 1024

unit_test_t lookup_duplicates_test =
  {  lookup_duplicates_test_run
  , "lookup_duplicates_test"
  , "Testing lookup insertion and deletion with duplicates."
  };

unit_test_result_t lookup_duplicates_test_run(unit_test_context_t *context)
{
  unit_test_result_t result = assert_success(context);

  typedef int value_type;

  lookup_t lookup_val;

  lookup_t *lookup         = &lookup_val;
  lookup_t *lookup_val_ref = &lookup_val;

  init_memory_methods();

  lookup_init_empty(lookup, sizeof(value_type));

  ENCLOSE()
  {
    static const size_t capacity = LOOKUP_DUPLICATES_TEST_CAPACITY;

    size_t num_deletions = 0;

    value_type value    = -1, *val  = &value;
    value_type retrieve = -1, *ret  = &retrieve;

    int is_duplicate = -1, *dp = &is_duplicate;
    int num_deleted  = -1, *nd = &num_deleted;

    callback_compare_t cmp = callback_compare_int();

    UNUSED10(num_deletions, value, val, retrieve, ret, is_duplicate, dp, num_deleted, nd, cmp);

    /* ---------------------------------------------------------------- */

    ASSERT2( objpeq, LOOKUP_EXPAND(lookup, capacity), lookup_val_ref);
    ASSERT2( inteq,  lookup_capacity(lookup), capacity );
    ASSERT1( true,   lookup_empty(lookup));
    ASSERT1( false,  lookup_max_capacity(lookup) );
    ASSERT2( inteq,  CHECKED_LOOKUP_INT_LEN(lookup), 0 );

    /* ---------------------------------------------------------------- */

    COMPOUND( UNIT_TEST_SKIPPED_CONTINUE );
  }

  LOOKUP_DEINIT(lookup);

  return result;
}
