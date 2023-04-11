from pathlib import Path
import click
import yaml # yaml configuration file parser

root_path = Path(__file__).parent.parent
resources_path = root_path / 'resources'

with open(resources_path/'example.yaml', 'r') as file:
    example_config = yaml.safe_load(file)

@click.group()
def main() -> None:
    """Transpile a UDF from PLpgSQL to C++."""
    pass

if __name__ == '__main__':
    # main()
    print(example_config['location'])


