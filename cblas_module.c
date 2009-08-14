/* cblas_example.c */

#include <stdio.h>
#include <stdlib.h>
#include "cblas.h"

#include "php.h"

#include <math.h>
#include <omp.h>
#include <iostream>
#include <stdlib.h>
#include "matrix.h"


ZEND_FUNCTION(pagerank);
zend_function_entry cblas_module_functions[] =
{
    ZEND_FE(pagerank, NULL)
    {NULL, NULL, NULL}
};

zend_module_entry cblas_module_module_entry =
{
    STANDARD_MODULE_HEADER,
    "PageRank Module",
    cblas_module_functions,
    NULL, 
    NULL, 
    NULL, 
    NULL, 
    NULL,
    NO_VERSION_YET,
    STANDARD_MODULE_PROPERTIES
};

#if COMPILE_DL_FIRST_MODULE
ZEND_GET_MODULE(cblas_module)
#endif

ZEND_FUNCTION(pagerank)
{


  int i, j;
  zval *arr, *pi, *v, **data;
  long n;
  double alpha, threshold;
  HashTable *arr_hash;
  HashPosition pointer;
  int array_count;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "aaaldd", &pi, &arr, &v, &n, &alpha, &threshold) == FAILURE) {
    RETURN_NULL();
  }

  //php_printf("n is %d", n);


  int dim = (int)n;

  Matrix A(1, dim), B(dim, dim), C(1, dim);
  Matrix V(1, dim);

  #pragma omp parallel for
  for (int i = 0; i < dim; i++)
    A(0, i) = 0;
  #pragma omp parallel for
  for (int i = 0; i < dim; i++)
    for (int j = 0; j < dim; j++)
      B(i, j) = 0;

  arr_hash = Z_ARRVAL_P(arr);
  array_count = zend_hash_num_elements(arr_hash);

  //php_printf("\nThe array passed contains %d elements.\n", array_count);

  for(zend_hash_internal_pointer_reset_ex(arr_hash, &pointer); zend_hash_get_current_data_ex(arr_hash, (void**) &data, &pointer) == SUCCESS; zend_hash_move_forward_ex(arr_hash, &pointer)) {

    if (Z_TYPE_PP(data) == IS_ARRAY) {
      char *str_index;
      ulong nindex;
      zval **ztype;

      int from = -1, to = -1;
      double weight = -1.0;

      //str_index = NULL;
      //zend_hash_get_current_key_ex(arr_hash, &str_index, NULL, &nindex, 0, &pointer);

      /*
      if (str_index) {
        from = atoi(str_index);
        php_printf("str from: %s ", str_index);
      }
      */

      //if (!str_index) {
      //  php_printf("int from: %d ", nindex);
      //  from = (int)nindex;
      //}
      if (zend_hash_index_find(Z_ARRVAL_PP(data), 0, (void **)&ztype) == SUCCESS) {

        if (Z_TYPE_PP(ztype) == IS_STRING) {
          //php_printf("from %s", Z_STRVAL_PP(ztype));
          //php_printf(" ");
          from = atoi(Z_STRVAL_PP(ztype));
        }
        else {
          RETURN_NULL();
        }
      }

      if (zend_hash_index_find(Z_ARRVAL_PP(data), 1, (void **)&ztype) == SUCCESS) {

        if (Z_TYPE_PP(ztype) == IS_STRING) {
          //php_printf("to %s", Z_STRVAL_PP(ztype));
          //php_printf(" ");
          to = atoi(Z_STRVAL_PP(ztype));
        }
        else {
          RETURN_NULL();
        }
      }

      if (zend_hash_index_find(Z_ARRVAL_PP(data), 2, (void **)&ztype) == SUCCESS) {

        if (Z_TYPE_PP(ztype) == IS_DOUBLE) {
          //php_printf("weight: %f ", Z_DVAL_PP(ztype));
          //php_printf(" ");
          weight = Z_DVAL_PP(ztype);
        }
        else {
          RETURN_NULL();
        }
      }

      if (from >= 0 && from < dim && to >= 0 && to < dim && weight >= 0.0) {
        //php_printf("from:%d to:%d = %f", from, to, weight);
        B(from, to) = weight;
      }

    }
   
    /*
    if (Z_TYPE_PP(data) == IS_STRING) {
        PHPWRITE(Z_STRVAL_PP(data), Z_STRLEN_PP(data));
        php_printf(" ");
    }
    */
  }



  HashTable* pi_hash;
  zval **pi_data;
 
  pi_hash = Z_ARRVAL_P(pi);  

  for(zend_hash_internal_pointer_reset_ex(pi_hash, &pointer); zend_hash_get_current_data_ex(pi_hash, (void**) &pi_data, &pointer) == SUCCESS; zend_hash_move_forward_ex(pi_hash, &pointer)) {


    char *str_index;
    ulong nindex;
    zval **ztype;

    str_index = NULL;
    zend_hash_get_current_key_ex(pi_hash, &str_index, NULL, &nindex, 0, &pointer);
    if (!str_index) {
      //php_printf("pi index: %d ", nindex);
    
      if (Z_TYPE_PP(pi_data) == IS_DOUBLE && (int)nindex < dim) {
        //php_printf("pi value: %f ", Z_DVAL_PP(pi_data));
        //php_printf(" ");
        A(0, (int)nindex) = Z_DVAL_PP(pi_data);
      }
      else {
        RETURN_NULL();
      }
    }
    else {
      RETURN_NULL();
    }
  }
  
  HashTable* v_hash;
  zval **v_data;
 
  v_hash = Z_ARRVAL_P(v);  

  for(zend_hash_internal_pointer_reset_ex(v_hash, &pointer); zend_hash_get_current_data_ex(v_hash, (void**) &v_data, &pointer) == SUCCESS; zend_hash_move_forward_ex(v_hash, &pointer)) {


    char *str_index;
    ulong nindex;
    zval **ztype;

    str_index = NULL;
    zend_hash_get_current_key_ex(v_hash, &str_index, NULL, &nindex, 0, &pointer);
    if (!str_index) {
      //php_printf("v index: %d ", nindex);
    
      if (Z_TYPE_PP(v_data) == IS_DOUBLE && (int)nindex < dim) {
        //php_printf("v value: %f ", Z_DVAL_PP(v_data));
        //php_printf(" ");
        V(0, (int)nindex) = Z_DVAL_PP(v_data);
      }
      else {
        RETURN_NULL();
      }
    }
    else {
      RETURN_NULL();
    }
  }

  #pragma omp parallel for
  for( i = 0; i < dim; i++ ) {
    double sum = 0;
    #pragma omp parallel for
    for( j = 0; j < dim; j++ )
      sum += B(i, j);
    if (sum == 0.0) {
      double fill = 1 / (double)dim;
      #pragma omp parallel for
      for( j = 0; j < dim; j++ )
        B(i, j) = fill;
    } 
    else {
      #pragma omp parallel for
      for( j = 0; j < dim; j++ )
        B(i, j) /= sum;
    }
  }

  /*
  for( i = 0; i < dim; i++ )
    for( j = 0; j < dim; j++ )
      php_printf(" H(%d, %d) = %f\n", i, j, B(i, j));
  */

  int iter = 0;
  double diff = 1.0;
  double beta = 0.0;
  Matrix pre_C(1, dim); 
  Timing mul;

  while (diff >= threshold) {
   
    /* 
    #pragma omp parallel for
    for( i = 0; i < dim; i++ )   
      pre_C(0, i) = A(0, i);
    */

    iter++;

    cblas_dgemm(CblasColMajor, CblasNoTrans, CblasNoTrans, 1, dim, dim, alpha, &*A.begin(), 1, &*B.begin(), dim, beta, &*C.begin(), 1);

    double minus_alpha = 1 - alpha; 
    #pragma omp parallel for
    for( i = 0; i < dim; i++ )    
      C(0, i) += minus_alpha * V(0, i);

    diff = 0;
    #pragma omp parallel for
    for( i = 0; i < dim; i++ ) {   
      diff += fabs(C(0, i) - A(0, i));
      A(0, i) = C(0, i);
      C(0, i) = 0.0;
    }
  }
  
  double time = mul.time();

  /* 
  #pragma omp parallel for
  for( i = 0; i < dim; i++ ) 
    php_printf(" y%d = %f\n", i, C(0, i));

  for( i = 0; i < dim; i++ ) 
    php_printf(" pi%d = %f\n", i, A(0, i));

  for( i = 0; i < dim; i++ ) 
    for( j = 0; j < dim; j++ ) 
      php_printf(" H(%d, %d) = %f\n", i, j, B(i, j));
  */

  //php_printf("time is %f", time);


  zval *mysubarray;

  array_init(return_value);

  ALLOC_INIT_ZVAL(mysubarray);
  array_init(mysubarray);

  #pragma omp parallel for  
  for( i = 0; i < dim; i++ ) 
    add_next_index_double(mysubarray, A(0, i));
  add_next_index_zval(return_value, mysubarray);
  //add_assoc_zval(return_value, "subarray", mysubarray);  
 
  add_next_index_double(return_value, time);
  add_next_index_long(return_value, iter);

}
