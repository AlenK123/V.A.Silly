#pragma once
#include <Python.h>
#include <sys/types.h>

#include "s_except.hpp"
#include "catagories.hpp"

#define ARGC 1
#define MNAME "use_100_model"
#define ARGV (wchar_t*)L"use_100_model"
#define OUT_PATH "../../images/out.jpg"
#define PREDICT_FUNC "_predict"

typedef struct to_delete {
    PyObject *p_func;
} module_t;

module_t * py_init();

void py_fin(module_t *tdt);

ssize_t _predict(module_t * tdt, int * _index, double * _doub);

const double py_obj_to_double(PyObject * o);

const int py_obj_to_int(PyObject * o);
