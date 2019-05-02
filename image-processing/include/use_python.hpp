#pragma once
#include <iostream>
#include "Python.h"

#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

#define ARGC 1
#define ARGV (wchar_t*)L"use_model"
#define FNAME "use_model"

typedef struct to_delete {
    PyObject *p_module;
    PyObject *p_func;
} to_delete_t;

#define PREDICT_FUNC "_predict"

to_delete_t * py_init();

void py_fin(to_delete_t *tdt);

PyObject * predict(to_delete_t * tdt, cv::Mat image);

const char * py_obj_to_string(PyObject * o);
