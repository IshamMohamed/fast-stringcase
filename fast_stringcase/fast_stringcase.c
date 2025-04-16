#include <Python.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h> // For malloc, free

// Forward declarations for functions that call each other
static PyObject* snakecase(PyObject* self, PyObject* args);
static PyObject* camelcase(PyObject* self, PyObject* args);
static PyObject* capitalcase(PyObject* self, PyObject* args);
static PyObject* lowercase(PyObject* self, PyObject* args);
static PyObject* uppercase(PyObject* self, PyObject* args);
static PyObject* trimcase(PyObject* self, PyObject* args);

// Helper to get C string from Python Unicode object safely
// Returns a newly allocated string that the caller must free, or NULL on error.
static char* get_string_from_pyobject(PyObject* py_obj) {
    if (!PyUnicode_Check(py_obj)) {
        PyErr_SetString(PyExc_TypeError, "Expected a string object");
        return NULL;
    }
    Py_ssize_t size;
    const char* temp_str = PyUnicode_AsUTF8AndSize(py_obj, &size);
    if (!temp_str) {
        // Exception set by PyUnicode_AsUTF8AndSize
        return NULL;
    }
    // Allocate memory and copy the string
    char* c_str = (char*)malloc(size + 1);
    if (!c_str) {
        PyErr_SetString(PyExc_MemoryError, "Failed to allocate memory for string copy");
        return NULL;
    }
    memcpy(c_str, temp_str, size + 1); // Copy null terminator too
    return c_str;
}

// --- Function Implementations based on stringcase_base.py ---

// Equivalent to Python's lowercase(string)
static PyObject* lowercase(PyObject* self, PyObject* args) {
    const char* string;
    if (!PyArg_ParseTuple(args, "s", &string)) { return NULL; }

    Py_ssize_t input_len = strlen(string);
    char* result_str = (char*)malloc(input_len + 1);
    if (!result_str) {
        PyErr_SetString(PyExc_MemoryError, "Failed to allocate memory for lowercase");
        return NULL;
    }

    for (Py_ssize_t i = 0; i < input_len; i++) {
        result_str[i] = tolower((unsigned char)string[i]);
    }
    result_str[input_len] = '\0';

    PyObject* py_result = PyUnicode_FromString(result_str);
    free(result_str);
    return py_result; // PyUnicode_FromString sets error on failure
}

// Equivalent to Python's uppercase(string)
static PyObject* uppercase(PyObject* self, PyObject* args) {
    const char* string;
    if (!PyArg_ParseTuple(args, "s", &string)) { return NULL; }

    Py_ssize_t input_len = strlen(string);
    char* result_str = (char*)malloc(input_len + 1);
    if (!result_str) {
        PyErr_SetString(PyExc_MemoryError, "Failed to allocate memory for uppercase");
        return NULL;
    }

    for (Py_ssize_t i = 0; i < input_len; i++) {
        result_str[i] = toupper((unsigned char)string[i]);
    }
    result_str[input_len] = '\0';

    PyObject* py_result = PyUnicode_FromString(result_str);
    free(result_str);
    return py_result; // PyUnicode_FromString sets error on failure
}

