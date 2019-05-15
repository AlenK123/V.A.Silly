#include "use_python.hpp"

to_delete_t * py_init() {
    to_delete_t *tdt = new to_delete_t;
    
    wchar_t *argv[ARGC] = { ARGV };
    Py_Initialize();
    PySys_SetArgv(ARGC, argv);
    PyObject * m = PyUnicode_FromString(MNAME);
    tdt->p_module = PyImport_Import(m);

    if (tdt->p_module == NULL) {
        PyErr_Print();
        throw s_except("Python module " MNAME " doesn\'t exist");
    }

    tdt->p_func = PyObject_GetAttrString(tdt->p_module, PREDICT_FUNC);
    
    if (tdt->p_func == NULL) {
        PyErr_Print();
        throw s_except("Function " PREDICT_FUNC " doesn\'t exist");
    }
    
    Py_XDECREF(m);
    return tdt;
}

void py_fin(to_delete_t *tdt) {
    Py_XDECREF(tdt->p_module);
    Py_XDECREF(tdt->p_func);
    delete tdt;
    Py_Finalize(); // causes problems
}

PyObject * _predict(to_delete_t * tdt) {
    PyObject *p_data = NULL, *p_args = NULL;

    p_args = PyTuple_New(0);

    if (p_args == NULL) {
        PyErr_Print();
        return NULL;
    }

    p_data = PyObject_CallObject(tdt->p_func, p_args);   

    if (p_data == NULL) {
        PyErr_Print();
    }
    
    Py_XDECREF(p_args);

    return p_data;
}

std::pair<const std::string, const double> predict(to_delete_t * tdt, cv::Mat image) {

    cv::resize(image, image, cv::Size(32, 32));

    if (cv::imwrite(OUT_PATH, image) == false) {
        throw cv::Exception();
    }

    PyObject * prediction = _predict(tdt);

    if (prediction == NULL) throw s_except("Predict function returned error value");

    if (PyTuple_Size(prediction) < 2) {
        Py_XDECREF(prediction);
        throw s_except("Predict function returned error value");
    }

    PyObject * index = PyTuple_GetItem(prediction, 0);

    PyObject * doub = PyTuple_GetItem(prediction, 1);

    if (index == NULL || doub == NULL) throw s_except("Predict function returned error value");

    const int _index = py_obj_to_int(index);
    
    const double _doub = py_obj_to_double(doub);

    std::string _label;
    {
        const char * catagories[100] = { CATAGORIES };
        _label = std::string(catagories[_index]);
    }

    Py_XDECREF(prediction);
    Py_XDECREF(index);
    Py_XDECREF(doub);

    return std::pair<const std::string, const double>(_label, _doub);
}

const int py_obj_to_int(PyObject  * o) {
    if (o == NULL) throw s_except("Python object is NULL");

    return PyLong_AsSize_t(o);
}

const double py_obj_to_double(PyObject * o) {
    if (o == NULL) throw s_except("Python object is NULL");
    
    return PyFloat_AsDouble(o);
}