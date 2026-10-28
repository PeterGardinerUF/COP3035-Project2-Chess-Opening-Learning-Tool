# Converts a PGN file into a one-game-per-line UCI format:
# 1-0 e2e4 e7e5 g1f3 b8c6 ...
#
# Caveats:
# - The trie only counts games with this exact move order. The hash map
#   counts every game that reached the same position, in any order. The
#   win/loss ratio should be similar, but the trie is a subset of those games.


# - The trie needs each user move from the board as the same UCI token
#   (start square + end square, like e2e4). Algebraic moves will not match.


# - The hash map holds 200000 positions. A full elite month is too many.
#   This script only writes the text file.


# - En passant and promotion already come from move.uci(). Castling is
#   rewritten so the king lands on its rook (e1g1 -> e1h1). Queenside
#   castling still breaks the hash map: ApplyContinuation clears the rook
#   it just placed and leaves the king on e1/e8.

import sys
import chess.pgn

# ---- Settings -----
INPUT_PGN_FILE = "lichess_elite_2025-11.pgn"
OUTPUT_UCI_FILE = "games.txt"
MAX_PLY = 30 # Maximum number of half-moves to keep per game
MIN_PLY = 10 # Drops games that are shorter than this
MAX_GAMES = None # Maximum number of games to process (None for no limit)
# -------------------

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
                print(f"Processed {kept + dropped} games...", file=sys.stderr)
    print(f"Kept {kept} games, dropped {dropped} games.", file=sys.stderr)

if __name__ == "__main__":
    convert()
