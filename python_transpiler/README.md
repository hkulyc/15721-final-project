# The REAL Transpiler

## Developement
First set up a python virtual environment with
```
python3 -m venv env
source env/bin/activate
```
Next install the transpiler
```
pip3 install -e udftranspiler/
```

## Usage

To get a list of commands and options:

```
udftranspiler --help
```

### File Structure
- env
  The packages for python virtual environment
- udftranspiler
  The transpiler module that can work independently.
  - libraries
    Self modified libraries
  - resources
    Configuration files and language information
  - udftranspiler
    The main source code
- setup.py
  Add new library dependency here.
