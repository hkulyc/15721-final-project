from pathlib import Path
import click
import yaml  # yaml configuration file parser
from .trans import translate_plpgsql_udf_str

root_path = Path(__file__).parent

with open(root_path/'example.yaml', 'r') as file:
    example_config = yaml.safe_load(file)

file_option = click.option(
    "--output",
    "-o",
    help=(
        "Output C++ to a file."
    ),
)


@click.command()
@click.argument("filepath")
@file_option
def main(filepath: str, output: str) -> None:
    """Transpile a UDF from PLpgSQL to C++."""
    with open(filepath, 'r') as file:
        cpp_output, regs, decls = translate_plpgsql_udf_str(file.read())
    if output:
        if not output.endswith('/'):
            output = output + '/'
        # the cpp file
        udf_file = output + 'udf.cpp'
        with open(udf_file, 'w') as file:
            file.write(cpp_output)
        # the hpp file
        udf_header = output + 'udf.hpp'
        param = {
            'udf_decls': decls
        }
        with open(udf_header, 'w') as file:
            file.write(example_config['udf.hpp'].format(**param))
        # the main test file
        test_file = output + 'test.cpp'
        param = {
            'udf_register': regs
        }
        with open(test_file, 'w') as file:
            file.write(example_config['main'].format(**param))
    else:
        print(cpp_output)
