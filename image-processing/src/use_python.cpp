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
    Py_Finalize();
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

std::pair<const char*, const double> predict(to_delete_t * tdt, cv::Mat image) {

    cv::resize(image, image, cv::Size(32, 32));

    if (cv::imwrite(OUT_PATH, image) == false) {
        throw cv::Exception();
    }

    PyObject * prediction = _predict(tdt);

    if (prediction == NULL) throw s_except("Predict function returned error value");

    PyObject * str = PyTuple_GetItem(prediction, 0);

    PyObject * doub = PyTuple_GetItem(prediction, 1);

    if (str == NULL || doub == NULL) throw s_except("Predict function returned error value");

    const char * _str = py_obj_to_string(str);
    
    const double _doub = py_obj_to_double(doub);

    Py_XDECREF(prediction);
    Py_XDECREF(str);
    Py_XDECREF(doub);

    return std::pair<const char *, const double>(_str, _doub);
}

const char * py_obj_to_string(PyObject * o) {

    if (o == NULL) s_except("Python object is NULL");

    PyObject* str = PyUnicode_AsEncodedString(o, "utf-8", "~E~");
    
    if (str == NULL) throw s_except("Python object not found");
    
    const char *bytes = PyBytes_AsString(str);

    if (bytes == NULL) {
        Py_XDECREF(str);    
        throw s_except("Could not convert str to bytes");
    }

    Py_XDECREF(str);

    return bytes;
}

const double py_obj_to_double(PyObject * o) {
    if (o == NULL) s_except("Python object is NULL");
    
    return PyFloat_AsDouble(o);
}