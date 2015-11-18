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

/* TODO: tests for bnode_t too, in addition to lookup_t tests. */

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

/* ---------------------------------------------------------------- */

unit_test_t lookup_memory_management_test =
  {  lookup_memory_management_test_run
  , "lookup_memory_management_test"
  , "Testing memory management with lookup values."
  };

unit_test_result_t lookup_memory_management_test_run(unit_test_context_t *context)
{
  unit_test_result_t result = assert_success(context);

  typedef type_t value_type;

  lookup_t lookup_val;
  lookup_t *lookup = &lookup_val;

  init_memory_methods();

  lookup_init_empty(lookup, sizeof(value_type));

  ENCLOSE()
  {
    LASSERT2( inteq,  lookup_capacity(lookup), 0 );
    LASSERT1( true,   lookup_empty(lookup) );

    LASSERT2( objpeq, LOOKUP_EXPAND(lookup, 3), &lookup_val );
    LASSERT2( inteq,  lookup_capacity(lookup), 3 );
    LASSERT1( true,   lookup_empty(lookup) );

    LASSERT2( objpeq, LOOKUP_EXPAND(lookup, 3), &lookup_val );
    LASSERT2( inteq,  lookup_capacity(lookup), 3 );
    LASSERT1( true,   lookup_empty(lookup) );

    LASSERT2( objpeq, LOOKUP_EXPAND(lookup, 1), &lookup_val );
    LASSERT2( inteq,  lookup_capacity(lookup), 3 );
    LASSERT1( true,   lookup_empty(lookup) );

    LASSERT2( objpeq, LOOKUP_EXPAND(lookup, 3), &lookup_val );
    LASSERT2( inteq,  lookup_capacity(lookup), 3 );
    LASSERT1( true,   lookup_empty(lookup) );

    LASSERT2( objpeq, LOOKUP_EXPAND(lookup, 0), &lookup_val );
    LASSERT2( inteq,  lookup_capacity(lookup), 3 );
    LASSERT1( true,   lookup_empty(lookup) );

    LASSERT2( objpeq, LOOKUP_EXPAND(lookup, 3), &lookup_val );
    LASSERT2( inteq,  lookup_capacity(lookup), 3 );
    LASSERT1( true,   lookup_empty(lookup) );

    LASSERT2( objpeq, LOOKUP_EXPAND(lookup, 7), &lookup_val );
    LASSERT2( inteq,  lookup_capacity(lookup), 7 );
    LASSERT1( true,   lookup_empty(lookup) );


    LOOKUP_DEINIT(lookup);
    LASSERT2( inteq, lookup_capacity(lookup), 0 );
    LASSERT1( true,  lookup_empty(lookup));

    lookup_init_empty(lookup, sizeof(value_type));
    LASSERT2( inteq, lookup_capacity(lookup), 0 );
    LASSERT1( true,  lookup_empty(lookup));


    LASSERT2( objpeq,LOOKUP_EXPAND(lookup, 3), &lookup_val);
    LASSERT2( inteq, lookup_capacity(lookup), 3 );
    LASSERT1( true,  lookup_empty(lookup));


    lookup_init_empty(lookup, sizeof(value_type));
    LASSERT2( inteq, lookup_capacity(lookup), 0 );
    LASSERT1( true,  lookup_empty(lookup));


    LASSERT2( objpeq,LOOKUP_EXPAND(lookup, 3), &lookup_val);
    LASSERT2( inteq, lookup_capacity(lookup), 3 );
    LASSERT1( true,  lookup_empty(lookup));
  }

  LOOKUP_DEINIT(lookup);

  return result;
}

/* ---------------------------------------------------------------- */

unit_test_t lookup_insert_test =
  {  lookup_insert_test_run
  , "lookup_insert_test"
  , "Testing lookup insertion."
  };

