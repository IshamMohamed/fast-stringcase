# Fast Stringcase Project
This project provides a fast and efficient way to convert strings between different cases. This is based on stringcase version 1.2.0. This is why the version has been kept same.

## Local Deployment and Testing
To build and install use the following:
```bash
python -m build
pip install .
```

To run unit tests:
```bash
python test_fast_stringcase.py
```

## Usage
To use the project, simply import the fast_stringcase module and call the desired function. For example:
```python
import fast_stringcase

print(stringcase.titlecase('FooBarBaz'))  # Output: Foo Bar Baz
```