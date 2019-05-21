#include "use_python.hpp"

module_t * py_init() {
    wchar_t *argv[ARGC] = { ARGV };
    Py_Initialize();
    PySys_SetArgv(ARGC, argv);
    
    PyObject * m = PyUnicode_FromString(MNAME);
    PyObject *p_module = PyImport_Import(m);
    Py_XDECREF(m);
    
    if (p_module == NULL) {
        PyErr_Print();
        return NULL;
    }

    module_t *tdt = new module_t;

    tdt->p_func = PyObject_GetAttrString(p_module, PREDICT_FUNC);
    Py_XDECREF(p_module);

    if (tdt->p_func == NULL) {
        delete tdt;
        PyErr_Print();
        return NULL;
    }
    
    return tdt;
}

void py_fin(module_t *tdt) {
    Py_XDECREF(tdt->p_func);
    delete tdt;
    PyErr_Print();
    Py_Finalize(); // causes problems
}

ssize_t _predict(module_t *tdt, int *_index, double *_doub) {
    PyObject *index = NULL, *doub = NULL,
    *p_data = NULL,
    *p_args = NULL;

    p_args = PyTuple_New(0);

    p_data = PyObject_CallObject(tdt->p_func, p_args);   

    if (p_data == NULL) {
        Py_XDECREF(p_args);
        return -1;    
    }

    Py_XDECREF(p_args);

    if ((index = PyTuple_GetItem(p_data, 0)) == NULL) {
        return -1;
    }

    (*_index) = py_obj_to_int(index);

    Py_XDECREF(index);

    if ((doub = PyTuple_GetItem(p_data, 1)) == NULL) {  
        return -1;
    }

    (*_doub) = py_obj_to_double(doub);

    Py_XDECREF(doub);

    Py_XDECREF(p_data);

    return 0;
}

const int py_obj_to_int(PyObject  * o) {
    if (o == NULL) return 0;

    return PyLong_AsSize_t(o);
}

const double py_obj_to_double(PyObject * o) {
    if (o == NULL) return 0;
    
    return PyFloat_AsDouble(o);
}