// Equivalent to Python's snakecase(string)
// string = re.sub(r"[\-\.\s]", '_', str(string))
// return lowercase(string[0]) + re.sub(r"[A-Z]", lambda matched: '_' + lowercase(matched.group(0)), string[1:])
static PyObject* snakecase(PyObject* self, PyObject* args) {
    const char* string;
    if (!PyArg_ParseTuple(args, "s", &string)) { return NULL; }

    Py_ssize_t input_len = strlen(string);
    if (input_len == 0) { return PyUnicode_FromString(""); }

    // Allocate for intermediate and final strings
    char* temp_str = (char*)malloc(input_len + 1); // Max length for temp_str is input_len
    char* result_str = (char*)malloc(input_len * 2 + 1); // Max length for result_str is input_len * 2

    if (!temp_str || !result_str) {
        free(temp_str); free(result_str);
        PyErr_SetString(PyExc_MemoryError, "Failed to allocate memory for snakecase");
        return NULL;
    }

    // Pass 1: Replace separators '-', '.', ' ' with '_'
    Py_ssize_t temp_idx = 0;
    for (Py_ssize_t i = 0; i < input_len; ++i) {
        char current_char = string[i];
        if (current_char == '-' || current_char == '.' || current_char == ' ') {
            temp_str[temp_idx++] = '_';
        } else {
            temp_str[temp_idx++] = current_char; // Keep others including existing '_'
        }
    }
    temp_str[temp_idx] = '\0';

    // Pass 2: Handle case and prepend underscores for uppercase letters after index 0
    Py_ssize_t result_idx = 0;
    if (temp_idx > 0) {
        result_str[result_idx++] = tolower((unsigned char)temp_str[0]); // Lowercase first char
        for (Py_ssize_t i = 1; i < temp_idx; ++i) {
            char current_char = temp_str[i];
            if (isupper(current_char)) {
                result_str[result_idx++] = '_';
                result_str[result_idx++] = tolower((unsigned char)current_char);
            } else {
                result_str[result_idx++] = current_char;
            }
        }
    }
    result_str[result_idx] = '\0';

    free(temp_str);
    PyObject* py_result = PyUnicode_FromString(result_str);
    free(result_str);
    return py_result; // PyUnicode_FromString sets error on failure
}

// Equivalent to Python's camelcase(string)
// string = re.sub(r"\w[\s\W]+\w", '', str(string)) # Skipping this complex regex part
// return lowercase(string[0]) + re.sub(r"[\-_\.\s]([a-z])", lambda matched: uppercase(matched.group(1)), string[1:])
static PyObject* camelcase(PyObject* self, PyObject* args) {
    const char* string;
    if (!PyArg_ParseTuple(args, "s", &string)) { return NULL; }

    Py_ssize_t input_len = strlen(string);
    if (input_len == 0) { return PyUnicode_FromString(""); }

    // Allocate memory. Max length is the same as input.
    char* result_str = (char*)malloc(input_len + 1);
    if (!result_str) {
        PyErr_SetString(PyExc_MemoryError, "Failed to allocate memory for camelcase");
        return NULL;
    }

    Py_ssize_t result_idx = 0;
    int capitalize_next = 0; // Flag to capitalize the next *alphanumeric* character

    // Process the string character by character
    for (Py_ssize_t i = 0; i < input_len; ++i) {
        char current_char = string[i];

        if (current_char == '-' || current_char == '_' || current_char == '.' || current_char == ' ') {
            capitalize_next = 1; // Mark next alphanumeric to be capitalized
        } else if (isalnum(current_char)) {
            if (result_idx == 0) { // First character of the result string
                result_str[result_idx++] = tolower((unsigned char)current_char);
            } else if (capitalize_next) {
                result_str[result_idx++] = toupper((unsigned char)current_char);
                capitalize_next = 0; // Reset flag
            } else {
                // Preserve existing case if not after separator
                result_str[result_idx++] = current_char;
            }
        }
        // Ignore other characters
    }
    result_str[result_idx] = '\0';

    PyObject* py_result = PyUnicode_FromString(result_str);
    free(result_str);
    return py_result; // PyUnicode_FromString sets error on failure
}

