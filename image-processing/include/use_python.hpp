#pragma once
#include <Python.h>
#include <sys/types.h>
#include <string.h>

#include "s_except.hpp"
#include "catagories.hpp"

#define ARGC 2
#define JSONCONF L"../../.config/config.json"
#define MODEL_NAME L"use_model"

#define MODULE_PATH L"../../NN"
#define PREDICT_FUNC "predict"

#define MNAME "use_model"

typedef struct module {
    PyObject *p_func;
} module_t;

module_t * py_init();

void py_fin(module_t *tdt);

ssize_t _predict(module_t *tdt, int *_index, double *_doub, const u_char * data);

const double py_obj_to_double(PyObject * o);

const int py_obj_to_int(PyObject * o);
