#include <ruby.h>
#include <gsl/gsl_matrix.h>

int gsl_matrix_nmf(gsl_matrix *v, int cols, gsl_matrix **w, gsl_matrix **h);
double difcost(gsl_matrix *a, gsl_matrix *b);

VALUE mNMF;
extern VALUE cgsl_matrix;

/*
 * call-seq:
 *   nmf(GSL::Matrix, columns) -> [GSL::Matrix, GSL::Matrix]
 *
 * Calculates the NMF of the given +matrix+, returns the W and H matrices
 */
static VALUE nmf_wrap(VALUE obj, VALUE matrix, VALUE cols)
{
  gsl_matrix *w, *h, *m;
  unsigned int c;
  VALUE arr;

  if ( ! FIXNUM_P(cols) || (c=NUM2INT(cols)) <= 0 ) {
    rb_raise(rb_eArgError, "Number of columns should be a positive integer.");
  }
  arr = rb_ary_new2(2);
  Data_Get_Struct(matrix, gsl_matrix, m);

  /* compute the NMF */
  gsl_matrix_nmf(m, c, &w, &h);

  rb_ary_push(arr, Data_Wrap_Struct(cgsl_matrix, 0, gsl_matrix_free, w));
  rb_ary_push(arr, Data_Wrap_Struct(cgsl_matrix, 0, gsl_matrix_free, h));

  return arr;
}

/*
 * call-seq:
 *   difcost(GSL::Matrix, GSL::Matrix) -> Float
 *
 * Calculates the geometric distance between two matrices
 */
static VALUE difcost_wrap(VALUE obj, VALUE matrix1, VALUE matrix2)
{
  gsl_matrix *m1, *m2;
  Data_Get_Struct(matrix1, gsl_matrix, m1);
  Data_Get_Struct(matrix2, gsl_matrix, m2);
  return rb_float_new(difcost(m1, m2));
}

void Init_gsl_matrix_nmf(void) {
/*
  cgsl = rb_define_module("GSL");
  cgsl_matrix = rb_define_module_under(cgsl, "Matrix");
  cgsl_matrix = rb_const_get(rb_const_get(rb_cObject, rb_intern("GSL")),
      rb_intern("Matrix"));
*/

  mNMF = rb_define_module_under(cgsl_matrix, "NMF");

  rb_define_singleton_method(mNMF, "nmf", nmf_wrap, 2);
  rb_define_singleton_method(mNMF, "difcost", difcost_wrap, 2);
}
