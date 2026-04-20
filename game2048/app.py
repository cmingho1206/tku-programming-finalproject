import ctypes
import os
import subprocess
import time

from flask import Flask, jsonify, render_template, request


BASE_DIR = os.path.dirname(os.path.abspath(__file__))
BACKEND_DIR = os.path.join(BASE_DIR, "backend")
LIB_PATH = os.path.join(BACKEND_DIR, "libgame2048.dylib")


class Game2048State(ctypes.Structure):
    _fields_ = [
        ("board", (ctypes.c_int * 4) * 4),
        ("score", ctypes.c_int),
        ("game_over", ctypes.c_int),
        ("win", ctypes.c_int),
        ("seed", ctypes.c_uint),
    ]


def build_c_library():
    source = os.path.join(BACKEND_DIR, "game2048.c")
    cmd = ["gcc", "-shared", "-fPIC", source, "-o", LIB_PATH]
    subprocess.run(cmd, check=True, cwd=BACKEND_DIR)


build_c_library()
lib = ctypes.CDLL(LIB_PATH)

lib.g2048_init.argtypes = [ctypes.POINTER(Game2048State), ctypes.c_uint]
lib.g2048_init.restype = ctypes.c_int
lib.g2048_move.argtypes = [ctypes.POINTER(Game2048State), ctypes.c_int]
lib.g2048_move.restype = ctypes.c_int

app = Flask(__name__)
state = Game2048State()


def board_payload():
    board = []
    for r in range(4):
        row = []
        for c in range(4):
            row.append(state.board[r][c])
        board.append(row)
    return board


def payload():
    return {
        "board": board_payload(),
        "score": state.score,
        "game_over": bool(state.game_over),
        "win": bool(state.win),
    }


def reset_game():
    seed = int(time.time())
    result = lib.g2048_init(ctypes.byref(state), seed)
    if result != 0:
        raise ValueError(f"init failed: {result}")


@app.get("/")
def index():
    return render_template("index.html")


@app.post("/api/new")
def api_new():
    reset_game()
    return jsonify(payload())


@app.post("/api/move")
def api_move():
    data = request.get_json(force=True)
    direction = int(data["direction"])
    lib.g2048_move(ctypes.byref(state), direction)
    return jsonify(payload())


if __name__ == "__main__":
    reset_game()
    app.run(debug=True)