// Equivalent to Python's capitalcase(string)
// return uppercase(string[0]) + string[1:]
static PyObject* capitalcase(PyObject* self, PyObject* args) {
    const char* string;
    if (!PyArg_ParseTuple(args, "s", &string)) { return NULL; }

    Py_ssize_t input_len = strlen(string);
    if (input_len == 0) { return PyUnicode_FromString(""); }

    char* result_str = (char*)malloc(input_len + 1);
    if (!result_str) {
        PyErr_SetString(PyExc_MemoryError, "Failed to allocate memory for capitalcase");
        return NULL;
    }

    result_str[0] = toupper((unsigned char)string[0]);
    if (input_len > 1) {
        memcpy(result_str + 1, string + 1, input_len - 1);
    }
    result_str[input_len] = '\0';

    PyObject* py_result = PyUnicode_FromString(result_str);
    free(result_str);
    return py_result; // PyUnicode_FromString sets error on failure
}

// Equivalent to Python's pascalcase(string)
// return capitalcase(camelcase(string))
static PyObject* pascalcase(PyObject* self, PyObject* args) {
    PyObject* camel_result_py = camelcase(self, args);
    if (!camel_result_py) { return NULL; }

    char* camel_result_c = get_string_from_pyobject(camel_result_py);
    Py_DECREF(camel_result_py); // Decref intermediate Python object
    if (!camel_result_c) { return NULL; }

    Py_ssize_t len = strlen(camel_result_c);
    if (len > 0) {
        camel_result_c[0] = toupper((unsigned char)camel_result_c[0]);
    }

    PyObject* final_result = PyUnicode_FromString(camel_result_c);
    free(camel_result_c); // Free the C string copy
    return final_result; // PyUnicode_FromString sets error on failure
}

// Equivalent to Python's constcase(string)
// return uppercase(snakecase(string))
static PyObject* constcase(PyObject* self, PyObject* args) {
    PyObject* snake_result_py = snakecase(self, args);
    if (!snake_result_py) { return NULL; }

    // Convert snake_result to uppercase
    PyObject* upper_result = PyObject_CallMethod(snake_result_py, "upper", NULL);
    Py_DECREF(snake_result_py); // Decref intermediate object

    return upper_result; // Returns NULL if .upper() fails
}

// Equivalent to Python's pathcase(string)
// string = snakecase(string)
// return re.sub(r"_", "/", string)
static PyObject* pathcase(PyObject* self, PyObject* args) {
    PyObject* snake_result_py = snakecase(self, args);
    if (!snake_result_py) { return NULL; }

    char* snake_result_c = get_string_from_pyobject(snake_result_py);
    Py_DECREF(snake_result_py);
    if (!snake_result_c) { return NULL; }

    Py_ssize_t len = strlen(snake_result_c);
    char* result_str = (char*)malloc(len + 1);
    if (!result_str) {
        free(snake_result_c);
        PyErr_SetString(PyExc_MemoryError, "Failed to allocate memory for pathcase");
        return NULL;
    }

    for (Py_ssize_t i = 0; i < len; ++i) {
        result_str[i] = (snake_result_c[i] == '_') ? '/' : snake_result_c[i];
    }
    result_str[len] = '\0';

    free(snake_result_c);
    PyObject* final_result = PyUnicode_FromString(result_str);
    free(result_str);
    return final_result; // PyUnicode_FromString sets error on failure
}

// Equivalent to Python's backslashcase(string)
// str1 = re.sub(r"_", r"\\", snakecase(string))
static PyObject* backslashcase(PyObject* self, PyObject* args) {
    PyObject* snake_result_py = snakecase(self, args);
    if (!snake_result_py) { return NULL; }

    char* snake_result_c = get_string_from_pyobject(snake_result_py);
    Py_DECREF(snake_result_py);
    if (!snake_result_c) { return NULL; }

    Py_ssize_t len = strlen(snake_result_c);
    // Need potentially more space if replacing '_' with '\\'
    char* result_str = (char*)malloc(len * 2 + 1); // Worst case: all '_'
    if (!result_str) {
        free(snake_result_c);
        PyErr_SetString(PyExc_MemoryError, "Failed to allocate memory for backslashcase");
        return NULL;
    }

    Py_ssize_t result_idx = 0;
    for (Py_ssize_t i = 0; i < len; ++i) {
        if (snake_result_c[i] == '_') {
            result_str[result_idx++] = '\\';
            // result_str[result_idx++] = '\\'; // C requires escaping the backslash literal
        } else {
            result_str[result_idx++] = snake_result_c[i];
        }
    }
    result_str[result_idx] = '\0';

    free(snake_result_c);
    PyObject* final_result = PyUnicode_FromString(result_str);
    free(result_str);
    return final_result; // PyUnicode_FromString sets error on failure
}

