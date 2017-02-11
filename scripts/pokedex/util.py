def base_form(pokedex, search_id):
    for id, value in pokedex.items():
        if search_id in value.get('formes', []):
            return id

    return None
