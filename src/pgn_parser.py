

# Parse a single chess game from PGN format
# Discards games that are too short. Caps games that are too long.
def parse_game(game, max_ply=30, min_ply=10):
    result = game.headers.get("Result", "*")
    if result not in VALID_RESULTS:
        return None
    
    moves = []
    for move in game.mainline_moves():
        moves.append(move.uci())
        if len(moves) >= max_ply:
            break
    
    if len(moves) < min_ply:
        return None

    return result + " " + " ".join(moves)