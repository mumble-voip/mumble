win32 {
	PYTHON=python
} else {
	PYTHON=$$system(which python 2>/dev/null)
	isEmpty(PYTHON) {
		PYTHON=$$system(which python2 2>/dev/null)
	}
	isEmpty(PYTHON) {
		PYTHON=$$system(which python2.7 2>/dev/null)
	}
	isEmpty(PYTHON) {
		PYTHON=$$system(which python3 2>/dev/null)
	}
	isEmpty(PYTHON) {
		PYTHON=$$system(which python3.0 2>/dev/null)
	}
	isEmpty(PYTHON) {
		PYTHON=$$system(which python3.1 2>/dev/null)
	}
	isEmpty(PYTHON) {
		PYTHON=$$system(which python3.2 2>/dev/null)
	}
	isEmpty(PYTHON) {
		PYTHON=$$system(which python3.3 2>/dev/null)
	}
	isEmpty(PYTHON) {
		PYTHON=$$system(which python3.4 2>/dev/null)
	}
	isEmpty(PYTHON) {
		PYTHON=$$system(which python3.5 2>/dev/null)
	}
	isEmpty(PYTHON) {
		error("Unable to find the system's Python binary. Some scripts invoked during the Mumble build use Python. You can manually specify it via the MUMBLE_PYTHON environment variable (either 2 or 3).")
	}
}
