import ctypes
import os
import subprocess
import time
from flask import Flask, jsonify, render_template, request


BASE_DIR = os.path.dirname(os.path.abspath(__file__))
BACKEND_DIR = os.path.join(BASE_DIR, "backend")
LIB_PATH = os.path.join(BACKEND_DIR, "libminesweeper.dylib")


class GameState(ctypes.Structure):
    _fields_ = [
        ("rows", ctypes.c_int),
        ("cols", ctypes.c_int),
        ("mine_count", ctypes.c_int),
        ("game_over", ctypes.c_int),
        ("win", ctypes.c_int),
        ("revealed_count", ctypes.c_int),
        ("seed", ctypes.c_uint),
        ("board", (ctypes.c_int * 30) * 30),
        ("revealed", (ctypes.c_int * 30) * 30),
        ("flagged", (ctypes.c_int * 30) * 30),
    ]


def build_c_library():
    if os.path.exists(LIB_PATH):
        return
    source = os.path.join(BACKEND_DIR, "minesweeper.c")
    cmd = ["gcc", "-shared", "-fPIC", source, "-o", LIB_PATH]
    subprocess.run(cmd, check=True, cwd=BACKEND_DIR)


build_c_library()
lib = ctypes.CDLL(LIB_PATH)

lib.ms_init.argtypes = [ctypes.POINTER(GameState), ctypes.c_int, ctypes.c_int, ctypes.c_int, ctypes.c_uint]
lib.ms_init.restype = ctypes.c_int
lib.ms_reveal.argtypes = [ctypes.POINTER(GameState), ctypes.c_int, ctypes.c_int]
lib.ms_reveal.restype = ctypes.c_int
lib.ms_toggle_flag.argtypes = [ctypes.POINTER(GameState), ctypes.c_int, ctypes.c_int]
lib.ms_toggle_flag.restype = ctypes.c_int
lib.ms_get_cell_display.argtypes = [ctypes.POINTER(GameState), ctypes.c_int, ctypes.c_int]
lib.ms_get_cell_display.restype = ctypes.c_int

app = Flask(__name__)
state = GameState()


def cell_to_text(v):
    if v == -3:
        return ""
    if v == -2:
        return "F"
    if v == -1:
        return "*"
    if v == 0:
        return ""
    return str(v)


def dump_board():
    board = []
    for r in range(state.rows):
        row = []
        for c in range(state.cols):
            value = lib.ms_get_cell_display(ctypes.byref(state), r, c)
            row.append({"value": value, "text": cell_to_text(value)})
        board.append(row)
    return board


def game_payload():
    return {
        "rows": state.rows,
        "cols": state.cols,
        "mines": state.mine_count,
        "game_over": bool(state.game_over),
        "win": bool(state.win),
        "board": dump_board(),
    }


def new_game(rows=9, cols=9, mines=10):
    seed = int(time.time())
    result = lib.ms_init(ctypes.byref(state), rows, cols, mines, seed)
    if result != 0:
        raise ValueError(f"failed to initialize game: {result}")


@app.get("/")
def index():
    return render_template("index.html")


@app.post("/api/new")
def api_new():
    data = request.get_json(silent=True) or {}
    rows = int(data.get("rows", 9))
    cols = int(data.get("cols", 9))
    mines = int(data.get("mines", 10))
    new_game(rows, cols, mines)
    return jsonify(game_payload())


@app.post("/api/reveal")
def api_reveal():
    data = request.get_json(force=True)
    row = int(data["row"])
    col = int(data["col"])
    lib.ms_reveal(ctypes.byref(state), row, col)
    return jsonify(game_payload())


@app.post("/api/flag")
def api_flag():
    data = request.get_json(force=True)
    row = int(data["row"])
    col = int(data["col"])
    lib.ms_toggle_flag(ctypes.byref(state), row, col)
    return jsonify(game_payload())


if __name__ == "__main__":
    new_game()
    app.run(debug=True)