// Equivalent to Python's spinalcase(string)
// return re.sub(r"_", "-", snakecase(string))
static PyObject* spinalcase(PyObject* self, PyObject* args) {
    PyObject* snake_result_py = snakecase(self, args);
    if (!snake_result_py) { return NULL; }

    char* snake_result_c = get_string_from_pyobject(snake_result_py);
    Py_DECREF(snake_result_py);
    if (!snake_result_c) { return NULL; }

    Py_ssize_t len = strlen(snake_result_c);
    char* result_str = (char*)malloc(len + 1);
    if (!result_str) {
        free(snake_result_c);
        PyErr_SetString(PyExc_MemoryError, "Failed to allocate memory for spinalcase");
        return NULL;
    }

    for (Py_ssize_t i = 0; i < len; ++i) {
        result_str[i] = (snake_result_c[i] == '_') ? '-' : snake_result_c[i];
    }
    result_str[len] = '\0';

    free(snake_result_c);
    PyObject* final_result = PyUnicode_FromString(result_str);
    free(result_str);
    return final_result; // PyUnicode_FromString sets error on failure
}

// Equivalent to Python's dotcase(string)
// return re.sub(r"_", ".", snakecase(string))
static PyObject* dotcase(PyObject* self, PyObject* args) {
    PyObject* snake_result_py = snakecase(self, args);
    if (!snake_result_py) { return NULL; }

    char* snake_result_c = get_string_from_pyobject(snake_result_py);
    Py_DECREF(snake_result_py);
    if (!snake_result_c) { return NULL; }

    Py_ssize_t len = strlen(snake_result_c);
    char* result_str = (char*)malloc(len + 1);
    if (!result_str) {
        free(snake_result_c);
        PyErr_SetString(PyExc_MemoryError, "Failed to allocate memory for dotcase");
        return NULL;
    }

    for (Py_ssize_t i = 0; i < len; ++i) {
        result_str[i] = (snake_result_c[i] == '_') ? '.' : snake_result_c[i];
    }
    result_str[len] = '\0';

    free(snake_result_c);
    PyObject* final_result = PyUnicode_FromString(result_str);
    free(result_str);
    return final_result; // PyUnicode_FromString sets error on failure
}

// Equivalent to Python's titlecase(string)
// return ' '.join([capitalcase(word) for word in snakecase(string).split("_")])
static PyObject* titlecase(PyObject* self, PyObject* args) {
    PyObject* snake_result_py = snakecase(self, args);
    if (!snake_result_py) { return NULL; }

    char* snake_result_c = get_string_from_pyobject(snake_result_py);
    Py_DECREF(snake_result_py);
    if (!snake_result_c) { return NULL; }

    Py_ssize_t len = strlen(snake_result_c);
    // Result length can be same as snake_case result
    char* result_str = (char*)malloc(len + 1);
    if (!result_str) {
        free(snake_result_c);
        PyErr_SetString(PyExc_MemoryError, "Failed to allocate memory for titlecase");
        return NULL;
    }

    Py_ssize_t result_idx = 0;
    int capitalize_next = 1; // Capitalize first letter of first word

    for (Py_ssize_t i = 0; i < len; ++i) {
        char current_char = snake_result_c[i];
        if (current_char == '_') {
            result_str[result_idx++] = ' '; // Replace underscore with space
            capitalize_next = 1; // Capitalize next letter
        } else {
            if (capitalize_next) {
                result_str[result_idx++] = toupper((unsigned char)current_char);
                capitalize_next = 0;
            } else {
                // Title case usually lowercases subsequent letters within a word
                result_str[result_idx++] = tolower((unsigned char)current_char);
            }
        }
    }
    result_str[result_idx] = '\0';

    free(snake_result_c);
    PyObject* final_result = PyUnicode_FromString(result_str);
    free(result_str);
    return final_result; // PyUnicode_FromString sets error on failure
}

