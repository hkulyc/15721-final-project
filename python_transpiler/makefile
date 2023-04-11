PIP_INSTALL_OPTIONS = --quiet --use-pep517 --disable-pip-version-check

run:
	@-pip3 install -e udftranspiler/ $(PIP_INSTALL_OPTIONS) || true
	@-udftranspiler || true

run-file:
	@-python udftranspiler/udftranspiler/__init__.py || true