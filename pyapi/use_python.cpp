#include <Python.h>
#include <iostream>
#include <numpy/ndarrayobject.h>
#include <string>

#include "opencv2/highgui.hpp"
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"

#define ARGC 1
#define ARGV L"use_model"
#define FNAME "use_model"
typedef PyObject* py_obj;

void py_init() {
    wchar_t *argv[ARGC] = { ARGV };
    Py_Initialize();
    PySys_SetArgv(ARGC, argv);
}

py_obj py_func(const char *fname, int argc, cv::Mat image, char **argv) {
    py_obj p_val = nullptr,
    p_module = nullptr,
    p_func = nullptr,
    p_data = nullptr,
    p_args = nullptr;

    p_module = PyImport_AddModuleObject(PyUnicode_FromString(FNAME));

    p_func = PyObject_GetAttrString(p_module, fname);
    
    Py_XDECREF(p_module);

    p_args = PyTuple_New(1);
    
    npy_intp dimensions[3] = { image.rows, image.cols, image.channels() };
    
    p_val = PyArray_SimpleNewFromData(image.dims + 1, (npy_intp*)dimensions, NPY_UINT8, image.data);
    
    std::cerr << "did not sigsegv" << std::endl;
    
    PyTuple_SetItem(p_args, 0, p_val);
    
    p_data = PyObject_CallObject(p_func, p_args);
    
    Py_XDECREF(p_val);
    Py_XDECREF(p_args);
    Py_XDECREF(p_func);

    if (p_data == nullptr) std::cerr << "error" << std::endl;

    return p_data;
}

int main(int argc, char ** argv) {
    if (argc < 2) {
        std::cerr << "image stupid" << std::endl;
        return EXIT_FAILURE;
    }

    py_init();

    cv::Mat image = cv::imread(argv[1]);
    char **_argv;

    try {
        py_obj a = py_func("check", 0, image, _argv);

        std::cout << (a == nullptr) << std::endl;

        Py_XDECREF(a);
    }  
    catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }

    Py_Finalize();
    return 0;
}