// Equivalent to Python's trimcase(string)
// return str(string).strip()
static PyObject* trimcase(PyObject* self, PyObject* args) {
    const char* string;
    if (!PyArg_ParseTuple(args, "s", &string)) { return NULL; }

    Py_ssize_t len = strlen(string);
    Py_ssize_t start = 0;
    while (start < len && isspace((unsigned char)string[start])) {
        start++;
    }

    // If the entire string is whitespace
    if (start == len) {
        return PyUnicode_FromString("");
    }

    Py_ssize_t end = len - 1;
    while (end > start && isspace((unsigned char)string[end])) {
        end--;
    }

    Py_ssize_t trimmed_len = end - start + 1;
    char* result_str = (char*)malloc(trimmed_len + 1);
    if (!result_str) {
        PyErr_SetString(PyExc_MemoryError, "Failed to allocate memory for trimcase");
        return NULL;
    }

    memcpy(result_str, string + start, trimmed_len);
    result_str[trimmed_len] = '\0';

    PyObject* py_result = PyUnicode_FromString(result_str);
    free(result_str);
    return py_result; // PyUnicode_FromString sets error on failure
}

// Equivalent to Python's alphanumcase(string)
// return re.sub("\W+", "", string)
static PyObject* alphanumcase(PyObject* self, PyObject* args) {
    const char* string;
    if (!PyArg_ParseTuple(args, "s", &string)) { return NULL; }

    Py_ssize_t input_len = strlen(string);
    // Result length <= input length
    char* result_str = (char*)malloc(input_len + 1);
    if (!result_str) {
        PyErr_SetString(PyExc_MemoryError, "Failed to allocate memory for alphanumcase");
        return NULL;
    }

    Py_ssize_t result_idx = 0;
    for (Py_ssize_t i = 0; i < input_len; i++) {
        if (isalnum((unsigned char)string[i])) {
            result_str[result_idx++] = string[i];
        }
    }
    result_str[result_idx] = '\0';

    PyObject* py_result = PyUnicode_FromString(result_str);
    free(result_str);
    return py_result; // PyUnicode_FromString sets error on failure
}

