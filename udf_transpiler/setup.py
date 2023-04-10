from pathlib import Path
from setuptools import setup

long_description = (Path(__file__).parent / "README.md").read_text()

setup(
    name="udftranspiler",
    python_requires=">=3.8",
    description="Transpile UDF's from PL/pgSQL to C++.",
    url="https://github.com/hkulyc/15721-final-project",
    version="0.0.1",
    license="MIT",
    long_description=long_description,
    long_description_content_type='text/markdown',
    install_requires=[
        "click"
    ],
    py_modules=['udftranspiler'],
    entry_points={
        'console_scripts': [
            'udftranspiler = udftranspiler:main',
        ]
    },
)