static int val_or_m1(const int *val)
{
  if (!val)
    return -1;

  return *val;
}

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
    value_type value    = -1, *val  = &value;
    value_type value2   = -1, *val2 = &value2;
    value_type retrieve = -1, *ret  = &retrieve;

    int already_exists = -1, *ae = &already_exists;

    callback_compare_t cmp = callback_compare_int();

    LASSERT2( inteq, lookup_capacity(lookup), 0);
    LASSERT1( true,  lookup_empty(lookup));

    /* ---------------------------------------------------------------- */

    /* 0-size lookup insertion. */

    value = 42;
    LASSERT2( objpeq, lookup_insert(lookup, val, 0, cmp, ae), NULL );
    LASSERT2( inteq,  already_exists, 0 );

    /* ---------------------------------------------------------------- */

    /* 3-size lookup insertion. */

    LASSERT2( objpeq, LOOKUP_EXPAND(lookup, 3), &lookup_val);
    LASSERT2( inteq,  lookup_capacity(lookup), 3 );
    LASSERT1( true,   lookup_empty(lookup));

    LASSERT2( inteq,  lookup_len(lookup), 0);

    retrieve = 42;
    LASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), -1 );

    /* Inserting same value. */

    value = 42;
    LASSERT2( objpeq, lookup_insert(lookup, val,  0, cmp, ae), &lookup_val );
    LASSERT2( inteq,  already_exists, 0 );
    LASSERT2( inteq,  lookup_len(lookup), 1);

    retrieve = 42;
    LASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 42 );
    retrieve = 43;
    LASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), -1 );


    value = 42;
    LASSERT2( objpeq, lookup_insert(lookup, val,  0, cmp, ae), &lookup_val );
    LASSERT2( inteq,  already_exists, 1 );
    LASSERT2( inteq,  lookup_len(lookup), 1);

    retrieve = 42;
    LASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 42 );
    retrieve = 43;
    LASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), -1 );


    value2 = 42;
    LASSERT2( objpeq, lookup_insert(lookup, val2, 0, cmp, ae), &lookup_val );
    LASSERT2( inteq,  already_exists, 1 );
    LASSERT2( inteq,  lookup_len(lookup), 1);

    retrieve = 42;
    LASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 42 );
    retrieve = 43;
    LASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), -1 );


    value2 = 42;
    LASSERT2( objpeq, lookup_insert(lookup, val2, 0, cmp, ae), &lookup_val );
    LASSERT2( inteq,  already_exists, 1 );
    LASSERT2( inteq,  lookup_len(lookup), 1);

    retrieve = 42;
    LASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 42 );
    retrieve = 43;
    LASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), -1 );


    /* Insert new value. */

    value = 43;
    LASSERT2( objpeq, lookup_insert(lookup, val,  0, cmp, ae), &lookup_val );
    LASSERT2( inteq,  already_exists, 0 );
    LASSERT2( inteq,  lookup_len(lookup), 2);

    retrieve = 42;
    LASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 42 );
    retrieve = 43;
    LASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 43 );


    value = 43;
    LASSERT2( objpeq, lookup_insert(lookup, val,  0, cmp, ae), &lookup_val );
    LASSERT2( inteq,  already_exists, 1 );
    LASSERT2( inteq,  lookup_len(lookup), 2);

    retrieve = 42;
    LASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 42 );
    retrieve = 43;
    LASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 43 );


    /* Inserting a multiple. */

    value = 42;
    LASSERT2( objpeq, lookup_insert(lookup, val,  0, cmp, ae), &lookup_val );
    LASSERT2( inteq,  already_exists, 1 );
    LASSERT2( inteq,  lookup_len(lookup), 2);
    LASSERT1( false,  lookup_max_capacity(lookup) );

    retrieve = 42;
    LASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 42 );
    retrieve = 43;
    LASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 43 );


    value = 42;
    LASSERT2( objpeq, lookup_insert(lookup, val,  1, cmp, ae), &lookup_val );
    LASSERT2( inteq,  already_exists, 1 );
    LASSERT2( inteq,  lookup_len(lookup), 3 );
    LASSERT1( true,   lookup_max_capacity(lookup) );

    retrieve = 42;
    LASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 42 );
    retrieve = 43;
    LASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 43 );


    value = 42;
    LASSERT2( objpeq, lookup_insert(lookup, val,  0, cmp, ae), &lookup_val );
    LASSERT2( inteq,  already_exists, 1 );
    LASSERT2( inteq,  lookup_len(lookup), 3 );
    LASSERT1( true,   lookup_max_capacity(lookup) );

    retrieve = 42;
    LASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 42 );
    retrieve = 43;
    LASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 43 );


    /* Exceeding capacity. */

    value = 7;
    LASSERT2( objpeq, lookup_insert(lookup, val,  0, cmp, ae), NULL );
    LASSERT2( inteq,  already_exists, 0 );
    LASSERT2( inteq,  lookup_len(lookup), 3 );
    LASSERT1( true,   lookup_max_capacity(lookup) );

    retrieve = 42;
    LASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 42 );
    retrieve = 43;
    LASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 43 );
    retrieve = 7;
    LASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), -1 );


    value = 7;
    LASSERT2( objpeq, lookup_insert(lookup, val,  1, cmp, ae), NULL );
    LASSERT2( inteq,  already_exists, 0 );
    LASSERT2( inteq,  lookup_len(lookup), 3 );
    LASSERT1( true,   lookup_max_capacity(lookup) );

    retrieve = 42;
    LASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 42 );
    retrieve = 43;
    LASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 43 );
    retrieve = 7;
    LASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), -1 );


    /* Same insertion after expansion. */

    LASSERT2( objpeq, LOOKUP_EXPAND(lookup, 7), &lookup_val);
    LASSERT2( inteq,  lookup_capacity(lookup), 7 );
    LASSERT1( false,  lookup_empty(lookup));
    LASSERT1( false,  lookup_max_capacity(lookup) );
    LASSERT2( inteq,  lookup_len(lookup), 3);


    value = 7;
    LASSERT2( objpeq, lookup_insert(lookup, val,  0, cmp, ae), &lookup_value );
    LASSERT2( inteq,  already_exists, 0 );
    LASSERT2( inteq,  lookup_len(lookup), 4 );
    LASSERT1( false,  lookup_max_capacity(lookup) );

    retrieve = 42;
    LASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 42 );
    retrieve = 43;
    LASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 43 );
    retrieve = 7;
    LASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 7  );


    value = 7;
    LASSERT2( objpeq, lookup_insert(lookup, val,  1, cmp, ae), &lookup_value );
    LASSERT2( inteq,  already_exists, 0 );
    LASSERT2( inteq,  lookup_len(lookup), 5 );
    LASSERT1( false,  lookup_max_capacity(lookup) );

    retrieve = 42;
    LASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 42 );
    retrieve = 43;
    LASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 43 );
    retrieve = 7;
    LASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 7  );

    /* More insertions. */

    retrieve = 42;
    LASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 42 );
    retrieve = 43;
    LASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 43 );
    retrieve = 7;
    LASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 7  );
    retrieve = 8;
    LASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), -1 );
    retrieve = 9;
    LASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), -1 );
    retrieve = 10;
    LASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), -1 );

    /* Insert 8. */
    value = 8;
    LASSERT2( objpeq, lookup_insert(lookup, val,  1, cmp, ae), &lookup_value );
    LASSERT2( inteq,  already_exists, 0 );
    LASSERT2( inteq,  lookup_len(lookup), 6 );
    LASSERT1( false,  lookup_max_capacity(lookup) );

    retrieve = 42;
    LASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 42 );
    retrieve = 43;
    LASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 43 );
    retrieve = 7;
    LASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 7  );
    retrieve = 8;
    LASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 8  );
    retrieve = 9;
    LASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), -1 );
    retrieve = 10;
    LASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), -1 );


    /* Insert 9. */
    value = 9;
    LASSERT2( objpeq, lookup_insert(lookup, val,  1, cmp, ae), &lookup_value );
    LASSERT2( inteq,  already_exists, 0 );
    LASSERT2( inteq,  lookup_len(lookup), 7 );
    LASSERT1( true,   lookup_max_capacity(lookup) );

    retrieve = 42;
    LASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 42 );
    retrieve = 43;
    LASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 43 );
    retrieve = 7;
    LASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 7  );
    retrieve = 8;
    LASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 8  );
    retrieve = 9;
    LASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 9  );
    retrieve = 10;
    LASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), -1 );


    /* Insert 10 beyond capacity. */
    value = 10;
    LASSERT2( objpeq, lookup_insert(lookup, val,  1, cmp, ae), NULL );
    LASSERT2( inteq,  already_exists, 0 );
    LASSERT2( inteq,  lookup_len(lookup), 7 );
    LASSERT1( true,   lookup_max_capacity(lookup) );

    retrieve = 42;
    LASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 42 );
    retrieve = 43;
    LASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 43 );
    retrieve = 7;
    LASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 7  );
    retrieve = 8;
    LASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 8  );
    retrieve = 9;
    LASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 9  );
    retrieve = 10;
    LASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), -1 );


    /* Beyond-capacity already-exists tests. */

    value = 1;
    LASSERT2( objpeq, lookup_insert(lookup, val,  1, cmp, ae), NULL );
    LASSERT2( inteq,  already_exists, 0 );
    LASSERT2( inteq,  lookup_len(lookup), 7 );
    LASSERT1( true,   lookup_max_capacity(lookup) );

    value = 1;
    LASSERT2( objpeq, lookup_insert(lookup, val,  0, cmp, ae), NULL );
    LASSERT2( inteq,  already_exists, 0 );
    LASSERT2( inteq,  lookup_len(lookup), 7 );
    LASSERT1( true,   lookup_max_capacity(lookup) );


    value = 7;
    LASSERT2( objpeq, lookup_insert(lookup, val,  1, cmp, ae), &lookup_val );
    LASSERT2( inteq,  already_exists, 1 );
    LASSERT2( inteq,  lookup_len(lookup), 7 );
    LASSERT1( true,   lookup_max_capacity(lookup) );

    value = 7;
    LASSERT2( objpeq, lookup_insert(lookup, val,  0, cmp, ae), NULL );
    LASSERT2( inteq,  already_exists, 1 );
    LASSERT2( inteq,  lookup_len(lookup), 7 );
    LASSERT1( true,   lookup_max_capacity(lookup) );


    value = 8;
    LASSERT2( objpeq, lookup_insert(lookup, val,  1, cmp, ae), &lookup_val );
    LASSERT2( inteq,  already_exists, 1 );
    LASSERT2( inteq,  lookup_len(lookup), 7 );
    LASSERT1( true,   lookup_max_capacity(lookup) );

    value = 8;
    LASSERT2( objpeq, lookup_insert(lookup, val,  0, cmp, ae), NULL );
    LASSERT2( inteq,  already_exists, 1 );
    LASSERT2( inteq,  lookup_len(lookup), 7 );
    LASSERT1( true,   lookup_max_capacity(lookup) );


    value = 10;
    LASSERT2( objpeq, lookup_insert(lookup, val,  1, cmp, ae), NULL );
    LASSERT2( inteq,  already_exists, 0 );
    LASSERT2( inteq,  lookup_len(lookup), 7 );
    LASSERT1( true,   lookup_max_capacity(lookup) );

    value = 10;
    LASSERT2( objpeq, lookup_insert(lookup, val,  0, cmp, ae), NULL );
    LASSERT2( inteq,  already_exists, 0 );
    LASSERT2( inteq,  lookup_len(lookup), 7 );
    LASSERT1( true,   lookup_max_capacity(lookup) );


    /* Expand and insert 10. */
    LASSERT2( objpeq, LOOKUP_EXPAND(lookup, 3), &lookup_val);
    LASSERT2( inteq,  lookup_capacity(lookup), 7 );
    LASSERT1( false,  lookup_empty(lookup));
    LASSERT1( true,   lookup_max_capacity(lookup) );
    LASSERT2( inteq,  lookup_len(lookup), 7);

    LASSERT2( objpeq, LOOKUP_EXPAND(lookup, 0), &lookup_val);
    LASSERT2( inteq,  lookup_capacity(lookup), 7 );
    LASSERT1( false,  lookup_empty(lookup));
    LASSERT1( true,   lookup_max_capacity(lookup) );
    LASSERT2( inteq,  lookup_len(lookup), 7);

    LASSERT2( objpeq, LOOKUP_EXPAND(lookup, 7), &lookup_val);
    LASSERT2( inteq,  lookup_capacity(lookup), 7 );
    LASSERT1( false,  lookup_empty(lookup));
    LASSERT1( true,   lookup_max_capacity(lookup) );
    LASSERT2( inteq,  lookup_len(lookup), 7);

    LASSERT2( objpeq, LOOKUP_EXPAND(lookup, 8), &lookup_val);
    LASSERT2( inteq,  lookup_capacity(lookup), 8 );
    LASSERT1( false,  lookup_empty(lookup));
    LASSERT1( false,  lookup_max_capacity(lookup) );
    LASSERT2( inteq,  lookup_len(lookup), 7);


    value = 10;
    LASSERT2( objpeq, lookup_insert(lookup, val,  1, cmp, ae), &lookup_val );
    LASSERT2( inteq,  already_exists, 0 );
    LASSERT2( inteq,  lookup_len(lookup), 7 );
    LASSERT1( true,   lookup_max_capacity(lookup) );

    retrieve = 42; LASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 42 );
    retrieve = 43; LASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 43 );
    retrieve = 7;  LASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 7  );
    retrieve = 8;  LASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 8  );
    retrieve = 9;  LASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 9  );
    retrieve = 10; LASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), 10 );
    retrieve = 0;  LASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), -1 );
    retrieve = 1;  LASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), -1 );
    retrieve = 2;  LASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), -1 );
    retrieve = 3;  LASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), -1 );
    retrieve = 4;  LASSERT2( inteq,  val_or_m1(lookup_retrieve(lookup, ret, cmp)), -1 );

    /* TODO: shrinking. */
#ifdef TODO
    #error "TODO: test with shrinking"
#endif /* #ifdef TODO */
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

static int val_or_m1(const int *val)
{
  if (!val)
    return -1;

  return *val;
}

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
  }

  LOOKUP_DEINIT(lookup);

  return result;
}
