import click

@click.group()
def main() -> None:
    """Transpile a UDF from PLpgSQL to C++."""
    pass