# Converts a PGN file into a one-game-per-line UCI format:
# 1-0 e2e4 e7e5 g1f3 b8c6 ...

import sys
import chess.pgn

VALID_RESULTS = {"1-0", "0-1", "1/2-1/2"}

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

# Convert a PGN file to a one-game-per-line UCI format file.
def convert(in_path, out_path, max_ply=30, min_ply=10):
    kept = dropped = 0
    with open(in_path, encoding="utf-8", errors="replace") as fin, open(out_path, "w", encoding="utf-8") as fout:
        while True:
            game = chess.pgn.read_game(fin)
            if game is None:
                break
            line = parse_game(game, max_ply, min_ply)
            if line is not None:
                fout.write(line + "\n")
                kept += 1
            else:
                dropped += 1
            if (kept + dropped) % 20000 == 0:
                print(f"Processed {kept + dropped} games...", file=sys.stderr)
    print(f"Kept {kept} games, dropped {dropped} games.", file=sys.stderr)

if __name__ == "__main__":
    if len(sys.argv) < 3:
        sys.exit(__doc__)
    max_ply = int(sys.argv[3]) if len(sys.argv) > 3 else 30
    min_ply = int(sys.argv[4]) if len(sys.argv) > 4 else 10
    convert(sys.argv[1], sys.argv[2], max_ply=max_ply, min_ply=min_ply)
