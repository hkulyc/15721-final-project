
def is_assignment(query:str):
    "Check if a query is an assignment statement"
    return ':=' in query

def parse_assignment(query:str, vars):
    "Parse an assignment query. May raise exception if wrong"
    pass