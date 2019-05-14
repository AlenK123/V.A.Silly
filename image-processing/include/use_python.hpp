#pragma once
#include <Python.h>

#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

#include "s_except.hpp"

#define ARGC 1
#define MNAME "use_100_model"
#define ARGV (wchar_t*)L"use_100_model"
#define OUT_PATH "../../images/out.jpg"

typedef struct to_delete {
    PyObject *p_module;
    PyObject *p_func;
} to_delete_t;

#define PREDICT_FUNC "_predict"

to_delete_t * py_init();

void py_fin(to_delete_t *tdt);

std::pair<const char*, const double> predict(to_delete_t * tdt, cv::Mat image);

PyObject * _predict(to_delete_t * tdt);

const double py_obj_to_double(PyObject * o);

const char * py_obj_to_string(PyObject * o);