// Equivalent to Python's sentencecase(string) - Simplified C implementation
// Python:
// joiner = ' '
// string = re.sub(r"[\-_\.\s]", joiner, str(string))
// return capitalcase(trimcase(re.sub(r"[A-Z]", lambda matched: joiner + lowercase(matched.group(0)), string)))
// C Implementation (approximated):
// 1. Replace separators with space.
// 2. Insert space before uppercase letters (except first char of string).
// 3. Trim result.
// 4. Capitalize first letter of trimmed result.
static PyObject* sentencecase(PyObject* self, PyObject* args) {
    const char* string;
    if (!PyArg_ParseTuple(args, "s", &string)) { return NULL; }

    Py_ssize_t input_len = strlen(string);
    if (input_len == 0) { return PyUnicode_FromString(""); }

    // Allocate generously for intermediate steps (spaces can be added)
    char* buffer1 = (char*)malloc(input_len * 2 + 1); // For separator replacement + spaces before caps
    char* buffer2 = (char*)malloc(input_len * 2 + 1); // For trimming
    if (!buffer1 || !buffer2) {
        free(buffer1); free(buffer2);
        PyErr_SetString(PyExc_MemoryError, "Failed to allocate memory for sentencecase");
        return NULL;
    }

    // Step 1 & 2: Replace separators and insert space before caps
    Py_ssize_t idx1 = 0;
    for (Py_ssize_t i = 0; i < input_len; ++i) {
        char current_char = string[i];
        if (current_char == '-' || current_char == '_' || current_char == '.' || current_char == ' ') {
            // Add space only if the last char wasn't already a space
            if (idx1 > 0 && buffer1[idx1 - 1] != ' ') {
                buffer1[idx1++] = ' ';
            }
        } else if (isupper(current_char)) {
            // Add space before uppercase if not the first char and previous wasn't space
            if (idx1 > 0 && buffer1[idx1 - 1] != ' ') {
                 buffer1[idx1++] = ' ';
            }
            buffer1[idx1++] = tolower((unsigned char)current_char); // Add lowercase version
        } else {
             buffer1[idx1++] = tolower((unsigned char)current_char); // Add other chars as lowercase
        }
    }
    buffer1[idx1] = '\0';

    // Step 3: Trim buffer1 into buffer2
    Py_ssize_t len1 = idx1;
    Py_ssize_t start = 0;
    while (start < len1 && isspace((unsigned char)buffer1[start])) { start++; }

    if (start == len1) { // All whitespace
        free(buffer1); free(buffer2);
        return PyUnicode_FromString("");
    }

    Py_ssize_t end = len1 - 1;
    while (end > start && isspace((unsigned char)buffer1[end])) { end--; }

    Py_ssize_t trimmed_len = end - start + 1;
    memcpy(buffer2, buffer1 + start, trimmed_len);
    buffer2[trimmed_len] = '\0';

    // Step 4: Capitalize first letter of buffer2
    if (trimmed_len > 0) {
        buffer2[0] = toupper((unsigned char)buffer2[0]);
    }

    // Create final Python object
    PyObject* py_result = PyUnicode_FromString(buffer2);

    free(buffer1);
    free(buffer2);
    return py_result; // PyUnicode_FromString sets error on failure
}


// --- Module Definition ---

static PyMethodDef StringCaseMethods[] = {
    {"camelcase", camelcase, METH_VARARGS, "Convert string into camel case."},
    {"capitalcase", capitalcase, METH_VARARGS, "Convert string into capital case."},
    {"constcase", constcase, METH_VARARGS, "Convert string into const case."},
    {"lowercase", lowercase, METH_VARARGS, "Convert string into lower case."},
    {"pascalcase", pascalcase, METH_VARARGS, "Convert string into pascal case."},
    {"pathcase", pathcase, METH_VARARGS, "Convert string into path case."},
    {"backslashcase", backslashcase, METH_VARARGS, "Convert string into backslash case."},
    {"sentencecase", sentencecase, METH_VARARGS, "Convert string into sentence case."},
    {"snakecase", snakecase, METH_VARARGS, "Convert string into snake case."},
    {"spinalcase", spinalcase, METH_VARARGS, "Convert string into spinal case."},
    {"dotcase", dotcase, METH_VARARGS, "Convert string into dot case."},
    {"titlecase", titlecase, METH_VARARGS, "Convert string into title case."},
    {"trimcase", trimcase, METH_VARARGS, "Convert string into trim case."},
    {"uppercase", uppercase, METH_VARARGS, "Convert string into upper case."},
    {"alphanumcase", alphanumcase, METH_VARARGS, "Convert string into alphanumeric case."},
    {NULL, NULL, 0, NULL} // Sentinel
};

static struct PyModuleDef fast_stringcase_module = {
    PyModuleDef_HEAD_INIT,
    "fast_stringcase",   // name of module
    "C extension for string case conversions based on stringcase_base.py.", // module documentation, may be NULL
    -1,       // size of per-interpreter state of the module, or -1 if the module keeps state in global variables.
    StringCaseMethods
};

PyMODINIT_FUNC PyInit_fast_stringcase(void) {
    return PyModule_Create(&fast_stringcase_module);
}
