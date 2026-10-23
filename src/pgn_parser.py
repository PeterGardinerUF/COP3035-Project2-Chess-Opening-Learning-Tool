
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

# RT Parser
import requests

# Chess.com blocks requests that do not send a User-Agent.
HEADERS = {"User-Agent": "chess-opening-tool"}

# Download the most recent month of games for a Chess.com username.
def get_pgn(username):
    url = "https://api.chess.com/pub/player/" + username + "/games/archives"
    response = requests.get(url, headers=HEADERS)
    archives = response.json()["archives"]

    if len(archives) == 0:
        return ""

    latest = archives[-1] + "/pgn"
    response = requests.get(latest, headers=HEADERS)
    return response.text

# Pull White, Black, Result, and the moves out of a PGN.
# Returns a list of [white, black, result, moves].
def get_facts(pgn):
    games = []
    white = ""
    black = ""
    result = ""
    moves = ""

    for line in pgn.splitlines():
        if line.startswith("[White "):
            white = line.split('"')[1]
        elif line.startswith("[Black "):
            black = line.split('"')[1]
        elif line.startswith("[Result "):
            result = line.split('"')[1]
        elif line != "" and not line.startswith("["):
            moves = moves + " " + line
        elif line == "" and moves != "":
            games.append([white, black, result, moves.strip()])
            white = ""
            black = ""
            result = ""
            moves = ""

    if moves != "":
        games.append([white, black, result, moves.strip()])

    return games

if __name__ == "__main__":
    username = input("Enter a Chess.com username: ")
    pgn = get_pgn(username.strip())
    games = get_facts(pgn)

    for game in games:
        print(game)