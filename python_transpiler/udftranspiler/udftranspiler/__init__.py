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
        cpp_output, reg = translate_plpgsql_udf_str(file.read())
    if output:
        with open(output, 'w') as file:
            file.write(cpp_output)
    else:
        print(cpp_output)
