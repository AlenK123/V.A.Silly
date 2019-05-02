#include <iostream>
#include <string>

#include <Python.h>

#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

#define ARGC 1
#define ARGV L"use_model"
#define FNAME "use_model"

void py_init() {
    wchar_t *argv[ARGC] = { ARGV };
    Py_Initialize();
    PySys_SetArgv(ARGC, argv);
}

PyObject * py_func(char *fname, int argc, cv::Mat image, char **argv) {
    PyObject * p_val = NULL,
    *p_module = NULL,
    *p_func = NULL,
    *p_data = NULL,
    *p_args = NULL;

    p_module = PyImport_Import(PyUnicode_FromString(FNAME));

    if (p_module == NULL) { 
        std::cerr << "err importing module" << std::endl;
    }

    p_func = PyObject_GetAttrString(p_module, (char*)fname);

    if (p_func == NULL) { 
        std::cerr << "err getting function" << std::endl;
    }

    p_args = PyTuple_New(1);

    cv::resize(image, image, cv::Size(32, 32));
    
    p_val = Py_BuildValue("(s)", "argument");
    
    PyTuple_SetItem(p_args, 0, p_val);

    if (p_func != NULL) {
        
    }

    p_data = PyObject_CallObject(p_func, p_args);   
    
    Py_XDECREF(p_module);
    Py_XDECREF(p_val);
    Py_XDECREF(p_args);
    Py_XDECREF(p_func);

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
        PyObject * a = py_func("_try", 0, image, _argv);

        PyObject* str = PyUnicode_AsEncodedString(a, "utf-8", "~E~");

        const char *bytes = PyBytes_AS_STRING(str);

        std::cout << bytes << std::endl;

        Py_XDECREF(a);
    }  
    catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }

    Py_Finalize();
    return 0;
}