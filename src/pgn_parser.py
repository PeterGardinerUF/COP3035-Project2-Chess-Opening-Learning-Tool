#Converts a PGN file into a one-game-per-line UCI format:
#1-0 e2e4 e7e5 g1f3 b8c6 ...

import chess.pgn

INPUT_PGN_FILE = "Data/lichess_elite_2022-05.pgn"
OUTPUT_UCI_FILE = "Data/games.txt"
MAX_PLY = 30 # Maximum number of half-moves to keep per game
MIN_PLY = 10 # Drops games that are shorter than this
MAX_GAMES = None # Maximum number of games to process (None for no limit)

VALID_RESULTS = {"1-0", "0-1", "1/2-1/2"}

# Parse a single chess game from PGN format
# Discards games that are too short. Caps games that are too long.
def parse_game(game):
    result = game.headers.get("Result", "*")
    if result not in VALID_RESULTS:
        return None

    moves = []
    for move in game.mainline_moves():
        uci = move.uci()

        # Rewrite castling moves
        if uci == "e1g1":
            uci = "e1h1"
        elif uci == "e1c1":
            uci = "e1a1"
        elif uci == "e8g8":
            uci = "e8h8"
        elif uci == "e8c8":
            uci = "e8a8"
        moves.append(uci)
        if len(moves) >= MAX_PLY:
            break

    if len(moves) < MIN_PLY:
        return None

    return result + " " + " ".join(moves)

# Convert a PGN file to a one-game-per-line UCI format file.
def convert():
    kept = dropped = 0
    with open(INPUT_PGN_FILE, encoding="utf-8", errors="replace") as fin, open(OUTPUT_UCI_FILE, "w", encoding="utf-8") as fout:
        while MAX_GAMES is None or kept < MAX_GAMES:
            game = chess.pgn.read_game(fin)
            if game is None:
                break
            line = parse_game(game)
            if line is not None:
                fout.write(line + "\n")
                kept += 1
            else:
                dropped += 1
            if (kept + dropped) % 20000 == 0:
                print("Processed " + str(kept + dropped) + " games...")
    print("Kept " + str(kept) + " games, dropped " + str(dropped) + " games.")

if name == "main":
    convert()
