#include "use_python.hpp"

to_delete_t * py_init() {
    to_delete_t *tdt = new to_delete_t;
    
    wchar_t *argv[ARGC] = { ARGV };
    Py_Initialize();
    PySys_SetArgv(ARGC, argv);

    tdt->p_module = PyImport_Import(PyUnicode_FromString(FNAME));

    if (tdt->p_module == NULL) {
        PyErr_Print();
        throw std::exception();
    }

    tdt->p_func = PyObject_GetAttrString(tdt->p_module, PREDICT_FUNC);
    
    if (tdt->p_func == NULL) {
        PyErr_Print();
        throw std::exception();
    }

    return tdt;
}

void py_fin(to_delete_t *tdt) {
    Py_XDECREF(tdt->p_module);
    Py_XDECREF(tdt->p_func);
    delete tdt;
    Py_Finalize();
}

PyObject * predict(to_delete_t * tdt, cv::Mat image) {
    PyObject * p_val = NULL,
    *p_data = NULL,
    *p_args = NULL;

    p_args = PyTuple_New(1);

    cv::resize(image, image, cv::Size(32, 32));

    if (cv::imwrite(OUT_PATH, image) == false) {
        throw std::exception();
    }

    p_val = Py_BuildValue("(s)", "argument");
    
    if (p_val == NULL) {
        PyErr_Print();
        return NULL;
    }

    PyTuple_SetItem(p_args, 0, p_val);

    p_data = PyObject_CallObject(tdt->p_func, p_args);   

    if (p_data == NULL) {
        PyErr_Print();
    }

    Py_XDECREF(p_val);
    Py_XDECREF(p_args);

    return p_data;
}

const char * py_obj_to_string(PyObject * o) {

    PyObject* str = PyUnicode_AsEncodedString(o, "utf-8", "~E~");
    
    if (str == NULL) throw std::exception();
    
    const char *bytes = PyBytes_AS_STRING(str);

    if (bytes == NULL) throw std::exception();

    Py_XDECREF(str);

    return bytes;
}
